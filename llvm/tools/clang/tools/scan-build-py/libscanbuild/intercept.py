# -*- coding: utf-8 -*-
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
""" This module is responsible to capture the compiler invocation of any
build process. The result of that should be a compilation database.

This implementation is using the LD_PRELOAD or DYLD_INSERT_LIBRARIES
mechanisms provided by the dynamic linker. The related library is implemented
in C language and can be found under 'libear' directory.

The 'libear' library is capturing all child process creation and logging the
relevant information about it into separate files in a specified directory.
The parameter of this process is the output directory name, where the report
files shall be placed. This parameter is passed as an environment variable.

The module also implements compiler wrappers to intercept the compiler calls.

The module implements the build command execution and the post-processing of
the output files, which will condensates into a compilation database. """

import sys
import os
import os.path
import re
import itertools
import json
import glob
import argparse
import logging
from libear import build_libear, TemporaryDirectory
from libscanbuild import command_entry_point, compiler_wrapper, \
    wrapper_environment, run_command, run_build, reconfigure_logging
from libscanbuild import duplicate_check, tempdir
from libscanbuild.compilation import split_command
from libscanbuild.shell import encode, decode

__all__ = ['capture', 'intercept_build_main', 'intercept_compiler_wrapper']

GS = chr(0x1d)
RS = chr(0x1e)
US = chr(0x1f)

COMPILER_WRAPPER_CC = 'intercept-cc'
COMPILER_WRAPPER_CXX = 'intercept-c++'
TRACE_FILE_EXTENSION = '.cmd'  # same as in ear.c
WRAPPER_ONLY_PLATFORMS = frozenset({'win32', 'cygwin'})


@command_entry_point
def intercept_build_main(bin_dir):
    """ Entry point for 'intercept-build' command. """

    parser = create_parser()
    args = parser.parse_args()

    reconfigure_logging(args.verbose)
    logging.debug('Raw arguments %s', sys.argv)

    if not args.build:
        parser.print_help()
        return 0

    return capture(args, bin_dir)


def capture(args, bin_dir):
    """ The entry point of build command interception. """

    def post_processing(commands):
        """ To make a compilation database, it needs to filter out commands
        which are not compiler calls. Needs to find the source file name
        from the arguments. And do shell escaping on the command.

        To support incremental builds, it is desired to read elements from
        an existing compilation database from a previous run. These elements
        shall be merged with the new elements. """

        # create entries from the current run
        current = itertools.chain.from_iterable(
            # creates a sequence of entry generators from an exec,
            format_entry(command) for command in commands)
        # read entries from previous run
        if 'append' in args and args.append and os.path.isfile(args.cdb):
            with open(args.cdb) as handle:
                previous = iter(json.load(handle))
        else:
            previous = iter([])
        # filter out duplicate entries from both
        duplicate = duplicate_check(entry_hash)
        return (entry
                for entry in itertools.chain(previous, current)
                if os.path.exists(entry['file']) and not duplicate(entry))

    with TemporaryDirectory(prefix='intercept-', dir=tempdir()) as tmp_dir:
        # run the build command
        environment = setup_environment(args, tmp_dir, bin_dir)
        exit_code = run_build(args.build, env=environment)
        # read the intercepted exec calls
        exec_traces = itertools.chain.from_iterable(
            parse_exec_trace(os.path.join(tmp_dir, filename))
            for filename in sorted(glob.iglob(os.path.join(tmp_dir, '*.cmd'))))
        # do post processing
        entries = post_processing(exec_traces)
        # dump the compilation database
        with open(args.cdb, 'w+') as handle:
            json.dump(list(entries), handle, sort_keys=True, indent=4)
        return exit_code


def setup_environment(args, destination, bin_dir):
    """ Sets up the environment for the build command.

    It sets the required environment variables and execute the given command.
    The exec calls will be logged by the 'libear' preloaded library or by the
    'wrapper' programs. """

    c_compiler = args.cc if 'cc' in args else 'cc'
    cxx_compiler = args.cxx if 'cxx' in args else 'c++'

    libear_path = None if args.override_compiler or is_preload_disabled(
        sys.platform) else build_libear(c_compiler, destination)

    environment = dict(os.environ)
    environment.update({'INTERCEPT_BUILD_TARGET_DIR': destination})

    if not libear_path:
        logging.debug('intercept gonna use compiler wrappers')
        environment.update(wrapper_environment(args))
        environment.update({
            'CC': os.path.join(bin_dir, COMPILER_WRAPPER_CC),
            'CXX': os.path.join(bin_dir, COMPILER_WRAPPER_CXX)
        })
    elif sys.platform == 'darwin':
        logging.debug('intercept gonna preload libear on OSX')
        environment.update({
            'DYLD_INSERT_LIBRARIES': libear_path,
            'DYLD_FORCE_FLAT_NAMESPACE': '1'
        })
    else:
        logging.debug('intercept gonna preload libear on UNIX')
        environment.update({'LD_PRELOAD': libear_path})

    return environment


@command_entry_point
def intercept_compiler_wrapper():
    """ Entry point for `intercept-cc` and `intercept-c++`. """

    return compiler_wrapper(intercept_compiler_wrapper_impl)


def intercept_compiler_wrapper_impl(_, execution):
    """ Implement intercept compiler wrapper functionality.

    It does generate execution report into target directory.
    The target directory name is from environment variables. """

    message_prefix = 'execution report might be incomplete: %s'

    target_dir = os.getenv('INTERCEPT_BUILD_TARGET_DIR')
    if not target_dir:
        logging.warning(message_prefix, 'missing target directory')
        return
    # write current execution info to the pid file
    try:
        target_file_name = str(os.getpid()) + TRACE_FILE_EXTENSION
        target_file = os.path.join(target_dir, target_file_name)
        logging.debug('writing execution report to: %s', target_file)
        write_exec_trace(target_file, execution)
    except IOError:
        logging.warning(message_prefix, 'io problem')


def write_exec_trace(filename, entry):
    """ Write execution report file.

    This method shall be sync with the execution report writer in interception
    library. The entry in the file is a JSON objects.

    :param filename:    path to the output execution trace file,
    :param entry:       the Execution object to append to that file. """

    with open(filename, 'ab') as handler:
        pid = str(entry.pid)
        command = US.join(entry.cmd) + US
        content = RS.join([pid, pid, 'wrapper', entry.cwd, command]) + GS
        handler.write(content.encode('utf-8'))


def parse_exec_trace(filename):
    """ Parse the file generated by the 'libear' preloaded library.

    Given filename points to a file which contains the basic report
    generated by the interception library or wrapper command. A single
    report file _might_ contain multiple process creation info. """

    logging.debug('parse exec trace file: %s', filename)
    with open(filename, 'r') as handler:
        content = handler.read()
        for group in filter(bool, content.split(GS)):
            records = group.split(RS)
            yield {
                'pid': records[0],
                'ppid': records[1],
                'function': records[2],
                'directory': records[3],
                'command': records[4].split(US)[:-1]
            }


def format_entry(exec_trace):
    """ Generate the desired fields for compilation database entries. """

    def abspath(cwd, name):
        """ Create normalized absolute path from input filename. """
        fullname = name if os.path.isabs(name) else os.path.join(cwd, name)
        return os.path.normpath(fullname)

    logging.debug('format this command: %s', exec_trace['command'])
    compilation = split_command(exec_trace['command'])
    if compilation:
        for source in compilation.files:
            compiler = 'c++' if compilation.compiler == 'c++' else 'cc'
            command = [compiler, '-c'] + compilation.flags + [source]
            logging.debug('formated as: %s', command)
            yield {
                'directory': exec_trace['directory'],
                'command': encode(command),
                'file': abspath(exec_trace['directory'], source)
            }


def is_preload_disabled(platform):
    """ Library-based interposition will fail silently if SIP is enabled,
    so this should be detected. You can detect whether SIP is enabled on
    Darwin by checking whether (1) there is a binary called 'csrutil' in
    the path and, if so, (2) whether the output of executing 'csrutil status'
    contains 'System Integrity Protection status: enabled'.

    :param platform: name of the platform (returned by sys.platform),
    :return: True if library preload will fail by the dynamic linker. """

    if platform in WRAPPER_ONLY_PLATFORMS:
        return True
    elif platform == 'darwin':
        command = ['csrutil', 'status']
        pattern = re.compile(r'System Integrity Protection status:\s+enabled')
        try:
            return any(pattern.match(line) for line in run_command(command))
        except:
            return False
    else:
        return False


def entry_hash(entry):
    """ Implement unique hash method for compilation database entries. """

    # For faster lookup in set filename is reverted
    filename = entry['file'][::-1]
    # For faster lookup in set directory is reverted
    directory = entry['directory'][::-1]
    # On OS X the 'cc' and 'c++' compilers are wrappers for
    # 'clang' therefore both call would be logged. To avoid
    # this the hash does not contain the first word of the
    # command.
    command = ' '.join(decode(entry['command'])[1:])

    return '<>'.join([filename, directory, command])


def create_parser():
    """ Command line argument parser factory method. """

    parser = argparse.ArgumentParser(
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        '--verbose', '-v',
        action='count',
        default=0,
        help="""Enable verbose output from '%(prog)s'. A second and third
                flag increases verbosity.""")
    parser.add_argument(
        '--cdb',
        metavar='<file>',
        default="compile_commands.json",
        help="""The JSON compilation database.""")
    group = parser.add_mutually_exclusive_group()
    group.add_argument(
        '--append',
        action='store_true',
        help="""Append new entries to existing compilation database.""")

    advanced = parser.add_argument_group('advanced options')
    advanced.add_argument(
        '--override-compiler',
        action='store_true',
        help="""Always resort to the compiler wrapper even when better
                intercept methods are available.""")
    advanced.add_argument(
        '--use-cc',
        metavar='<path>',
        dest='cc',
        default='cc',
        help="""When '%(prog)s' analyzes a project by interposing a compiler
                wrapper, which executes a real compiler for compilation and
                do other tasks (record the compiler invocation). Because of
                this interposing, '%(prog)s' does not know what compiler your
                project normally uses. Instead, it simply overrides the CC
                environment variable, and guesses your default compiler.

                If you need '%(prog)s' to use a specific compiler for
                *compilation* then you can use this option to specify a path
                to that compiler.""")
    advanced.add_argument(
        '--use-c++',
        metavar='<path>',
        dest='cxx',
        default='c++',
        help="""This is the same as "--use-cc" but for C++ code.""")

    parser.add_argument(
        dest='build',
        nargs=argparse.REMAINDER,
        help="""Command to run.""")

    return parser

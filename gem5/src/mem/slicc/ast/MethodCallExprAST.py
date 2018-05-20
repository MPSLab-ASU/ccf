# Copyright (c) 1999-2008 Mark D. Hill and David A. Wood
# Copyright (c) 2009 The Hewlett-Packard Development Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

from slicc.ast.ExprAST import ExprAST

class MethodCallExprAST(ExprAST):
    def __init__(self, slicc, proc_name, expr_ast_vec):
        super(MethodCallExprAST, self).__init__(slicc)
        self.proc_name = proc_name
        self.expr_ast_vec = expr_ast_vec

    def generate(self, code):
        tmp = self.slicc.codeFormatter()
        paramTypes = []
        for expr_ast in self.expr_ast_vec:
            return_type = expr_ast.generate(tmp)
            paramTypes.append(return_type)

        obj_type, methodId, prefix = self.generate_prefix(paramTypes)

        # generate code
        params = []
        for expr_ast in self.expr_ast_vec:
            return_type,tcode = expr_ast.inline(True)
            params.append(str(tcode))
        fix = code.nofix()
        code("$prefix${{self.proc_name}}(${{', '.join(params)}}))")
        code.fix(fix)

        # Verify that this is a method of the object
        if methodId not in obj_type.methods:
            self.error("Invalid method call: Type '%s' does not have a method '%s'",
                       obj_type, methodId)

        if len(self.expr_ast_vec) != \
               len(obj_type.methods[methodId].param_types):
            # Right number of parameters
            self.error("Wrong number of parameters for function name: '%s', " + \
                       "expected: , actual: ", proc_name,
                  len(obj_type.methods[methodId].param_types),
                  len(self.expr_ast_vec))

        for actual_type, expected_type in \
                zip(paramTypes, obj_type.methods[methodId].param_types):
            if actual_type != expected_type and \
                   str(actual_type["interface"]) != str(expected_type):
                self.error("Type mismatch: expected: %s actual: %s",
                           expected_type, actual_type)

        # Return the return type of the method
        return obj_type.methods[methodId].return_type

    def findResources(self, resources):
        pass

class MemberMethodCallExprAST(MethodCallExprAST):
    def __init__(self, slicc, obj_expr_ast, proc_name, expr_ast_vec):
        s = super(MemberMethodCallExprAST, self)
        s.__init__(slicc, proc_name, expr_ast_vec)

        self.obj_expr_ast = obj_expr_ast

    def __repr__(self):
        return "[MethodCallExpr: %r%r %r]" % (self.proc_name,
                                              self.obj_expr_ast,
                                              self.expr_ast_vec)
    def generate_prefix(self, paramTypes):
        code = self.slicc.codeFormatter()

        # member method call
        obj_type = self.obj_expr_ast.generate(code)
        methodId = obj_type.methodId(self.proc_name, paramTypes)

        prefix = ""
        implements_interface = False

        if methodId in obj_type.methods:
            return_type = obj_type.methods[methodId].return_type

        else:
            #
            # Check whether the method is implemented by the super class
            if "interface" in obj_type:
                interface_type = self.symtab.find(obj_type["interface"]);

                if methodId in interface_type.methods:
                    return_type = interface_type.methods[methodId].return_type
                    obj_type = interface_type

                else:
                    self.error("Invalid method call: " \
                               "Type '%s' does not have a method %s, '%s'",
                               obj_type, self.proc_name, methodId)

            else:
                  #
                  # The initial method check has failed, but before generating an
                  # error we must check whether any of the paramTypes implement
                  # an interface.  If so, we must check if the method ids using
                  # the inherited types exist.
                  #
                  # This code is a temporary fix and only checks for the methodId
                  # where all paramTypes are converted to their inherited type.  The
                  # right way to do this is to replace slicc's simple string
                  # comparison for determining the correct overloaded method, with a
                  # more robust param by param check.
                  #
                  implemented_paramTypes = []
                  for paramType in paramTypes:
                      implemented_paramType = paramType
                      if paramType.isInterface:
                          implements_interface = True
                          implemented_paramType.abstract_ident = paramType["interface"]
                      else:
                          implemented_paramType.abstract_ident = paramType.c_ident

                      implemented_paramTypes.append(implemented_paramType)

                  if implements_interface:
                      implementedMethodId = obj_type.methodIdAbstract(self.proc_name,
                                                                      implemented_paramTypes)
                  else:
                      implementedMethodId = ""

                  if implementedMethodId not in obj_type.methods:
                      self.error("Invalid method call: " \
                                 "Type '%s' does not have a method %s, '%s' nor '%s'",
                                 obj_type, self.proc_name, methodId, implementedMethodId)
                  else:
                      #
                      # Replace the methodId with the implementedMethodId found in
                      # the method list.
                      #
                      methodId = implementedMethodId
                      return_type = obj_type.methods[methodId].return_type

        if return_type.isInterface:
            prefix = "static_cast<%s &>" % return_type.c_ident

        if str(obj_type) == "AbstractCacheEntry" or \
           str(obj_type) == "AbstractEntry" or \
           ("interface" in obj_type and (
            obj_type["interface"] == "AbstractCacheEntry" or
            obj_type["interface"] == "AbstractEntry")):
            prefix = "%s((*(%s))." % (prefix, code)
        else:
            prefix = "%s((%s)." % (prefix, code)

        return obj_type, methodId, prefix


class ClassMethodCallExprAST(MethodCallExprAST):
    def __init__(self, slicc, type_ast, proc_name, expr_ast_vec):
        s = super(ClassMethodCallExprAST, self)
        s.__init__(slicc, proc_name, expr_ast_vec)

        self.type_ast = type_ast

    def __repr__(self):
        return "[MethodCallExpr: %r %r]" % (self.proc_name, self.expr_ast_vec)

    def generate_prefix(self, paramTypes):

        # class method call
        prefix = "(%s::" % self.type_ast
        obj_type = self.type_ast.type
        methodId = obj_type.methodId(self.proc_name, paramTypes)

        return obj_type, methodId, prefix

__all__ = [ "MemberMethodCallExprAST", "ClassMethodCallExprAST" ]

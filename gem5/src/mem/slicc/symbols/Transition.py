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

from slicc.symbols.Symbol import Symbol

class Transition(Symbol):
    def __init__(self, table, machine, state, event, nextState, actions,
                 request_types, location, pairs):
        ident = "%s|%s" % (state, event)
        super(Transition, self).__init__(table, ident, location, pairs)

        self.state = machine.states[state]
        self.event = machine.events[event]
        self.nextState = machine.states[nextState]
        self.actions = [ machine.actions[a] for a in actions ]
        self.request_types = [ machine.request_types[s] for s in request_types ]
        self.resources = {}

        for action in self.actions:
            for var,value in action.resources.iteritems():
                if var.type.ident != "DNUCAStopTable":
                    num = int(value)
                    if var in self.resources:
                        num += int(value)
                    self.resources[var] = str(num)
                else:
                    self.resources[var] = value

    def __repr__(self):
      return "[Transition: (%r, %r) -> %r, %r]" % \
             (self.state, self.event, self.nextState, self.actions)

    def getActionShorthands(self):
        assert self.actions

        return ''.join(a.short for a in self.actions)

__all__ = [ "Transition" ]

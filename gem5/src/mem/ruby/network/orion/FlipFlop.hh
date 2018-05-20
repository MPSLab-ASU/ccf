/*
 * Copyright (c) 2009 Princeton University
 * Copyright (c) 2009 The Regents of the University of California
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors:  Hangsheng Wang (Orion 1.0, Princeton)
 *           Xinping Zhu (Orion 1.0, Princeton)
 *           Xuning Chen (Orion 1.0, Princeton)
 *           Bin Li (Orion 2.0, Princeton)
 *           Kambiz Samadi (Orion 2.0, UC San Diego)
 */

#ifndef __FLIPFLOP_H__
#define __FLIPFLOP_H__

#include "mem/ruby/network/orion/Type.hh"

class TechParameter;

class FlipFlop
{
  public:
    enum FFModel
    {
      NO_MODEL = 0,
      NEG_DFF
    };

  public:
    FlipFlop(
      const string& ff_model_str_,
      double load_,
      const TechParameter* tech_param_ptr_
    );
    ~FlipFlop();

  public:
    double get_e_switch() const { return m_e_switch; }
    double get_e_keep_1() const { return m_e_keep_1; }
    double get_e_keep_0() const { return m_e_keep_0; }
    double get_e_clock() const { return m_e_clock; }
    double get_i_static() const { return m_i_static; }

  private:
    void init();
    double calc_node_cap(uint32_t num_fanin_, uint32_t num_fanout_);
    double calc_clock_cap();
    double calc_i_static();

  private:
    FFModel m_ff_model;
    const TechParameter* m_tech_param_ptr;

    double m_load;

    double m_e_switch;
    double m_e_keep_1;
    double m_e_keep_0;
    double m_e_clock;

    double m_i_static;
};

#endif

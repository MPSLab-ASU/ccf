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

#ifndef __MATRIXCROSSBAR_H__
#define __MATRIXCROSSBAR_H__

#include "mem/ruby/network/orion/Crossbar/Crossbar.hh"
#include "mem/ruby/network/orion/Type.hh"

class TechParameter;

class MatrixCrossbar : public Crossbar
{
  public:
    MatrixCrossbar(
      const string& conn_type_str_,
      const string& trans_type_str_,
      uint32_t num_in_,
      uint32_t num_out_,
      uint32_t data_width_,
      uint32_t num_in_seg_,
      uint32_t num_out_seg_,
      double len_in_wire_,
      double len_out_wire_,
      const TechParameter* tech_param_ptr_
    );
    ~MatrixCrossbar();

  public:
    double get_dynamic_energy(bool is_max_) const;

  private:
    void init();
    double calc_i_static();

  private:
    double m_len_in_wire;
    double m_len_out_wire;
};

#endif

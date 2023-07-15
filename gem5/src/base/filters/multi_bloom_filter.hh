/*
 * Copyright (c) 2019 Inria
 * Copyright (c) 1999-2008 Mark D. Hill and David A. Wood
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
 */

#ifndef __BASE_FILTERS_MULTI_BLOOM_FILTER_HH__
#define __BASE_FILTERS_MULTI_BLOOM_FILTER_HH__

#include <vector>

#include "base/filters/base.hh"

struct BloomFilterMultiParams;

namespace BloomFilter {

/**
 * This BloomFilter has multiple sub-filters, each with its own hashing
 * functionality. The results of the operations are the results of applying
 * them to each sub-filter.
 */
class Multi : public Base
{
  public:
    Multi(const BloomFilterMultiParams* p);
    ~Multi();

    void clear() override;
    void set(Addr addr) override;
    void unset(Addr addr) override;

    void merge(const Base* other) override;
    bool isSet(Addr addr) const override;
    int getCount(Addr addr) const override;
    int getTotalCount() const override;

  private:
    /** Sub-filters used by this filter. */
    std::vector<Base*> filters;
};

} // namespace BloomFilter

#endif // __BASE_FILTERS_MULTI_BLOOM_FILTER_HH__

/*
 * Copyright (c) 2003-2005 The Regents of The University of Michigan
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
 * Authors: Steve Reinhardt
 *          Nathan Binkert
 */

#ifndef __BASE_BITFIELD_HH__
#define __BASE_BITFIELD_HH__

#include "base/types.hh"

/**
 * Generate a 64-bit mask of 'nbits' 1s, right justified.
 */
inline uint64_t
mask(int nbits)
{
    return (nbits == 64) ? (uint64_t)-1LL : (1ULL << nbits) - 1;
}



/**
 * Extract the bitfield from position 'first' to 'last' (inclusive)
 * from 'val' and right justify it.  MSB is numbered 63, LSB is 0.
 */
template <class T>
inline
T
bits(T val, int first, int last)
{
    int nbits = first - last + 1;
    return (val >> last) & mask(nbits);
}

/**
 * Extract the bit from this position from 'val' and right justify it.
 */
template <class T>
inline
T
bits(T val, int bit)
{
    return bits(val, bit, bit);
}

/**
 * Mask off the given bits in place like bits() but without shifting.
 * msb = 63, lsb = 0
 */
template <class T>
inline
T
mbits(T val, int first, int last)
{
    return val & (mask(first+1) & ~mask(last));
}

inline uint64_t
mask(int first, int last)
{
    return mbits((uint64_t)-1LL, first, last);
}

/**
 * Sign-extend an N-bit value to 64 bits.
 */
template <int N>
inline
int64_t
sext(uint64_t val)
{
    int sign_bit = bits(val, N-1, N-1);
    return sign_bit ? (val | ~mask(N)) : val;
}

/**
 * Return val with bits first to last set to bit_val
 */
template <class T, class B>
inline
T
insertBits(T val, int first, int last, B bit_val)
{
    T t_bit_val = bit_val;
    T bmask = mask(first - last + 1) << last;
    return ((t_bit_val << last) & bmask) | (val & ~bmask);
}

/**
 * Overloaded for access to only one bit in value
 */
template <class T, class B>
inline
T
insertBits(T val, int bit, B bit_val)
{
    return insertBits(val, bit, bit, bit_val);
}

/**
 * A convenience function to replace bits first to last of val with bit_val
 * in place.
 */
template <class T, class B>
inline
void
replaceBits(T& val, int first, int last, B bit_val)
{
    val = insertBits(val, first, last, bit_val);
}

/** Overloaded function to allow to access only 1 bit*/
template <class T, class B>
inline
void
replaceBits(T& val, int bit, B bit_val)
{
    val = insertBits(val, bit, bit, bit_val);
}
/**
 * Returns the bit position of the MSB that is set in the input
 */
inline
int
findMsbSet(uint64_t val) {
    int msb = 0;
    if (!val)
        return 0;
    if (bits(val, 63,32)) { msb += 32; val >>= 32; }
    if (bits(val, 31,16)) { msb += 16; val >>= 16; }
    if (bits(val, 15,8))  { msb += 8;  val >>= 8;  }
    if (bits(val, 7,4))   { msb += 4;  val >>= 4;  }
    if (bits(val, 3,2))   { msb += 2;  val >>= 2;  }
    if (bits(val, 1,1))   { msb += 1; }
    return msb;
}

/**
 * Returns the bit position of the LSB that is set in the input
 */
inline int
findLsbSet(uint64_t val) {
    int lsb = 0;
    if (!val)
        return sizeof(val) * 8;
    if (!bits(val, 31,0)) { lsb += 32; val >>= 32; }
    if (!bits(val, 15,0)) { lsb += 16; val >>= 16; }
    if (!bits(val, 7,0))  { lsb += 8;  val >>= 8;  }
    if (!bits(val, 3,0))  { lsb += 4;  val >>= 4;  }
    if (!bits(val, 1,0))  { lsb += 2;  val >>= 2;  }
    if (!bits(val, 0,0))  { lsb += 1; }
    return lsb;
}

#endif // __BASE_BITFIELD_HH__

/*  This file is part of the Vc library. {{{
Copyright © 2017 Matthias Kretz <kretz@kde.org>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the names of contributing organizations nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

}}}*/

#include "../common/macros.h"

#ifndef VC_NEON_MACROS_H_
#define VC_NEON_MACROS_H_

#ifndef _NEON128
# define _NEON128 float32x4_t
//# define _NEON128 float16x4_t
//# define _NEON128 float16x8_t
#endif

#ifndef _NEON128I
//# define _NEON128I int8x16_t
//# define _NEON128I int16x8_t
# define _NEON128I int32x4_t
//# define _NEON128I int64x2_t
//# define _NEON128I uint8x16_t
//# define _NEON128I uint16x8_t
//# define _NEON128I uint32x4_t
//# define _NEON128I uint64x2_t
//# define _NEON128I poly8x16_t
//# define _NEON128I poly16x8_t
#endif

#ifndef _NEON128D
# define _NEON128D float64x2_t
#endif

#endif // VC_NEON_MACROS_H_

/*  This file is part of the Vc library.

    Copyright (C) 2009-2010 Matthias Kretz <kretz@kde.org>

    Vc is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    Vc is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Vc.  If not, see <http://www.gnu.org/licenses/>.

*/

/* The log implementations are based on code from Julien Pommier which carries the following
   copyright information:
 */
/*
   Inspired by Intel Approximate Math library, and based on the
   corresponding algorithms of the cephes math library
*/
/* Copyright (C) 2007  Julien Pommier

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  (this is the zlib license)
*/

#ifndef AVX_VECTORHELPER_H
#define AVX_VECTORHELPER_H

#include "vectorbase.h"
#include <limits>

namespace Vc
{
namespace AVX
{
    template<typename T> struct GatherHelper
    {
        typedef VectorBase<T> Base;
        typedef typename Base::VectorType VectorType;
        typedef typename Base::EntryType  EntryType;
        typedef typename Base::IndexType  IndexType;
        typedef VectorMemoryUnion<VectorType, EntryType> UnionType;
        enum { Size = Base::Size, Shift = sizeof(EntryType) };
        static void gather(Base &v, const unsigned int *indexes, const EntryType *baseAddr);
        static void gather(Base &v, const IndexType &indexes, const EntryType *baseAddr);
        template<typename S1> static void gather(Base &v, const IndexType &indexes, const S1 *baseAddr,
                const EntryType S1::* member1);
        template<typename S1, typename S2> static void gather(Base &v, const IndexType &indexes,
                const S1 *baseAddr, const S2 S1::* member1, const EntryType S2::* member2);
    };

    template<typename VectorType, unsigned int Size> struct SortHelper
    {
        static VectorType sort(VectorType);
    };

    template<typename T> struct ScatterHelper
    {
        typedef VectorBase<T> Base;
        typedef typename Base::VectorType VectorType;
        typedef typename Base::EntryType  EntryType;
        typedef typename Base::IndexType  IndexType;
        typedef VectorMemoryUnion<VectorType, EntryType> UnionType;
        enum { Size = Base::Size, Shift = sizeof(EntryType) };

        static void scatter(const Base &v, const IndexType &indexes, EntryType *baseAddr);

        static void scatter(const Base &v, const IndexType &indexes, int mask, EntryType *baseAddr);

        template<typename S1>
        static void scatter(const Base &v, const IndexType &indexes, S1 *baseAddr, EntryType S1::* member1);

        template<typename S1>
        static void scatter(const Base &v, const IndexType &indexes, int mask, S1 *baseAddr, EntryType S1::* member1);

        template<typename S1, typename S2>
        static void scatter(const Base &v, const IndexType &indexes, S1 *baseAddr, S2 S1::* member1, EntryType S2::* member2);

        template<typename S1, typename S2>
        static void scatter(const Base &v, const IndexType &indexes, int mask, S1 *baseAddr, S2 S1::* member1, EntryType S2::* member2);
    };

#undef OP_DECL
#undef PARENT_DATA
#undef PARENT_DATA_CONST

        template<typename T> struct CtorTypeHelper { typedef T Type; };
        template<> struct CtorTypeHelper<short> { typedef int Type; };
        template<> struct CtorTypeHelper<unsigned short> { typedef unsigned int Type; };
        template<> struct CtorTypeHelper<float> { typedef double Type; };

        template<typename T> struct ExpandTypeHelper { typedef T Type; };
        template<> struct ExpandTypeHelper<short> { typedef int Type; };
        template<> struct ExpandTypeHelper<unsigned short> { typedef unsigned int Type; };
        template<> struct ExpandTypeHelper<float> { typedef double Type; };

#define OP0(name, code) static inline VectorType name() { return code; }
#define OP1(name, code) static inline VectorType name(const VectorType &a) { return code; }
#define OP2(name, code) static inline VectorType name(const VectorType &a, const VectorType &b) { return code; }
#define OP3(name, code) static inline VectorType name(const VectorType &a, const VectorType &b, const VectorType &c) { return code; }

        template<> struct VectorHelper<_M256>
        {
            typedef _M256 VectorType;
            static inline VectorType load(const float *x) { return _mm256_load_ps(x); }
            static inline VectorType loadUnaligned(const float *x) { return _mm256_loadu_ps(x); }
            static inline void store(float *mem, const VectorType &x) { _mm256_store_ps(mem, x); }
            static inline void storeUnaligned(float *mem, const VectorType &x) { _mm256_storeu_ps(mem, x); }
            static inline void storeUnaligned(float *mem, const VectorType &x, const VectorType &m) {
                _mm256_maskmoveu_si256(_mm256_castps_si256(x), _mm256_castps_si256(m), reinterpret_cast<char *>(mem));
            }
            static inline void storeStreaming(float *mem, const VectorType &x) { _mm256_stream_ps(mem, x); }
            OP0(allone, _mm256_setallone_ps())
            OP0(zero, _mm256_setzero_ps())
            OP2(or_, _mm256_or_ps(a, b))
            OP2(xor_, _mm256_xor_ps(a, b))
            OP2(and_, _mm256_and_ps(a, b))
            OP2(andnot_, _mm256_andnot_ps(a, b))
            OP3(blend, _mm256_blendv_ps(a, b, c))
        };

        template<> struct VectorHelper<_M256D>
        {
            typedef _M256D VectorType;
            static inline VectorType load(const double *x) { return _mm256_load_pd(x); }
            static inline VectorType loadUnaligned(const double *x) { return _mm256_loadu_pd(x); }
            static inline void store(double *mem, const VectorType &x) { _mm256_store_pd(mem, x); }
            static inline void storeUnaligned(double *mem, const VectorType &x) { _mm256_storeu_pd(mem, x); }
            static inline void storeUnaligned(double *mem, const VectorType &x, const VectorType &m) {
                _mm256_maskmoveu_si256(_mm256_castpd_si256(x), _mm256_castpd_si256(m), reinterpret_cast<char *>(mem));
            }
            static inline void storeStreaming(double *mem, const VectorType &x) { _mm256_stream_pd(mem, x); }
            OP0(allone, _mm256_setallone_pd())
            OP0(zero, _mm256_setzero_pd())
            OP2(or_, _mm256_or_pd(a, b))
            OP2(xor_, _mm256_xor_pd(a, b))
            OP2(and_, _mm256_and_pd(a, b))
            OP2(andnot_, _mm256_andnot_pd(a, b))
            OP3(blend, _mm256_blendv_pd(a, b, c))
        };

        template<> struct VectorHelper<_M256I>
        {
            typedef _M256I VectorType;
            template<typename T> static inline VectorType load(const T *x) { return _mm256_load_si256(reinterpret_cast<const VectorType *>(x)); }
            template<typename T> static inline VectorType loadUnaligned(const T *x) { return _mm256_loadu_si256(reinterpret_cast<const VectorType *>(x)); }
            template<typename T> static inline void store(T *mem, const VectorType &x) { _mm256_store_si256(reinterpret_cast<VectorType *>(mem), x); }
            template<typename T> static inline void storeUnaligned(T *mem, const VectorType &x) { _mm256_storeu_si256(reinterpret_cast<VectorType *>(mem), x); }
            template<typename T> static inline void storeUnaligned(T *mem, const VectorType &x, const VectorType &m) {
                _mm256_maskmoveu_si256(x, m, reinterpret_cast<char *>(mem));
            }
            template<typename T> static inline void storeStreaming(T *mem, const VectorType &x) { _mm256_stream_si256(reinterpret_cast<VectorType *>(mem), x); }
            OP0(allone, _mm256_setallone_si256())
            OP0(zero, _mm256_setzero_si256())
            OP2(or_, _mm256_or_si256(a, b))
            OP2(xor_, _mm256_xor_si256(a, b))
            OP2(and_, _mm256_and_si256(a, b))
            OP2(andnot_, _mm256_andnot_si256(a, b))
            OP3(blend, _mm256_blendv_epi8(a, b, c))
        };
#undef OP1
#undef OP2
#undef OP3

#define OP1(op) \
        static inline VectorType op(const VectorType &a) { return CAT(_mm256_##op##_, SUFFIX)(a); }
#define OP(op) \
        static inline VectorType op(const VectorType &a, const VectorType &b) { return CAT(_mm256_##op##_ , SUFFIX)(a, b); }
#define OP_(op) \
        static inline VectorType op(const VectorType &a, const VectorType &b) { return CAT(_mm256_##op    , SUFFIX)(a, b); }
#define OPx(op, op2) \
        static inline VectorType op(const VectorType &a, const VectorType &b) { return CAT(_mm256_##op2##_, SUFFIX)(a, b); }
#define OPcmp(op) \
        static inline VectorType cmp##op(const VectorType &a, const VectorType &b) { return CAT(_mm256_cmp##op##_, SUFFIX)(a, b); }
#define OP_CAST_(op) \
        static inline VectorType op(const VectorType &a, const VectorType &b) { return CAT(_mm256_castps_, SUFFIX)( \
            _mm256_##op##ps(CAT(CAT(_mm256_cast, SUFFIX), _ps)(a), \
              CAT(CAT(_mm256_cast, SUFFIX), _ps)(b))); \
        }
#define MINMAX \
        static inline VectorType min(VectorType a, VectorType b) { return CAT(_mm256_min_, SUFFIX)(a, b); } \
        static inline VectorType max(VectorType a, VectorType b) { return CAT(_mm256_max_, SUFFIX)(a, b); }

        // _mm256_sll_* does not take a count parameter with different counts per vector element. So we
        // have to do it manually
#define SHIFT4 \
            static inline VectorType sll(VectorType v, __m256i count) { \
                STORE_VECTOR(unsigned int, shifts, count); \
                union { __m256i v; unsigned int i[4]; } data; \
                _mm256_store_si256(&data.v, v); \
                data.i[0] <<= shifts[0]; \
                data.i[1] <<= shifts[1]; \
                data.i[2] <<= shifts[2]; \
                data.i[3] <<= shifts[3]; \
                return data.v; } \
            static inline VectorType slli(VectorType v, int count) { return CAT(_mm256_slli_, SUFFIX)(v, count); } \
            static inline VectorType srl(VectorType v, __m256i count) { \
                STORE_VECTOR(unsigned int, shifts, count); \
                union { __m256i v; unsigned int i[4]; } data; \
                _mm256_store_si256(&data.v, v); \
                data.i[0] >>= shifts[0]; \
                data.i[1] >>= shifts[1]; \
                data.i[2] >>= shifts[2]; \
                data.i[3] >>= shifts[3]; \
                return data.v; } \
            static inline VectorType srli(VectorType v, int count) { return CAT(_mm256_srli_, SUFFIX)(v, count); }
#define SHIFT8 \
            static inline VectorType sll(VectorType v, __m256i count) { \
                STORE_VECTOR(unsigned short, shifts, count); \
                union { __m256i v; unsigned short i[8]; } data; \
                _mm256_store_si256(&data.v, v); \
                data.i[0] <<= shifts[0]; \
                data.i[1] <<= shifts[1]; \
                data.i[2] <<= shifts[2]; \
                data.i[3] <<= shifts[3]; \
                data.i[4] <<= shifts[4]; \
                data.i[5] <<= shifts[5]; \
                data.i[6] <<= shifts[6]; \
                data.i[7] <<= shifts[7]; \
                return data.v; } \
            static inline VectorType slli(VectorType v, int count) { return CAT(_mm256_slli_, SUFFIX)(v, count); } \
            static inline VectorType srl(VectorType v, __m256i count) { \
                STORE_VECTOR(unsigned short, shifts, count); \
                union { __m256i v; unsigned short i[8]; } data; \
                _mm256_store_si256(&data.v, v); \
                data.i[0] >>= shifts[0]; \
                data.i[1] >>= shifts[1]; \
                data.i[2] >>= shifts[2]; \
                data.i[3] >>= shifts[3]; \
                data.i[4] >>= shifts[4]; \
                data.i[5] >>= shifts[5]; \
                data.i[6] >>= shifts[6]; \
                data.i[7] >>= shifts[7]; \
                return data.v; } \
            static inline VectorType srli(VectorType v, int count) { return CAT(_mm256_srli_, SUFFIX)(v, count); }

        template<> struct VectorHelper<double> {
            typedef _M256D VectorType;
            typedef double EntryType;
#define SUFFIX pd

            static inline VectorType notMaskedToZero(VectorType a, _M256 mask) { return CAT(_mm256_and_, SUFFIX)(_mm256_castps_pd(mask), a); }
            static inline VectorType set(const double a) { return CAT(_mm256_set1_, SUFFIX)(a); }
            static inline VectorType set(const double a, const double b) { return CAT(_mm256_set_, SUFFIX)(a, b); }
            static inline VectorType zero() { return CAT(_mm256_setzero_, SUFFIX)(); }
            static inline VectorType one()  { return CAT(_mm256_setone_, SUFFIX)(); }// set(1.); }

            static inline void multiplyAndAdd(VectorType &v1, VectorType v2, VectorType v3) { v1 = add(mul(v1, v2), v3); }
            static inline VectorType mul(VectorType a, VectorType b, _M256 _mask) {
                _M256D mask = _mm256_castps_pd(_mask);
                return _mm256_or_pd(
                    _mm256_and_pd(mask, _mm256_mul_pd(a, b)),
                    _mm256_andnot_pd(mask, a)
                    );
            }
            static inline VectorType div(VectorType a, VectorType b, _M256 _mask) {
                _M256D mask = _mm256_castps_pd(_mask);
                return _mm256_or_pd(
                    _mm256_and_pd(mask, _mm256_div_pd(a, b)),
                    _mm256_andnot_pd(mask, a)
                    );
            }

            OP(add) OP(sub) OP(mul) OP(div)
            OPcmp(eq) OPcmp(neq)
            OPcmp(lt) OPcmp(nlt)
            OPcmp(le) OPcmp(nle)

            OP1(sqrt)
            static inline VectorType rsqrt(VectorType x) {
                return _mm256_div_pd(one(), sqrt(x));
            }
            static inline VectorType negate(VectorType x) {
                return _mm256_xor_pd(x, _mm256_setsignmask_pd());
            }
            static inline VectorType reciprocal(VectorType x) {
                return _mm256_div_pd(one(), x);
            }
            static inline VectorType isNaN(VectorType x) {
                return _mm256_cmpunord_pd(x, x);
            }
            static inline VectorType isFinite(VectorType x) {
                return _mm256_cmpord_pd(x, _mm256_mul_pd(zero(), x));
            }
            static VectorType log(VectorType x) {
                const _M256D one = set(1.);
                const _M256D invalid_mask = cmplt(x, zero());
                const _M256D infinity_mask = cmpeq(x, zero());

                x = max(x, set(std::numeric_limits<double>::min()));  // lazy: cut off denormalized numbers

                _M256I emm0 = _mm256_srli_epi64(_mm256_castpd_si256(x), 52);
                emm0 = _mm256_sub_epi32(emm0, _mm256_set1_epi32(1023));
                _M256D e = _mm256_cvtepi32_pd(_mm256_shuffle_epi32(emm0, _MM_SHUFFLE(2, 0, 2, 0)));
                e = add(e, one);

                // keep only the fractional part
                const union {
                    unsigned long long int i;
                    double d;
                } mantissa_mask = { 0x800fffffffffffffull };
                x = _mm256_and_pd(x, set(mantissa_mask.d));
                x = _mm256_or_pd(x, set(0.5));

                const _M256D mask = cmplt(x, set(0.70710678118654757273731092936941422522068023681640625));

                const _M256D tmp = _mm256_and_pd(x, mask);
                x = sub(x, one);
                x = add(x, tmp);

                e = sub(e, _mm256_and_pd(one, mask));

                const _M256D z = mul(x, x);

                static const union {
                    unsigned short s[6 * 4];
                    double d[6];
                } P = { {
                    0x1bb0,0x93c3,0xb4c2,0x3f1a,
                    0x52f2,0x3f56,0xd6f5,0x3fdf,
                    0x6911,0xed92,0xd2ba,0x4012,
                    0xeb2e,0xc63e,0xff72,0x402c,
                    0xc84d,0x924b,0xefd6,0x4031,
                    0xdcf8,0x7d7e,0xd563,0x401e
                } };
                static const union {
                    unsigned short s[5 * 4];
                    double d[5];
                } Q = { {
                    0xef8e,0xae97,0x9320,0x4026,
                    0xc033,0x4e19,0x9d2c,0x4046,
                    0xbdbd,0xa326,0xbf33,0x4054,
                    0xae21,0xeb5e,0xc9e2,0x4051,
                    0x25b2,0x9e1f,0x200a,0x4037
                } };

                _M256D y = set(P.d[0]);
                for (int i = 1; i < 6; ++i) {
                    y = add(mul(y, x), set(P.d[i]));
                }
                _M256D y2 = add(set(Q.d[0]), x);
                for (int i = 1; i < 5; ++i) {
                    y2 = add(mul(y2, x), set(Q.d[i]));
                }
                y = mul(y, x);
                y = div(y, y2);

                y = mul(y, z);
                y = sub(y, mul(e, set(2.121944400546905827679e-4)));
                y = sub(y, mul(z, set(0.5)));

                x = add(x, y);
                x = add(x, mul(e, set(0.693359375)));
                x = _mm256_or_pd(x, invalid_mask); // negative arg will be NAN
                x = _mm256_or_pd(_mm256_andnot_pd(infinity_mask, x), _mm256_and_pd(infinity_mask, set(-std::numeric_limits<double>::infinity())));
                return x;
            }
            static inline VectorType abs(const VectorType a) {
                return CAT(_mm256_and_, SUFFIX)(a, _mm256_setabsmask_pd());
            }

            MINMAX
            static inline EntryType min(VectorType a) {
                a = _mm256_min_sd(a, _mm256_unpackhi_pd(a, a));
                return _mm256_cvtsd_f64(a);
            }
            static inline EntryType max(VectorType a) {
                a = _mm256_max_sd(a, _mm256_unpackhi_pd(a, a));
                return _mm256_cvtsd_f64(a);
            }
            static inline EntryType mul(VectorType a) {
                a = _mm256_mul_sd(a, _mm256_shuffle_pd(a, a, _MM_SHUFFLE2(0, 1)));
                return _mm256_cvtsd_f64(a);
            }
            static inline EntryType add(VectorType a) {
                a = _mm256_add_sd(a, _mm256_shuffle_pd(a, a, _MM_SHUFFLE2(0, 1)));
                return _mm256_cvtsd_f64(a);
            }
#undef SUFFIX
            static inline VectorType round(VectorType a) {
#if VC_IMPL_AVX4_1
                return _mm256_round_pd(a, _MM_FROUND_NINT);
#else
                //XXX: slow: _MM_SET_ROUNDING_MODE(_MM_ROUND_NEAREST);
                return _mm256_cvtepi32_pd(_mm256_cvtpd_epi32(a));
#endif
            }
        };

        template<> struct VectorHelper<float> {
            typedef float EntryType;
            typedef _M256 VectorType;
#define SUFFIX ps

            static inline VectorType notMaskedToZero(VectorType a, _M256 mask) { return CAT(_mm256_and_, SUFFIX)(mask, a); }
            static inline VectorType set(const float a) { return CAT(_mm256_set1_, SUFFIX)(a); }
            static inline VectorType set(const float a, const float b, const float c, const float d) { return CAT(_mm256_set_, SUFFIX)(a, b, c, d); }
            static inline VectorType zero() { return CAT(_mm256_setzero_, SUFFIX)(); }
            static inline VectorType one()  { return CAT(_mm256_setone_, SUFFIX)(); }// set(1.f); }
            static inline _M256 concat(_M256D a, _M256D b) { return _mm256_movelh_ps(_mm256_cvtpd_ps(a), _mm256_cvtpd_ps(b)); }

            static inline void multiplyAndAdd(VectorType &v1, VectorType v2, VectorType v3) { v1 = add(mul(v1, v2), v3); }
            static inline VectorType mul(VectorType a, VectorType b, _M256 mask) {
                return _mm256_or_ps(
                    _mm256_and_ps(mask, _mm256_mul_ps(a, b)),
                    _mm256_andnot_ps(mask, a)
                    );
            }
            static inline VectorType div(VectorType a, VectorType b, _M256 mask) {
                return _mm256_or_ps(
                    _mm256_and_ps(mask, _mm256_div_ps(a, b)),
                    _mm256_andnot_ps(mask, a)
                    );
            }

            OP(add) OP(sub) OP(mul) OP(div)
            OPcmp(eq) OPcmp(neq)
            OPcmp(lt) OPcmp(nlt)
            OPcmp(le) OPcmp(nle)

            OP1(sqrt) OP1(rsqrt)
            static inline VectorType isNaN(VectorType x) {
                return _mm256_cmpunord_ps(x, x);
            }
            static inline VectorType isFinite(VectorType x) {
                return _mm256_cmpord_ps(x, _mm256_mul_ps(zero(), x));
            }
            static inline VectorType reciprocal(VectorType x) {
                return _mm256_rcp_ps(x);
            }
            static inline VectorType negate(VectorType x) {
                return _mm256_xor_ps(x, _mm256_setsignmask_ps());
            }
            static VectorType log(VectorType x) {
                const _M256 one = set(1.);
                const _M256 invalid_mask = cmplt(x, zero());
                const _M256 infinity_mask = cmpeq(x, zero());

                x = max(x, set(std::numeric_limits<float>::min()));  // cut off denormalized stuff

                const _M256I emm0 = _mm256_srli_epi32(_mm256_castps_si256(x), 23);
                _M256 e = _mm256_cvtepi32_ps(_mm256_sub_epi32(emm0, _mm256_set1_epi32(127)));
                e = add(e, one);

                // keep only the fractional part
                const union {
                    unsigned int i;
                    float f;
                } mantissa_mask = { 0x807fffff };
                x = _mm256_and_ps(x, set(mantissa_mask.f));
                x = _mm256_or_ps(x, set(0.5));

                const _M256 mask = cmplt(x, set(0.707106781186547524f));

                const _M256 tmp = _mm256_and_ps(x, mask);
                x = sub(x, one);
                x = add(x, tmp);

                e = sub(e, _mm256_and_ps(one, mask));

                const _M256 z = mul(x, x);

                _M256 y = set( 7.0376836292e-2f);
                y = mul(y, x);
                y = add(y, set(-1.1514610310e-1f));
                y = mul(y, x);
                y = add(y, set( 1.1676998740e-1f));
                y = mul(y, x);
                y = add(y, set(-1.2420140846e-1f));
                y = mul(y, x);
                y = add(y, set( 1.4249322787e-1f));
                y = mul(y, x);
                y = add(y, set(-1.6668057665e-1f));
                y = mul(y, x);
                y = add(y, set( 2.0000714765e-1f));
                y = mul(y, x);
                y = add(y, set(-2.4999993993e-1f));
                y = mul(y, x);
                y = add(y, set( 3.3333331174e-1f));
                y = mul(y, x);

                y = mul(y, z);
                y = add(y, mul(e, set(-2.12194440e-4f)));
                y = sub(y, mul(z, set(0.5)));

                x = add(x, y);
                x = add(x, mul(e, set(0.693359375f)));
                x = _mm256_or_ps(x, invalid_mask); // negative arg will be NAN
                x = _mm256_or_ps(_mm256_andnot_ps(infinity_mask, x), _mm256_and_ps(infinity_mask, set(-std::numeric_limits<float>::infinity())));
                return x;
            }
            static inline VectorType abs(const VectorType a) {
                return CAT(_mm256_and_, SUFFIX)(a, _mm256_setabsmask_ps());
            }

            MINMAX
            static inline EntryType min(VectorType a) {
                a = _mm256_min_ps(a, _mm256_movehl_ps(a, a));   // a = min(a0, a2), min(a1, a3), min(a2, a2), min(a3, a3)
                a = _mm256_min_ss(a, _mm256_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1))); // a = min(a0, a1), a1, a2, a3
                return _mm256_cvtss_f32(a);
            }
            static inline EntryType max(VectorType a) {
                a = _mm256_max_ps(a, _mm256_movehl_ps(a, a));   // a = max(a0, a2), max(a1, a3), max(a2, a2), max(a3, a3)
                a = _mm256_max_ss(a, _mm256_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1))); // a = max(a0, a1), a1, a2, a3
                return _mm256_cvtss_f32(a);
            }
            static inline EntryType mul(VectorType a) {
                a = _mm256_mul_ps(a, _mm256_shuffle_ps(a, a, _MM_SHUFFLE(0, 1, 2, 3)));
                a = _mm256_mul_ss(a, _mm256_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 0, 1)));
                return _mm256_cvtss_f32(a);
            }
            static inline EntryType add(VectorType a) {
                a = _mm256_add_ps(a, _mm256_shuffle_ps(a, a, _MM_SHUFFLE(0, 1, 2, 3)));
                a = _mm256_add_ss(a, _mm256_shuffle_ps(a, a, _MM_SHUFFLE(3, 2, 0, 1)));
                return _mm256_cvtss_f32(a);
            }
#undef SUFFIX
            static inline VectorType round(VectorType a) {
#if VC_IMPL_AVX4_1
                return _mm256_round_ps(a, _MM_FROUND_NINT);
#else
                //XXX slow: _MM_SET_ROUNDING_MODE(_MM_ROUND_NEAREST);
                return _mm256_cvtepi32_ps(_mm256_cvtps_epi32(a));
#endif
            }
        };

        template<> struct VectorHelper<int> {
            typedef int EntryType;
            typedef _M256I VectorType;
#define SUFFIX si256

            OP_(or_) OP_(and_) OP_(xor_)
            static inline VectorType zero() { return CAT(_mm256_setzero_, SUFFIX)(); }
            static inline VectorType notMaskedToZero(VectorType a, _M256 mask) { return CAT(_mm256_and_, SUFFIX)(_mm256_castps_si256(mask), a); }
#undef SUFFIX
#define SUFFIX epi32
            static inline VectorType one() { return CAT(_mm256_setone_, SUFFIX)(); }

            static inline VectorType set(const int a) { return CAT(_mm256_set1_, SUFFIX)(a); }
            static inline VectorType set(const int a, const int b, const int c, const int d) { return CAT(_mm256_set_, SUFFIX)(a, b, c, d); }

            static inline void multiplyAndAdd(VectorType &v1, VectorType v2, VectorType v3) { v1 = add(mul(v1, v2), v3); }

            SHIFT4


            OP1(abs)

            MINMAX
            static inline EntryType min(VectorType a) {
                a = min(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                // using lo_epi16 for speed here
                a = min(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                return _mm256_cvtsi256_si32(a);
            }
            static inline EntryType max(VectorType a) {
                a = max(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                // using lo_epi16 for speed here
                a = max(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                return _mm256_cvtsi256_si32(a);
            }
            static inline EntryType add(VectorType a) {
                a = add(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = add(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                return _mm256_cvtsi256_si32(a);
            }
#if VC_IMPL_AVX4_1
            static inline VectorType mul(VectorType a, VectorType b) { return _mm256_mullo_epi32(a, b); }
            static inline EntryType mul(VectorType a) {
                a = mul(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = mul(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                return _mm256_cvtsi256_si32(a);
            }
#else
            static inline VectorType mul(const VectorType &a, const VectorType &b) {
                const VectorType &aShift = _mm256_srli_si256(a, 4);
                const VectorType &ab02 = _mm256_mul_epu32(a, b); // [a0 * b0, a2 * b2]
                const VectorType &bShift = _mm256_srli_si256(b, 4);
                const VectorType &ab13 = _mm256_mul_epu32(aShift, bShift); // [a1 * b1, a3 * b3]
                return _mm256_unpacklo_epi32(_mm256_shuffle_epi32(ab02, 8), _mm256_shuffle_epi32(ab13, 8));
            }
            static inline EntryType mul(VectorType a) {
                STORE_VECTOR(int, _a, a);
                return _a[0] * _a[1] * _a[2] * _a[3];
            }
#endif
            static inline VectorType mul(const VectorType a, const VectorType b, _M256 _mask) {
                return _mm256_blendv_epi8(a, mul(a, b), _mm256_castps_si256(_mask));
            }

            static inline VectorType div(const VectorType a, const VectorType b, _M256 _mask) {
                const int mask = _mm256_movemask_ps(_mask);
                STORE_VECTOR(int, _a, a);
                STORE_VECTOR(int, _b, b);
                union {
                    int i[4];
                    VectorType v;
                } x = { {
                    (mask & 1 ? _a[0] / _b[0] : _a[0]),
                    (mask & 2 ? _a[1] / _b[1] : _a[1]),
                    (mask & 4 ? _a[2] / _b[2] : _a[2]),
                    (mask & 8 ? _a[3] / _b[3] : _a[3])
                } };
                return x.v;
            }
            static inline VectorType div(const VectorType a, const VectorType b) {
                STORE_VECTOR(int, _a, a);
                STORE_VECTOR(int, _b, b);
                union {
                    int i[4];
                    VectorType v;
                } x = { {
                    _a[0] / _b[0],
                    _a[1] / _b[1],
                    _a[2] / _b[2],
                    _a[3] / _b[3]
                } };
                return x.v;
            }

            OP(add) OP(sub)
            OPcmp(eq)
            OPcmp(lt)
            OPcmp(gt)
            static inline VectorType cmpneq(const VectorType &a, const VectorType &b) { _M256I x = cmpeq(a, b); return _mm256_andnot_si256(x, _mm256_setallone_si256()); }
            static inline VectorType cmpnlt(const VectorType &a, const VectorType &b) { _M256I x = cmplt(a, b); return _mm256_andnot_si256(x, _mm256_setallone_si256()); }
            static inline VectorType cmple (const VectorType &a, const VectorType &b) { _M256I x = cmpgt(a, b); return _mm256_andnot_si256(x, _mm256_setallone_si256()); }
            static inline VectorType cmpnle(const VectorType &a, const VectorType &b) { return cmpgt(a, b); }
#undef SUFFIX
            static inline VectorType round(VectorType a) { return a; }
        };

        template<> struct VectorHelper<unsigned int> {
            typedef unsigned int EntryType;
            typedef _M256I VectorType;
#define SUFFIX si256
            OP_CAST_(or_) OP_CAST_(and_) OP_CAST_(xor_)
            static inline VectorType zero() { return CAT(_mm256_setzero_, SUFFIX)(); }
            static inline VectorType notMaskedToZero(VectorType a, _M256 mask) { return CAT(_mm256_and_, SUFFIX)(_mm256_castps_si256(mask), a); }

#undef SUFFIX
#define SUFFIX epu32
            static inline VectorType one() { return CAT(_mm256_setone_, SUFFIX)(); }

            MINMAX
            static inline EntryType min(VectorType a) {
                a = min(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                // using lo_epi16 for speed here
                a = min(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                return _mm256_cvtsi256_si32(a);
            }
            static inline EntryType max(VectorType a) {
                a = max(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                // using lo_epi16 for speed here
                a = max(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                return _mm256_cvtsi256_si32(a);
            }
            static inline EntryType mul(VectorType a) {
                a = mul(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                // using lo_epi16 for speed here
                a = mul(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                return _mm256_cvtsi256_si32(a);
            }
            static inline EntryType add(VectorType a) {
                a = add(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                // using lo_epi16 for speed here
                a = add(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                return _mm256_cvtsi256_si32(a);
            }

            static inline VectorType mul(const VectorType a, const VectorType b, _M256 _mask) {
                return _mm256_blendv_epi8(a, mul(a, b), _mm256_castps_si256(_mask));
            }
            static inline VectorType mul(const VectorType &a, const VectorType &b) {
                return VectorHelper<int>::mul(a, b);
            }
//X             template<unsigned int b> static inline VectorType mul(const VectorType a) {
//X                 switch (b) {
//X                     case    0: return zero();
//X                     case    1: return a;
//X                     case    2: return _mm256_slli_epi32(a,  1);
//X                     case    4: return _mm256_slli_epi32(a,  2);
//X                     case    8: return _mm256_slli_epi32(a,  3);
//X                     case   16: return _mm256_slli_epi32(a,  4);
//X                     case   32: return _mm256_slli_epi32(a,  5);
//X                     case   64: return _mm256_slli_epi32(a,  6);
//X                     case  128: return _mm256_slli_epi32(a,  7);
//X                     case  256: return _mm256_slli_epi32(a,  8);
//X                     case  512: return _mm256_slli_epi32(a,  9);
//X                     case 1024: return _mm256_slli_epi32(a, 10);
//X                     case 2048: return _mm256_slli_epi32(a, 11);
//X                 }
//X                 return mul(a, set(b));
//X             }
            static inline VectorType div(const VectorType &_a, const VectorType &_b, const _M256 _mask) {
                const int mask = _mm256_movemask_ps(_mask);
                VectorType _r = _a;
                typedef unsigned int uintA MAY_ALIAS;
                const uintA *b = reinterpret_cast<const uintA *>(&_b);
                uintA *r = reinterpret_cast<uintA *>(&_r);
                unrolled_loop16(i, 0, 4,
                    if (mask & (1 << i)) r[i] /= b[i];
                    );
                return _r;
            }
            static inline VectorType div(const VectorType &_a, const VectorType &_b) {
                VectorType _r;
                typedef unsigned int uintA MAY_ALIAS;
                const uintA *a = reinterpret_cast<const uintA *>(&_a);
                const uintA *b = reinterpret_cast<const uintA *>(&_b);
                uintA *r = reinterpret_cast<uintA *>(&_r);
                unrolled_loop16(i, 0, 4,
                    r[i] = a[i] / b[i];
                    );
                return _r;
            }

#undef SUFFIX
#define SUFFIX epi32
            static inline VectorType set(const unsigned int a) { return CAT(_mm256_set1_, SUFFIX)(a); }
            static inline VectorType set(const unsigned int a, const unsigned int b, const unsigned int c, const unsigned int d) { return CAT(_mm256_set_, SUFFIX)(a, b, c, d); }

            SHIFT4
            OP(add) OP(sub)
            OPcmp(eq)
            static inline VectorType cmpneq(const VectorType &a, const VectorType &b) { return _mm256_andnot_si256(cmpeq(a, b), _mm256_setallone_si256()); }

#ifndef USE_INCORRECT_UNSIGNED_COMPARE
            static inline VectorType cmplt(const VectorType &a, const VectorType &b) {
                return _mm256_cmplt_epu32(a, b);
            }
            static inline VectorType cmpgt(const VectorType &a, const VectorType &b) {
                return _mm256_cmpgt_epu32(a, b);
            }
#else
            OPcmp(lt)
            OPcmp(gt)
#endif
            static inline VectorType cmpnlt(const VectorType &a, const VectorType &b) { return _mm256_andnot_si256(cmplt(a, b), _mm256_setallone_si256()); }
            static inline VectorType cmple (const VectorType &a, const VectorType &b) { return _mm256_andnot_si256(cmpgt(a, b), _mm256_setallone_si256()); }
            static inline VectorType cmpnle(const VectorType &a, const VectorType &b) { return cmpgt(a, b); }

#undef SUFFIX
            static inline VectorType round(VectorType a) { return a; }
        };

        template<> struct VectorHelper<signed short> {
            typedef _M256I VectorType;
            typedef signed short EntryType;
#define SUFFIX si256

            OP_(or_) OP_(and_) OP_(xor_)
            static inline VectorType zero() { return CAT(_mm256_setzero_, SUFFIX)(); }
            static inline VectorType notMaskedToZero(VectorType a, _M256 mask) { return CAT(_mm256_and_, SUFFIX)(_mm256_castps_si256(mask), a); }
            static inline _M256I concat(_M256I a, _M256I b) { return _mm256_packs_epi32(a, b); }
            static inline _M256I expand0(_M256I x) { return _mm256_srai_epi32(_mm256_unpacklo_epi16(x, x), 16); }
            static inline _M256I expand1(_M256I x) { return _mm256_srai_epi32(_mm256_unpackhi_epi16(x, x), 16); }

#undef SUFFIX
#define SUFFIX epi16
            static inline VectorType one() { return CAT(_mm256_setone_, SUFFIX)(); }
            SHIFT8

            static inline VectorType set(const EntryType a) { return CAT(_mm256_set1_, SUFFIX)(a); }
            static inline VectorType set(const EntryType a, const EntryType b, const EntryType c, const EntryType d,
                    const EntryType e, const EntryType f, const EntryType g, const EntryType h) {
                return CAT(_mm256_set_, SUFFIX)(a, b, c, d, e, f, g, h);
            }

            static inline void multiplyAndAdd(VectorType &v1, VectorType v2, VectorType v3) {
                v1 = add(mul(v1, v2), v3); }

            OP1(abs)

            static inline VectorType mul(VectorType a, VectorType b, _M256 _mask) {
                _M256I mask = _mm256_castps_si256(_mask);
                return _mm256_or_si256(
                    _mm256_and_si256(mask, mul(a, b)),
                    _mm256_andnot_si256(mask, a)
                    );
            }
            OPx(mul, mullo)
            OP(min) OP(max)
            static inline EntryType min(VectorType a) {
                // reminder: _MM_SHUFFLE(3, 2, 1, 0) means "no change"
                a = min(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = min(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = min(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm256_cvtsi256_si32(a); // & 0xffff is implicit
            }
            static inline EntryType max(VectorType a) {
                // reminder: _MM_SHUFFLE(3, 2, 1, 0) means "no change"
                a = max(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = max(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = max(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm256_cvtsi256_si32(a); // & 0xffff is implicit
            }
            static inline EntryType mul(VectorType a) {
                a = mul(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = mul(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = mul(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm256_cvtsi256_si32(a); // & 0xffff is implicit
            }
            static inline EntryType add(VectorType a) {
                a = add(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = add(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = add(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm256_cvtsi256_si32(a); // & 0xffff is implicit
            }

            static inline VectorType div(const VectorType &a, const VectorType &b, const _M256 _mask) {
                const int mask = _mm256_movemask_epi8(_mm256_castps_si256(_mask));
                VectorType r = a;
                typedef EntryType Alias MAY_ALIAS;
                const Alias *bb = reinterpret_cast<const Alias *>(&b);
                Alias *rr = reinterpret_cast<Alias *>(&r);
                unrolled_loop16(i, 0, 8,
                    if (mask & (1 << i * 2)) rr[i] /= bb[i];
                    );
                return r;
            }
            static inline VectorType div(const VectorType &a, const VectorType &b) {
                VectorType r;
                typedef EntryType Alias MAY_ALIAS;
                const Alias *aa = reinterpret_cast<const Alias *>(&a);
                const Alias *bb = reinterpret_cast<const Alias *>(&b);
                Alias *rr = reinterpret_cast<Alias *>(&r);
                unrolled_loop16(i, 0, 8,
                    rr[i] = aa[i] / bb[i];
                    );
                return r;
            }

            OP(add) OP(sub)
            OPcmp(eq)
            OPcmp(lt)
            OPcmp(gt)
            static inline VectorType cmpneq(const VectorType &a, const VectorType &b) { _M256I x = cmpeq(a, b); return _mm256_andnot_si256(x, _mm256_setallone_si256()); }
            static inline VectorType cmpnlt(const VectorType &a, const VectorType &b) { _M256I x = cmplt(a, b); return _mm256_andnot_si256(x, _mm256_setallone_si256()); }
            static inline VectorType cmple (const VectorType &a, const VectorType &b) { _M256I x = cmpgt(a, b); return _mm256_andnot_si256(x, _mm256_setallone_si256()); }
            static inline VectorType cmpnle(const VectorType &a, const VectorType &b) { return cmpgt(a, b); }
#undef SUFFIX
            static inline VectorType round(VectorType a) { return a; }
        };

        template<> struct VectorHelper<unsigned short> {
            typedef _M256I VectorType;
            typedef unsigned short EntryType;
#define SUFFIX si256
            OP_CAST_(or_) OP_CAST_(and_) OP_CAST_(xor_)
            static inline VectorType zero() { return CAT(_mm256_setzero_, SUFFIX)(); }
            static inline VectorType notMaskedToZero(VectorType a, _M256 mask) { return CAT(_mm256_and_, SUFFIX)(_mm256_castps_si256(mask), a); }
#if VC_IMPL_AVX4_1
            static inline _M256I concat(_M256I a, _M256I b) { return _mm256_packus_epi32(a, b); }
#else
            // XXX too bad, but this is broken without AVX 4.1
            static inline _M256I concat(_M256I a, _M256I b) { return _mm256_packs_epi32(a, b); }
#endif
            static inline _M256I expand0(_M256I x) { return _mm256_srli_epi32(_mm256_unpacklo_epi16(x, x), 16); }
            static inline _M256I expand1(_M256I x) { return _mm256_srli_epi32(_mm256_unpackhi_epi16(x, x), 16); }

#undef SUFFIX
#define SUFFIX epu16
            static inline VectorType one() { return CAT(_mm256_setone_, SUFFIX)(); }
            static inline VectorType div(const VectorType &a, const VectorType &b, const _M256 _mask) {
                const int mask = _mm256_movemask_epi8(_mm256_castps_si256(_mask));
                VectorType r = a;
                typedef EntryType Alias MAY_ALIAS;
                const Alias *bb = reinterpret_cast<const Alias *>(&b);
                Alias *rr = reinterpret_cast<Alias *>(&r);
                unrolled_loop16(i, 0, 8,
                    if (mask & (1 << i * 2)) rr[i] /= bb[i];
                    );
                return r;
            }
            static inline VectorType div(const VectorType &a, const VectorType &b) {
                VectorType r;
                typedef EntryType Alias MAY_ALIAS;
                const Alias *aa = reinterpret_cast<const Alias *>(&a);
                const Alias *bb = reinterpret_cast<const Alias *>(&b);
                Alias *rr = reinterpret_cast<Alias *>(&r);
                unrolled_loop16(i, 0, 8,
                    rr[i] = aa[i] / bb[i];
                    );
                return r;
            }

            static inline VectorType mul(VectorType a, VectorType b, _M256 _mask) {
                _M256I mask = _mm256_castps_si256(_mask);
                return _mm256_or_si256(
                    _mm256_and_si256(mask, mul(a, b)),
                    _mm256_andnot_si256(mask, a)
                    );
            }
//X             template<unsigned int b> static inline VectorType mul(const VectorType a) {
//X                 switch (b) {
//X                     case    0: return zero();
//X                     case    1: return a;
//X                     case    2: return _mm256_slli_epi16(a,  1);
//X                     case    4: return _mm256_slli_epi16(a,  2);
//X                     case    8: return _mm256_slli_epi16(a,  3);
//X                     case   16: return _mm256_slli_epi16(a,  4);
//X                     case   32: return _mm256_slli_epi16(a,  5);
//X                     case   64: return _mm256_slli_epi16(a,  6);
//X                     case  128: return _mm256_slli_epi16(a,  7);
//X                     case  256: return _mm256_slli_epi16(a,  8);
//X                     case  512: return _mm256_slli_epi16(a,  9);
//X                     case 1024: return _mm256_slli_epi16(a, 10);
//X                     case 2048: return _mm256_slli_epi16(a, 11);
//X                 }
//X                 return mul(a, set(b));
//X             }
#if !defined(USE_INCORRECT_UNSIGNED_COMPARE) || VC_IMPL_AVX4_1
            OP(min) OP(max)
#endif
#undef SUFFIX
#define SUFFIX epi16
            SHIFT8
            OPx(mul, mullo) // should work correctly for all values
#if defined(USE_INCORRECT_UNSIGNED_COMPARE) && !defined(VC_IMPL_AVX4_1)
            OP(min) OP(max) // XXX breaks for values with MSB set
#endif
            static inline EntryType min(VectorType a) {
                // reminder: _MM_SHUFFLE(3, 2, 1, 0) means "no change"
                a = min(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = min(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = min(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm256_cvtsi256_si32(a); // & 0xffff is implicit
            }
            static inline EntryType max(VectorType a) {
                // reminder: _MM_SHUFFLE(3, 2, 1, 0) means "no change"
                a = max(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = max(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = max(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm256_cvtsi256_si32(a); // & 0xffff is implicit
            }
            static inline EntryType mul(VectorType a) {
                // reminder: _MM_SHUFFLE(3, 2, 1, 0) means "no change"
                a = mul(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = mul(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = mul(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm256_cvtsi256_si32(a); // & 0xffff is implicit
            }
            static inline EntryType add(VectorType a) {
                // reminder: _MM_SHUFFLE(3, 2, 1, 0) means "no change"
                a = add(a, _mm256_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = add(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = add(a, _mm256_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm256_cvtsi256_si32(a); // & 0xffff is implicit
            }
            static inline VectorType set(const EntryType a) { return CAT(_mm256_set1_, SUFFIX)(a); }
            static inline VectorType set(const EntryType a, const EntryType b, const EntryType c,
                    const EntryType d, const EntryType e, const EntryType f,
                    const EntryType g, const EntryType h) {
                return CAT(_mm256_set_, SUFFIX)(a, b, c, d, e, f, g, h);
            }

            OP(add) OP(sub)
            OPcmp(eq)
            static inline VectorType cmpneq(const VectorType &a, const VectorType &b) { return _mm256_andnot_si256(cmpeq(a, b), _mm256_setallone_si256()); }

#ifndef USE_INCORRECT_UNSIGNED_COMPARE
            static inline VectorType cmplt(const VectorType &a, const VectorType &b) {
                return _mm256_cmplt_epu16(a, b);
            }
            static inline VectorType cmpgt(const VectorType &a, const VectorType &b) {
                return _mm256_cmpgt_epu16(a, b);
            }
#else
            OPcmp(lt)
            OPcmp(gt)
#endif
            static inline VectorType cmpnlt(const VectorType &a, const VectorType &b) { return _mm256_andnot_si256(cmplt(a, b), _mm256_setallone_si256()); }
            static inline VectorType cmple (const VectorType &a, const VectorType &b) { return _mm256_andnot_si256(cmpgt(a, b), _mm256_setallone_si256()); }
            static inline VectorType cmpnle(const VectorType &a, const VectorType &b) { return cmpgt(a, b); }
#undef SUFFIX
            static inline VectorType round(VectorType a) { return a; }
        };
#undef SHIFT4
#undef SHIFT8
#undef OP1
#undef OP
#undef OP_
#undef OPx
#undef OPcmp

} // namespace AVX
} // namespace Vc

#include "vectorhelper.tcc"

#endif // AVX_VECTORHELPER_H

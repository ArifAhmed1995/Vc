/*  This file is part of the Vc library. {{{
Copyright © 2014 Matthias Kretz <kretz@kde.org>
All rights reserved.

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

#ifndef VC_TRAITS_IS_FUNCTOR_ARGUMENT_IMMUTABLE_H_
#define VC_TRAITS_IS_FUNCTOR_ARGUMENT_IMMUTABLE_H_

namespace Vc_VERSIONED_NAMESPACE
{
namespace Traits
{
namespace is_functor_argument_immutable_impl
{
// this indirection for decltype is required for EDG based compilers
template <typename F, typename A> struct workaround_edg
{
#ifdef VC_NVCC
    // this is still not working for NVCC for some reason
    typedef decltype(&F::operator()) type;
#else
    typedef decltype(&F::template operator()<A>) type;
#endif
};

template <typename F, typename A,
          typename MemberPtr = typename workaround_edg<F, A>::type,
          typename = decltype((std::declval<F &>().*
                               (std::declval<MemberPtr>()))(std::declval<const A &>()))>
std::true_type test(int);
template <typename F, typename A> std::false_type test(...);

// CUDA's C++11 support is broken (v7.0) - this workaround replaces the previous alias template
// declaration for is_functor_argument_immutable
template<typename F, typename A>
struct nvcc_alias_template_workaround
{
    using type = decltype(test<F, A>(1));
};

}  // namespace is_functor_argument_immutable_impl

template <typename F, typename A>
using is_functor_argument_immutable = typename is_functor_argument_immutable_impl::nvcc_alias_template_workaround<F, A>::type;


}  // namespace Traits
}  // namespace Vc

#endif  // VC_TRAITS_IS_FUNCTOR_ARGUMENT_IMMUTABLE_H_

// vim: foldmethod=marker
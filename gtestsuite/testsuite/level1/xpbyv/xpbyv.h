/*

   BLIS
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
	- Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	- Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	- Neither the name(s) of the copyright holder(s) nor the names of its
	  contributors may be used to endorse or promote products derived
	  from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#pragma once

#include "blis.h"
#include "common/testing_helpers.h"

/**
 * @brief Performs the operation:
 *             y := beta * y + x or y := beta * y + conj(x) (BLIS_TYPED only)
 * @param[in] conjx denotes if x or conj(x) will be used for this operation
 * @param[in] n vector length of x and y
 * @param[in] x pointer which points to the first element of x
 * @param[in] incx increment of x
 * @param[in] beta scalar
 * @param[in, out] y pointer which points to the first element of y
 * @param[in] incy increment of y
 */

template<typename T>
static void typed_xpbyv(char conj_x, gtint_t n, T* x, gtint_t incx, T beta, T* y, gtint_t incy)
{
    conj_t conjx;
    // Map parameter characters to BLIS constants.
    testinghelpers::char_to_blis_conj( conj_x, &conjx );
    if constexpr (std::is_same<T, float>::value)
        bli_sxpbyv( conjx, n, x, incx, &beta, y, incy );
    else if constexpr (std::is_same<T, double>::value)
        bli_dxpbyv( conjx, n, x, incx, &beta, y, incy );
    else if constexpr (std::is_same<T, scomplex>::value)
        bli_cxpbyv( conjx, n, x, incx, &beta, y, incy );
    else if constexpr (std::is_same<T, dcomplex>::value)
        bli_zxpbyv( conjx, n, x, incx, &beta, y, incy );
    else
        throw std::runtime_error("Error in testsuite/level1/xpbyv.h: Invalid typename in typed_xpbyv().");
}

template<typename T>
static void xpbyv(char conj_x, gtint_t n, T* x, gtint_t incx, T beta, T* y, gtint_t incy)
{
#ifdef TEST_BLAS
    throw std::runtime_error("Error in testsuite/level1/xpbyv.h: BLAS interface is not available.");
#elif TEST_CBLAS
    throw std::runtime_error("Error in testsuite/level1/xpbyv.h: CBLAS interface is not available.");
#elif TEST_BLIS_TYPED
    typed_xpbyv<T>( conj_x, n, x, incx, beta, y, incy );
#else
    throw std::runtime_error("Error in testsuite/level1/xpbyv.h: No interfaces are set to be tested.");
#endif
}
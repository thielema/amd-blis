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

#include "syr.h"
#include "level2/ref_syr.h"
#include "inc/check_error.h"
#include <stdexcept>
#include <algorithm>

template<typename T>
void test_syr( char storage, char uploa, char conjx, gtint_t n, T alpha,
               gtint_t incx, gtint_t lda_inc, double thresh )
{
    // Compute the leading dimensions for matrix size calculation.
    gtint_t lda = testinghelpers::get_leading_dimension( storage, 'n', n, n, lda_inc );

    //----------------------------------------------------------
    //        Initialize matrics with random integer numbers.
    //----------------------------------------------------------
    std::vector<T> a = testinghelpers::get_random_matrix<T>( -2, 5, storage, 'n', n, n, lda );
    std::vector<T> x = testinghelpers::get_random_vector<T>( -3, 3, n, incx );

    testinghelpers::make_triangular<T>( storage, uploa, n, a.data(), lda );

    // Create a copy of c so that we can check reference results.
    std::vector<T> a_ref(a);
    //----------------------------------------------------------
    //                  Call BLIS function
    //----------------------------------------------------------
    syr<T>( storage, uploa, conjx, n, &alpha, x.data(), incx, a.data(), lda );

    //----------------------------------------------------------
    //                  Call reference implementation.
    //----------------------------------------------------------
    testinghelpers::ref_syr<T>( storage, uploa, conjx, n, alpha,
                                      x.data(), incx, a_ref.data(), lda );

    //----------------------------------------------------------
    //              check component-wise error.
    //----------------------------------------------------------
    computediff<T>( storage, n, n, a.data(), a_ref.data(), lda, thresh );
}
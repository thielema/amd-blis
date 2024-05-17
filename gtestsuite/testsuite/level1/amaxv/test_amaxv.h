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

#include "amaxv.h"
#include "level1/ref_amaxv.h"
#include "inc/check_error.h"

/**
 * @brief Generic test body for amaxv operation.
 */

template<typename T>
void test_amaxv( gtint_t n, gtint_t incx, double thresh )
{
    //----------------------------------------------------------
    //        Initialize vectors with random numbers.
    //----------------------------------------------------------
    std::vector<T> x = testinghelpers::get_random_vector<T>( -10, 10, n, incx );

    //----------------------------------------------------------
    //    Call reference implementation to get ref results.
    //----------------------------------------------------------
    gtint_t idx_ref = testinghelpers::ref_amaxv<T>( n, x.data(), incx );

    //----------------------------------------------------------
    //                  Call BLIS function.
    //----------------------------------------------------------
    gtint_t idx = amaxv<T>( n, x.data(), incx );

    //----------------------------------------------------------
    //              Compute component-wise error.
    //----------------------------------------------------------
    EXPECT_EQ( idx, idx_ref );
}
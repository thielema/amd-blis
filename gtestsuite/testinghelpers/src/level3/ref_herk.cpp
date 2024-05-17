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

#include "blis.h"
#include "level3/ref_herk.h"

namespace testinghelpers {

template <typename T, typename RT>
void ref_herk(
    char storage, char uplo, char trnsa,
    gtint_t m, gtint_t k,
    RT alpha,
    T* ap, gtint_t lda,
    RT beta,
    T* cp, gtint_t ldc
) {
    enum CBLAS_ORDER cblas_order;
    enum CBLAS_UPLO cblas_uplo;
    enum CBLAS_TRANSPOSE cblas_transa;

    char_to_cblas_order( storage, &cblas_order );
    char_to_cblas_uplo( uplo, &cblas_uplo );
    char_to_cblas_trans( trnsa, &cblas_transa );

    typedef void (*Fptr_ref_cblas_herk)( const CBLAS_ORDER, const CBLAS_UPLO, const CBLAS_TRANSPOSE,
                    const f77_int, const f77_int, const RT, const T*, f77_int,
                    const RT, T*, f77_int);
    Fptr_ref_cblas_herk ref_cblas_herk;

    // Call C function
    /* Check the typename T passed to this function template and call respective function.*/
    if (typeid(T) == typeid(scomplex))
    {
        ref_cblas_herk = (Fptr_ref_cblas_herk)refCBLASModule.loadSymbol("cblas_cherk");
    }
    else if (typeid(T) == typeid(dcomplex))
    {
        ref_cblas_herk = (Fptr_ref_cblas_herk)refCBLASModule.loadSymbol("cblas_zherk");
    }
    else
    {
        throw std::runtime_error("Error in ref_herk.cpp: Invalid typename is passed function template.");
    }
    if( !ref_cblas_herk ) {
        throw std::runtime_error("Error in ref_herk.cpp: Function pointer == 0 -- symbol not found.");
    }

    ref_cblas_herk( cblas_order, cblas_uplo, cblas_transa,
                  m, k, alpha, ap, lda, beta, cp, ldc );
}

// Explicit template instantiations
template void ref_herk<scomplex>(char, char, char, gtint_t, gtint_t, float,
                      scomplex*, gtint_t, float, scomplex*, gtint_t );
template void ref_herk<dcomplex>(char, char, char, gtint_t, gtint_t, double,
                      dcomplex*, gtint_t, double, dcomplex*, gtint_t );

} //end of namespace testinghelpers
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

#include <gtest/gtest.h>
#include "test_gemm.h"

class DGemmTest :
        public ::testing::TestWithParam<std::tuple<char,
                                                   char,
                                                   char,
                                                   gtint_t,
                                                   gtint_t,
                                                   gtint_t,
                                                   double,
                                                   double,
                                                   gtint_t,
                                                   gtint_t,
                                                   gtint_t>> {};

TEST_P(DGemmTest, RandomData)
{
    using T = double;
    //----------------------------------------------------------
    // Initialize values from the parameters passed through
    // test suite instantiation (INSTANTIATE_TEST_SUITE_P).
    //----------------------------------------------------------
    // matrix storage format(row major, column major)
    char storage = std::get<0>(GetParam());
    // denotes whether matrix a is n,c,t,h
    char transa = std::get<1>(GetParam());
    // denotes whether matrix b is n,c,t,h
    char transb = std::get<2>(GetParam());
    // matrix size m
    gtint_t m  = std::get<3>(GetParam());
    // matrix size n
    gtint_t n  = std::get<4>(GetParam());
    // matrix size k
    gtint_t k  = std::get<5>(GetParam());
    // specifies alpha value
    T alpha = std::get<6>(GetParam());
    // specifies beta value
    T beta = std::get<7>(GetParam());
    // lda, ldb, ldc increments.
    // If increments are zero, then the array size matches the matrix size.
    // If increments are nonnegative, the array size is bigger than the matrix size.
    gtint_t lda_inc = std::get<8>(GetParam());
    gtint_t ldb_inc = std::get<9>(GetParam());
    gtint_t ldc_inc = std::get<10>(GetParam());

    // Set the threshold for the errors:
    double thresh = 10*m*n*testinghelpers::getEpsilon<T>();

    //----------------------------------------------------------
    //     Call test body using these parameters
    //----------------------------------------------------------
    test_gemm<T>( storage, transa, transb, m, n, k, lda_inc, ldb_inc, ldc_inc, alpha, beta, thresh );
}

class DGemmTestPrint {
public:
    std::string operator()(
        testing::TestParamInfo<std::tuple<char, char, char, gtint_t, gtint_t, gtint_t, double, double, gtint_t, gtint_t, gtint_t>> str) const {
        char sfm        = std::get<0>(str.param);
        char tsa        = std::get<1>(str.param);
        char tsb        = std::get<2>(str.param);
        gtint_t m       = std::get<3>(str.param);
        gtint_t n       = std::get<4>(str.param);
        gtint_t k       = std::get<5>(str.param);
        double alpha    = std::get<6>(str.param);
        double beta     = std::get<7>(str.param);
        gtint_t lda_inc = std::get<8>(str.param);
        gtint_t ldb_inc = std::get<9>(str.param);
        gtint_t ldc_inc = std::get<10>(str.param);
#ifdef TEST_BLAS
        std::string str_name = "dgemm_";
#elif TEST_CBLAS
        std::string str_name = "cblas_dgemm";
#else  //#elif TEST_BLIS_TYPED
        std::string str_name = "bli_dgemm";
#endif
        str_name = str_name + "_" + sfm+sfm+sfm;
        str_name = str_name + "_" + tsa + tsb;
        str_name = str_name + "_" + std::to_string(m);
        str_name = str_name + "_" + std::to_string(n);
        str_name = str_name + "_" + std::to_string(k);
        std::string alpha_str = ( alpha > 0) ? std::to_string(int(alpha)) : "m" + std::to_string(int(std::abs(alpha)));
        str_name = str_name + "_a" + alpha_str;
        std::string beta_str = ( beta > 0) ? std::to_string(int(beta)) : "m" + std::to_string(int(std::abs(beta)));
        str_name = str_name + "_b" + beta_str;
        str_name = str_name + "_" + std::to_string(lda_inc);
        str_name = str_name + "_" + std::to_string(ldb_inc);
        str_name = str_name + "_" + std::to_string(ldc_inc);
        return str_name;
    }
};

// Black box testing.
INSTANTIATE_TEST_SUITE_P(
        Blackbox,
        DGemmTest,
        ::testing::Combine(
            ::testing::Values('c'
#ifndef TEST_BLAS
            ,'r'
#endif
            ),                                                               // storage format
            ::testing::Values('n','t'),                                      // transa
            ::testing::Values('n','t'),                                      // transb
            ::testing::Range(gtint_t(10), gtint_t(31), 10),                  // m
            ::testing::Range(gtint_t(10), gtint_t(31), 10),                  // n
            ::testing::Range(gtint_t(10), gtint_t(31), 10),                  // k
            ::testing::Values( 1.0, -2.0),                                   // alpha
            ::testing::Values(-1.0,  1.0),                                   // beta
            ::testing::Values(gtint_t(0), gtint_t(4)),                       // increment to the leading dim of a
            ::testing::Values(gtint_t(0), gtint_t(7)),                       // increment to the leading dim of b
            ::testing::Values(gtint_t(0), gtint_t(2))                        // increment to the leading dim of c
        ),
        ::DGemmTestPrint()
    );


// Tests 5 loops
INSTANTIATE_TEST_SUITE_P(
        tiny_dgemm_kernel,
        DGemmTest,
        ::testing::Combine(
            // No condition based on storage scheme of matrices
            ::testing::Values('c'),                       // storage format
            // No conditions based on trans of matrices
            ::testing::Values('n', 't'),                       // transa
            ::testing::Values('n', 't'),                       // transb

            ::testing::Values(13, 25, 48, 60, 256, 512, 1000),       // m

            ::testing::Values(8, 48, 72, 144, 237),            // n

            ::testing::Values(16, 24, 48, 64, 128, 557),             // k
            // No condition based on alpha
            ::testing::Values( -1.0),                     // alpha
            // No condition based on betaa
            ::testing::Values(-1.0),                      // beta
            ::testing::Values(0,3),                       // increment to the leading dim of a
            ::testing::Values(0,3),                       // increment to the leading dim of b
            ::testing::Values(0,3)                      // increment to the leading dim of c
        ),
        ::DGemmTestPrint()
    );

//zero beta test case
INSTANTIATE_TEST_SUITE_P(
        zero_beta,
        DGemmTest,
        ::testing::Combine(
            // No condition based on storage scheme of matrices
            ::testing::Values('c'),                       // storage format
            // No conditions based on trans of matrices
            ::testing::Values('n', 't'),                       // transa
            ::testing::Values('n', 't'),                       // transb

            ::testing::Values(13, 25, 48, 60, 256, 512, 1000),       // m

            ::testing::Values(8, 48, 72, 144, 237),            // n

            ::testing::Values(16, 24, 48, 64, 128, 557),             // k

            ::testing::Values( -1.0),                     // alpha
            ::testing::Values(0.0),                      // beta
            ::testing::Values(0,3),                       // increment to the leading dim of a
            ::testing::Values(0,3),                       // increment to the leading dim of b
            ::testing::Values(0,3)                       // increment to the leading dim of c
        ),
        ::DGemmTestPrint()
    );

//zero alpha test case
INSTANTIATE_TEST_SUITE_P(
        zero_alpha,
        DGemmTest,
        ::testing::Combine(
            // No condition based on storage scheme of matrices
            ::testing::Values('c'),                       // storage format
            // No conditions based on trans of matrices
            ::testing::Values('n', 't'),                       // transa
            ::testing::Values('n', 't'),                       // transb

            ::testing::Values(13, 25, 48, 60, 256, 512, 1000),       // m

            ::testing::Values(8, 48, 72, 144, 237),            // n

            ::testing::Values(16, 24, 48, 64, 128, 557),             // k

            ::testing::Values( 0.0),                     // alpha
            ::testing::Values(-1.0),                      // beta
            ::testing::Values(0,3),                       // increment to the leading dim of a
            ::testing::Values(0,3),                       // increment to the leading dim of b
            ::testing::Values(0,3)                       // increment to the leading dim of c
        ),
        ::DGemmTestPrint()
    );

//unit beta test case
INSTANTIATE_TEST_SUITE_P(
        unit_beta,
        DGemmTest,
        ::testing::Combine(
            // No condition based on storage scheme of matrices
            ::testing::Values('c'),                       // storage format
            // No conditions based on trans of matrices
            ::testing::Values('n', 't'),                       // transa
            ::testing::Values('n', 't'),                       // transb

            ::testing::Values(13, 25, 48, 60, 256, 512, 1000),       // m

            ::testing::Values(8, 48, 72, 144, 237),            // n

            ::testing::Values(16, 24, 48, 64, 128, 557),             // k

            ::testing::Values( -1.0),                     // alpha
            ::testing::Values(1.0),                      // beta
            ::testing::Values(0,3),                       // increment to the leading dim of a
            ::testing::Values(0,3),                       // increment to the leading dim of b
            ::testing::Values(0,3)                       // increment to the leading dim of c
        ),
        ::DGemmTestPrint()
    );

// Covers all corner cases of tiny dgemm kernel
INSTANTIATE_TEST_SUITE_P(
        tiny_edge_kernels,
        DGemmTest,
        ::testing::Combine(
		// To test col storage of C
		// Storage of A and B is handled by packing
		::testing::Values('c'),                            // storage format
		// Tests scalar code of 8xk and 6xk pack kernels for both storage formats
		::testing::Values('n','t'),                        // transa
		::testing::Values('n','t'),                        // transb

		::testing::Range(gtint_t(1), gtint_t(23), 1),       // m
		::testing::Range(gtint_t(1), gtint_t(7), 1),       // n

		::testing::Values(24),                              // k
		// No condition based on alpha
		::testing::Values( -1.0, 1.0),                          // alpha
		// checks for beta-zero and beta non-zero cases
		::testing::Values(0.0, 1.0, -1.0),                      // beta
		::testing::Values(23),                             // increment to the leading dim of a
		::testing::Values(23),                             // increment to the leading dim of b
		::testing::Values(23)                             // increment to the leading dim of c
        ),
        ::DGemmTestPrint()
    );


//m = 0, n = 0 k = 0 testcase
INSTANTIATE_TEST_SUITE_P(
        mnkzero,
        DGemmTest,
        ::testing::Combine(
            // No condition based on storage scheme of matrices
            ::testing::Values('c'),                       // storage format
            // No conditions based on trans of matrices
            ::testing::Values('n', 't'),                       // transa
            ::testing::Values('n', 't'),                       // transb

            ::testing::Values(0, 8, 24),       // m

            ::testing::Values(0, 6, 8),            // n

            ::testing::Values(3),             // k

            ::testing::Values( -1.0),                     // alpha
            ::testing::Values(1.0),                      // beta
            ::testing::Values(0,3),                       // increment to the leading dim of a
            ::testing::Values(0,3),                       // increment to the leading dim of b
            ::testing::Values(0,3)                       // increment to the leading dim of c
        ),
        ::DGemmTestPrint()
    );
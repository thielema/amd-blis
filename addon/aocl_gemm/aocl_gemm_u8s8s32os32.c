/*

   BLIS
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2022 - 2023, Advanced Micro Devices, Inc. All rights reserved.

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
#include "aocl_gemm_interface_apis.h"
#include "aocl_gemm_check.h"
#include "lpgemm_types.h"
#include "lpgemm_post_ops.h"
#include "lpgemm_thread_decor_openmp.h"
#include "lpgemm_5loop_interface_apis.h"
#include "lpgemm_config.h"
#include "lpgemm_utils.h"

AOCL_GEMM_MATMUL(uint8_t,int8_t,int32_t,int32_t,u8s8s32os32)
{
	trans_t blis_transa;
	trans_t blis_transb;

	// Check if avx512_vnni ISA is supported, lpgemm matmul only works with it.
	if ( bli_cpuid_is_avx512vnni_supported() == FALSE )
	{
		bli_print_msg(" AVX512_VNNI ISA not supported by processor, "
				"cannot perform u8s8s32 gemm.", __FILE__, __LINE__ );
		return; // Error.
	}

	/* Initialize BLIS. */
	bli_init_auto();

	// Set MC, NC, KC, NR, MR.
	aocl_lpgemm_init_global_cntx();

	// check for validity of params.
	AOCL_GEMM_CHECK
	(
	  "u8s8s32os32",
	  order, transa, transb,
	  m, n, k,
	  a, lda, mem_format_a,
	  b, ldb, mem_format_b,
	  c, ldc
	);

	/* Map BLAS chars to their corresponding BLIS enumerated type value. */
	bli_param_map_netlib_to_blis_trans( transa, &blis_transa );
	bli_param_map_netlib_to_blis_trans( transb, &blis_transb );

	/* Perform BLAS parameter checking. */
	// Transpose not supported.
	if ( ( blis_transa != BLIS_NO_TRANSPOSE ) ||
	     ( blis_transb != BLIS_NO_TRANSPOSE ) )
	{
		bli_print_msg(" Transpose of matrices is not supported.", __FILE__, __LINE__ );
		return; // Error.
	}

	if ( ( order != 'r' ) && ( order != 'R' ) )
	{
		bli_print_msg(" Operation only supports row-major matrices.", __FILE__, __LINE__ );
		return; // Only row major supported.
	}

	const inc_t rs_a = lda;
	const inc_t cs_a = 1;
	const inc_t rs_b = ldb;
	const inc_t cs_b = 1;
	const inc_t rs_c = ldc;
	const inc_t cs_c = 1;

	AOCL_MEMORY_TAG mtag_a;
	AOCL_MEMORY_TAG mtag_b;

	bli_param_map_char_to_lpmtag( mem_format_a, &mtag_a );
	bli_param_map_char_to_lpmtag( mem_format_b, &mtag_b );

	// B matrix needs to be packed in a certain format in order to be loaded
	// and used in VNNI instrution. As such the mtag_b always needs to be either
	// packed or reordered. B matrix as it is (unpacked) cannot be used, and
	// the mtag_b is set to packed to enable runtime packing.
	if ( mtag_b == UNPACKED )
	{
		mtag_b = PACK;
	}

	// Only unpacked A supported now.
	if ( mtag_a != UNPACKED )
	{
		bli_print_msg(" A matrix needs to be unpacked.", __FILE__, __LINE__ );
		return; // Error.
	}

	// Convert post op struct to post op linked list format.
	lpgemm_post_op post_op_list[AOCL_MAX_POST_OPS];
	err_t err = lpgemm_translate_to_post_ops_list
	(
	  post_op_unparsed, post_op_list,
	  ( void* )c, ( void* )( &order )
	);

	if( err != BLIS_SUCCESS ) return;

	// Initialize a local runtime with global settings if necessary. Note
	// that in the case that a runtime is passed in, we make a local copy.
	rntm_t rntm_g;
	bli_rntm_init_from_global( &rntm_g );
	bli_pba_rntm_set_pba( &rntm_g );

	lpgemm_cntx_t* lcntx_g = lpgemm_get_global_cntx_obj( U8S8S32OS32 );

#ifdef BLIS_ENABLE_OPENMP
	lpgemm_u8s8s32o32_openmp_thread_decorator
	(
	  m, n, k,
	  a, rs_a, cs_a, mtag_a,
	  b, rs_b, cs_b, mtag_b,
	  c, rs_c, cs_c,
	  alpha, beta,
	  &rntm_g, lcntx_g,
	  post_op_list, S32
	);
#else
	lpgemm_u8s8s32o32_thread_decorator
	(
	  m, n, k,
	  a, rs_a, cs_a, mtag_a,
	  b, rs_b, cs_b, mtag_b,
	  c, rs_c, cs_c,
	  alpha, beta,
	  &rntm_g, lcntx_g,
	  post_op_list, S32
	);
#endif
}
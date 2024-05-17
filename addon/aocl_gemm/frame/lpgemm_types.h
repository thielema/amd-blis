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

#ifndef LPGEMM_TYPES_H
#define LPGEMM_TYPES_H

typedef enum
{
	INT8 = 0,
	INT16 = 1,
	INT32 = 2
} AOCL_ARRAY_TYPE;

// Enum to denote the storage data type (output matrix).
// It is expected that the enum entries are in ascending order of
// storage data type size.
typedef enum
{
	S8 = 0,
	U8 = 1,
	S16 = 2,
	U16 = 3,
	BF16 = 4,
	S32 = 5,
	U32 = 6,
	F32 = 7,
	S64 = 8,
	U64 = 9,
	F64 = 10
} AOCL_STORAGE_TYPE;

// Enum name template:A_mat_type ## B_mat_type ## Accumulate_type ## C_mat_type.
typedef enum
{
	U8S8S16OS16 = 0, // uint8_t - A, int8_t - B, int16_t - C
	U8S8S32OS32 = 1, // uint8_t - A, int8_t - B, int32_t - C
	F32F32F32OF32 = 2, // float - A, float - B, float - C
	BF16BF16F32OF32 = 3, // bf16 - A, bf16 - B, float - C
	S8S8S32OS32 = 4, // int8_t - A, int8_t - B, int32_t - C
	S8S8S16OS16 = 5  // int8_t - A, int8_t - B, int16_t - C
} AOCL_OPERATION_TYPE;
#define AOCL_OPERATION_TYPE_LEN 6

typedef enum
{
	F32_GELU_TANH = 0,
	F32_GELU_ERF = 1,
	F32_SOFTMAX = 2
} AOCL_UTIL_OPERATION_TYPE;
#define AOCL_UTIL_OPERATION_TYPE_LEN 3

typedef enum
{
	UNPACKED = 0,
	PACK = 1,
	REORDERED = 2,
} AOCL_MEMORY_TAG;

typedef enum
{
	ROW_MAJOR = 0,
	COLUMN_MAJOR = 1,
} AOCL_STOR_TAG;

typedef enum
{
	A_MATRIX = 0,
	B_MATRIX = 1,
} AOCL_MATRIX_TYPE;

typedef struct
{
	void* aligned_buffer;
	void* origin_buffer;
} lpgemm_mem_t;

typedef struct
{
	dim_t length;
	dim_t width;

	dim_t elem_size;

	dim_t rs;
	dim_t cs;

	AOCL_MEMORY_TAG mtag;

	lpgemm_mem_t storage;
} lpgemm_obj_t;

typedef struct
{
	dim_t MC;
	dim_t NC;
	dim_t KC;
	dim_t NR;
	dim_t MR;
} lpgemm_block_size_t;

typedef struct
{
	dim_t packa_rs;
	dim_t packa_cs;
	dim_t packb_rs;
	dim_t packb_cs;
} lpgemm_pack_strides_t;

typedef struct
{
	lpgemm_block_size_t blksz;
	void_fp kern_fun_ptr;
	void_fp packa_fun_ptr;
	void_fp packb_fun_ptr;
	lpgemm_pack_strides_t pack_s;
} lpgemm_cntx_t;

typedef struct
{
	void_fp kern_fun_ptr;
} lpgemm_util_cntx_t;

typedef struct
{
	dim_t n_threads;
	dim_t tid;
	dim_t ic_ways;
	dim_t jc_ways;
	thrcomm_t* comm;
} lpgemm_thrinfo_t;

#endif //LPGEMM_TYPES_H
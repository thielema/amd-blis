/*

   BLIS
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2014, The University of Texas at Austin
   Copyright (C) 2018 - 2019, Advanced Micro Devices, Inc.

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

#ifndef BLIS_THRCOMM_H
#define BLIS_THRCOMM_H

// Include definitions (mostly thrcomm_t) specific to the method of
// multithreading.
#include "bli_thrcomm_single.h"
#include "bli_thrcomm_openmp.h"
#include "bli_thrcomm_pthreads.h"


// thread entry point prototype.
void* bli_l3_thread_entry( void* data_void );


// thrcomm_t query (field only)

BLIS_INLINE dim_t bli_thrcomm_num_threads( thrcomm_t* comm )
{
	return comm->n_threads;
}


// Thread communicator prototypes.
thrcomm_t* bli_thrcomm_create( rntm_t* rntm, dim_t n_threads );
void       bli_thrcomm_free( rntm_t* rntm, thrcomm_t* comm );
void       bli_thrcomm_init( dim_t n_threads, thrcomm_t* comm );
void       bli_thrcomm_cleanup( thrcomm_t* comm );

BLIS_EXPORT_BLIS void  bli_thrcomm_barrier( dim_t thread_id, thrcomm_t* comm );
BLIS_EXPORT_BLIS void* bli_thrcomm_bcast( dim_t inside_id, void* to_send, thrcomm_t* comm );

void       bli_thrcomm_barrier_atomic( dim_t thread_id, thrcomm_t* comm );

#endif


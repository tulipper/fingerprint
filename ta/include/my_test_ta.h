/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef TA_MY_TEST_H
#define TA_MY_TEST_H

/* This UUID is generated with uuidgen
   the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html */
/* 9269fadd-99d5-4afb-a1dc-ee3e9c61b04c  */
/*34b1c7a2-45f7-11e9-b210-d663bd873d93*/
#define TA_MY_TEST_UUID { 0x34b1c7a2, 0x45f7, 0x11e9, \
		{ 0xb2, 0x10, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93} }

/* The Trusted Application Function ID(s) implemented in this TA */
#define TA_MY_TEST_CMD_INC_VALUE	0
#define TA_MY_TEST_CMD_HASH             1
#define TA_MY_TEST_CMD_RANDOM	    2
#define TA_MY_TEST_CMD_RAND	    3
#define TA_MY_TEST_CMD_GS	    4
#define TA_MY_TEST_CMD_GenModel     5

#define FAIL -1
#define OK 0
#define TEE_ALG_INVALID     0xFFFFFFFFU


#endif /*TA_HELLO_WORLD_H*/

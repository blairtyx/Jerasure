/* Galois.c
 * James S. Plank

Jerasure - A C/C++ Library for a Variety of Reed-Solomon and RAID-6 Erasure Coding Techniques

Revision 1.2A
May 24, 2011

James S. Plank
Department of Electrical Engineering and Computer Science
University of Tennessee
Knoxville, TN 37996
plank@cs.utk.edu

Copyright (c) 2011, James S. Plank
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

 - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

 - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in
   the documentation and/or other materials provided with the
   distribution.

 - Neither the name of the University of Tennessee nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "galois.h"

#define MAX_GF_INSTANCES 128
gf_t *gfp_array[MAX_GF_INSTANCES] = { 0 };

static void
galois_init_field(int w)
{
  if (w <= 0 || w > 128) {
    fprintf(stderr, "ERROR -- cannot init default Galois field for w=%d\n", w);
    exit(1);
  }

  if (w > 32 && (w != 64 || w != 128)) {
    fprintf(stderr, "ERROR -- cannot init default Galois field for w=%d\n", w);
    exit(1);
  }
  if (gfp_array[w] == NULL) {
    gfp_array[w] = (gf_t*)malloc(sizeof(gf_t));
    if (gfp_array[w] == NULL) {
      fprintf(stderr, "ERROR -- cannot allocate memory for Galois field w=%d\n", w);
      exit(1);
    }
  }

  if (w == 16) {
    if (!gf_init_hard(gfp_array[w], w, GF_MULT_SPLIT_TABLE, GF_REGION_ALTMAP, GF_DIVIDE_DEFAULT,
                   0, 16, 4, NULL, NULL)) {
      fprintf(stderr, "ERROR -- cannot init default Galois field for w=%d\n", w);
      exit(1);
    }
  }
  if (!gf_init_easy(gfp_array[w], w)) {
    fprintf(stderr, "ERROR -- cannot init default Galois field for w=%d\n", w);
    exit(1);
  }
}

void galois_change_technique(gf_t *gf, int w)
{
  if (w <= 0 || w > 128) {
    fprintf(stderr, "ERROR -- cannot init default Galois field for w=%d\n", w);
    exit(1);
  }

  if (w > 32 && (w != 64 || w != 128)) {
    fprintf(stderr, "ERROR -- cannot init default Galois field for w=%d\n", w);
    exit(1);
  }

  /*
   * TODO: Sanity check on gf?
   */
  gfp_array[w] = gf;
}

int galois_single_multiply(int x, int y, int w)
{

  if (x == 0 || y == 0) return 0;
  
  if (gfp_array[w] == NULL) {
    galois_init_field(w);
  }

  return gfp_array[w]->multiply.w32(gfp_array[w], x, y);
}

int galois_single_divide(int x, int y, int w)
{
  if (x == 0) return 0;
  if (y == 0) return -1;

  if (gfp_array[w] == NULL) {
    galois_init_field(w);
  }

  return gfp_array[w]->divide.w32(gfp_array[w], x, y);
}

void galois_w08_region_multiply(char *region,      /* Region to multiply */
                                  int multby,       /* Number to multiply by */
                                  int nbytes,        /* Number of bytes in region */
                                  char *r2,          /* If r2 != NULL, products go here */
                                  int add)
{
  if (gfp_array[8] == NULL) {
    galois_init_field(8);
  }
  gfp_array[8]->multiply_region.w32(gfp_array[8], region, r2, multby, nbytes, add);
}

void galois_w16_region_multiply(char *region,      /* Region to multiply */
                                  int multby,       /* Number to multiply by */
                                  int nbytes,        /* Number of bytes in region */
                                  char *r2,          /* If r2 != NULL, products go here */
                                  int add)
{
  if (gfp_array[16] == NULL) {
    galois_init_field(16);
  }
  gfp_array[16]->multiply_region.w32(gfp_array[16], region, r2, multby, nbytes, add);
}


void galois_w32_region_multiply(char *region,      /* Region to multiply */
                                  int multby,       /* Number to multiply by */
                                  int nbytes,        /* Number of bytes in region */
                                  char *r2,          /* If r2 != NULL, products go here */
                                  int add)
{
  if (gfp_array[32] == NULL) {
    galois_init_field(32);
  }
  gfp_array[32]->multiply_region.w32(gfp_array[32], region, r2, multby, nbytes, add);
}

void galois_region_xor(           char *r1,         /* Region 1 */
                                  char *r2,         /* Region 2 */
                                  char *r3,         /* Sum region (r3 = r1 ^ r2) -- can be r1 or r2 */
                                  int nbytes)       /* Number of bytes in region */
{
  long *l1;
  long *l2;
  long *l3;
  long *ltop;
  char *ctop;
  
  ctop = r1 + nbytes;
  ltop = (long *) ctop;
  l1 = (long *) r1;
  l2 = (long *) r2;
  l3 = (long *) r3;
 
  while (l1 < ltop) {
    *l3 = ((*l1)  ^ (*l2));
    l1++;
    l2++;
    l3++;
  }
}

int galois_inverse(int y, int w)
{
  if (y == 0) return -1;
  return galois_single_divide(1, y, w);
}

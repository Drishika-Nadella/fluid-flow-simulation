/*
 * Copyright (c) 1997-1999, 2003 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* This file was automatically generated --- DO NOT EDIT */
/* Generated on Mon Mar 24 02:08:33 EST 2003 */

#include "fftw-int.h"
#include "fftw.h"

/* Generated by: /homee/stevenj/cvs/fftw/gensrc/genfft -magic-alignment-check -magic-twiddle-load-all -magic-variables 4 -magic-loopi -twiddleinv 10 */

/*
 * This function contains 102 FP additions, 60 FP multiplications,
 * (or, 72 additions, 30 multiplications, 30 fused multiply/add),
 * 42 stack variables, and 40 memory accesses
 */
static const fftw_real K951056516 =
FFTW_KONST(+0.951056516295153572116439333379382143405698634);
static const fftw_real K587785252 =
FFTW_KONST(+0.587785252292473129168705954639072768597652438);
static const fftw_real K250000000 =
FFTW_KONST(+0.250000000000000000000000000000000000000000000);
static const fftw_real K559016994 =
FFTW_KONST(+0.559016994374947424102293417182819058860154590);

/*
 * Generator Id's : 
 * $Id: exprdag.ml,v 1.43 2003/03/16 23:43:46 stevenj Exp $
 * $Id: fft.ml,v 1.44 2003/03/16 23:43:46 stevenj Exp $
 * $Id: to_c.ml,v 1.26 2003/03/16 23:43:46 stevenj Exp $
 */

void fftwi_twiddle_10(fftw_complex *A, const fftw_complex *W, int iostride,
		      int m, int dist)
{
     int i;
     fftw_complex *inout;
     inout = A;
     for (i = m; i > 0; i = i - 1, inout = inout + dist, W = W + 9) {
	  fftw_real tmp7;
	  fftw_real tmp55;
	  fftw_real tmp100;
	  fftw_real tmp115;
	  fftw_real tmp41;
	  fftw_real tmp52;
	  fftw_real tmp53;
	  fftw_real tmp59;
	  fftw_real tmp60;
	  fftw_real tmp61;
	  fftw_real tmp75;
	  fftw_real tmp78;
	  fftw_real tmp113;
	  fftw_real tmp89;
	  fftw_real tmp90;
	  fftw_real tmp96;
	  fftw_real tmp18;
	  fftw_real tmp29;
	  fftw_real tmp30;
	  fftw_real tmp56;
	  fftw_real tmp57;
	  fftw_real tmp58;
	  fftw_real tmp68;
	  fftw_real tmp71;
	  fftw_real tmp112;
	  fftw_real tmp86;
	  fftw_real tmp87;
	  fftw_real tmp95;
	  ASSERT_ALIGNED_DOUBLE;
	  {
	       fftw_real tmp1;
	       fftw_real tmp99;
	       fftw_real tmp6;
	       fftw_real tmp98;
	       ASSERT_ALIGNED_DOUBLE;
	       tmp1 = c_re(inout[0]);
	       tmp99 = c_im(inout[0]);
	       {
		    fftw_real tmp3;
		    fftw_real tmp5;
		    fftw_real tmp2;
		    fftw_real tmp4;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp3 = c_re(inout[5 * iostride]);
		    tmp5 = c_im(inout[5 * iostride]);
		    tmp2 = c_re(W[4]);
		    tmp4 = c_im(W[4]);
		    tmp6 = (tmp2 * tmp3) + (tmp4 * tmp5);
		    tmp98 = (tmp2 * tmp5) - (tmp4 * tmp3);
	       }
	       tmp7 = tmp1 - tmp6;
	       tmp55 = tmp1 + tmp6;
	       tmp100 = tmp98 + tmp99;
	       tmp115 = tmp99 - tmp98;
	  }
	  {
	       fftw_real tmp35;
	       fftw_real tmp73;
	       fftw_real tmp51;
	       fftw_real tmp77;
	       fftw_real tmp40;
	       fftw_real tmp74;
	       fftw_real tmp46;
	       fftw_real tmp76;
	       ASSERT_ALIGNED_DOUBLE;
	       {
		    fftw_real tmp32;
		    fftw_real tmp34;
		    fftw_real tmp31;
		    fftw_real tmp33;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp32 = c_re(inout[4 * iostride]);
		    tmp34 = c_im(inout[4 * iostride]);
		    tmp31 = c_re(W[3]);
		    tmp33 = c_im(W[3]);
		    tmp35 = (tmp31 * tmp32) + (tmp33 * tmp34);
		    tmp73 = (tmp31 * tmp34) - (tmp33 * tmp32);
	       }
	       {
		    fftw_real tmp48;
		    fftw_real tmp50;
		    fftw_real tmp47;
		    fftw_real tmp49;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp48 = c_re(inout[iostride]);
		    tmp50 = c_im(inout[iostride]);
		    tmp47 = c_re(W[0]);
		    tmp49 = c_im(W[0]);
		    tmp51 = (tmp47 * tmp48) + (tmp49 * tmp50);
		    tmp77 = (tmp47 * tmp50) - (tmp49 * tmp48);
	       }
	       {
		    fftw_real tmp37;
		    fftw_real tmp39;
		    fftw_real tmp36;
		    fftw_real tmp38;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp37 = c_re(inout[9 * iostride]);
		    tmp39 = c_im(inout[9 * iostride]);
		    tmp36 = c_re(W[8]);
		    tmp38 = c_im(W[8]);
		    tmp40 = (tmp36 * tmp37) + (tmp38 * tmp39);
		    tmp74 = (tmp36 * tmp39) - (tmp38 * tmp37);
	       }
	       {
		    fftw_real tmp43;
		    fftw_real tmp45;
		    fftw_real tmp42;
		    fftw_real tmp44;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp43 = c_re(inout[6 * iostride]);
		    tmp45 = c_im(inout[6 * iostride]);
		    tmp42 = c_re(W[5]);
		    tmp44 = c_im(W[5]);
		    tmp46 = (tmp42 * tmp43) + (tmp44 * tmp45);
		    tmp76 = (tmp42 * tmp45) - (tmp44 * tmp43);
	       }
	       tmp41 = tmp35 - tmp40;
	       tmp52 = tmp46 - tmp51;
	       tmp53 = tmp41 + tmp52;
	       tmp59 = tmp35 + tmp40;
	       tmp60 = tmp46 + tmp51;
	       tmp61 = tmp59 + tmp60;
	       tmp75 = tmp73 - tmp74;
	       tmp78 = tmp76 - tmp77;
	       tmp113 = tmp75 + tmp78;
	       tmp89 = tmp73 + tmp74;
	       tmp90 = tmp76 + tmp77;
	       tmp96 = tmp89 + tmp90;
	  }
	  {
	       fftw_real tmp12;
	       fftw_real tmp66;
	       fftw_real tmp28;
	       fftw_real tmp70;
	       fftw_real tmp17;
	       fftw_real tmp67;
	       fftw_real tmp23;
	       fftw_real tmp69;
	       ASSERT_ALIGNED_DOUBLE;
	       {
		    fftw_real tmp9;
		    fftw_real tmp11;
		    fftw_real tmp8;
		    fftw_real tmp10;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp9 = c_re(inout[2 * iostride]);
		    tmp11 = c_im(inout[2 * iostride]);
		    tmp8 = c_re(W[1]);
		    tmp10 = c_im(W[1]);
		    tmp12 = (tmp8 * tmp9) + (tmp10 * tmp11);
		    tmp66 = (tmp8 * tmp11) - (tmp10 * tmp9);
	       }
	       {
		    fftw_real tmp25;
		    fftw_real tmp27;
		    fftw_real tmp24;
		    fftw_real tmp26;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp25 = c_re(inout[3 * iostride]);
		    tmp27 = c_im(inout[3 * iostride]);
		    tmp24 = c_re(W[2]);
		    tmp26 = c_im(W[2]);
		    tmp28 = (tmp24 * tmp25) + (tmp26 * tmp27);
		    tmp70 = (tmp24 * tmp27) - (tmp26 * tmp25);
	       }
	       {
		    fftw_real tmp14;
		    fftw_real tmp16;
		    fftw_real tmp13;
		    fftw_real tmp15;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp14 = c_re(inout[7 * iostride]);
		    tmp16 = c_im(inout[7 * iostride]);
		    tmp13 = c_re(W[6]);
		    tmp15 = c_im(W[6]);
		    tmp17 = (tmp13 * tmp14) + (tmp15 * tmp16);
		    tmp67 = (tmp13 * tmp16) - (tmp15 * tmp14);
	       }
	       {
		    fftw_real tmp20;
		    fftw_real tmp22;
		    fftw_real tmp19;
		    fftw_real tmp21;
		    ASSERT_ALIGNED_DOUBLE;
		    tmp20 = c_re(inout[8 * iostride]);
		    tmp22 = c_im(inout[8 * iostride]);
		    tmp19 = c_re(W[7]);
		    tmp21 = c_im(W[7]);
		    tmp23 = (tmp19 * tmp20) + (tmp21 * tmp22);
		    tmp69 = (tmp19 * tmp22) - (tmp21 * tmp20);
	       }
	       tmp18 = tmp12 - tmp17;
	       tmp29 = tmp23 - tmp28;
	       tmp30 = tmp18 + tmp29;
	       tmp56 = tmp12 + tmp17;
	       tmp57 = tmp23 + tmp28;
	       tmp58 = tmp56 + tmp57;
	       tmp68 = tmp66 - tmp67;
	       tmp71 = tmp69 - tmp70;
	       tmp112 = tmp68 + tmp71;
	       tmp86 = tmp66 + tmp67;
	       tmp87 = tmp69 + tmp70;
	       tmp95 = tmp86 + tmp87;
	  }
	  {
	       fftw_real tmp64;
	       fftw_real tmp54;
	       fftw_real tmp63;
	       fftw_real tmp80;
	       fftw_real tmp82;
	       fftw_real tmp72;
	       fftw_real tmp79;
	       fftw_real tmp81;
	       fftw_real tmp65;
	       ASSERT_ALIGNED_DOUBLE;
	       tmp64 = K559016994 * (tmp30 - tmp53);
	       tmp54 = tmp30 + tmp53;
	       tmp63 = tmp7 - (K250000000 * tmp54);
	       tmp72 = tmp68 - tmp71;
	       tmp79 = tmp75 - tmp78;
	       tmp80 = (K587785252 * tmp72) - (K951056516 * tmp79);
	       tmp82 = (K951056516 * tmp72) + (K587785252 * tmp79);
	       c_re(inout[5 * iostride]) = tmp7 + tmp54;
	       tmp81 = tmp64 + tmp63;
	       c_re(inout[iostride]) = tmp81 - tmp82;
	       c_re(inout[9 * iostride]) = tmp81 + tmp82;
	       tmp65 = tmp63 - tmp64;
	       c_re(inout[7 * iostride]) = tmp65 - tmp80;
	       c_re(inout[3 * iostride]) = tmp65 + tmp80;
	  }
	  {
	       fftw_real tmp114;
	       fftw_real tmp116;
	       fftw_real tmp117;
	       fftw_real tmp111;
	       fftw_real tmp120;
	       fftw_real tmp109;
	       fftw_real tmp110;
	       fftw_real tmp119;
	       fftw_real tmp118;
	       ASSERT_ALIGNED_DOUBLE;
	       tmp114 = K559016994 * (tmp112 - tmp113);
	       tmp116 = tmp112 + tmp113;
	       tmp117 = tmp115 - (K250000000 * tmp116);
	       tmp109 = tmp18 - tmp29;
	       tmp110 = tmp41 - tmp52;
	       tmp111 = (K951056516 * tmp109) + (K587785252 * tmp110);
	       tmp120 = (K587785252 * tmp109) - (K951056516 * tmp110);
	       c_im(inout[5 * iostride]) = tmp116 + tmp115;
	       tmp119 = tmp117 - tmp114;
	       c_im(inout[3 * iostride]) = tmp119 - tmp120;
	       c_im(inout[7 * iostride]) = tmp120 + tmp119;
	       tmp118 = tmp114 + tmp117;
	       c_im(inout[iostride]) = tmp111 + tmp118;
	       c_im(inout[9 * iostride]) = tmp118 - tmp111;
	  }
	  {
	       fftw_real tmp84;
	       fftw_real tmp62;
	       fftw_real tmp83;
	       fftw_real tmp92;
	       fftw_real tmp94;
	       fftw_real tmp88;
	       fftw_real tmp91;
	       fftw_real tmp93;
	       fftw_real tmp85;
	       ASSERT_ALIGNED_DOUBLE;
	       tmp84 = K559016994 * (tmp58 - tmp61);
	       tmp62 = tmp58 + tmp61;
	       tmp83 = tmp55 - (K250000000 * tmp62);
	       tmp88 = tmp86 - tmp87;
	       tmp91 = tmp89 - tmp90;
	       tmp92 = (K587785252 * tmp88) - (K951056516 * tmp91);
	       tmp94 = (K951056516 * tmp88) + (K587785252 * tmp91);
	       c_re(inout[0]) = tmp55 + tmp62;
	       tmp93 = tmp84 + tmp83;
	       c_re(inout[6 * iostride]) = tmp93 - tmp94;
	       c_re(inout[4 * iostride]) = tmp93 + tmp94;
	       tmp85 = tmp83 - tmp84;
	       c_re(inout[2 * iostride]) = tmp85 - tmp92;
	       c_re(inout[8 * iostride]) = tmp85 + tmp92;
	  }
	  {
	       fftw_real tmp105;
	       fftw_real tmp97;
	       fftw_real tmp104;
	       fftw_real tmp103;
	       fftw_real tmp108;
	       fftw_real tmp101;
	       fftw_real tmp102;
	       fftw_real tmp107;
	       fftw_real tmp106;
	       ASSERT_ALIGNED_DOUBLE;
	       tmp105 = K559016994 * (tmp95 - tmp96);
	       tmp97 = tmp95 + tmp96;
	       tmp104 = tmp100 - (K250000000 * tmp97);
	       tmp101 = tmp56 - tmp57;
	       tmp102 = tmp59 - tmp60;
	       tmp103 = (K587785252 * tmp101) - (K951056516 * tmp102);
	       tmp108 = (K951056516 * tmp101) + (K587785252 * tmp102);
	       c_im(inout[0]) = tmp97 + tmp100;
	       tmp107 = tmp105 + tmp104;
	       c_im(inout[4 * iostride]) = tmp107 - tmp108;
	       c_im(inout[6 * iostride]) = tmp108 + tmp107;
	       tmp106 = tmp104 - tmp105;
	       c_im(inout[2 * iostride]) = tmp103 + tmp106;
	       c_im(inout[8 * iostride]) = tmp106 - tmp103;
	  }
     }
}

static const int twiddle_order[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
fftw_codelet_desc fftwi_twiddle_10_desc = {
     "fftwi_twiddle_10",
     (void (*)()) fftwi_twiddle_10,
     10,
     FFTW_BACKWARD,
     FFTW_TWIDDLE,
     231,
     9,
     twiddle_order,
};

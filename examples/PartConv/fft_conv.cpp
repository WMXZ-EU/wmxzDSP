/*
 * fft_conv.cpp
 *
 *  Created on: May 10, 2017
 *      Author: WMXZ
 */

#include "fft_conv.h"

/*
 * filter window M+1
 * FFTlength N = L + M
 * overlap add method
 * take L sample from input
 * zero pad to N , i.e. M zeros
 * filter (fft - mult - ifft)
 * add previous last M samples to actual first M samples
 * example
 * 		M = 128
 * 		L = 3*128
 * 		N = 512 = 4*128
 */

const arm_rfft_fast_instance_f32  arm_rfft_f32_len256  = {arm_cfft_sR_f32_len128,   256, (float *) twiddleCoef_rfft_256};
const arm_rfft_fast_instance_f32  arm_rfft_f32_len512  = {arm_cfft_sR_f32_len256,   512, (float *) twiddleCoef_rfft_512};
const arm_rfft_fast_instance_f32  arm_rfft_f32_len1024 = {arm_cfft_sR_f32_len512,  1024, (float *) twiddleCoef_rfft_1024};


/**
 * Filter initialization routine
 * @param fc	normalized cut-off frequency (LP and HP) or center frequency (BP and Stop) (normalized to Nyquist)
 * @param type	frequency type (0 = LP, 1 = HP, 2 = BP, 3 = Stop, 4 = all-pass Hilbert)
 * @param dfc	half bandwidth (irrelevant for LP, HP, and all-pass Hilbert) (normalized to Nyquist)
 * @param hilb	general Hilbert transform (0: off, 1: on )
 */

void C_CONV::init(float *imp)
{
	int ii,jj;
	for(ii=0; ii<NF*MM;ii++) ov[ii]=0;

#if NN==256
		 rfft_instance = (arm_rfft_fast_instance_f32*)&arm_rfft_f32_len256;
#elif NN==512
		 rfft_instance = (arm_rfft_fast_instance_f32*)&arm_rfft_f32_len512;
#elif NN==1024
		 rfft_instance = (arm_rfft_fast_instance_f32*)&arm_rfft_f32_len1024;
#endif

  for(jj=0;jj<NF;jj++)
  {
    for(ii=0; ii<(MM+1); ii++) uu[ii]=imp[jj*MM+ii];
    for(ii=MM+1; ii<NN; ii++)  uu[ii]=0;
    arm_rfft_fast_f32(rfft_instance, uu, &bb[jj*NN], 0);
  }
}

/**
 * filter with overlap and discard method
 * here sig 1: LL = 10
 *      sig 2: MM + 1 = 7
 *      fft length: NN+MM = 16
 * -----------------------------------
 * input: (with overlap)
 * 000000XXXXXXXXXX
 *          ooooooXXXXXXXXXX
 *                    ooooooXXXXXXXXXX
 * output: (overlap and discard)
 * ------YYYYYYYYYY
 *          ------YYYYYYYYYY
 *                   -------YYYYYYYYYY
 */

/**
 * uniform partitioned convolution execution step
 * @param yy	first output vector (is filtered output)
 * @param xx	input vector
 */
void C_CONV::exec_upos(float *yy, float *xx)
{	int ii,jj,kk;

    static int nn=0;
    
    float *zp = &zz[nn*NN];
    float *bp;
    nn++; nn %=NF;

    // copy data to FFT buffer
    for(ii = 0; ii < MM; ii++) uu[ii] = ov[ii];   // first overlap
    for(ii = 0; ii < LL; ii++) uu[MM+ii] = xx[ii];  // then input
    // perform forward fft
    arm_rfft_fast_f32(rfft_instance, uu, zp, 0);

    // clear accumulator
    for(ii=0;ii<NN;ii++) ww[ii]=0;

    // multiply and sum all spectra
    for(jj=0; jj<NF; jj++)
    {
      // multiply with filter
      zp = &zz[((nn-jj+NN)%NN)*NN];
      bp = &bb[jj*NN];
      arm_cmplx_mult_cmplx_f32(zp, bp, vv, NN/2);
      // accumulate previous filtered spectra
      for(ii=0;ii<NN;ii++) ww[ii] += vv[ii];
    }
    // inverse FFT
    arm_rfft_fast_f32(rfft_instance, ww, uu, 1);

    // copy data to result buffer
    for(ii = 0; ii < LL; ii++) yy[ii] = uu[MM+ii];  // discard first part

    // store overlap
    for(ii = 0; ii < MM; ii++) ov[ii] = xx[LL+ii];  //store last part from input
}


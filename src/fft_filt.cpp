/*
 * fft_filt.cpp
 *
 *  Created on: May 10, 2017
 *      Author: WMXZ
 */

#include "fft_filt.h"

//#define PI 3.14159265358979f // already defined in arm_math.h
#define TPI	 6.28318530717959
#define PIH	 1.57079632679490f

/**
 * Sinc functon
 * @param m	: tap
 * @param fc: LP cut-frequency
 * @return
 */

float m_sinc(int m, float fc)
{	// fc is f_cut/(Fsamp/2)
	// m is between -M and M step 2
	//
	float x = m*PIH;
	if(m == 0)
		return 1.0f;
	else
		return sinf(x*fc)/(fc*x);
}

/**
 * Bessel function
 * @param x: argument
 * @return
 */
float32_t Izero (float x)
{
    float32_t x2 = x / 2.0f;
    float32_t summe = 1.0f;
    float32_t ds = 1.0f;
    float32_t di = 1.0f;
    float32_t errorlimit = 1e-9;
    float32_t tmp;
    do
    {   tmp = x2 / di;
        tmp *= tmp;
        ds *= tmp;
        summe += ds;
        di += 1.0;
    }   while (ds >= errorlimit * summe);
    return (summe);
}  // END Izero


/**
 * Filter coefficient estimation using Kaiser Window
 * @param coeffs	coefficients
 * @param numCoeffs	number of coeffcients
 * @param fc		cutoff / center frequency (normalized to Nyquist)
 * @param Astop		stopband attenuation (dB)
 * @param type		filter type
 * @param dfc		half filter bandwidth (normalized to Nyquist)
 */
void calc_FIR_coeffs (float * coeffs, int numCoeffs, float fc, float Astop, int type, float dfc)
 {	// modified after
	// Wheatley, M. (2011): CuteSDR Technical Manual. www.metronix.com, pages 118 - 120, FIR with Kaiser-Bessel Window
	// assess required number of coefficients by
	//     numCoeffs = (Astop - 8.0) / (2.285 * TPI * normFtrans);
	// selecting high-pass, numCoeffs is adapter to become an even number as required for high-pass

	 int ii,jj;
     float32_t Beta;
     float32_t izb;
     float fcf;
     int nc;

     // calculate Kaiser-Bessel window shape factor beta from stop-band attenuation
     if (Astop < 20.96f)
    	 Beta = 0.0f;
     else if (Astop >= 50.0f)
    	 Beta = 0.1102f * (Astop - 8.71f);
     else
    	 Beta = 0.5842f * powf((Astop - 20.96f), 0.4f) + 0.7886f * (Astop - 20.96f);

     izb = Izero (Beta);
     if(type == LPF) // low pass filter
     {	fcf = fc;
     	nc =  numCoeffs;
     }
     else if(type == HPF) // high-pass filter
     {	fcf = -fc;
     	nc =  2*(numCoeffs/2);
     }
     else if ((type == BPF) || (type==SBF)) // band-pass filter (2,3)
     {
    	 fcf = dfc;
     	 nc =  2*(numCoeffs/2); // maybe not needed
     }
     else if (type==HIT)	// Hilbert transform (4)
	 {
      	 nc =  2*(numCoeffs/2);
    	 // clear coefficients
    	 for(ii=0; ii< 2*(nc-1); ii++) coeffs[ii]=0;
    	 // set real delay
    	 coeffs[nc]=1;

    	 // set imaginary Hilbert coefficients
    	 for(ii=1; ii< (nc+1); ii+=2)
    	 {
    		 if(2*ii==nc) continue;
			 float x =(float)(2*ii - nc)/(float)nc;
			 float w = Izero(Beta*sqrtf(1.0f - x*x))/izb; // Kaiser window
			 coeffs[2*ii+1] = 1.0f/(PIH*(float)(ii-nc/2)) * w ;
    	 }
    	 return;
	 }

     for(ii= - nc, jj=0; ii< nc; ii+=2,jj++)
     {
    	 float x =(float)ii/(float)nc;
    	 float w = Izero(Beta*sqrtf(1.0f - x*x))/izb;	// Kaiser window
    	 coeffs[jj] = fcf * m_sinc(ii,fcf) * w;
     }

     if(type==HPF)
     {
    	 coeffs[nc/2] += 1;
     }
     else if (type==BPF)
     {
       	 for(jj=0; jj< nc+1; jj++) coeffs[jj] *= 2.0f*cosf(PIH*(2*jj-nc)*fc);
     }
     else if (type==SBF)
     {
       	 for(jj=0; jj< nc+1; jj++) coeffs[jj] *= -2.0f*cosf(PIH*(2*jj-nc)*fc);
    	 coeffs[nc/2] += 1;
     }

 } // END calc_FIR_coeffs


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



/**
  * @brief Instance structure for the floating-point RFFT/RIFFT function.
  */
//typedef struct
// {
//   arm_cfft_instance_f32 Sint;      /**< Internal CFFT structure. */
//   uint16_t fftLenRFFT;             /**< length of the real sequence */
//   float32_t * pTwiddleRFFT;        /**< Twiddle factors real stage  */
// } arm_rfft_fast_instance_f32 ;

const arm_rfft_fast_instance_f32  arm_rfft_f32_len256  = {arm_cfft_sR_f32_len128,   256, (float *) twiddleCoef_rfft_256};
const arm_rfft_fast_instance_f32  arm_rfft_f32_len512  = {arm_cfft_sR_f32_len256,   512, (float *) twiddleCoef_rfft_512};
const arm_rfft_fast_instance_f32  arm_rfft_f32_len1024 = {arm_cfft_sR_f32_len512,  1024, (float *) twiddleCoef_rfft_1024};

/**
 * Filter initialization routine
 * @param fc      normalized cut-off frequency (LP and HP) or center frequency (BP and Stop) (normalized to Nyquist)
 * @param type    frequency type (0 = LP, 1 = HP, 2 = BP, 3 = Stop, 4 = all-pass Hilbert)
 * @param dfc     half bandwidth (irrelevant for LP, HP, and all-pass Hilbert) (normalized to Nyquist)
 * @param hilb    general Hilbert transform (0: off, 1: on ) 
 * @param buffer  memory for tem buffer
 * @param nch     number of channels
 * @param nn      fft size
 * @param ll      size of input data
 */

void C_FILTER::init(float fc, float_t dfc, uint16_t hilb, float *buffer, int nch, int nn, int ll)
{
  this->nch=nch;
  this->nn=nn;
  this->ll=ll;
  mm=nn-ll;
  //
  uu=buffer;
  ww=uu+nn;
  bb=ww+nn;
  coeffs=bb+nn;
  ov=coeffs+(mm+1);
  //
  
	int ii;
	for(ii=0; ii<nch*mm;ii++) ov[ii]=0;

  if(nn==256)
		 rfft_instance = (arm_rfft_fast_instance_f32*)&arm_rfft_f32_len256;
  else if(nn==512)
		 rfft_instance = (arm_rfft_fast_instance_f32*)&arm_rfft_f32_len512;
  else if(nn==1024)
		 rfft_instance = (arm_rfft_fast_instance_f32*)&arm_rfft_f32_len1024;

	//generate and transform filter coefficients
	if(dfc<0)
		calc_FIR_coeffs(coeffs, mm, fc, ASTOP, 0, 0.0f);
	else
		calc_FIR_coeffs(coeffs, mm, fc, ASTOP, 2, dfc);
	//
	for(ii=0; ii<(mm+1); ii++) uu[ii]=coeffs[ii];
	for(ii=mm+1; ii<nn; ii++)  uu[ii]=0;
	arm_rfft_fast_f32(rfft_instance, uu, bb, 0);

	//
	if(hilb==1) // change filter mask to act as hilbert transform
	{
		for(ii=2; ii<nn/2; ii++) bb[ii] *=2;	// maintain spectral energy
		for(ii=nn/2; ii<nn; ii++) bb[ii] = 0;	// remove negative frequencies
	}
}

/**
 * Filter with overlap and add method
 * here sig 1: LL = 10
 *      sig 2: MM + 1 = 7
 *      fft length: NN+MM = 16
 * -----------------------------------
 * input: (zero pad)
 * XXXXXXXXXX000000
 *           XXXXXXXXXX000000
 *                     XXXXXXXXXX000000
 * output: (overlap and add)
 * YYYYYYYYYYYYYYYY
 *           ++++++
 *           YYYYYYYYYYYYYYYY
 *                     ++++++
 *                     YYYYYYYYYYYYYYYY
 */

/**
 * filter execution step
 * @param yy	first output vector (is filtered output)
 * @param zz	second output vector (is spectrum)
 * @param xx	input vector
 */
void C_FILTER::exec_oa(float *yy, float * zz, float *xx)
{	int ii,kk;

// for all channels
	for(kk=0; kk<nch; kk++)
	{
		float *xp = &xx[kk*ll];
		float *zp = &zz[kk*nn];
		float *yp = &yy[kk*ll];
		float *op = &ov[kk*mm];

		// copy data to FFT buffer
		for(ii = 0; ii < ll; ii++) uu[ii] = xp[ii];
		for(ii = 0; ii < mm; ii++) uu[ll+ii] = 0.0f;

		// perform fft
		arm_rfft_fast_f32(rfft_instance, uu, zp, 0);
	    // multiply with filter
		arm_cmplx_mult_cmplx_f32(zp, bb, ww, nn/2);
		// inverse FFTs
		arm_rfft_fast_f32(rfft_instance, ww, uu, 1);

		// copy data to result buffer
		for(ii = 0; ii < ll; ii++) yp[ii] = uu[ii];
		for(ii = 0; ii < mm; ii++) yp[ii] += op[ii];

		// store overlap
		for(ii = 0; ii < mm; ii++) op[ii] = uu[ll+ii];

	}
}


/**
 * alternative filter with overlap and discard method
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
 * filter execution step
 * @param yy	first output vector (is filtered output)
 * @param zz	second output vector (is spectrum)
 * @param xx	input vector
 */
void C_FILTER::exec_os(float *yy, float * zz, float *xx)
{	int ii,kk;

// for all channels
	for(kk=0; kk<nch; kk++)
	{
		float *xp = &xx[kk*ll];
		float *yp = &yy[kk*ll];
		float *zp = &zz[kk*nn];
		float *op = &ov[kk*mm];

		// copy data to FFT buffer
		for(ii = 0; ii < mm; ii++) uu[ii] = op[ii];		// first overlap
		for(ii = 0; ii < ll; ii++) uu[mm+ii] = xp[ii];	// then input

		// perform fft
		arm_rfft_fast_f32(rfft_instance, uu, zp, 0);
	    // multiply with filter
		arm_cmplx_mult_cmplx_f32(zp, bb, ww, nn/2);
		// inverse FFTs
		arm_rfft_fast_f32(rfft_instance, ww, uu, 1);

		// copy data to result buffer
		for(ii = 0; ii < ll; ii++) yp[ii] = uu[mm+ii];	// discarding first part

		// store overlap
		for(ii = 0; ii < mm; ii++) op[ii] = xp[ll+ii];	//store last part from input
	}
}


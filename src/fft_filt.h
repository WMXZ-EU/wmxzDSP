/*
 * fft_filt.h
 *
 *  Created on: May 11, 2017
 *      Author: Walter
 */

#ifndef FFT_FILT_H_
#define FFT_FILT_H_

#include "cmsis.h"  // forces include to local 

#define ASTOP 90.0f // stop band attenuation for Kaiser FIR

#define LPF 0	// low-pass filter
#define HPF 1	// high-pass Filter
#define BPF 2	// band-pass filter
#define SBF 3	// stop-band filter
#define HIT 4	// hilbert filter

void calc_FIR_coeffs (float * coeffs, int numCoeffs, float fc, float Astop, int type, float dfc);

class C_FILTER
{
private:
	float *uu;      //[NN];
	float *ww;      //[NN];
	float *bb;      //[NN];
	//
	float *coeffs;  //[MM+1];
	float *ov;      //[NCH*MM];
	int nch,nn,ll,mm;
	//
	arm_rfft_fast_instance_f32 *rfft_instance;

public:
	void init(float fc, float_t dfc, uint16_t hilb, float *buffer, int nch, int nn, int ll);
	void exec_oa(float *yy, float * zz, float *xx);
	void exec_os(float *yy, float * zz, float *xx);
};

#endif /* FFT_FILT_H_ */

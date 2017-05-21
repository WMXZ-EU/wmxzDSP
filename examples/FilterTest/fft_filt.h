/*
 * fft_filt.h
 *
 *  Created on: May 11, 2017
 *      Author: Walter
 */

#ifndef FFT_FILT_H_
#define FFT_FILT_H_

#include "arm_math.h"
#include "arm_const_structs.h"

#define NCH 4		// number of channels
#define NN 1024		// FFT size
#define MM 128		// filter length - 1
#define LL (NN-MM) 	// step size (length of input)
#define ASTOP 90.0f // stop band attenuation for Kaiser FIR

class C_FILTER
{
private:
	float uu[NN];
	float ww[NN];
	float bb[NN];
	//
	float coeffs[MM+1];
	float ov[NCH*MM];
	//
	arm_rfft_fast_instance_f32 *rfft_instance;

public:
	void init(float fc, float_t dfc, uint16_t hilb);
	void exec_oa(float *yy, float * zz, float *xx);
	void exec_os(float *yy, float * zz, float *xx);
};

#endif /* FFT_FILT_H_ */

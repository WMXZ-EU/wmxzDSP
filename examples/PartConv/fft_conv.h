/*
 * fft_filt.h
 *
 *  Created on: May 11, 2017
 *      Author: Walter
 */

#ifndef FFT_CONV_H_
#define FFT_CONV_H_

#include "arm_math.h"
#include "arm_const_structs.h"

#define NF 70		// number of Filters
#define NN 256		// FFT size
#define MM (NN/2)		// filter length - 1
#define LL (NN-MM) 	// step size (length of input)

class C_CONV
{
private:
	float uu[NN];
  float vv[NN];
	float ww[NN];
	float bb[NF*NN]; // storage for filter-spectra
  float zz[NF*NN]; // storage for data-spectra
	//
	float ov[NF*MM]; // storage for data overlap
	//
	arm_rfft_fast_instance_f32 *rfft_instance;

public:
	void init(float *imp);
	void exec_upos(float *yy, float *xx);
};

#endif /* FFT_FILT_H_ */

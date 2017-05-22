# wmxzDSP
Keep here routines that are extracted from CMSIS V5


arm_math.h has been modified to work with teensy. In particular the following lines where added

#include <stdint.h>

#define ARM_MATH_CM4

#undef __VFP_FP__



## Examples are

### FilterTest: 
tests the use FFT based filter using Kaiser FIR
### PartConv:
implements uniform partitiond Convolution
on T3.6 convolution filter up to 70*128 coefficients are feasable (ca 85% of dynamic memory) 
[ Caveat: Functionality not verified (as of 21-05-2017)]
### AudioReverb:
integrates the partitioned convolution into audio framework for pjrc audio board 
[Caveat: to be tested]
### testRFFT:
implements and test dual channel 256 point real to complex FFT 

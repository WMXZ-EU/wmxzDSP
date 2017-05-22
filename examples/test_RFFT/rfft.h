#ifndef RFFT_H
#define RFFT_H

#include "arm_math.h"
#include "arm_const_structs.h"

class c_rfft
{
  const arm_cfft_instance_q15 *S_arm_cfft_q15;

  public:
  void init(void);
  void exec_256(short *xx, short *yy);
};

#endif


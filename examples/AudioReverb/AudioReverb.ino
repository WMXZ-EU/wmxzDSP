/*
 * AudioReverb.cpp
 *
 *  Created on: May 21, 2017
 *      Author: WMXZ
 */
#include "arm_math.h" // override cores/teensy3/arm_math.h with local one
#include <Audio.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=233,329
AudioRecordQueue         queue1;         //xy=428,329
AudioPlayQueue           queue2;         //xy=574,328
AudioOutputI2S           i2s2;           //xy=779,331
AudioConnection          patchCord1(i2s1, 0, queue1, 0);
AudioConnection          patchCord2(queue2, 0, i2s2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=234,173
// GUItool: end automatically generated code


const int myInput = AUDIO_INPUT_MIC;

// some forward declarations
void reverbInit(void);
void reverbExec(int16_t *out, int16_t *inp);

void setup() {
  // put your setup code here, to run once:

  // allocate memory for the audio library
  AudioMemory(8);
/* uncomment this block if there is a PJRC audioboard
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.micGain(10); // 0 - 63 dB (mic)
  sgtl5000_1.volume(0.5); // 0 - 1.0 (Headphone)
*/

  pinMode(6,OUTPUT); // use for visual debugger
  reverbInit();
  queue1.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  if (queue1.available() > 0) {
    int16_t inp[128];
    int16_t out[128];
    
    // Fetch 1 blocks from the audio library and copy
    // into input buffer.
    memcpy(inp, queue1.readBuffer(), 256);
    queue1.freeBuffer();

    // do processing
    digitalWriteFast(6,HIGH); 
    reverbExec(out, inp);
    digitalWriteFast(6,LOW); 

    // Store processed block into output queue
    // and send to I2S
    memcpy(queue2.getBuffer(),out,256);
    queue2.playBuffer();
  }
}
/********************** REVERB *********************************/
#include "fft_conv.h"
C_CONV mConv;

float xx[LL]; // filter input
float yy[LL]; // filter output
float impulse[NF*MM]; // impuls time series

#define SCALE 0.1f // to be adjusted to avoid output overflow
void reverbInit(void)
{
  // impulse function to be adjusted (total NF*MM taps)
  for(int ii=0; ii<NF; ii++) impulse[ii*MM + ii]=1.0f/(float)(ii+1);
  //
  mConv.init(impulse);
  return; 
}

void reverbExec(int16_t *out, int16_t *inp)
{ int ii;
    if(!(LL==128)) return; // let compiler decide
    for(ii=0;ii<128;ii++) xx[ii]=(float) inp[ii];  
    
    // yy is filtered output
    // xx in input
    mConv.exec_upos(yy,xx);
    
    for(ii=0;ii<128;ii++) out[ii]=(short)(yy[ii]*SCALE);  
    return;
}


//Copyright 2017 by Walter Zimmer
// Version 18-05-17
//
// general teensy includes
#include "kinetis.h"
#include "core_pins.h"
#include "usb_serial.h"
//
// application specifific includes
#include "myApp.h"

#include "fft_conv.h"
C_CONV mConv;

#define NBLK (44100/LL+1)
float xx[LL]; // filter input
float yy[LL]; // filter output
float zz[NF*NN]; // spectrum
float imp[NF*MM]; // impuls time series

//============================ Asynchronous Blink =============================================
void blink(uint32_t msec)
{ static uint32_t to=0;
  uint32_t t1 = millis();
  if(t1-to<msec) {yield(); return;}
  digitalWriteFast(13,!digitalReadFast(13)); 
  to=t1;
}

void c_myApp::setup()
{
  // wait for serial line to come up
  pinMode(13,OUTPUT);
  pinMode(13,HIGH);
  while(!Serial) blink(100);
  Serial.println("partitioned Convolution");

  for(int ii=0; ii<NF; ii++) imp[ii*MM + ii]=1;
  mConv.init(imp);

  uint32_t t0,t1;
  t0=micros();
  for(int ii=0; ii<NBLK; ii++)
  {
    // yy is filtered output
    // xx in input
    mConv.exec_upos(yy,xx);
  }
  t1=micros();
  Serial.printf("%d\n\r",(t1-t0));
}

void c_myApp::loop()
{ 
}


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

#include "fft_filt.h"
C_FILTER mFilt;

#define NBLK (44100/LL+1)
float xx[NCH*LL]; // filter input
float yy[NCH*LL]; // filter output
float zz[NCH*NN]; // spectrum

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
  Serial.println("multi-channel Filter");

  float fc  = 30.0f/375.0f*2.0f;
  float dfc = 10.0f/375.0f*2.0f;
  mFilt.init(fc,dfc,0);

  uint32_t t0,t1;
  t0=micros();
  for(int ii=0; ii<NBLK; ii++)
  {
    // yy is filtered output
    // zz is spectrum
    // xx in input
    mFilt.exec_os(yy,zz,xx);
  }
  t1=micros();
  Serial.printf("%d\n\r",(t1-t0));
}

void c_myApp::loop()
{ 
}


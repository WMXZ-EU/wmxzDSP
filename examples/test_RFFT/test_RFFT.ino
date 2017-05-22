#include "rfft.h"
c_rfft rfft;

#define NN 256
short xx[2*NN], yy[2*NN];

/**
 * Matlab code for check
 * xx=zeros(256,1);
 * xx(2)=1;
 * yy1=fft(xx,256);
 * xx=zeros(256,1);
 * xx(3)=1;
 * yy2=fft(xx,256);
 * fprintf('%5d,%5d,%5d,%5d,\n', ...
 * round(128*[real(yy1(1:128)), imag(yy1(1:128)), real(yy2(1:128)), imag(yy2(1:128))]'));
 */
short zz[512] ={
  128,    0,  128,    0,
  128,   -3,  128,   -6,
  128,   -6,  127,  -13,
  128,   -9,  127,  -19,
  127,  -13,  126,  -25,
  127,  -16,  124,  -31,
  127,  -19,  122,  -37,
  126,  -22,  121,  -43,
  126,  -25,  118,  -49,
  125,  -28,  116,  -55,
  124,  -31,  113,  -60,
  123,  -34,  110,  -66,
  122,  -37,  106,  -71,
  122,  -40,  103,  -76,
  121,  -43,   99,  -81,
  119,  -46,   95,  -86,
  118,  -49,   91,  -91,
  117,  -52,   86,  -95,
  116,  -55,   81,  -99,
  114,  -58,   76, -103,
  113,  -60,   71, -106,
  111,  -63,   66, -110,
  110,  -66,   60, -113,
  108,  -68,   55, -116,
  106,  -71,   49, -118,
  105,  -74,   43, -121,
  103,  -76,   37, -122,
  101,  -79,   31, -124,
   99,  -81,   25, -126,
   97,  -84,   19, -127,
   95,  -86,   13, -127,
   93,  -88,    6, -128,
   91,  -91,    0, -128,
   88,  -93,   -6, -128,
   86,  -95,  -13, -127,
   84,  -97,  -19, -127,
   81,  -99,  -25, -126,
   79, -101,  -31, -124,
   76, -103,  -37, -122,
   74, -105,  -43, -121,
   71, -106,  -49, -118,
   68, -108,  -55, -116,
   66, -110,  -60, -113,
   63, -111,  -66, -110,
   60, -113,  -71, -106,
   58, -114,  -76, -103,
   55, -116,  -81,  -99,
   52, -117,  -86,  -95,
   49, -118,  -91,  -91,
   46, -119,  -95,  -86,
   43, -121,  -99,  -81,
   40, -122, -103,  -76,
   37, -122, -106,  -71,
   34, -123, -110,  -66,
   31, -124, -113,  -60,
   28, -125, -116,  -55,
   25, -126, -118,  -49,
   22, -126, -121,  -43,
   19, -127, -122,  -37,
   16, -127, -124,  -31,
   13, -127, -126,  -25,
    9, -128, -127,  -19,
    6, -128, -127,  -13,
    3, -128, -128,   -6,
    0, -128, -128,    0,
   -3, -128, -128,    6,
   -6, -128, -127,   13,
   -9, -128, -127,   19,
  -13, -127, -126,   25,
  -16, -127, -124,   31,
  -19, -127, -122,   37,
  -22, -126, -121,   43,
  -25, -126, -118,   49,
  -28, -125, -116,   55,
  -31, -124, -113,   60,
  -34, -123, -110,   66,
  -37, -122, -106,   71,
  -40, -122, -103,   76,
  -43, -121,  -99,   81,
  -46, -119,  -95,   86,
  -49, -118,  -91,   91,
  -52, -117,  -86,   95,
  -55, -116,  -81,   99,
  -58, -114,  -76,  103,
  -60, -113,  -71,  106,
  -63, -111,  -66,  110,
  -66, -110,  -60,  113,
  -68, -108,  -55,  116,
  -71, -106,  -49,  118,
  -74, -105,  -43,  121,
  -76, -103,  -37,  122,
  -79, -101,  -31,  124,
  -81,  -99,  -25,  126,
  -84,  -97,  -19,  127,
  -86,  -95,  -13,  127,
  -88,  -93,   -6,  128,
  -91,  -91,    0,  128,
  -93,  -88,    6,  128,
  -95,  -86,   13,  127,
  -97,  -84,   19,  127,
  -99,  -81,   25,  126,
 -101,  -79,   31,  124,
 -103,  -76,   37,  122,
 -105,  -74,   43,  121,
 -106,  -71,   49,  118,
 -108,  -68,   55,  116,
 -110,  -66,   60,  113,
 -111,  -63,   66,  110,
 -113,  -60,   71,  106,
 -114,  -58,   76,  103,
 -116,  -55,   81,   99,
 -117,  -52,   86,   95,
 -118,  -49,   91,   91,
 -119,  -46,   95,   86,
 -121,  -43,   99,   81,
 -122,  -40,  103,   76,
 -122,  -37,  106,   71,
 -123,  -34,  110,   66,
 -124,  -31,  113,   60,
 -125,  -28,  116,   55,
 -126,  -25,  118,   49,
 -126,  -22,  121,   43,
 -127,  -19,  122,   37,
 -127,  -16,  124,   31,
 -127,  -13,  126,   25,
 -128,   -9,  127,   19,
 -128,   -6,  127,   13,
 -128,   -3,  128,    6
 };

void setup() {
  // put your setup code here, to run once:
  while(!Serial);
  Serial.println("test_RFFT");
  rfft.init();
  uint32_t t0,t1;


  for(int ii=0; ii<2*NN; ii++) xx[ii]=0;
  xx[2]=1<<15;
  xx[2*2+1]=1<<15;
  t0=micros();
  rfft.exec_256(xx, yy);
  t1=micros();
  Serial.printf("%d %d %d\n\r",t0,t1,t1-t0);
  for(int ii=0;ii<NN/2; ii++) 
  { Serial.printf("%5d %5d %10d %5d",
                yy[4*ii+0],yy[4*ii+1],yy[4*ii+2],yy[4*ii+3]);
    Serial.printf("%15d %5d %10d %5d\n\r",
                zz[4*ii+0],zz[4*ii+1],zz[4*ii+2],zz[4*ii+3]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}


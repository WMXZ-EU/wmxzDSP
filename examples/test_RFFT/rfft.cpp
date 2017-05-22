// rfft.cpp

#include "rfft.h"

// have to give twiddleCoef_256_q15 here because Arduino does not find it
// even if it is in arm_common_table.c
const q15_t twiddleCoef_256_q15[384] = {
  (q15_t)0x7FFF, (q15_t)0x0000, (q15_t)0x7FF6, (q15_t)0x0324, 
  (q15_t)0x7FD8, (q15_t)0x0647, (q15_t)0x7FA7, (q15_t)0x096A, 
  (q15_t)0x7F62, (q15_t)0x0C8B, (q15_t)0x7F09, (q15_t)0x0FAB, 
  (q15_t)0x7E9D, (q15_t)0x12C8, (q15_t)0x7E1D, (q15_t)0x15E2, 
  (q15_t)0x7D8A, (q15_t)0x18F8, (q15_t)0x7CE3, (q15_t)0x1C0B, 
  (q15_t)0x7C29, (q15_t)0x1F19, (q15_t)0x7B5D, (q15_t)0x2223, 
  (q15_t)0x7A7D, (q15_t)0x2528, (q15_t)0x798A, (q15_t)0x2826, 
  (q15_t)0x7884, (q15_t)0x2B1F, (q15_t)0x776C, (q15_t)0x2E11, 
  (q15_t)0x7641, (q15_t)0x30FB, (q15_t)0x7504, (q15_t)0x33DE, 
  (q15_t)0x73B5, (q15_t)0x36BA, (q15_t)0x7255, (q15_t)0x398C, 
  (q15_t)0x70E2, (q15_t)0x3C56, (q15_t)0x6F5F, (q15_t)0x3F17, 
  (q15_t)0x6DCA, (q15_t)0x41CE, (q15_t)0x6C24, (q15_t)0x447A, 
  (q15_t)0x6A6D, (q15_t)0x471C, (q15_t)0x68A6, (q15_t)0x49B4, 
  (q15_t)0x66CF, (q15_t)0x4C3F, (q15_t)0x64E8, (q15_t)0x4EBF, 
  (q15_t)0x62F2, (q15_t)0x5133, (q15_t)0x60EC, (q15_t)0x539B, 
  (q15_t)0x5ED7, (q15_t)0x55F5, (q15_t)0x5CB4, (q15_t)0x5842, 
  (q15_t)0x5A82, (q15_t)0x5A82, (q15_t)0x5842, (q15_t)0x5CB4, 
  (q15_t)0x55F5, (q15_t)0x5ED7, (q15_t)0x539B, (q15_t)0x60EC, 
  (q15_t)0x5133, (q15_t)0x62F2, (q15_t)0x4EBF, (q15_t)0x64E8, 
  (q15_t)0x4C3F, (q15_t)0x66CF, (q15_t)0x49B4, (q15_t)0x68A6, 
  (q15_t)0x471C, (q15_t)0x6A6D, (q15_t)0x447A, (q15_t)0x6C24, 
  (q15_t)0x41CE, (q15_t)0x6DCA, (q15_t)0x3F17, (q15_t)0x6F5F, 
  (q15_t)0x3C56, (q15_t)0x70E2, (q15_t)0x398C, (q15_t)0x7255, 
  (q15_t)0x36BA, (q15_t)0x73B5, (q15_t)0x33DE, (q15_t)0x7504, 
  (q15_t)0x30FB, (q15_t)0x7641, (q15_t)0x2E11, (q15_t)0x776C, 
  (q15_t)0x2B1F, (q15_t)0x7884, (q15_t)0x2826, (q15_t)0x798A, 
  (q15_t)0x2528, (q15_t)0x7A7D, (q15_t)0x2223, (q15_t)0x7B5D, 
  (q15_t)0x1F19, (q15_t)0x7C29, (q15_t)0x1C0B, (q15_t)0x7CE3, 
  (q15_t)0x18F8, (q15_t)0x7D8A, (q15_t)0x15E2, (q15_t)0x7E1D, 
  (q15_t)0x12C8, (q15_t)0x7E9D, (q15_t)0x0FAB, (q15_t)0x7F09, 
  (q15_t)0x0C8B, (q15_t)0x7F62, (q15_t)0x096A, (q15_t)0x7FA7, 
  (q15_t)0x0647, (q15_t)0x7FD8, (q15_t)0x0324, (q15_t)0x7FF6, 
  (q15_t)0x0000, (q15_t)0x7FFF, (q15_t)0xFCDB, (q15_t)0x7FF6, 
  (q15_t)0xF9B8, (q15_t)0x7FD8, (q15_t)0xF695, (q15_t)0x7FA7, 
  (q15_t)0xF374, (q15_t)0x7F62, (q15_t)0xF054, (q15_t)0x7F09, 
  (q15_t)0xED37, (q15_t)0x7E9D, (q15_t)0xEA1D, (q15_t)0x7E1D, 
  (q15_t)0xE707, (q15_t)0x7D8A, (q15_t)0xE3F4, (q15_t)0x7CE3, 
  (q15_t)0xE0E6, (q15_t)0x7C29, (q15_t)0xDDDC, (q15_t)0x7B5D, 
  (q15_t)0xDAD7, (q15_t)0x7A7D, (q15_t)0xD7D9, (q15_t)0x798A, 
  (q15_t)0xD4E0, (q15_t)0x7884, (q15_t)0xD1EE, (q15_t)0x776C, 
  (q15_t)0xCF04, (q15_t)0x7641, (q15_t)0xCC21, (q15_t)0x7504, 
  (q15_t)0xC945, (q15_t)0x73B5, (q15_t)0xC673, (q15_t)0x7255, 
  (q15_t)0xC3A9, (q15_t)0x70E2, (q15_t)0xC0E8, (q15_t)0x6F5F, 
  (q15_t)0xBE31, (q15_t)0x6DCA, (q15_t)0xBB85, (q15_t)0x6C24, 
  (q15_t)0xB8E3, (q15_t)0x6A6D, (q15_t)0xB64B, (q15_t)0x68A6, 
  (q15_t)0xB3C0, (q15_t)0x66CF, (q15_t)0xB140, (q15_t)0x64E8, 
  (q15_t)0xAECC, (q15_t)0x62F2, (q15_t)0xAC64, (q15_t)0x60EC, 
  (q15_t)0xAA0A, (q15_t)0x5ED7, (q15_t)0xA7BD, (q15_t)0x5CB4, 
  (q15_t)0xA57D, (q15_t)0x5A82, (q15_t)0xA34B, (q15_t)0x5842, 
  (q15_t)0xA128, (q15_t)0x55F5, (q15_t)0x9F13, (q15_t)0x539B, 
  (q15_t)0x9D0D, (q15_t)0x5133, (q15_t)0x9B17, (q15_t)0x4EBF, 
  (q15_t)0x9930, (q15_t)0x4C3F, (q15_t)0x9759, (q15_t)0x49B4, 
  (q15_t)0x9592, (q15_t)0x471C, (q15_t)0x93DB, (q15_t)0x447A, 
  (q15_t)0x9235, (q15_t)0x41CE, (q15_t)0x90A0, (q15_t)0x3F17, 
  (q15_t)0x8F1D, (q15_t)0x3C56, (q15_t)0x8DAA, (q15_t)0x398C, 
  (q15_t)0x8C4A, (q15_t)0x36BA, (q15_t)0x8AFB, (q15_t)0x33DE, 
  (q15_t)0x89BE, (q15_t)0x30FB, (q15_t)0x8893, (q15_t)0x2E11, 
  (q15_t)0x877B, (q15_t)0x2B1F, (q15_t)0x8675, (q15_t)0x2826, 
  (q15_t)0x8582, (q15_t)0x2528, (q15_t)0x84A2, (q15_t)0x2223, 
  (q15_t)0x83D6, (q15_t)0x1F19, (q15_t)0x831C, (q15_t)0x1C0B, 
  (q15_t)0x8275, (q15_t)0x18F8, (q15_t)0x81E2, (q15_t)0x15E2, 
  (q15_t)0x8162, (q15_t)0x12C8, (q15_t)0x80F6, (q15_t)0x0FAB, 
  (q15_t)0x809D, (q15_t)0x0C8B, (q15_t)0x8058, (q15_t)0x096A, 
  (q15_t)0x8027, (q15_t)0x0647, (q15_t)0x8009, (q15_t)0x0324, 
  (q15_t)0x8000, (q15_t)0x0000, (q15_t)0x8009, (q15_t)0xFCDB, 
  (q15_t)0x8027, (q15_t)0xF9B8, (q15_t)0x8058, (q15_t)0xF695, 
  (q15_t)0x809D, (q15_t)0xF374, (q15_t)0x80F6, (q15_t)0xF054, 
  (q15_t)0x8162, (q15_t)0xED37, (q15_t)0x81E2, (q15_t)0xEA1D, 
  (q15_t)0x8275, (q15_t)0xE707, (q15_t)0x831C, (q15_t)0xE3F4, 
  (q15_t)0x83D6, (q15_t)0xE0E6, (q15_t)0x84A2, (q15_t)0xDDDC, 
  (q15_t)0x8582, (q15_t)0xDAD7, (q15_t)0x8675, (q15_t)0xD7D9, 
  (q15_t)0x877B, (q15_t)0xD4E0, (q15_t)0x8893, (q15_t)0xD1EE, 
  (q15_t)0x89BE, (q15_t)0xCF04, (q15_t)0x8AFB, (q15_t)0xCC21, 
  (q15_t)0x8C4A, (q15_t)0xC945, (q15_t)0x8DAA, (q15_t)0xC673, 
  (q15_t)0x8F1D, (q15_t)0xC3A9, (q15_t)0x90A0, (q15_t)0xC0E8, 
  (q15_t)0x9235, (q15_t)0xBE31, (q15_t)0x93DB, (q15_t)0xBB85, 
  (q15_t)0x9592, (q15_t)0xB8E3, (q15_t)0x9759, (q15_t)0xB64B, 
  (q15_t)0x9930, (q15_t)0xB3C0, (q15_t)0x9B17, (q15_t)0xB140, 
  (q15_t)0x9D0D, (q15_t)0xAECC, (q15_t)0x9F13, (q15_t)0xAC64, 
  (q15_t)0xA128, (q15_t)0xAA0A, (q15_t)0xA34B, (q15_t)0xA7BD, 
  (q15_t)0xA57D, (q15_t)0xA57D, (q15_t)0xA7BD, (q15_t)0xA34B, 
  (q15_t)0xAA0A, (q15_t)0xA128, (q15_t)0xAC64, (q15_t)0x9F13, 
  (q15_t)0xAECC, (q15_t)0x9D0D, (q15_t)0xB140, (q15_t)0x9B17, 
  (q15_t)0xB3C0, (q15_t)0x9930, (q15_t)0xB64B, (q15_t)0x9759, 
  (q15_t)0xB8E3, (q15_t)0x9592, (q15_t)0xBB85, (q15_t)0x93DB, 
  (q15_t)0xBE31, (q15_t)0x9235, (q15_t)0xC0E8, (q15_t)0x90A0, 
  (q15_t)0xC3A9, (q15_t)0x8F1D, (q15_t)0xC673, (q15_t)0x8DAA, 
  (q15_t)0xC945, (q15_t)0x8C4A, (q15_t)0xCC21, (q15_t)0x8AFB, 
  (q15_t)0xCF04, (q15_t)0x89BE, (q15_t)0xD1EE, (q15_t)0x8893, 
  (q15_t)0xD4E0, (q15_t)0x877B, (q15_t)0xD7D9, (q15_t)0x8675, 
  (q15_t)0xDAD7, (q15_t)0x8582, (q15_t)0xDDDC, (q15_t)0x84A2, 
  (q15_t)0xE0E6, (q15_t)0x83D6, (q15_t)0xE3F4, (q15_t)0x831C, 
  (q15_t)0xE707, (q15_t)0x8275, (q15_t)0xEA1D, (q15_t)0x81E2, 
  (q15_t)0xED37, (q15_t)0x8162, (q15_t)0xF054, (q15_t)0x80F6, 
  (q15_t)0xF374, (q15_t)0x809D, (q15_t)0xF695, (q15_t)0x8058, 
  (q15_t)0xF9B8, (q15_t)0x8027, (q15_t)0xFCDB, (q15_t)0x8009
};


/*
const arm_cfft_instance_q15 arm_cfft_sR_q15_len256 = {
  256, twiddleCoef_256_q15, armBitRevIndexTable_fixed_256, ARMBITREVINDEXTABLE_FIXED__256_TABLE_LENGTH
};
*/
#include "arduino.h"
#include "usb_serial.h"
void c_rfft::init(void)
{
  S_arm_cfft_q15 = (arm_cfft_instance_q15 *) &arm_cfft_sR_q15_len256;
  return;
}

// dual channel rfft function (forward or real to complex)
void c_rfft::exec_256(short *xx, short *yy)
{
// xx input vector length 2*n
// xx[0:2:2*n-2] first time series
// xx[1:2:2*n-1] second time series
// assume n=4^m  or m=log2(n)/log2(4)
//
// yy[0:4:2*n-2] first spectrum (real,imag interleaved) {yy[0], yy[1], yy[4], yy[5] ...}
// yy[2:4:2*n-1] second spectrum (real, imag interleaved) {yy[2], yy[3], yy[6], yy[7] ...}

// size(xx) = 2*nn shorts = 4*nn bytes
// size(yy) = 2*nn shorts = 4*nn bytes

	// separate two spectra
	// Fn = (Hn + conj(HN-n)/2
	// Gn = -i(Hn - conj(HN-n))/2
	//top
	// u1r(ii) = (yr(ii)+yr(jj))/2
	// u1i(ii) = (yi(ii)-yi(jj))/2
	//bot
	// u2r(ii) = (yi(ii)+yi(jj))/2
	// u2i(ii) = -(yr(ii)-yr(jj))/2
	// or
	// u2r(ii) = (yi(ii)+yi(jj))/2
	// u2i(ii) = (yr(jj)-yr(ii))/2
	// 
	//in1 = ((uint32_t) in1 >> 16) | ((uint32_t) in1 << 16);

	int nn;
	nn=256; //have a 256 point complex FFT
	arm_cfft_q15(S_arm_cfft_q15, xx,0,1);
	
	yy[0]=- xx[0]; yy[1]=0;
	yy[2]=- xx[1]; yy[3]=0;

	int *rna=(int*) &xx[2];       // points to first non-dc frequency pair (re + im)
	int *rma=(int*) &xx[2*nn-2];  // points to last frequency pair (re + im)
	int *rr1 = (int*)&yy[4];      // points to first non-dc frequency pair chan 1
	int *rr2 = (int*)&yy[4+2];    // points to first non-dc frequency pair chan 2

	for(int ii=1; ii<nn/2-3; ii+=4) // there are nn/2 frequencies
	{ int rn,rm,rm1;
		//
		rn = *rna;
		rm = *rma;
		rm1=__ROR(rm,16);
		*rr1= - __SHSAX(rn,rm1);
		*rr2= - __SHSAX(rm1,rn);
		rna++;
		rma--;
		rr1+=2;
		rr2+=2;
		//
		rn = *rna;
		rm = *rma;
		rm1=__ROR(rm,16);
		*rr1= - __SHSAX(rn,rm1);
		*rr2= - __SHSAX(rm1,rn);
		rna++;
		rma--;
		rr1+=2;
		rr2+=2;
		//
		rn = *rna;
		rm = *rma;
		rm1=__ROR(rm,16);
		*rr1= - __SHSAX(rn,rm1);
		*rr2= - __SHSAX(rm1,rn);
		rna++;
		rma--;
		rr1+=2;
		rr2+=2;
		//
		rn = *rna;
		rm = *rma;
		rm1=__ROR(rm,16);
		*rr1= - __SHSAX(rn,rm1);
		*rr2= - __SHSAX(rm1,rn);
		rna++;
		rma--;
		rr1+=2;
		rr2+=2;
	}
  // final three frequencies
    int rn,rm,rm1;
    rn = *rna;
    rm = *rma;
    rm1=__ROR(rm,16);
    *rr1= - __SHSAX(rn,rm1);
    *rr2= - __SHSAX(rm1,rn);
    rna++;
    rma--;
    rr1+=2;
    rr2+=2;
    //
    rn = *rna;
    rm = *rma;
    rm1=__ROR(rm,16);
    *rr1= - __SHSAX(rn,rm1);
    *rr2= - __SHSAX(rm1,rn);
    rna++;
    rma--;
    rr1+=2;
    rr2+=2;
    //
    rn = *rna;
    rm = *rma;
    rm1=__ROR(rm,16);
    *rr1= - __SHSAX(rn,rm1);
    *rr2= - __SHSAX(rm1,rn);
    rna++;
    rma--;
    rr1+=2;
    rr2+=2;
   
	// yy1 is n words long (n/2 complex spectra)
	// yy2 is n words long (n/2 complex spectra)
}




// for signalProcess working storage
#define NBUF 256
static float  yy[NBUF];
static float  vv[NBUF];

/*
 *
 */
#define HPF 4 // HP filter length
#define LPF 4 // LP filter length
void TaegerKaiser(float *out, float *inp)
{ int ii;
	// IIR HP filter (2nd order) // HPF = 2
    // [b,a]=butter(2,0.5,'high')
	// HPF 3-4 are higher order butter filter
	#define A1 (0.2929f)
	#define B1 (0.1716f)

	#define A2 (0.1667f)
	#define B2 (0.3333f)

	#define A3 (0.0940f)
	#define B3 (0.4680f)
	#define C3 (0.0177f)

	// LP filter (1st order)
	// [b,a]=butter(1,0.1,'low');
	#define A4 (0.1367f)
	#define B4 (0.7265f)
	
#if HPF==2
	yy[0] =inp[0];
	yy[1] =(inp[1]-inp[0])*0.5f;
	for(ii=2;ii<NBUF-2; )
	{	yy[ii]  =(inp[ii] - 2.0f*inp[ii-1] + inp[ii-2])*A1  	- yy[ii-2]*B1; ii++;
		yy[ii]  =(inp[ii] - 2.0f*inp[ii-1] + inp[ii-2])*A1  	- yy[ii-2]*B1; ii++;
		yy[ii]  =(inp[ii] - 2.0f*inp[ii-1] + inp[ii-2])*A1  	- yy[ii-2]*B1; ii++;
		yy[ii]  =(inp[ii] - 2.0f*inp[ii-1] + inp[ii-2])*A1  	- yy[ii-2]*B1; ii++;
    }
	// mow missing last two filters
	yy[ii]  =(inp[ii] - inp[ii-1] - inp[ii-1 ] + inp[ii-2])*A1  	- yy[ii-2]*B1; ii++;
	yy[ii]  =(inp[ii] - inp[ii-1] - inp[ii-1 ] + inp[ii-2])*A1  	- yy[ii-2]*B1; ii++;

#elif HPF==3
	yy[0] =inp[0];
	yy[1] =(inp[1]-inp[0])*0.5f;
	yy[2] =(inp[2] - 2.0f*inp[1] + inp[0])*A1  	- yy[0]*B1;

	for(ii=3;ii<NBUF-1; )
	{	yy[ii]  =(inp[ii] - 3.0f*inp[ii-1] + 3.0f*inp[ii-2] - inp[ii-3])*A2  	- yy[ii-2]*B2; ii++;
		yy[ii]  =(inp[ii] - 3.0f*inp[ii-1] + 3.0f*inp[ii-2] - inp[ii-3])*A2  	- yy[ii-2]*B2; ii++;
		yy[ii]  =(inp[ii] - 3.0f*inp[ii-1] + 3.0f*inp[ii-2] - inp[ii-3])*A2  	- yy[ii-2]*B2; ii++;
		yy[ii]  =(inp[ii] - 3.0f*inp[ii-1] + 3.0f*inp[ii-2] - inp[ii-3])*A2  	- yy[ii-2]*B2; ii++;
    }
	// mow missing last filter
	yy[ii] =(inp[ii] - 3.0f*inp[ii-1] + 3.0*inp[ii-2] - inp[ii-4])*A2  	- yy[ii-2]*B2; ii++;

#elif HPF==4
	yy[0] = inp[0];
	yy[1] =(inp[1] - inp[0])*0.5f;
	yy[2] =(inp[2] - 2.0f*inp[1] + inp[0])*A1  	- yy[0]*B1;
	yy[3] =(inp[3] - 3.0f*inp[2] + 3.0f*inp[1] - inp[0])*A2  	- yy[1]*B2; 

	for(ii=4;ii<NBUF; )
	{	yy[ii]  =(inp[ii] - 4.0f*inp[ii-1] + 6.0f*inp[ii-2] -  4.0f*inp[ii-3] + inp[ii-4])*A3
					- yy[ii-2]*B3  - yy[ii-4]*C3; ii++;
		yy[ii]  =(inp[ii] - 4.0f*inp[ii-1] + 6.0f*inp[ii-2] -  4.0f*inp[ii-3] + inp[ii-4])*A3
					- yy[ii-2]*B3  - yy[ii-4]*C3; ii++;
		yy[ii]  =(inp[ii] - 4.0f*inp[ii-1] + 6.0f*inp[ii-2] -  4.0f*inp[ii-3] + inp[ii-4])*A3
					- yy[ii-2]*B3  - yy[ii-4]*C3; ii++;
		yy[ii]  =(inp[ii] - 4.0f*inp[ii-1] + 6.0f*inp[ii-2] -  4.0f*inp[ii-3] + inp[ii-4])*A3
					- yy[ii-2]*B3  - yy[ii-4]*C3; ii++;
    }
#endif

	// TKO
	vv[0]=0;
	for(ii=1;ii<NBUF-3; )
	{	vv[ii]  =yy[ii]*yy[ii] - yy[ii-1]*yy[ii+1]; ii++;
		vv[ii]  =yy[ii]*yy[ii] - yy[ii-1]*yy[ii+1]; ii++;
		vv[ii]  =yy[ii]*yy[ii] - yy[ii-1]*yy[ii+1]; ii++;
		vv[ii]  =yy[ii]*yy[ii] - yy[ii-1]*yy[ii+1]; ii++;
	}
	// now missing last three TKO
	vv[ii]  =yy[ii]*yy[ii] - yy[ii-1]*yy[ii+1]; ii++;
	vv[ii]  =yy[ii]*yy[ii] - yy[ii-1]*yy[ii+1]; ii++;
	vv[ii]	=0;

	// LP filter (1st order)
	out[0] =vv[0];
	for(ii=1;ii<NBUF-3;)
	{	out[ii]  =(vv[ii] + vv[ii-1])*A4   - out[ii-1]*B4; ii++;
		out[ii]  =(vv[ii] + vv[ii-1])*A4   - out[ii-1]*B4; ii++;
		out[ii]  =(vv[ii] + vv[ii-1])*A4   - out[ii-1]*B4; ii++;
		out[ii]  =(vv[ii] + vv[ii-1])*A4   - out[ii-1]*B4; ii++;
	}
	// mow missing last three filters
	out[ii]  =(vv[ii] + vv[ii-1])*A4   - out[ii-1]*B4; ii++;
	out[ii]  =(vv[ii] + vv[ii-1])*A4   - out[ii-1]*B4; ii++;
	out[ii]  =(vv[ii] + vv[ii-1])*A4   - out[ii-1]*B4; ii++;

}


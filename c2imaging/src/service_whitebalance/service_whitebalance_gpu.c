/*service_whitebalance_gpu.c
 * Shades of Gray Algorithm with Minkowski p norm Method
 * Algorithm 3 from http://www.stanford.edu/class/ee368/Project_11/Reports/Cohen_A_New_Color_Balancing_Method.pdf
 * Algorithm works only for 0<p<8, for higher values the range of float is to small.
 * */
#include "service_whitebalance.h"

int32_t run_service_whitebalance_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t p){
	int32_t i, j, pos;
	int32_t tempr;
	int32_t tempg;
	int32_t tempb;
	float Rm = 0.0;
	float Gm = 0.0;
	float Bm = 0.0;
	float Sr = 0.0;
	float Sg = 0.0;
	float Sb = 0.0;
	float max=0.0;
	
	//check the parameter
	if(p<1 || p > 7){
		fprintf(stderr,"Wrong parameter for whitebalance: %d\np should be between 1 and 7.\n", p);
		return -1;
	}
	
	float p_fl = (1.0/(float)p);
	
	#pragma acc kernels copyin(pixpack_source[0:columns*rows], Rm, Gm, Bm, Sr, Sg, Sb) copyout(pixpack_target[0:columns*rows])
	{
		#pragma acc loop independent reduction(+: Rm) reduction( +: Gm) reduction(+: Bm)
		for (i=0; i<rows; ++i) { //summation of all pixel to the power of p
			#pragma acc loop independent private(pos, Rm, Gm, Bm)
			for (j=0; j<columns; ++j) {
				pos = i*columns+j;
				Rm += powf((float)pixpack_source[pos].red, (float)p);
				Gm += powf((float)pixpack_source[pos].green, (float)p);
				Bm += powf((float)pixpack_source[pos].blue, (float)p);
			}
		}

		Rm = powf((Rm/(columns*rows)),p_fl);
		Gm = powf((Gm/(columns*rows)),p_fl);
		Bm = powf((Bm/(columns*rows)),p_fl);

		if(Rm>=Gm && Rm>=Bm) //max=max(Rm, Gm, Bm)
			max = Rm;
		else if(Gm>=Rm && Gm>=Bm)
			max = Gm;
		else
			max = Bm;
		
		//scaling to the maximum channel

		Sr = max/Rm;
		Sg = max/Gm;
		Sb = max/Bm;

		#pragma acc loop independent
		for (i=0; i<rows; ++i) {
			#pragma acc loop independent private(pos, tempr, tempg, tempb) 
			for (j=0; j<columns; ++j) {
				pos = i*columns+j;
				//calculate new rgb values
				tempr = (int32_t)(Sr*pixpack_source[pos].red);
				tempg = (int32_t)(Sg*pixpack_source[pos].green);
				tempb = (int32_t)(Sb*pixpack_source[pos].blue);

				//check new values
				pixpack_target[pos].red		= (tempr>65535) ? 65535 : tempr;
				pixpack_target[pos].green	= (tempg>65535) ? 65535 : tempg;
				pixpack_target[pos].blue	= (tempb>65535) ? 65535 : tempb;
				pixpack_target[pos].opacity	= 0;
			}
		}
	}
	
	return 0;
}

/*service_whitebalance_cpu.c
 * Shades of Gray Algorithm with Minkowski p norm Method
 * Algorithm 3 from http://www.stanford.edu/class/ee368/Project_11/Reports/Cohen_A_New_Color_Balancing_Method.pdf
 * Algorithm works only for 0<p<8, for higher values the range of float is to small.
 * */
#include "service_whitebalance.h"

int32_t run_service_whitebalance_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t p) {
	int32_t i, j, pos;
	int32_t temprgb[3];
	float Rm = 0;
	float Gm = 0;
	float Bm = 0;
	float Sr, Sg, Sb;
	float max = 0;
	float p_fl;

	//check the parameter
	if (p < 1 || p > 7) {
		fprintf(stderr, "Wrong parameter for whitebalance: %d\np should be between 1 and 7.\n", p);
		return -1;
	}
	p_fl = (1 / (float)p);

	#pragma omp parallel
	{
		#pragma omp for private(pos, i, j) reduction(+: Rm) reduction( +: Gm) reduction(+: Bm)
		for (i = 0; i < rows; ++i) { //summation of all pixel to the power of p
			for (j = 0; j < columns; ++j) {
				pos = i * columns + j;
				Rm += pow(pixpack_source[pos].red, p);
				Gm += pow(pixpack_source[pos].green, p);
				Bm += pow(pixpack_source[pos].blue, p);
			}
		}
	}

	Rm = pow((Rm / (columns * rows)), p_fl);
	Gm = pow((Gm / (columns * rows)), p_fl);
	Bm = pow((Bm / (columns * rows)), p_fl);

	if (Rm >= Gm && Rm >= Bm) //max=max(Rm, Gm, Bm)
		max = Rm;
	else {
		if (Gm >= Rm && Gm >= Bm)
			max = Gm;
		else
			max = Bm;
	}
	//scaling to the maximum channel
	Sr = max / Rm;
	Sg = max / Gm;
	Sb = max / Bm;
	#pragma omp parallel
	{
		#pragma omp for private(pos, i, j, temprgb)
		for (i = 0; i < rows; ++i) {
			for (j = 0; j < columns; ++j) {
				pos = i * columns + j;
				//calculate new rgb values
				temprgb[0] = (Sr * pixpack_source[pos].red);
				temprgb[1] = (Sg * pixpack_source[pos].green);
				temprgb[2] = (Sb * pixpack_source[pos].blue);

				//check new values
				if (temprgb[0] > 65535)
					pixpack_target[pos].red = 65535;
				else
					pixpack_target[pos].red = temprgb[0];
				if (temprgb[1] > 65535)
					pixpack_target[pos].green = 65535;
				else
					pixpack_target[pos].green = temprgb[1];
				if (temprgb[2] > 65535)
					pixpack_target[pos].blue = 65535;
				else
					pixpack_target[pos].blue = temprgb[2];
				pixpack_target[pos].opacity	= 0;
			}
		}
	}
	return 0;
}



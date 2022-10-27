/*service_whitebalance.c
 * Shades of Gray Algorithm with Minkowski p norm Method
 * Algorithm 3 from http://www.stanford.edu/class/ee368/Project_11/Reports/Cohen_A_New_Color_Balancing_Method.pdf
 * Algorithm works only for 0<p<8, for higher values the range of float is to small.
 * */

#include "service_whitebalance.h"

int32_t run_service_whitebalance_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t p) {
	float Rm = 0;
	float Gm = 0;
	float Bm = 0;

	float Sr, Sg, Sb;
	float max = 0;
	float p_fl;
	int i = 0;
	uint32_t burst_size_in_bytes	= 384;
	uint32_t burst_size				= 384 / sizeof(PixelPacket);
	uint32_t pic_size				= columns * rows;
	uint32_t alignedPicSize;
	PixelPacket *pixpack_temp;
	
	if((columns*rows)%burst_size == 0){
		alignedPicSize = pic_size;
	} else {
		alignedPicSize	= ((columns * rows) / burst_size + 1) * burst_size;
	}

	float *Rm1 = malloc(sizeof(float));
	float *Gm1 = malloc(sizeof(float));
	float *Bm1 = malloc(sizeof(float));
	
	if(alignedPicSize != pic_size){
		pixpack_temp = malloc(alignedPicSize * sizeof(PixelPacket));
		if (NULL == pixpack_temp) {
			fprintf(stderr, "Couldn't allocate memory. (file: %s, line: %i)\n", __FILE__, __LINE__);
			return -1;
		}

		memcpy(pixpack_temp, pixpack_source, pic_size * sizeof(PixelPacket));
	}

	//check the parameter
	if (p < 1 || p > 7) {
		fprintf(stderr, "Wrong parameter for whitebalance: %d\np should be between 1 and 7.\n", p);
		return -1;
	}
	p_fl = (1 / (float)p);

	void *engine = NULL;

	//summation of all pixel to the power of p
	if(alignedPicSize != pic_size){
		engine = run_service_whitebalance_max_k1((uint16_t *) pixpack_temp, rows, columns, alignedPicSize, (float) p, Rm1, Gm1, Bm1);
	} else {
		engine = run_service_whitebalance_max_k1((uint16_t *) pixpack_source, rows, columns, alignedPicSize, (float) p, Rm1, Gm1, Bm1);
	}

	Rm = pow((*Rm1 / (columns * rows)), p_fl);
	Gm = pow((*Gm1 / (columns * rows)), p_fl);
	Bm = pow((*Bm1 / (columns * rows)), p_fl);

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

	run_service_whitebalance_max_k2(engine, (uint16_t *) pixpack_target, rows, columns, alignedPicSize, Sr, Sg, Sb);

	free(Rm1);
	free(Gm1);
	free(Bm1);
	if(alignedPicSize != pic_size){
		free(pixpack_temp);
	}
	return 0;
}



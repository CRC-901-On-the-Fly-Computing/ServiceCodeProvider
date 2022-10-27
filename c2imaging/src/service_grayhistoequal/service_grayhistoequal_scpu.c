/* service_grayhistoequal_scpu.c */

#include "service_grayhistoequal.h"

int32_t run_service_grayhistoequal_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	uint32_t histogram[QuantumRange+1] = {0};
	
	Quantum max = 0, min = QuantumRange;
	
	int32_t i, j, pos;
	for (i=0; i<rows; ++i) {
		for (j=0; j<columns; ++j) {
			pos = i*columns+j;
			pixpack_target[pos].red = (pixpack_source[pos].red + pixpack_source[pos].green + pixpack_source[pos].blue)/3;
			max = pixpack_target[pos].red > max ? pixpack_target[pos].red : max;
			min = pixpack_target[pos].red < min ? pixpack_target[pos].red : min;
			histogram[pixpack_target[pos].red]++;
		}
	}
	
#if 1
	//printf("old min: %5i, old max: %5i\n", min, max);
	
	// 100/quantil %
	int32_t quantil = 5;
	int32_t temp = columns * rows / quantil;
	pos = min;
	while (temp > 0) {
		temp -= histogram[pos++];
	}
	min = --pos;
	
	temp = columns * rows / quantil;
	pos = max;
	while (temp > 0) {
		temp -= histogram[pos--];
	}
	max = ++pos;
	
	//printf("new min: %5i, new max: %5i\n", min, max);
#endif
	
	if (max > min) {
		for (i=0; i<rows; ++i) {
			for (j=0; j<columns; ++j) {
				pos = i*columns+j;
				pixpack_target[pos].red = (pixpack_target[pos].red < min) ? min : ((pixpack_target[pos].red > max) ? max : pixpack_target[pos].red);
				
				pixpack_target[pos].red		= (uint64_t)QuantumRange * (pixpack_target[pos].red-min) / (max-min);
				pixpack_target[pos].green	= pixpack_target[pos].red;
				pixpack_target[pos].blue	= pixpack_target[pos].red;
				pixpack_target[pos].opacity	= 0;
			}
		}
	} else {
		for (i=0; i<rows; ++i) {
			for (j=0; j<columns; ++j) {
				pos = i*columns+j;
				pixpack_target[pos].green	= pixpack_target[pos].red;
				pixpack_target[pos].blue	= pixpack_target[pos].red;
				pixpack_target[pos].opacity	= 0;
			}
		}
	}
	
	return 0;
}

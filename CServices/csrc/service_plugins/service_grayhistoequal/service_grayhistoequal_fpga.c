/* service_grayhistoequal_fpga.c */

#include "service_grayhistoequal.h"

int32_t run_service_grayhistoequal_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	uint32_t histogram[QuantumRange+1] = {0};
	
	int32_t pic_size = columns * rows;
	
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
	int32_t temp = pic_size / quantil;
	pos = min;
	while (temp > 0) {
		temp -= histogram[pos++];
	}
	min = --pos;
	
	temp = pic_size / quantil;
	pos = max;
	while (temp > 0) {
		temp -= histogram[pos--];
	}
	max = ++pos;
	
	//printf("new min: %5i, new max: %5i\n", min, max);
#endif
	
	if (max > min) {
		run_service_grayhistoequal_max((uint16_t *)pixpack_target, (uint16_t *)pixpack_source, (uint32_t)pic_size, (uint16_t)min, (uint16_t)max);
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

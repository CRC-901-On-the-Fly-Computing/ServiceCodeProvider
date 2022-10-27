/* service_dim_scpu.c */

#include "service_dim.h"

int32_t run_service_dim_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	int32_t i, j, pos;
	
	for (i=0; i<rows; ++i) {
		for (j=0; j<columns; ++j) {
			pos = i*columns+j;
			pixpack_target[pos].red		= pixpack_source[pos].red		>> 2;
			pixpack_target[pos].green	= pixpack_source[pos].green		>> 2;
			pixpack_target[pos].blue	= pixpack_source[pos].blue		>> 2;
			pixpack_target[pos].opacity	= 0;
		}
	}
	
	return 0;
}

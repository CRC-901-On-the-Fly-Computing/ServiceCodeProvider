/* service_negative_cpu.c */

#include "service_negative.h"

run_service_negative_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	int32_t i, j, pos;
	
	#pragma omp parallel
	{
		#pragma omp for private(j, pos)
		for (i=0; i<rows; ++i) {
			for (j=0; j<columns; ++j) {
				pos = i*columns+j;
				pixpack_target[pos].red		= 65535 - pixpack_source[pos].red		;
				pixpack_target[pos].green	= 65535 - pixpack_source[pos].green	;
				pixpack_target[pos].blue	= 65535 - pixpack_source[pos].blue	;
				pixpack_target[pos].opacity	= 0;
			}
		}
	}
	
	return 0;
}

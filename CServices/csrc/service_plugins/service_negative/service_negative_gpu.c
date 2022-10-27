/* service_negative_gpu.c */

#include "service_negative.h"

int32_t run_service_negative_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	int32_t i, j, pos;
	#pragma acc kernels copyin(pixpack_source[0:columns*rows]) copyout(pixpack_target[0:columns*rows])
	{
		#pragma acc loop independent
		for (i=0; i<rows; ++i) {
			#pragma acc loop independent private(pos)
			for (j=0; j<columns; ++j) {
				pos = i*columns+j;
				pixpack_target[pos].red	= 65535 - pixpack_source[pos].red	;
				pixpack_target[pos].green	= 65535 - pixpack_source[pos].green	;
				pixpack_target[pos].blue	= 65535 - pixpack_source[pos].blue	;
				pixpack_target[pos].opacity	= 0;
			}
		}
	}

	return 0;
}

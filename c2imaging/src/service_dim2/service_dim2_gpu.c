/* service_dim2_gpu.c */

#include "service_dim2.h"

int32_t run_service_dim2_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float factor) {
	int32_t i, j, pos, tempred, tempgreen, tempblue;
	
	if (factor < 0) {
		fprintf(stderr, "Wrong parameter! Factor needs to be larger than 0\n");
		return -1;
	}
	
	#pragma acc kernels copyin(pixpack_source[0:columns*rows]) copyout(pixpack_target[0:columns*rows])
	{
		#pragma acc loop independent
		for (i=0; i<rows; ++i) {
			#pragma acc loop independent private(pos, tempred, tempgreen, tempblue)
			for (j=0; j<columns; ++j) {
				pos = i*columns+j;
				tempred	= pixpack_source[pos].red * factor;
				tempgreen = pixpack_source[pos].green * factor;
				tempblue = pixpack_source[pos].blue * factor;		
				
				pixpack_target[pos].red		= (tempred > 65535) ? 65535: tempred;
				pixpack_target[pos].green	= (tempgreen > 65535) ? 65535: tempgreen;
				pixpack_target[pos].blue	= (tempblue > 65535) ? 65535: tempblue;
				pixpack_target[pos].opacity	= 0;
			}
		}
	}
	
	return 0;
}

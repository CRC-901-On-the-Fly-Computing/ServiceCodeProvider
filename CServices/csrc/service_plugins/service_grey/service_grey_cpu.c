/*service_grey_cpu.c*/
 
#include "service_grey.h"


int32_t run_service_grey_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
		//Tranform RGB to Grey Picture
		int32_t pos,i,j;
		#pragma omp parallel
		{
		#pragma omp for private(pos)
			for (i=0; i<rows; ++i) {
				for (j=0; j<columns; ++j) {
					pos = i*columns+j;
					pixpack_target[pos].red = (pixpack_source[pos].red+pixpack_source[pos].green+pixpack_source[pos].blue)/3;
					pixpack_target[pos].green = (pixpack_source[pos].red+pixpack_source[pos].green+pixpack_source[pos].blue)/3;
					pixpack_target[pos].blue = (pixpack_source[pos].red+pixpack_source[pos].green+pixpack_source[pos].blue)/3;
					pixpack_target[pos].opacity	= 0;
				}
			}
		}
	return 0;
}

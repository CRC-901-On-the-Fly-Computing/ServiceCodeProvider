/* service_sobel.c */

#include "service_sobel.h"

int32_t run_service_sobel_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	int32_t i, j, pos;
	
	#pragma acc kernels copyin(pixpack_source[0:columns*rows]) copyout(pixpack_target[0:columns*rows])
	{
		#pragma acc loop independent
		for (i=0; i<rows; ++i) {
			#pragma acc loop independent private(pos)
			for (j=0; j<columns; ++j) {
				pos = i*columns+j;
				pixpack_source[pos].red = (pixpack_source[pos].red + pixpack_source[pos].green + pixpack_source[pos].blue)/3;
			}
		}
		
		#pragma acc loop independent
		for (i=1; i<rows-1; ++i) {
			int temp_red_x = 0, temp_red_y = 0;
			#pragma acc loop independent private(pos, temp_red_x, temp_red_y)
			for (j=1; j<columns-1; ++j) {
				pos = i*columns+j;
				temp_red_x	=	-1*pixpack_source[pos-columns-1].red
							+	-2*pixpack_source[pos-columns].red
							+	-1*pixpack_source[pos-columns+1].red
							+	   pixpack_source[pos+columns-1].red
							+	 2*pixpack_source[pos+columns].red
							+	   pixpack_source[pos+columns+1].red;
				temp_red_y 	=	-1*pixpack_source[pos-columns-1].red
							+	   pixpack_source[pos-columns+1].red
							+	-2*pixpack_source[pos-1].red
							+	 2*pixpack_source[pos+1].red
							+	-1*pixpack_source[pos+columns-1].red
							+	   pixpack_source[pos+columns+1].red;
	#if 1
				pixpack_target[pos].red		= (abs(temp_red_x) > abs(temp_red_y)) ? abs(temp_red_x) : abs(temp_red_y);
	#else
				// don't forget to include <math.h> for sqrt() support
				pixpack_target[pos].red		= sqrt(temp_red_x*temp_red_x + temp_red_y*temp_red_y);
	#endif
				pixpack_target[pos].green	= pixpack_target[pos].red;
				pixpack_target[pos].blue	= pixpack_target[pos].red;
				pixpack_target[pos].opacity	= 0;
			}
		}
	}
	
	return 0;
}

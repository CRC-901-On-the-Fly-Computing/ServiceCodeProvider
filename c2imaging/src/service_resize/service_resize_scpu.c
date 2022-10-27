/* service_resize_scpu.c */

#include "service_resize.h"

int32_t run_service_resize_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t resize_factor) {
	int32_t temp_red, temp_green, temp_blue;
	
	int32_t i, j, k, l, pos, pos_target;
	
	int32_t new_columns	= columns/resize_factor;
	int32_t new_rows	= rows/resize_factor;
	
	for (i=0; i<rows-1; i+=2) {
		k = i>>1;
		for (j=0; j<columns-1; j+=2) {
			l			= j>>1;
			pos			= i*columns+j;
			pos_target	= k*new_columns+l;
			
			temp_red	=	pixpack_source[pos].red
						+	pixpack_source[pos+1].red
						+	pixpack_source[pos+columns].red
						+	pixpack_source[pos+columns+1].red;
			temp_green	=	pixpack_source[pos].green
						+	pixpack_source[pos+1].green
						+	pixpack_source[pos+columns].green
						+	pixpack_source[pos+columns+1].green;
			temp_blue	=	pixpack_source[pos].blue
						+	pixpack_source[pos+1].blue
						+	pixpack_source[pos+columns].blue
						+	pixpack_source[pos+columns+1].blue;
			
			pixpack_target[pos_target].red		= temp_red		>> 2;
			pixpack_target[pos_target].green	= temp_green	>> 2;
			pixpack_target[pos_target].blue		= temp_blue		>> 2;
			pixpack_target[pos_target].opacity	= 0;
		}
	}
	
	return 0;
}

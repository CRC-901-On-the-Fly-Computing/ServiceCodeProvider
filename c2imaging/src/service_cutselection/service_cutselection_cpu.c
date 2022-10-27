/* service_cutselection_cpu.c */

#include "service_cutselection.h"

int32_t run_service_cutselection_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns,  int32_t p1_x, int32_t p1_y, int32_t p2_x, int32_t p2_y) {

	if( p1_x > p2_x || p1_y > p2_y || p1_x < 0 || p1_y < 0 || p2_x < 0 || p2_y < 0 || p2_x > columns || p1_x > columns || p2_y > rows || p1_y > rows ) {
			// invalid parameters, because P1 is not the Point of the left corner of the selected area or P1 / P2 are not Points of the source
			fprintf(stderr, "invalid parameters, P1 is not the Point of the left corner of the selected area or P1 / P2 are not Points of the source");
			return -1;
		}
	else
		{
		#pragma omp parallel
			{
			int32_t i, j, pos, pos_new, i_new, j_new, col_new = p2_x+1-p1_x;
			// pos_new: position targetpic, col_new: length of column of targetpic 
			
			#pragma omp for private(j, pos, pos_new, i_new, j_new)
			for (i=p1_y; i<p2_y+1; ++i) {
				i_new = i-p1_y;
				// i_new: i reset to 0
				for (j=p1_x; j<p2_x+1; ++j) {
					pos = i*columns+j;
					j_new = j-p1_x;
					//j_new: j reset to 0
					pos_new = i_new*col_new+j_new;
					pixpack_target[pos_new].red		= pixpack_source[pos].red;	     
					pixpack_target[pos_new].green		= pixpack_source[pos].green;	   
					pixpack_target[pos_new].blue		= pixpack_source[pos].blue;	     
					pixpack_target[pos_new].opacity	= pixpack_source[pos].opacity;
					}
				
				}
			}
		}
	return 0;
}

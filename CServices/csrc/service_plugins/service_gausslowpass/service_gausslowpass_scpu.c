/* service_gausslowpass_scpu.c */

#include "service_gausslowpass.h"

int32_t run_service_gausslowpass_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
 	int32_t temp_red = 0, temp_green = 0, temp_blue = 0;
	int32_t i, j, pos;
	
	for (i=2; i<rows-2; ++i) {
		for (j=2; j<columns-2; ++j) {
			pos = i*columns+j;
			temp_red	=	   pixpack_source[pos-(2*columns)-2].red
						+	 5*pixpack_source[pos-(2*columns)-1].red
						+	 7*pixpack_source[pos-(2*columns)  ].red
						+	 5*pixpack_source[pos-(2*columns)+1].red
						+	   pixpack_source[pos-(2*columns)+2].red;
			temp_green	=	   pixpack_source[pos-(2*columns)-2].green
						+	 5*pixpack_source[pos-(2*columns)-1].green
						+	 7*pixpack_source[pos-(2*columns)  ].green
						+	 5*pixpack_source[pos-(2*columns)+1].green
						+	   pixpack_source[pos-(2*columns)+2].green;
			temp_blue	=	   pixpack_source[pos-(2*columns)-2].blue
						+	 5*pixpack_source[pos-(2*columns)-1].blue
						+	 7*pixpack_source[pos-(2*columns)  ].blue
						+	 5*pixpack_source[pos-(2*columns)+1].blue
						+	   pixpack_source[pos-(2*columns)+2].blue;
			
			temp_red	+=	 5*pixpack_source[pos-(  columns)-2].red
						+	20*pixpack_source[pos-(  columns)-1].red
						+	33*pixpack_source[pos-(  columns)  ].red
						+	20*pixpack_source[pos-(  columns)+1].red
						+	 5*pixpack_source[pos-(  columns)+2].red;
			temp_green	+=	 5*pixpack_source[pos-(  columns)-2].green
						+	20*pixpack_source[pos-(  columns)-1].green
						+	33*pixpack_source[pos-(  columns)  ].green
						+	20*pixpack_source[pos-(  columns)+1].green
						+	 5*pixpack_source[pos-(  columns)+2].green;
			temp_blue	+=	 5*pixpack_source[pos-(  columns)-2].blue
						+	20*pixpack_source[pos-(  columns)-1].blue
						+	33*pixpack_source[pos-(  columns)  ].blue
						+	20*pixpack_source[pos-(  columns)+1].blue
						+	 5*pixpack_source[pos-(  columns)+2].blue;
			
			temp_red	+=	 7*pixpack_source[pos-2].red
						+	33*pixpack_source[pos-1].red
						+	55*pixpack_source[pos  ].red
						+	33*pixpack_source[pos+1].red
						+	 7*pixpack_source[pos+2].red;
			temp_green	+=	 7*pixpack_source[pos-2].green
						+	33*pixpack_source[pos-1].green
						+	55*pixpack_source[pos  ].green
						+	33*pixpack_source[pos+1].green
						+	 7*pixpack_source[pos+2].green;
			temp_blue	+=	 7*pixpack_source[pos-2].blue
						+	33*pixpack_source[pos-1].blue
						+	55*pixpack_source[pos  ].blue
						+	33*pixpack_source[pos+1].blue
						+	 7*pixpack_source[pos+2].blue;
			
			temp_red	+=	 5*pixpack_source[pos+(  columns)-2].red
						+	20*pixpack_source[pos+(  columns)-1].red
						+	33*pixpack_source[pos+(  columns)  ].red
						+	20*pixpack_source[pos+(  columns)+1].red
						+	 5*pixpack_source[pos+(  columns)+2].red;
			temp_green	+=	 5*pixpack_source[pos+(  columns)-2].green
						+	20*pixpack_source[pos+(  columns)-1].green
						+	33*pixpack_source[pos+(  columns)  ].green
						+	20*pixpack_source[pos+(  columns)+1].green
						+	 5*pixpack_source[pos+(  columns)+2].green;
			temp_blue	+=	 5*pixpack_source[pos+(  columns)-2].blue
						+	20*pixpack_source[pos+(  columns)-1].blue
						+	33*pixpack_source[pos+(  columns)  ].blue
						+	20*pixpack_source[pos+(  columns)+1].blue
						+	 5*pixpack_source[pos+(  columns)+2].blue;
			
			temp_red	+=	   pixpack_source[pos+(2*columns)-2].red
						+	 5*pixpack_source[pos+(2*columns)-1].red
						+	 7*pixpack_source[pos+(2*columns)  ].red
						+	 5*pixpack_source[pos+(2*columns)+1].red
						+	   pixpack_source[pos+(2*columns)+2].red;
			temp_green	+=	   pixpack_source[pos+(2*columns)-2].green
						+	 5*pixpack_source[pos+(2*columns)-1].green
						+	 7*pixpack_source[pos+(2*columns)  ].green
						+	 5*pixpack_source[pos+(2*columns)+1].green
						+	   pixpack_source[pos+(2*columns)+2].green;
			temp_blue	+=	   pixpack_source[pos+(2*columns)-2].blue
						+	 5*pixpack_source[pos+(2*columns)-1].blue
						+	 7*pixpack_source[pos+(2*columns)  ].blue
						+	 5*pixpack_source[pos+(2*columns)+1].blue
						+	   pixpack_source[pos+(2*columns)+2].blue;
			
			pixpack_target[pos].red		= temp_red		/ 339;
			pixpack_target[pos].green	= temp_green	/ 339;
			pixpack_target[pos].blue	= temp_blue		/ 339;
			pixpack_target[pos].opacity	= 0;
		}
	}
	
	return 0;
}

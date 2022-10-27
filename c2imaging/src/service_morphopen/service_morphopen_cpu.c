/* service_morphopen_cpu.c */

#include "service_morphopen.h"

int32_t run_service_morphopen_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	PixelPacket *pixpack_temp = malloc(columns * rows * sizeof(PixelPacket));
	if (NULL == pixpack_temp) {
		fprintf(stderr, "Couldn't allocate memory. (file: %s, line: %i)\n", __FILE__, __LINE__);
		return -1;
	}
	
	int32_t temp_red = 0, temp_green = 0, temp_blue = 0;
	
	int32_t i, j, pos;
	
	int32_t threshold = QuantumRange>>1;
	
	#pragma omp parallel
	{
		#pragma omp for private(j, pos)
		for (i=0; i<rows; ++i) {
			for (j=0; j<columns; ++j) {
				pos = i*columns+j;
				pixpack_source[pos].red = (pixpack_source[pos].red + pixpack_source[pos].green + pixpack_source[pos].blue)/3;
			}
		}
		
		// erosion
		#pragma omp for private(j, pos)
		for (i=1; i<rows-1; ++i) {
			for (j=1; j<columns-1; ++j) {
				pos = i*columns+j;
				if (pixpack_source[pos-columns-1].red < threshold &&
					pixpack_source[pos-columns].red < threshold &&
					pixpack_source[pos-columns+1].red < threshold &&
					pixpack_source[pos-1].red < threshold &&
					pixpack_source[pos].red < threshold &&
					pixpack_source[pos+1].red < threshold &&
					pixpack_source[pos+columns-1].red < threshold &&
					pixpack_source[pos+columns].red < threshold &&
					pixpack_source[pos+columns+1].red < threshold) {
					
					pixpack_temp[pos].red = 0;
				} else {
					pixpack_temp[pos].red = QuantumRange;
				}
			}
		}
		
		// dilatation
		#pragma omp for private(j, pos)
		for (i=1; i<rows-1; ++i) {
			for (j=1; j<columns-1; ++j) {
				pos = i*columns+j;
				if (pixpack_temp[pos-columns-1].red < threshold ||
					pixpack_temp[pos-columns].red < threshold ||
					pixpack_temp[pos-columns+1].red < threshold ||
					pixpack_temp[pos-1].red < threshold ||
					pixpack_temp[pos].red < threshold ||
					pixpack_temp[pos+1].red < threshold ||
					pixpack_temp[pos+columns-1].red < threshold ||
					pixpack_temp[pos+columns].red < threshold ||
					pixpack_temp[pos+columns+1].red < threshold) {

					pixpack_target[pos].red = 0;
				} else {
					pixpack_target[pos].red = QuantumRange;
				}
			}
		}
		
		#pragma omp for private(j, pos)
		for (i=0; i<rows; ++i) {
			for (j=0; j<columns; ++j) {
				pos = i*columns+j;
				pixpack_target[pos].green	= pixpack_target[pos].red;
				pixpack_target[pos].blue	= pixpack_target[pos].red;
				pixpack_target[pos].opacity	= 0;
			}
		}
	}
	
	free(pixpack_temp);
	
	return 0;
}

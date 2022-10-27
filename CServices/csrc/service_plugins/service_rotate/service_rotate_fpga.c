/* service_rotate_fpga.c */

#include "service_rotate.h"

#include <math.h>

int32_t run_service_rotate_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float degree) {
	int32_t *rotate_matrix = malloc(columns * rows * sizeof(int32_t));
	if (NULL == rotate_matrix) {
		fprintf(stderr, "Couldn't allocate memory. (file: %s, line: %i)\n", __FILE__, __LINE__);
		return -1;
	}
	
	float teta = degree * M_PI / 180;
	float sin_teta = sin(teta);
	float cos_teta = cos(teta);
	
	run_service_rotate_max((int32_t *)rotate_matrix, (uint32_t)rows, (uint32_t)columns, (float)sin_teta, (float)cos_teta);

	int32_t i, j, pos;
	for (i=0; i<rows; ++i) {
		for (j=0; j<columns; ++j) {
			pos = i*columns+j;
			if (rotate_matrix[pos] < 0) {
				pixpack_target[pos].red		= QuantumRange;
				pixpack_target[pos].green	= QuantumRange;
				pixpack_target[pos].blue	= QuantumRange;
			} else {
				pixpack_target[pos].red		= pixpack_source[rotate_matrix[pos]].red;
				pixpack_target[pos].green	= pixpack_source[rotate_matrix[pos]].green;
				pixpack_target[pos].blue	= pixpack_source[rotate_matrix[pos]].blue;
			}
		}
	}
	
	free(rotate_matrix);
	
	return 0;
}

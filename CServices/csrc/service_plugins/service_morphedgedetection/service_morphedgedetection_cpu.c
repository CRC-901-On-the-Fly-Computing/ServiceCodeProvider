/* service_morphedgedetection_cpu.c */

#include "service_morphedgedetection.h"

int32_t run_service_morphedgedetection_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t matrix_size) {
	if (matrix_size < 1) {
		fprintf(stderr, "ERROR: matrix_size, has to be at least 1!\n");
		return -1;
	}

	int32_t pos = 0;
	int i_struct = 0;
	int i_offset = 0;
	int offsetindex = 0;
	int offsetsize = matrix_size * 4 + 1;

	int32_t *offset = malloc(offsetsize * sizeof(int32_t));

	//structure element (a cross)
	for (i_struct = -matrix_size; i_struct <= matrix_size; i_struct++) {
		offset[i_offset] = i_struct;
		i_offset++;
		if (i_struct != 0) {
			offset[i_offset] = columns * i_struct;
			i_offset++;
		}
	}

	#pragma omp parallel
	{
		//calculate grayscale picture
		//Important: May not be parallelized! Grayscale picture has to be calculated before algorithm starts
		#pragma omp for
		for (pos = 0; pos < rows * columns; pos++) {
			pixpack_source[pos].red = (pixpack_source[pos].red + pixpack_source[pos].green + pixpack_source[pos].blue) / 3;
		}

		//so here has to be a pragma barrier
		#pragma omp barrier

		int max = 0;
		int min = 0;

		//for every pixel of original image
		#pragma omp for private(min, max, offsetindex)
		for (pos = 0; pos < rows * columns; pos++) {

			min = pixpack_source[pos].red;
			max = pixpack_source[pos].red;

			//find minimum & maximum
			for (offsetindex = 0; offsetindex < offsetsize; offsetindex++) {
				if ((pos + offset[offsetindex]) > 0 && (pos + offset[offsetindex]) < rows * columns) {
					if (pixpack_source[pos + offset[offsetindex]].red < min) {
						min = pixpack_source[pos + offset[offsetindex]].red;
					}
					if (pixpack_source[pos + offset[offsetindex]].red > max) {
						max = pixpack_source[pos + offset[offsetindex]].red;
					}
				}
			}

			pixpack_target[pos].red = pixpack_source[pos].red - min;
			pixpack_target[pos].green = pixpack_source[pos].red - min;
			pixpack_target[pos].blue = pixpack_source[pos].red - min;

		}
	}

	free(offset);

	return 0;
}

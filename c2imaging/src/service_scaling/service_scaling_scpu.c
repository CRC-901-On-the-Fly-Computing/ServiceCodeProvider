/* service_scaling_scpu.c */

#include "service_scaling.h"

int32_t run_service_scaling_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t scale_up_factor, int32_t scale_down_factor) {
	int32_t temp_red, temp_green, temp_blue;
	int32_t i, j, k, l , pos, pos_temp , pos_target;

	if (scale_down_factor == 0 || scale_up_factor == 0) {
		fprintf(stderr, "Couldn't run with scalefactor zero (file: %s, line: %i)\n", __FILE__, __LINE__);
		return -1;
	}

	int32_t columns_up	= columns * scale_up_factor;
	int32_t columns_down;
	int32_t rows_up	= rows * scale_up_factor;
	int32_t rows_down;

	if (rows % scale_down_factor != 0) {
		rows_down = rows_up / scale_down_factor + 1;
	} else rows_down = (rows_up) / scale_down_factor;
	if (columns % scale_down_factor != 0) {
		columns_down = columns_up / scale_down_factor + 1;
	} else columns_down = columns_up / scale_down_factor;

	if((((double)columns_up * rows_up) * (double)sizeof(PixelPacket)) > 6144000000){
		fprintf(stderr, "WARNING: File size is too large(over 6 GB), malloc may crash");
	}
	PixelPacket *pixpack_temp = malloc(((double)columns_up * rows_up) * (double)sizeof(PixelPacket));
	if (NULL == pixpack_temp) {
		fprintf(stderr, "Couldn't allocate memory (file: %s, line: %i)\n", __FILE__, __LINE__);
		return -1;
	}

	for (i = 0; i < rows_up; i++) {
		k = i / scale_up_factor;
		for (j = 0; j < columns_up; j++) {
			l			= j / scale_up_factor;
			pos			= k * columns + l;
			pos_temp	= i * columns_up + j;

			temp_red = pixpack_source[pos].red;
			temp_green = pixpack_source[pos].green;
			temp_blue = pixpack_source[pos].blue;

			pixpack_temp[pos_temp].red		= temp_red;
			pixpack_temp[pos_temp].green	= temp_green;
			pixpack_temp[pos_temp].blue		= temp_blue;
			pixpack_temp[pos_temp].opacity	= 0;
		}
	}

	for (i = 0; i < rows_up - 1; i += scale_down_factor) {
		k = i / scale_down_factor;
		for (j = 0; j < columns_up - 1; j += scale_down_factor) {
			l = j / scale_down_factor;
			pos_temp			= i * columns_up + j;
			pos_target	= k * columns_down + l;

			if (scale_down_factor != 1) {
				int32_t n;

				temp_red	= 0;
				temp_green	= 0;
				temp_blue	= 0;

				for (n = 0; n < (scale_down_factor * scale_down_factor) / 2; n++) {
					temp_red	+= pixpack_temp[pos_temp + n].red;
					temp_green	+= pixpack_temp[pos_temp + n].green;
					temp_blue	+= pixpack_temp[pos_temp + n].blue;
				}
				for (n = 0; n < (scale_down_factor * scale_down_factor) / 2; n++) {
					temp_red	+= pixpack_temp[pos_temp + columns_up + n].red;
					temp_green	+= pixpack_temp[pos_temp + columns_up + n].green;
					temp_blue	+= pixpack_temp[pos_temp + columns_up + n].blue;
				}
			} else {
				temp_red	= pixpack_temp[pos_temp].red;
				temp_green	= pixpack_temp[pos_temp].green;
				temp_blue	= pixpack_temp[pos_temp].blue;
			}
			pixpack_target[pos_target].red		= temp_red / (scale_down_factor * scale_down_factor);
			pixpack_target[pos_target].green	= temp_green / (scale_down_factor * scale_down_factor);
			pixpack_target[pos_target].blue		= temp_blue / (scale_down_factor * scale_down_factor);
			pixpack_target[pos_target].opacity	= 0;

		}
	}

	free(pixpack_temp);

	return 0;
}

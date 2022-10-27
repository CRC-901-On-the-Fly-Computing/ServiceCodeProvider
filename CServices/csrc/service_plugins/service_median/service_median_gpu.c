/* service_median_gpu.c */

#include "service_median.h"

int32_t run_service_median_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t filter) {
	int32_t filter_size = filter * filter;	//Benötigte Anzahl an Speicher
	int32_t i, j, k, m, pos, array_pos;	//Laufvariablen
	int32_t limit = (filter - 1) / 2;	//Die Grenzen abhängig von der Filtergröße
	int32_t median_red[filter_size];	//Array zum Sortieren
	int32_t median_green[filter_size];
	int32_t median_blue[filter_size];
	int32_t a, b, tmp;	//Bubblesort

	if (filter % 2 == 0) {
		fprintf(stderr, "Wrong filter size, filter size hast to be odd");
		return -1;
	}

#pragma acc kernels copyin(pixpack_source[0:columns*rows]) copyout(pixpack_target[0:columns*rows])
	{	
		#pragma acc loop independent
		for (i = 0; i < rows; ++i) {
			#pragma acc loop independent private(median_red, median_green, median_blue,pos)
			for (j = 0; j < columns; ++j) {
				pos = columns*i+j;
				if (i < limit || i >= rows - limit || j < limit || j >= columns - limit) {
					pixpack_target[pos].red = 0;
					pixpack_target[pos].green = 0;
					pixpack_target[pos].blue = 0;
				} else {
					#pragma acc loop independent
					for (k = -limit; k <= limit; ++k) {
						#pragma acc loop independent private(array_pos)
						for (m = -limit; m <= limit; ++m) {
							array_pos = (k + limit) * (2 * limit + 1) + (m + limit);
							median_red[array_pos] = pixpack_source[((i + k) * columns) + j + m].red;
							median_green[array_pos] = pixpack_source[((i + k) * columns) + j + m].green;
							median_blue[array_pos] = pixpack_source[((i + k) * columns) + j + m].blue;
						}
					}
					//Bubblesort für Roten-Kanal
					for (a = 0; a < (filter_size) - 1; ++a) {
						for (b = 0; b < (filter_size) - a - 1; ++b) {
							if (median_red[b] > median_red[b + 1]) {
								tmp = median_red[b];
								median_red[b] = median_red[b + 1];
								median_red[b + 1] = tmp;
							}
						}
					}
					//Bubblesort für Grünen-Kanal
					for (a = 0; a < (filter_size) - 1; ++a) {
						for (b = 0; b < (filter_size) - a - 1; ++b) {
							if (median_green[b] > median_green[b + 1]) {
								tmp = median_green[b];
								median_green[b] = median_green[b + 1];
								median_green[b + 1] = tmp;
							}
						}
					}
					//Bubblesort für Roten-Kanal
					for (a = 0; a < (filter_size) - 1; ++a) {
						for (b = 0; b < (filter_size) - a - 1; ++b) {
							if (median_blue[b] > median_blue[b + 1]) {
								tmp = median_blue[b];
								median_blue[b] = median_blue[b + 1];
								median_blue[b + 1] = tmp;
							}
						}
					}

					pixpack_target[pos].red = median_red[filter_size / 2]; //Bubblesort(median_red, filter_size);
					pixpack_target[pos].green = median_green[filter_size / 2];	//Bubblesort(median_green, filter_size);
					pixpack_target[pos].blue = median_blue[filter_size / 2];	//Bubblesort(median_blue, filter_size);
				}
				pixpack_target[pos].opacity = 0;
			}
		}
	}

	return 0;
}



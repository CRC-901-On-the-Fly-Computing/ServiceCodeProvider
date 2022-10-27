/* service_median_scpu.c */

#include "service_median.h"

int32_t run_service_median_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t filter) {
	int32_t filter_size = filter * filter;	//Benötigte Anzahl an Speicher
	int32_t i, j, k, m, pos, array_pos;	//Laufvariablen
	int32_t limit = (filter - 1) / 2;	//Die Grenzen abhängig von der Filtergröße
	int32_t median_red[filter_size];	//Array zum Sortieren
	int32_t median_green[filter_size];
	int32_t median_blue[filter_size];

	if (filter % 2 == 0) {
		fprintf(stderr, "Wrong filter size, filter size hast to be odd");
		return -1;
	}
	
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < columns; ++j) {
			pos = i*columns+j;
			if (i < limit || i >= rows - limit || j < limit || j >= columns - limit) {
				pixpack_target[pos].red = 0;
				pixpack_target[pos].green = 0;
				pixpack_target[pos].blue = 0;
			} else {
				for (k = -limit; k <= limit; ++k) {
					for (m = -limit; m <= limit; ++m) {
						array_pos = (k + limit) * (2 * limit + 1) + (m + limit);
						median_red[array_pos] = pixpack_source[((i + k) * columns) + j + m].red;
						median_green[array_pos] = pixpack_source[((i + k) * columns) + j + m].green;
						median_blue[array_pos] = pixpack_source[((i + k) * columns) + j + m].blue;
					}
				}

				pixpack_target[pos].red = Bubblesort(median_red, filter_size);
				pixpack_target[pos].green = Bubblesort(median_green, filter_size);
				pixpack_target[pos].blue = Bubblesort(median_blue, filter_size);
			}
			pixpack_target[pos].opacity = 0;
		}
	}


	return 0;
}

int Bubblesort(int32_t median[], int32_t length) {
	int a, b, tmp;
	for (a = 0; a < length - 1; ++a) {
		for (b = 0; b < length - a - 1; ++b) {
			if (median[b] > median[b + 1]) {
				tmp = median[b];
				median[b] = median[b + 1];
				median[b + 1] = tmp;
			}
		}
	}
	return median[length / 2];
}




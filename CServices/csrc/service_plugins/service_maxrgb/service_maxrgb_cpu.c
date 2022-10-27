/* service_maxrgb_cpu.c */

#include "service_maxrgb.h"

int32_t run_service_maxrgb_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mode) {
	int32_t i, j, pos;

	/**
	  * Modes:
	  * 1: maxRGB
	  * 2: minRGB
	  * 3: max increase
	  * 4: min erase
	**/

	if ((mode == 1) || (mode == 3)) {
		#pragma omp parallel
		{
			#pragma omp for private(j, pos)
			for (i = 0; i < rows; ++i) {
				for (j = 0; j < columns; ++j) {
					pos = i * columns + j;

					//check if color is grey
					if ((pixpack_source[pos].red == pixpack_source[pos].green) && (pixpack_source[pos].red == pixpack_source[pos].blue)) {
						pixpack_target[pos].red	  = pixpack_source[pos].red;
						pixpack_target[pos].green = pixpack_source[pos].green;
						pixpack_target[pos].blue  = pixpack_source[pos].blue;
						continue;
					}


					uint16_t *maxPtr;
					maxPtr = &pixpack_source[pos].blue;
					uint16_t *colorPtr;
					colorPtr = &pixpack_source[pos].blue;
					int8_t c;

					for (c = 1; c < 3; c++) {
						colorPtr++;
						if (*colorPtr > *maxPtr)
							maxPtr = colorPtr;
					}

					uint16_t *colorPtrSource;
					colorPtrSource = &pixpack_source[pos].blue;
					uint16_t *colorPtrTarget;
					colorPtrTarget = &pixpack_target[pos].blue;

					switch (mode) {
						case 1:
							for (c = 0; c < 3; c++) {
								if (colorPtrSource == maxPtr)
									*colorPtrTarget = *colorPtrSource;
								else
									*colorPtrTarget = 0;

								colorPtrSource++;
								colorPtrTarget++;
							}
							break;
						case 3:
							for (c = 0; c < 3; c++) {
								if (colorPtrSource == maxPtr)
									*colorPtrTarget = 65535;
								else
									*colorPtrTarget = *colorPtrSource;

								colorPtrSource++;
								colorPtrTarget++;
							}
					}
				}
			}
		}
	} else if ((mode == 2) || (mode == 4)) {
		#pragma omp parallel
		{
			#pragma omp for private(j, pos)
			for (i = 0; i < rows; ++i) {
				for (j = 0; j < columns; ++j) {
					pos = i * columns + j;

					//check if color is grey
					if ((pixpack_source[pos].red == pixpack_source[pos].green) && (pixpack_source[pos].red == pixpack_source[pos].blue)) {
						pixpack_target[pos].red	  = pixpack_source[pos].red;
						pixpack_target[pos].green = pixpack_source[pos].green;
						pixpack_target[pos].blue  = pixpack_source[pos].blue;
						continue;
					}


					uint16_t *minPtr;
					minPtr = &pixpack_source[pos].blue;
					uint16_t *colorPtr;
					colorPtr = &pixpack_source[pos].blue;
					int8_t c;

					for (c = 1; c < 3; c++) {
						colorPtr++;
						if (*colorPtr < *minPtr)
							minPtr = colorPtr;
					}

					uint16_t *colorPtrSource;
					colorPtrSource = &pixpack_source[pos].blue;
					uint16_t *colorPtrTarget;
					colorPtrTarget = &pixpack_target[pos].blue;

					switch (mode) {
						case 2:
							for (c = 0; c < 3; c++) {
								if (colorPtrSource == minPtr)
									*colorPtrTarget = *colorPtrSource;
								else
									*colorPtrTarget = 0;

								colorPtrSource++;
								colorPtrTarget++;
							}
							break;
						case 4:
							for (c = 0; c < 3; c++) {
								if (colorPtrSource == minPtr)
									*colorPtrTarget = 0;
								else
									*colorPtrTarget = *colorPtrSource;

								colorPtrSource++;
								colorPtrTarget++;
							}
					}
				}
			}
		}
	}
	return 0;
}

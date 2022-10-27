/* service_contrast_cpu.c */

#include "service_contrast.h"

int32_t run_service_contrast_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t factor) {
	int32_t i, j, pos, val, red, green, blue;
	uint16_t minVal;
	uint16_t maxVal;
	float x;


	if ((factor < -128) || (factor > 127)) { //factor out of range
		fprintf(stderr, "factor out of range.");
		return -1;
	}

	if (factor == -128) {
		//auto-contrast, no brightness-change
		minVal = 65535;
		maxVal = 0;

		#pragma omp parallel
		{
			#pragma omp for private(j, pos)
			for (i = 0; i < rows; ++i) {
				for (j = 0; j < columns; ++j) {
					pos = i * columns + j;

					uint16_t brightness;
					brightness = (uint16_t)(pixpack_source[pos].red + pixpack_source[pos].green + pixpack_source[pos].blue) / 3;

					if (brightness < minVal)
						minVal = brightness;
					else if (brightness > maxVal)
						maxVal = brightness;
				}
			}
		}
	} else if (factor > 0)
		x = (float) factor * factor / 127;
	else
		x = ((float) factor) / 127;

	if ((factor == 128) || (factor == -128)) {
		//auto-contrast: calc x-value

		int32_t absBorder;
		if (minVal < (65535 - maxVal))
			absBorder = minVal;
		else
			absBorder = 65535 - maxVal;

		x = -(float)absBorder / (float)(absBorder - 32767);
	}

	if (x >= 0) {
		#pragma omp parallel
		{
			#pragma omp for private(j, pos)
			for (i = 0; i < rows; ++i) {
				for (j = 0; j < columns; ++j) {
					pos = i * columns + j;
					red = pixpack_source[pos].red;
					val = red + x * (red - 32767);
					pixpack_target[pos].red = val < 0 ? 0 : (val > 65535 ? 65535 : val);

					green = pixpack_source[pos].green;
					val = green + x * (green - 32767);
					pixpack_target[pos].green = val < 0 ? 0 : (val > 65535 ? 65535 : val);

					blue = pixpack_source[pos].blue;
					val = blue + x * (blue - 32767);
					pixpack_target[pos].blue = val < 0 ? 0 : (val > 65535 ? 65535 : val);

					pixpack_target[pos].opacity	= pixpack_source[pos].opacity;
				}
			}
		}
	} else {
		#pragma omp parallel
		{
			#pragma omp for private(j, pos)
			for (i = 0; i < rows; ++i) {
				for (j = 0; j < columns; ++j) {
					pos = i * columns + j;

					val = ((int32_t)pixpack_source[pos].red - 32767) + x * ((int32_t)pixpack_source[pos].red - 32767);
					if (val < ((int32_t)pixpack_source[pos].red - 32767))
						val = 0;
					pixpack_target[pos].red = (32767 + val) > 65535 ? 65535 : 32767 + val;

					val = ((int32_t)pixpack_source[pos].green - 32767) + x * ((int32_t)pixpack_source[pos].green - 32767);
					if (val < ((int32_t)pixpack_source[pos].green - 32767))
						val = 0;
					pixpack_target[pos].green = (32767 + val) > 65535 ? 65535 : 32767 + val;

					val = ((int32_t)pixpack_source[pos].blue - 32767) + x * ((int32_t)pixpack_source[pos].blue - 32767);
					if (val < ((int32_t)pixpack_source[pos].blue - 32767))
						val = 0;
					pixpack_target[pos].blue = (32767 + val) > 65535 ? 65535 : 32767 + val;

					pixpack_target[pos].opacity	= pixpack_source[pos].opacity;
				}
			}
		}
	}

	return 0;
}

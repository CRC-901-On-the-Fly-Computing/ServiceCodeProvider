/* service_contrast_gpu.c */

#include "service_contrast.h"

int32_t run_service_contrast_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t factor) {
	int32_t i, j, pos, val, red, green, blue;
	uint16_t minVal;
	uint16_t maxVal;
	float x;

	if ((factor < -128) || (factor > 127)) { //factor out of range
		fprintf(stderr, "factor out of range.");
		return 1;
	}
		
	if (factor == -128) {
		//auto-contrast, no brightness-change
		minVal = 65535;
		maxVal = 0;
		#pragma acc enter data copyin(pixpack_source[0:columns*rows])
		#pragma acc kernels present(pixpack_source[0:columns*rows])
			{
			#pragma acc loop independent reduction(min: minVal) reduction(max: maxVal)
			for (i = 0; i < rows; ++i) {
				#pragma acc loop independent private(pos, minVal, maxVal)
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

	#pragma acc kernels present_or_copyin(pixpack_source[0:columns*rows]) copyout(pixpack_target[0:columns*rows])
	{
		if (x >= 0) {
			#pragma acc loop independent
			for (i = 0; i < rows; ++i) {
				#pragma acc loop independent private(pos, val, red, green, blue)
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
		} else {
			#pragma acc loop independent
			for (i = 0; i < rows; ++i) {
				#pragma acc loop independent private(pos, val)
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
	#pragma acc exit data delete(pixpack_source[0:columns*rows])

	return 0;
}

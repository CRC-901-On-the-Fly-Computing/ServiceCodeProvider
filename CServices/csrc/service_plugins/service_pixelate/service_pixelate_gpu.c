/* service_pixelate_gpu.c */

#include "service_pixelate.h"

int32_t run_service_pixelate_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t area) {

	//area ist die Größes des Bereichs der zu einem Pixel zusammengefasst wird

	int32_t i, j, k, l, pos;
	uint64_t r, g, b;
	if (area < 1 || area > rows) {
		fprintf(stderr, "Wrong parameter area: %d\n area must be larger than 0 and smaller than the height of the image .\n", area);
		return -1;
	}
	#pragma acc kernels copyin(pixpack_source[0:columns*rows]) copyout(pixpack_target[0:columns*rows])
	{
		#pragma acc loop independent
		for (i = 0; i < rows + 1 - area; i += area) {
			#pragma acc loop independent private (pos, r, g, b)
			for (j = 0; j < columns + 1 - area; j = j + area) {
				pos = i * columns + j;
				r = 0;
				g = 0;
				b = 0;
				#pragma acc loop seq
				for (l = pos; l < (pos + columns * area); l = l + columns) {
					#pragma acc loop seq
					for (k = l; k < l + area; ++k) {
						r += pixpack_source[k].red;
						g += pixpack_source[k].green;
						b += pixpack_source[k].blue;
						// Berechnet die Summe für R,G,B im angegebenen Bereich
					}
				}
				#pragma acc loop seq
				for (l = pos; l < (pos + columns * area); l = l + columns) {
					#pragma acc loop seq
					for (k = l; k < l + area; ++k) {
						uint64_t area2 = area * area;
						pixpack_target[k].red     = ((float)r / (float)area2);
						pixpack_target[k].green   = ((float)g / (float)area2);
						pixpack_target[k].blue    = ((float)b / (float)area2);
						pixpack_target[k].opacity = 0;
						// Setzt den Bereich auf die berechneten Mittelwerte
					}
				}
			}
		}
	}
	return 0;

}

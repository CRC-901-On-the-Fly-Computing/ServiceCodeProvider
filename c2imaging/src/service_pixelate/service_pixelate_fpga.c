/* service_pixelate_fpga.c */

#include "service_pixelate.h"

int32_t run_service_pixelate_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t area) {

	if (area > 25 || area < 1) {
		fprintf(stderr, "area must be between 0 and 25! The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	int32_t index1, i, j, l, k, pos, m;
	PixelPacket *temp = malloc(rows * columns * sizeof(PixelPacket)); /* Temporary variable to get output from FPGA! */
	if (NULL == temp) {
		printf("Couldn't allocate memory. The Algorithm will Exit!");
		return -1;
	}
	int32_t constantWidth = 25; /* mask is set to 25! It's efficient to set it less say maybe 10 (DON'T forget to change constantWidth in Maxeler - ServicePixelateManager)! */

	int32_t *validPos = malloc(constantWidth * sizeof(uint32_t)); /* Allocate dynmanic memory */
	if (NULL == validPos) {
		printf("Couldn't allocate memory. The Algorithm will Exit!");
		return -1;
	}

	for (index1 = 0; index1 < constantWidth; ++index1) { /* unwanted values are set to 0; */
		if (index1 < area)
			validPos[index1] = 1;
		else
			validPos[index1] = 0;
	}

	run_service_pixelate_max((uint16_t *)temp, (uint16_t *)pixpack_source, validPos, rows, columns, area);

	for (i = 0; i < rows + 1 - area; i += area) { /* loop to pixelate picture */
		for (j = 0; j < columns + 1 - area; j = j + area) {
			pos = i * columns + j;
			m = pos;
			for (l = pos; l < (pos + columns * area); l = l + columns) {
				for (k = l; k < l + area; ++k) { /* recover relevant picture positions! */
					pixpack_target[k].red		= temp[m].red;
					pixpack_target[k].green 	= temp[m].green;
					pixpack_target[k].blue 		= temp[m].blue;
					pixpack_target[k].opacity       = 0;
				}
			}
		}
	}

	/* free allocated memory */

	free(temp);
	free(validPos);

	return 0;
}

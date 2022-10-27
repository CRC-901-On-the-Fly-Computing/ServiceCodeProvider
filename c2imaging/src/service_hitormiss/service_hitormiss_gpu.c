/* service_hitormiss_gpu.c */

#include "service_hitormiss.h"

int32_t run_service_hitormiss_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source0, PixelPacket *pixpack_source1, PixelPacket *pixpack_source2, int32_t rows0, int32_t columns0,  int32_t rows1, int32_t columns1,  int32_t rows2, int32_t columns2, int32_t threshold) {
	int i;
	int pos;
	int i_offset;
	int anchor;
	int offsetsize1, offsetsize2;
	int offsetindex;
	int min;

	/** Calculate Offset Array from first Picture - Hit **/


	// Source Image 0
	int32_t *offset1 = malloc(rows0 * columns0 * sizeof(int32_t));
	if (NULL == offset1) {
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	// Source Image 1
	int32_t *offset2 = malloc(rows1 * columns1 * sizeof(int32_t));
	if (NULL == offset2) {
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	// Source Image 2
	PixelPacket *pixpack_temp1 = malloc(columns2 * rows2 * sizeof(PixelPacket));
	PixelPacket *pixpack_temp2 = malloc(columns2 * rows2 * sizeof(PixelPacket));
	if (NULL == pixpack_temp1 || NULL == pixpack_temp2) {
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}


	i_offset = 0;
	anchor = 0;
	i = 0;
	for (i = 0; i < rows0 * columns0; i++) {
		if (pixpack_source0[i].red == 0 && pixpack_source0[i].green == 0 && pixpack_source0[i].blue == 0) {

			offset1[i_offset] = i - anchor;
			i_offset++;

		}
		if (pixpack_source0[i].red != 0 && pixpack_source0[i].green == 0 && pixpack_source0[i].blue == 0) {
			anchor = i;
		}
	}
	offsetsize1 = i_offset;

	i = 0;

	for (i = 0; i < offsetsize1; i++) {
		offset1[i] = offset1[i] - anchor;
	}

	/** Calculate Offset Array from second Picture - Miss **/
	i_offset = 0;
	anchor = 0;
	i = 0;
	for (i = 0; i < rows1 * columns1; i++) {
		if (pixpack_source1[i].red == 0 && pixpack_source1[i].green == 0 && pixpack_source1[i].blue == 0) {

			offset2[i_offset] = i - anchor ;
			i_offset++;

		}
		if (pixpack_source1[i].red != 0 && pixpack_source1[i].green == 0 && pixpack_source1[i].blue == 0) {
			anchor = i;
		}
	}
	offsetsize2 = i_offset;

	i = 0;
	for (i = 0; i < offsetsize2; i++) {
		offset2[i] = offset2[i] - anchor;
	}

	PixelPacket *temp_img = pixpack_source2;
	#pragma acc kernels copyin(offset1[0:rows0*columns0]) copyin(offset2[0:rows1*columns1]) copyin(temp_img[0:rows2*columns2]) copyout(pixpack_target[0:rows2*columns2]) create(pixpack_temp1[0:columns2*rows2]) create(pixpack_temp2[0:columns2*rows2])
	{
		/** Calculate binary picture **/
		#pragma acc loop independent
		for (pos = 0; pos < rows2 * columns2; pos++) {
			temp_img[pos].red = (temp_img[pos].red + temp_img[pos].green + temp_img[pos].blue) / 3;
			if (temp_img[pos].red > threshold) {
				temp_img[pos].red = 65535;
				temp_img[pos].green = 65535;
				temp_img[pos].blue = 65535;
			} else {
				temp_img[pos].red = 0;
				temp_img[pos].green = 0;
				temp_img[pos].blue = 0;
			}
		}

		//so here has to be a barrier

		/** Calculate first Erosion - Hit **/
		min = 0;
		//for every pixel of original image

		#pragma acc loop independent private(min)
		for (pos = 0; pos < rows2 * columns2; pos++) {

			min = temp_img[pos].red;

			//find minimum
			#pragma acc loop seq
			for (offsetindex = 0; offsetindex < offsetsize1; offsetindex++) {
				if ((pos + offset1[offsetindex]) >= 0 && (pos + offset1[offsetindex]) < rows2 * columns2) {
					if (temp_img[pos + offset1[offsetindex]].red < min) {
						min = temp_img[pos + offset1[offsetindex]].red;
					}
				}
			}

			pixpack_temp1[pos].red = min;
			pixpack_temp1[pos].green = min;
			pixpack_temp1[pos].blue = min;
		}

		// here again a barrier

		/** Calculate negative Image **/
		#pragma acc loop independent
		for (pos = 0; pos < rows2 * columns2; pos++) {
			temp_img[pos].red = 65535 - temp_img[pos].red;
			temp_img[pos].green = 65535 - temp_img[pos].green;
			temp_img[pos].blue = 65535 - temp_img[pos].blue;
		}

		/** Calculate second Erosion - Miss **/
		min = 0;
		//for every pixel of original image
		#pragma acc loop independent private (min)
		for (pos = 0; pos < rows2 * columns2; pos++) {

			min = temp_img[pos].red;

			//find minimum
			#pragma acc loop seq
			for (offsetindex = 0; offsetindex < offsetsize2; offsetindex++) {
				if ((pos + offset2[offsetindex]) >= 0 && (pos + offset2[offsetindex]) < rows2 * columns2) {
					if (temp_img[pos + offset2[offsetindex]].red < min) {
						min = temp_img[pos + offset2[offsetindex]].red;
					}
				}
			}

			pixpack_temp2[pos].red = min;
			pixpack_temp2[pos].green = min;
			pixpack_temp2[pos].blue = min;
		}
		//so here has to be a barrier


		/** Calculate negative Image **/
		#pragma acc loop independent
		for (pos = 0; pos < rows2 * columns2; pos++) {
			pixpack_temp2[pos].red = 65535 - pixpack_temp2[pos].red;
			pixpack_temp2[pos].green = 65535 - pixpack_temp2[pos].green;
			pixpack_temp2[pos].blue = 65535 - pixpack_temp2[pos].blue;
		}

		/** Merge both images **/
		#pragma acc loop independent
		for (pos = 0; pos < rows2 * columns2; pos++) {
			if (pixpack_temp1[pos].red == pixpack_temp2[pos].red && pixpack_temp1[pos].green == pixpack_temp2[pos].green && pixpack_temp1[pos].blue == pixpack_temp2[pos].blue) {
				pixpack_target[pos].red = pixpack_temp2[pos].red;
				pixpack_target[pos].green = pixpack_temp2[pos].green;
				pixpack_target[pos].blue = pixpack_temp2[pos].blue;
			} else {
				pixpack_target[pos].red = 0;
				pixpack_target[pos].green = 0;
				pixpack_target[pos].blue = 0;
			}
		}
	}
	free(offset1);
	free(offset2);
	free(pixpack_temp1);
	free(pixpack_temp2);
	return 0;
}

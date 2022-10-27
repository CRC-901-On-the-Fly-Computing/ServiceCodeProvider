/* service_gausslowpass2_cpu.c */

#include "service_gausslowpass2.h"

/* =====================================================================
 * Function: double factorial(int n);
 * To calculate the n!-Factorial .
 */

double factorial(int32_t n) {
	int32_t c;
	double result = 1.0;

	for (c = 1 ; c <= n ; c++)
		result = result * c;

	return (result);
}

int32_t run_service_gausslowpass2_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mask_size) {
	int64_t temp_red = 0, temp_green = 0, temp_blue = 0, sum = 0;;

	int32_t i, j, index1, index2, row_cnt, col_cnt, mask_cnt, p_size, pos;

	if (mask_size % 2 == 0) { /* This Algorithm doesn't support even numbers for mask_size.*/
		fprintf(stderr, "This Algorithm doesn't support even numbers for mask_size! The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	if (mask_size > 25 || mask_size < 0) {
		fprintf(stderr, "This Algorithm ONLY supports odd numbers for mask_size less or equal to 25 (<=25) and greater than 0! The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	p_size = mask_size - 1; /* The row of the Pascal Triangle */

	mask_cnt = mask_size / 2;

	int64_t *pascalVector = malloc(mask_size * sizeof(int64_t)); /* Allocate dynmanic memory */

	if (NULL == pascalVector) {
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	int64_t **convolutionMatrix = NULL;
	convolutionMatrix = (int64_t **) malloc(mask_size * sizeof(int64_t *)); /* Allocate dynmanic memory */

	if (NULL == convolutionMatrix) {
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	/* Dynmanic memory Allocation for 2D */
	for (i = 0; i < mask_size; i++) {
		convolutionMatrix[i] = (int64_t *) malloc(mask_size * sizeof(int64_t));
		if (NULL == convolutionMatrix[i]) {
			fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
			return -1;
		}
	}


	#pragma omp parallel
	{
		#pragma omp for
		for (index1 = 0 ; index1 <= p_size ; index1++)
			pascalVector[index1] = factorial(p_size) / (factorial(index1) * factorial(p_size - index1)); /* The (index1)mth entry of the (p_size)nth row of Pascal's triangle! */

		#pragma omp single
		for (i = 0; i < mask_size; i++) { /* Fill in the "convolutionMatrix"  and calculate sum! */
			for (j = 0; j < mask_size; j++) {
				convolutionMatrix [i][j] = pascalVector[i] * pascalVector[j];
				sum += convolutionMatrix [i][j];
			}
		}

		#pragma omp for private(j, pos, index1, index2, temp_red, temp_green, temp_blue) reduction(+: col_cnt) reduction(+: row_cnt)
		for (i = 0; i < rows; ++i) { /* Apply the low-pass filter to the image (the red, green and blue canals) */
			for (j = 0; j < columns; ++j) {
				pos = i * columns + j;
				temp_red = 0, temp_green = 0, temp_blue = 0;
				col_cnt = -mask_cnt;
				if (i < mask_cnt || i >= rows - mask_cnt || j < mask_cnt || j >= columns - mask_cnt) {
					pixpack_target[pos].red = 0;
					pixpack_target[pos].green = 0;
					pixpack_target[pos].blue = 0;
				} else {
					for (index1 = 0; index1 < mask_size; index1++) {   				/* Create (mask_size x mask_size) Mask! */
						row_cnt = -mask_cnt;
						for (index2 = 0; index2 < mask_size; index2++) {
							temp_red 	+= (convolutionMatrix[index1][index2]) * pixpack_source[pos + (col_cnt * columns) + row_cnt].red;
							temp_green 	+= (convolutionMatrix[index1][index2]) * pixpack_source[pos + (col_cnt * columns) + row_cnt].green;
							temp_blue 	+= (convolutionMatrix[index1][index2]) * pixpack_source[pos + (col_cnt * columns) + row_cnt].blue;
							row_cnt++;
						}
						col_cnt++;
					}



					pixpack_target[pos].red		= (uint16_t)(temp_red / sum);
					pixpack_target[pos].green	= (uint16_t)(temp_green / sum);
					pixpack_target[pos].blue	= (uint16_t)(temp_blue / sum);
				}
				pixpack_target[pos].opacity	= 0;

			}
		}
	}

	free(pascalVector);
	for (i = 0; i < mask_size; i++)
		free(convolutionMatrix[i]);
	free(convolutionMatrix);


	return 0;
}



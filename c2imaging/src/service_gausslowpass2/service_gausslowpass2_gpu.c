/* service_gausslowpass2_gpu.c */

#include "service_gausslowpass2.h"

int32_t run_service_gausslowpass2_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mask_size) {
	const double factorial[26] = {
		1.0,							//  0!
		1.0,							//  1!
		2.0,							//  2!
		6.0,							//  3!
		24.0,							//  4!
		120.0,							//  5!
		720.0,							//  6!
		5040.0,							//  7!
		40320.0,						//  8!
		362880.0,						//  9!
		3628800.0,						// 10!
		39916800.0,						// 11!
		479001600.0,					// 12!
		6227020800.0,					// 13!
		87178291200.0,					// 14!
		1307674368000.0,				// 15!
		20922789888000.0,				// 16!
		355687428096000.0,				// 17!
		6402373705728000.0,				// 18!
		121645100408832000.0,			// 19!
		2432902008176640000.0,			// 20!
		51090942171709440000.0,			// 21!
		1124000727777607680000.0,		// 22!
		25852016738884976640000.0,		// 23!
		620448401733239439360000.0,		// 24!
		15511210043330985984000000.0	// 25!
	};

	//int64_t temp_red = 0, temp_green = 0, temp_blue = 0, sum=0;
	int64_t sum = 0;
	int32_t i, j, index1, index2, mask_cnt, p_size, pos;
	float temp_red, temp_green, temp_blue;
	int32_t index;


	if (mask_size % 2 == 0) { /* This Algorithm doesn't support even numbers for mask_size, hence when such is given, the Program terminates with output message! */
		fprintf(stderr, "This Algorithm doesn't support even numbers for mask_size! The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	if (mask_size > 25 || mask_size < 0) {
		fprintf(stderr, "This Algorithm ONLY supports odd numbers for mask_size less or equal to 25 (<=25) and greater than 0! The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	p_size = mask_size - 1; /* The row of the Pascal Triangle */

	mask_cnt = mask_size / 2;

	int64_t *pascalVector = NULL;
	pascalVector = malloc(mask_size * sizeof(int64_t)); /* Allocate dynmanic memory */

	if (NULL == pascalVector) {
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	int64_t *convolutionMatrix = NULL;
	convolutionMatrix = (int64_t *) malloc(mask_size * mask_size * sizeof(int64_t)); /* Allocate dynmanic memory */

	if (NULL == convolutionMatrix) {
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}


	for (index1 = 0 ; index1 <= p_size ; index1++)
		pascalVector[index1] = factorial[p_size] / (factorial[index1] * factorial[p_size - index1]); /* The (index1)mth entry of the (p_size)nth row of Pascal's triangle! */

	for (i = 0; i < mask_size * mask_size; ++i) { /* Fill in the "convolutionMatrix"  and calculate sum! */
		convolutionMatrix [i] = pascalVector[ i / mask_size ] * pascalVector[ i % mask_size ];
		sum += convolutionMatrix [i];
	}


#pragma acc kernels copyin(pixpack_source[0:columns*rows], convolutionMatrix[0:mask_size*mask_size]) copyout(pixpack_target[0:columns*rows])
	{
		#pragma acc loop independent
		for (i = 0; i < rows; ++i) { /* Apply the low-pass filter to the image (the red, green and blue canals) */
			#pragma acc loop independent private(pos, i, index, temp_red, temp_green, temp_blue)
			for (j = 0; j < columns; ++j) {
				pos = i * columns + j;
				temp_red = 0;
				temp_green = 0;
				temp_blue = 0;
				if (i < mask_cnt || i >= rows - mask_cnt || j < mask_cnt || j >= columns - mask_cnt) {
					pixpack_target[pos].red = 0;
					pixpack_target[pos].green = 0;
					pixpack_target[pos].blue = 0;
				} else {

					#pragma acc loop independent reduction(+ : temp_red) reduction(+ : temp_green) reduction(+ : temp_blue)
					for (index = 0; index < mask_size * mask_size; ++index) {   				/* Create (mask_size x mask_size) Mask! */
						if(pos + (((index / mask_size)  * columns) + (-mask_cnt + (index % mask_size))) < (rows*columns)){
							temp_red 	+= convolutionMatrix[index] * pixpack_source[pos + ((index / mask_size)  * columns) + (-mask_cnt + (index % mask_size)) ].red;
							temp_green 	+= convolutionMatrix[index] * pixpack_source[pos + ((index / mask_size)  * columns) + (-mask_cnt + (index % mask_size)) ].green;
							temp_blue 	+= convolutionMatrix[index] * pixpack_source[pos + ((index / mask_size)  * columns) + (-mask_cnt + (index % mask_size)) ].blue;
						}
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

	free(convolutionMatrix);

	return 0;
}

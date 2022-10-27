/* service_gausslowpass2_fpga.c */

#include "service_gausslowpass2.h"

/* =====================================================================
 * Function: double factorial(int n); 
 * To calculate the n!-Factorial .
 */ 

double factorial(int32_t n)
{
   int32_t c;
   double result = 1.0;
 
   for( c = 1 ; c <= n ; c++ )
         result = result*c;
 
   return ( result );
}


/* =====================================================================
 * Main?!
 */ 

int32_t run_service_gausslowpass2_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mask_size) {
	
	if (mask_size > 13 || mask_size < 0) {
		fprintf(stderr, "This Algorithm ONLY supports odd numbers for mask_size less or equal to 13 (<=13) and greater than 0! The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	uint32_t i, j, index1, p_size, mask_size_const, mask_size_cnt, mask_2temp;
	uint32_t sum;

	mask_size_const = 13; 
	mask_size_cnt = 13 >> 1;
	mask_2temp = mask_size  >> 1;
	p_size = mask_size - 1; /* The row of the Pascal Triangle */

	uint32_t *pascalVector = (uint32_t*) malloc (mask_size_const*sizeof(uint32_t)); /* Allocate dynmanic memory */
	
	if (NULL == pascalVector){
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}
	
	 uint32_t * convolutionMatrix = NULL;  
	 convolutionMatrix = (uint32_t*) malloc(mask_size_const*mask_size_const*sizeof(uint32_t));  /* Allocate dynmanic memory */

	if (NULL == convolutionMatrix){
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}
      
	for( index1 = 0 ; index1 < mask_size_const ; ++index1) {
		if (index1 >= (mask_size_cnt - mask_2temp) && index1 <= (mask_size_cnt + mask_2temp)) {
		  pascalVector[index1] = factorial(p_size) / (factorial(index1 - (mask_size_cnt - mask_2temp)) * factorial(p_size - (index1 - (mask_size_cnt - mask_2temp))));  /* The (index1)mth entry of the (p_size)nth row of Pascal's triangle! */
		}
		else
			pascalVector[index1] = 0; // unwanted values are set to 0!
	}

	sum = 0;
	for (i = 0; i < mask_size_const; ++i) { /* Fill in the "convolutionMatrix"  and calculate sum! */
	  for (j = 0; j < mask_size_const; ++j) {
	    convolutionMatrix [i* mask_size_const + j] = pascalVector[i] * pascalVector[j]; // unwanted values are set to 0!
	    sum += convolutionMatrix [i * mask_size_const + j];
	  }
	}

	run_service_gausslowpass2_max((uint16_t *)pixpack_target, (uint16_t *)pixpack_source, convolutionMatrix, rows, columns, sum);
	
	/* free allocated memory */

	free (pascalVector);
	free (convolutionMatrix);
	return 0;
}

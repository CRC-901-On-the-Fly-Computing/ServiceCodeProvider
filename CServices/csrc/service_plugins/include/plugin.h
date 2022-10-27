/* service_plugin.h */

#ifndef __SERVICE_PLUGIN_H__
#define __SERVICE_PLUGIN_H__

#include <stdint.h>

#define QuantumRange UINT16_MAX

typedef uint16_t Quantum;

typedef struct __PixelPacket__ {
	Quantum blue;
	Quantum green;
	Quantum red;
	Quantum opacity;
} PixelPacket;

typedef struct __Image__ {
	PixelPacket *image;
	int32_t rows;
	int32_t columns;
	void *j_arr;
} Image;

void run_service(char resource, void *handler_gpu, Image **target_images, int32_t *target_images_size, Image *source_images, int32_t source_images_size, double *params, int32_t *status_code);

void get_service_ids(char ***id_strings, int32_t *id_strings_size);
void get_service_info(char ***info_strings, int32_t *info_strings_size);
void get_service_params(char ***param_strings, int32_t *param_strings_size);
void get_service_resources(char ***resource_strings, int32_t *resource_strings_size);
int32_t get_number_of_input_images(void);
int32_t get_number_of_output_images(void);

#endif /* __SERVICE_PLUGIN_H__ */

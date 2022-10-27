/* service_deform.c */

#include "service_deform.h"

typedef int32_t (*run_service_deform_gpu_t)(PixelPacket *, PixelPacket *, int32_t, int32_t, float, float, int32_t, int32_t, int32_t);

void run_service(char resource, void *handler_gpu, Image **target_images, int32_t *target_images_size, Image *source_images, int32_t source_images_size, double *params, int32_t *status_code) {
	*target_images = (Image *)malloc(sizeof(Image));
	if (NULL == *target_images) {
		fprintf(stderr, "!!! ERROR: Cannot allocate memory.");
		*status_code = -1;
		return;
	}

	(*target_images)[0].rows	= source_images[0].rows;
	(*target_images)[0].columns	= source_images[0].columns;
	*target_images_size			= 1;
	(*target_images)[0].image	= malloc((*target_images)[0].rows * (*target_images)[0].columns * sizeof(PixelPacket));
	if (NULL == (*target_images)[0].image) {
		fprintf(stderr, "!!! ERROR: Cannot allocate memory.");
		*status_code = -1;
		return;
	}

	run_service_deform_gpu_t run_service_deform_gpu;
	char *dlsym_error;

	switch (resource) {
		case 'c':
			*status_code = run_service_deform_cpu((*target_images)[0].image, source_images[0].image, source_images[0].rows, source_images[0].columns, (float) params[0], (float) params[1], (int32_t) params[2], (int32_t) params[3], (int32_t) params[4]);
			break;
		case 'g':
			#ifdef PGCC
			handler_gpu = dlopen("../bin/plugins/libservice_deform_gpu.so", RTLD_LAZY | RTLD_GLOBAL);
			if (NULL == handler_gpu) {
				fprintf(stderr, "Cannot open library libservice_deform_gpu.so: %s (file: %s, line: %i)\n", dlerror(), __FILE__, __LINE__);
				exit(EXIT_FAILURE);
			}
			#endif
			run_service_deform_gpu = (run_service_deform_gpu_t) dlsym(handler_gpu, "run_service_deform_gpu");
			dlsym_error = dlerror();
			if (dlsym_error) {
				fprintf(stderr, "Cannot load symbol \"run_service_deform_gpu\": %s (file: %s, line: %i)\n", dlsym_error, __FILE__, __LINE__);
				break;
			}
			*status_code = run_service_deform_gpu((*target_images)[0].image, source_images[0].image, source_images[0].rows, source_images[0].columns, (float) params[0], (float) params[1], (int32_t) params[2], (int32_t) params[3], (int32_t) params[4]);
			#ifdef PGCC
			dlclose(handler_gpu);
			#endif
			break;
		case 'f':
			*status_code = run_service_deform_fpga((*target_images)[0].image, source_images[0].image, source_images[0].rows, source_images[0].columns, (float) params[0], (float) params[1], (int32_t) params[2], (int32_t) params[3], (int32_t) params[4]);
			break;
		case 's':
		default:
			*status_code = run_service_deform_scpu((*target_images)[0].image, source_images[0].image, source_images[0].rows, source_images[0].columns, (float) params[0], (float) params[1], (int32_t) params[2], (int32_t) params[3], (int32_t) params[4]);
			break;
	}
}

void get_service_ids(char ***id_strings, int32_t *id_strings_size) {
	char *strings[] = {
		"DEF",
		"def",
		"DEFORM",
		"deform"
	};
	*id_strings_size = 4;

	malloc_copy_string_array(id_strings, strings, *id_strings_size);
}

void get_service_info(char ***info_strings, int32_t *info_strings_size) {
	char *strings[] = {
		"Deforms the image from the point (picturewidth*positionx/100, pictureheight*positiony/100).\n",
		"The origin is the top left corner."
	};
	*info_strings_size = 2;

	malloc_copy_string_array(info_strings, strings, *info_strings_size);
}

void get_service_params(char ***param_strings, int32_t *param_strings_size) {
	char *strings[] = {"radius", "angle", "positionx", "positiony", "mode"};
	*param_strings_size = 5;

	malloc_copy_string_array(param_strings, strings, *param_strings_size);
}

void get_service_resources(char ***resource_strings, int32_t *resource_strings_size) {
	char *strings[] = {
		"CPU", "SCPU", "GPU", "FPGA",
		"cpu", "scpu", "gpu", "fpga",
		"C"  , "S"   , "G"  , "F"   ,
		"c"  , "s"   , "g"  , "f"
	};
	*resource_strings_size = 16;

	malloc_copy_string_array(resource_strings, strings, *resource_strings_size);
}

int32_t get_number_of_input_images(void) {
	return 1;
}

int32_t get_number_of_output_images(void) {
	return 1;
}

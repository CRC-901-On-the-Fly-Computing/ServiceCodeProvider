/* plugin_bridge.c */

#include "include/C2Plugins_Plugin.h"
#include "include/common.h"

#include "../service_plugins/include/plugin.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>

typedef void (*get_string_array_t)(char ***, int32_t *);
typedef void (*run_service_t)(char, void *, Image **, int32_t *, Image *, int32_t, double *, int32_t *);
typedef int32_t (*get_number_of_images_t)(void);

JNIEXPORT jlong JNICALL Java_C2Plugins_Plugin_c_1loadLibrary(JNIEnv *env, jobject obj, jstring j_filename) {
	const char *filename = (*env)->GetStringUTFChars(env, j_filename, 0);

	void *handler = dlopen(filename, RTLD_LAZY | RTLD_LOCAL);
	if (NULL == handler) {
		fprintf(stderr, "Cannot open library \"%s\": %s (file: %s, line: %i)\n", filename, dlerror(), __FILE__, __LINE__);
	}
	
	(*env)->ReleaseStringUTFChars(env, j_filename, filename);
	
	jlong j_handler = (jlong)handler;
	
	return j_handler;
}

JNIEXPORT jlong JNICALL Java_C2Plugins_Plugin_c_1loadLibraryGPU(JNIEnv *env, jobject obj, jlong j_handler, jstring j_filename) {
	void *handler		= (void *)j_handler;
	void *handler_gpu	= NULL;
	
	char **resource_strings;
	int32_t resource_strings_size = 0;

	#ifndef PGCC
	// Find pointer to the correct plugin's get_service_resources.
	get_string_array_t get_service_resources = (get_string_array_t) dlsym(handler, "get_service_resources");
	const char *dlsym_error = dlerror();
	if (dlsym_error) {
		fprintf(stderr, "Cannot load symbol \"get_service_resources\": %s (file: %s, line: %i)\n", dlsym_error, __FILE__, __LINE__);
	}
	get_service_resources(&resource_strings, &resource_strings_size);
	
	int32_t i;
	for (i=0; i<resource_strings_size; ++i) {
		if (resource_strings[i][0] == 'g' && resource_strings[i][1] == 0) {
			const char *filename = (*env)->GetStringUTFChars(env, j_filename, 0);
			
			handler_gpu = dlopen(filename, RTLD_LAZY | RTLD_GLOBAL);
			if (NULL == handler_gpu) {
				fprintf(stderr, "Cannot open library \"%s\": %s (file: %s, line: %i)\n", filename, dlerror(), __FILE__, __LINE__);
			}
			
			(*env)->ReleaseStringUTFChars(env, j_filename, filename);
			
			break;
		}
	}
	
	free_string_array(&resource_strings, resource_strings_size);
	#endif
		
	return (jlong)handler_gpu;
}

JNIEXPORT void JNICALL Java_C2Plugins_Plugin_c_1unloadLibraries(JNIEnv *env, jobject obj, jlong j_handler, jlong j_handler_gpu) {
	
	void *handler_gpu	= (void *)j_handler_gpu;
	void *handler		= (void *)j_handler;
	
	#ifndef PGCC
	if (NULL != handler_gpu) {
		dlclose(handler_gpu);
	}
	#endif
	
	dlclose(handler);
}

JNIEXPORT jobjectArray JNICALL Java_C2Plugins_Plugin_c_1getServiceIDs(JNIEnv *env, jobject obj, jlong j_handler) {
	void *handler = (void *)j_handler;
	
	jobjectArray j_string_array;
	
	char **id_strings;
	int32_t id_strings_size = 0;
	
	// Find pointer to the correct plugin's get_service_ids.
	get_string_array_t get_service_ids = (get_string_array_t) dlsym(handler, "get_service_ids");
	const char *dlsym_error = dlerror();
	if (dlsym_error) {
		fprintf(stderr, "Cannot load symbol \"get_service_ids\": %s (file: %s, line: %i)\n", dlsym_error, __FILE__, __LINE__);
	}
	get_service_ids(&id_strings, &id_strings_size);
	
	// New string array for id strings.
	j_string_array = (jobjectArray)(*env)->NewObjectArray(env, id_strings_size, (*env)->FindClass(env, "java/lang/String"), (*env)->NewStringUTF(env, ""));
	
	int32_t i;
	for (i=0; i<id_strings_size; ++i) {
		// Store id strings to Java string array.
		(*env)->SetObjectArrayElement(env, j_string_array, i, (*env)->NewStringUTF(env, id_strings[i]));
	}
	
	free_string_array(&id_strings, id_strings_size);
	
	return j_string_array;
}

JNIEXPORT jobjectArray JNICALL Java_C2Plugins_Plugin_c_1getServiceInfo(JNIEnv *env, jobject obj, jlong j_handler) {
	void *handler = (void *)j_handler;
	
	jobjectArray j_string_array;
	
	char **info_strings;
	int32_t info_strings_size = 0;
	
	// Find pointer to the correct plugin's get_service_info.
	get_string_array_t get_service_info = (get_string_array_t) dlsym(handler, "get_service_info");
	const char *dlsym_error = dlerror();
	if (dlsym_error) {
		fprintf(stderr, "Cannot load symbol \"get_service_info\": %s (file: %s, line: %i)\n", dlsym_error, __FILE__, __LINE__);
	}
	get_service_info(&info_strings, &info_strings_size);
	
	// New string array for info strings.
	j_string_array = (jobjectArray)(*env)->NewObjectArray(env, info_strings_size, (*env)->FindClass(env, "java/lang/String"), (*env)->NewStringUTF(env, ""));
	
	int32_t i;
	for (i=0; i<info_strings_size; ++i) {
		// Store info strings to Java string array.
		(*env)->SetObjectArrayElement(env, j_string_array, i, (*env)->NewStringUTF(env, info_strings[i]));
	}
	
	free_string_array(&info_strings, info_strings_size);
	
	return j_string_array;
}

JNIEXPORT jobjectArray JNICALL Java_C2Plugins_Plugin_c_1getServiceParams(JNIEnv *env, jobject obj, jlong j_handler) {
	void *handler = (void *)j_handler;
	
	jobjectArray j_string_array;
	
	char **param_strings;
	int32_t param_strings_size = 0;
	
	// Find pointer to the correct plugin's get_service_params.
	get_string_array_t get_service_params = (get_string_array_t) dlsym(handler, "get_service_params");
	const char *dlsym_error = dlerror();
	if (dlsym_error) {
		fprintf(stderr, "Cannot load symbol \"get_service_params\": %s (file: %s, line: %i)\n", dlsym_error, __FILE__, __LINE__);
	}
	get_service_params(&param_strings, &param_strings_size);
	
	// New string array for param strings.
	j_string_array = (jobjectArray)(*env)->NewObjectArray(env, param_strings_size, (*env)->FindClass(env, "java/lang/String"), (*env)->NewStringUTF(env, ""));
	
	int32_t i;
	for (i=0; i<param_strings_size; ++i) {
		// Store param strings to Java string array.
		(*env)->SetObjectArrayElement(env, j_string_array, i, (*env)->NewStringUTF(env, param_strings[i]));
	}
	
	free_string_array(&param_strings, param_strings_size);
	
	return j_string_array;
}

JNIEXPORT jobjectArray JNICALL Java_C2Plugins_Plugin_c_1getServiceResources(JNIEnv *env, jobject obj, jlong j_handler) {
	void *handler = (void *)j_handler;
	
	jobjectArray j_string_array;
	
	char **resource_strings;
	int32_t resource_strings_size = 0;
	
	// Find pointer to the correct plugin's get_service_resources.
	get_string_array_t get_service_resources = (get_string_array_t) dlsym(handler, "get_service_resources");
	const char *dlsym_error = dlerror();
	if (dlsym_error) {
		fprintf(stderr, "Cannot load symbol \"get_service_resources\": %s (file: %s, line: %i)\n", dlsym_error, __FILE__, __LINE__);
	}
	get_service_resources(&resource_strings, &resource_strings_size);
	
	// New string array for resource strings.
	j_string_array = (jobjectArray)(*env)->NewObjectArray(env, resource_strings_size, (*env)->FindClass(env, "java/lang/String"), (*env)->NewStringUTF(env, ""));
	
	int32_t i;
	for (i=0; i<resource_strings_size; ++i) {
		// Store resource strings to Java string array.
		(*env)->SetObjectArrayElement(env, j_string_array, i, (*env)->NewStringUTF(env, resource_strings[i]));
	}
	
	free_string_array(&resource_strings, resource_strings_size);
	
	return j_string_array;
}

JNIEXPORT jobject JNICALL Java_C2Plugins_Plugin_c_1process(JNIEnv *env, jobject obj, jlong j_handler, jlong j_handler_gpu, jchar j_resource, jobject j_param_list, jobject j_source_image_list) {
	int32_t i;

	void *handler		= (void *)j_handler;
	void *handler_gpu	= (void *)j_handler_gpu;
	char resource		= (char)j_resource;

	jclass cls_list				= (*env)->FindClass(env, "java/util/ArrayList");
	jmethodID mid_list_init		= (*env)->GetMethodID(env, cls_list, "<init>", "()V");
	jmethodID mid_list_size		= (*env)->GetMethodID(env, cls_list, "size", "()I");
	jmethodID mid_list_get		= (*env)->GetMethodID(env, cls_list, "get", "(I)Ljava/lang/Object;");
	jmethodID mid_list_add		= (*env)->GetMethodID(env, cls_list, "add", "(Ljava/lang/Object;)Z");

	jclass cls_double			= (*env)->FindClass(env, "java/lang/Double");
	jmethodID mid_double_value	= (*env)->GetMethodID(env, cls_double, "doubleValue", "()D");

	jclass cls_image			= (*env)->FindClass(env, "C2Data/C2Image");
	jmethodID mid_image_init	= (*env)->GetMethodID(env, cls_image, "<init>", "([SII)V");
	jmethodID mid_image_rows	= (*env)->GetMethodID(env, cls_image, "getRows", "()I");
	jmethodID mid_image_columns	= (*env)->GetMethodID(env, cls_image, "getColumns", "()I");
	jmethodID mid_image_pixels	= (*env)->GetMethodID(env, cls_image, "getPixels", "()[S");

	// get the parameters
	int32_t param_list_size		= (*env)->CallIntMethod(env, j_param_list, mid_list_size);
	double params[param_list_size];

	for (i=0; i<param_list_size; ++i) {
		jobject j_double_object	= (*env)->CallObjectMethod(env, j_param_list, mid_list_get, i);
		params[i]				= (*env)->CallDoubleMethod(env, j_double_object, mid_double_value);
	}

	// get source images and pack it to image structs
	int32_t source_image_list_size		= (*env)->CallIntMethod(env, j_source_image_list, mid_list_size);
	Image source_images[source_image_list_size];

	for (i=0; i<source_image_list_size; ++i) {
		jobject j_image_object		= (*env)->CallObjectMethod(env, j_source_image_list, mid_list_get, i);
		source_images[i].rows		= (*env)->CallIntMethod(env, j_image_object, mid_image_rows);
		source_images[i].columns	= (*env)->CallIntMethod(env, j_image_object, mid_image_columns);

		jshortArray	j_image_pixels	= (jshortArray)(*env)->CallObjectMethod(env, j_image_object, mid_image_pixels);
		source_images[i].j_arr		= j_image_pixels;
		jshort *pixels				= (*env)->GetShortArrayElements(env, j_image_pixels, 0);
		source_images[i].image		= (PixelPacket *)pixels;
	}

	int32_t status					= -1;
	Image *target_images			= NULL;
	int32_t target_image_list_size	= -1;

	// run service on one of the available resources
	run_service_t run_service = (run_service_t) dlsym(handler, "run_service");
	const char *dlsym_error = dlerror();
	if (dlsym_error) {
		fprintf(stderr, "Cannot load symbol \"run_service\": %s (file: %s, line: %i)\n", dlsym_error, __FILE__, __LINE__);
	}
	run_service(resource, handler_gpu, &target_images, &target_image_list_size, source_images, source_image_list_size, params, &status);

	if (status) {
		fprintf(stderr, "!!! ERROR: error while executing cpu/gpu/fpga code.");
		(*env)->ThrowNew(env, (*env)->FindClass(env, "java.lang.Error"), "!!! ERROR: error while executing cpu/gpu/fpga code.");
		return j_source_image_list;
	}

	// release the source images
	for (i=0; i<source_image_list_size; ++i) {
		(*env)->ReleaseShortArrayElements(env, source_images[i].j_arr, (jshort *)(source_images[i].image), JNI_ABORT);
	}

	// build and fill target image list
	jobject j_target_image_list = (*env)->NewObject(env, cls_list, mid_list_init);
	for (i=0; i<target_image_list_size; ++i) {
		int32_t size = target_images[i].rows * target_images[i].columns * 4;
		jshortArray j_image_pixels	= (*env)->NewShortArray(env, size);
		(*env)->SetShortArrayRegion(env, j_image_pixels, 0, size, (jshort *)(target_images[i].image));
		jobject j_image_object		= (*env)->NewObject(env, cls_image, mid_image_init, j_image_pixels, target_images[i].rows, target_images[i].columns);

		(*env)->CallBooleanMethod(env, j_target_image_list, mid_list_add, j_image_object);
	}

	// free allocated heap space (allocated in run_service_*() functions)
	for (i=0; i<target_image_list_size; ++i) {
		free(target_images[i].image);
		free(target_images+i);
	}

	return j_target_image_list;
}

JNIEXPORT jint JNICALL Java_C2Plugins_Plugin_c_1getNumberOfInputImages(JNIEnv *env, jobject obj, jlong j_handler) {
	void *handler = (void *)j_handler;
	
	int32_t number_of_images;
	
	// Find pointer to the correct plugin's get_service_resources.
	get_number_of_images_t get_number_of_input_images = (get_number_of_images_t) dlsym(handler, "get_number_of_input_images");
	const char *dlsym_error = dlerror();
	if (dlsym_error) {
		fprintf(stderr, "Cannot load symbol \"get_number_of_input_images\": %s (file: %s, line: %i)\n", dlsym_error, __FILE__, __LINE__);
	}
	number_of_images = get_number_of_input_images();
	
	return number_of_images;
}

JNIEXPORT jint JNICALL Java_C2Plugins_Plugin_c_1getNumberOfOutputImages(JNIEnv *env, jobject obj, jlong j_handler) {
	void *handler = (void *)j_handler;
	
	int32_t number_of_images;
	
	// Find pointer to the correct plugin's get_service_resources.
	get_number_of_images_t get_number_of_output_images = (get_number_of_images_t) dlsym(handler, "get_number_of_output_images");
	const char *dlsym_error = dlerror();
	if (dlsym_error) {
		fprintf(stderr, "Cannot load symbol \"get_number_of_output_images\": %s (file: %s, line: %i)\n", dlsym_error, __FILE__, __LINE__);
	}
	number_of_images = get_number_of_output_images();
	
	return number_of_images;
}

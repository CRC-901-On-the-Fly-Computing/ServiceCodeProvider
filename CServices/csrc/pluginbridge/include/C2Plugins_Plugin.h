/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class C2Plugins_Plugin */

#ifndef _Included_C2Plugins_Plugin
#define _Included_C2Plugins_Plugin
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     C2Plugins_Plugin
 * Method:    c_loadLibrary
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_C2Plugins_Plugin_c_1loadLibrary
  (JNIEnv *, jobject, jstring);

/*
 * Class:     C2Plugins_Plugin
 * Method:    c_loadLibraryGPU
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_C2Plugins_Plugin_c_1loadLibraryGPU
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     C2Plugins_Plugin
 * Method:    c_unloadLibraries
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_C2Plugins_Plugin_c_1unloadLibraries
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     C2Plugins_Plugin
 * Method:    c_getServiceIDs
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_C2Plugins_Plugin_c_1getServiceIDs
  (JNIEnv *, jobject, jlong);

/*
 * Class:     C2Plugins_Plugin
 * Method:    c_getServiceInfo
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_C2Plugins_Plugin_c_1getServiceInfo
  (JNIEnv *, jobject, jlong);

/*
 * Class:     C2Plugins_Plugin
 * Method:    c_getServiceParams
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_C2Plugins_Plugin_c_1getServiceParams
  (JNIEnv *, jobject, jlong);

/*
 * Class:     C2Plugins_Plugin
 * Method:    c_getServiceResources
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_C2Plugins_Plugin_c_1getServiceResources
  (JNIEnv *, jobject, jlong);

/*
 * Class:     C2Plugins_Plugin
 * Method:    c_process
 * Signature: (JJCLjava/util/List;Ljava/util/List;)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_C2Plugins_Plugin_c_1process
  (JNIEnv *, jobject, jlong, jlong, jchar, jobject, jobject);

/*
 * Class:     C2Plugins_Plugin
 * Method:    c_getNumberOfInputImages
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_C2Plugins_Plugin_c_1getNumberOfInputImages
  (JNIEnv *, jobject, jlong);

/*
 * Class:     C2Plugins_Plugin
 * Method:    c_getNumberOfOutputImages
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_C2Plugins_Plugin_c_1getNumberOfOutputImages
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif

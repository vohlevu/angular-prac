#include <stdio.h>
#include "jni.h"
#include <debug.h>

static jint
add(JNIEnv *env, jobject thiz, jint a, jint b) {
	int result = a + b;
	printf("%d + %d = %d", a, b, result);
	return result;
}

static const char *classPathName = "com.example.samplejni.Native";

static JNINativeMethod methods[] = {
	{"add", "(II)I", (void*)add },
};

/*
* Register several native methods for one class.
*/
static int registerNativeMethods(JNIEnv* env, const char* className,
JNINativeMethod* gMethods, int numMethods)
{
	jclass clazz;
	clazz = env->FindClass(className);
	if (clazz == NULL) {
		printf("Native registration unable to find class '%s'", className);
		return JNI_FALSE;
	}
	if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
		printf("RegisterNatives failed for '%s'", className);
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

/*
* Register native methods for all classes we know about.
*
* returns JNI_TRUE on success.
*/
static int registerNatives(JNIEnv* env)
{
	if (!registerNativeMethods(env, classPathName,
				methods, sizeof(methods) / sizeof(methods[0]))) {
		return JNI_FALSE;
	}

	return JNI_TRUE;
}


// ----------------------------------------------------------------------------

/*
* This is called by the VM when the shared library is first loaded.
*/

typedef union {
	JNIEnv* env;
	void* venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	UnionJNIEnvToVoid uenv;
	uenv.venv = NULL;
	jint result = -1;
	JNIEnv* env = NULL;
	
	printf("JNI_OnLoad");

	if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
		printf("ERROR: GetEnv failed");
		goto bail;
	}
	env = uenv.env;

	if (registerNatives(env) != JNI_TRUE) {
		printf("ERROR: registerNatives failed");
		goto bail;
	}
	
	result = JNI_VERSION_1_4;
	
bail:
	return result;
}

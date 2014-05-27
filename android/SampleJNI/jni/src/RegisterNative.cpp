#include <stdio.h>
#include "jni.h"
#include "debug.h"
#include "NativeFunction.h"


static const char *classPathName = "com/example/samplejni/Native";

static JNINativeMethod methods[] = {
	{"sum", "(ID)D", (void*)sum },
	{"passBooleanReturnBoolean", "(Z)Z", (void*)Function_passBooleanReturnBoolean },
	{"passStrReturnStr", "(Ljava/lang/String;)Ljava/lang/String;", (void*)Function_passStrReturnStr },
	{"passCharReturnChar", "(C)C", (void*)Function_passCharReturnChar },
	{"staticFieldAccess", "()V", (void*)Function_staticFieldAccess },
	//{"staticMethodCall", "()V", (void*)Function_staticMethodCall },
	{"reverse2", "([I)[I", (void*)Function_reverse2 },
	{"modifyInstanceVariable", "()V", (void*)Function_modifyInstanceVariable },
	
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
	gJavaVM = vm;
	UnionJNIEnvToVoid uenv;
	uenv.venv = NULL;
	jint result = -1;
	JNIEnv* env = NULL;
	
	printf("JNI_OnLoad");

	if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_6) != JNI_OK) {
		printf("ERROR: GetEnv failed");
		goto bail;
	}
	env = uenv.env;

	if (registerNatives(env) != JNI_TRUE) {
		printf("ERROR: registerNatives failed");
		goto bail;
	}
	
	result = JNI_VERSION_1_6;
	
bail:
	return result;
}

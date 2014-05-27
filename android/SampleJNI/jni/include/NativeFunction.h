#ifndef FUNCTION_H
#define FUNCTION_H
#ifdef __cplusplus        // c and c++
extern "C" {
#endif
static JavaVM *gJavaVM;

	jdouble sum(JNIEnv *env, jobject thiz, jint a, jdouble b);
	jboolean Function_passBooleanReturnBoolean(JNIEnv *pEnv,jobject pObj, jboolean pBooleanP);
	jchar Function_passCharReturnChar(JNIEnv *pEnv,jobject pObj, jchar pCharP);
	jstring  Function_passStrReturnStr(JNIEnv *env, jobject thisObj, jstring inJNIStr);
	void Function_staticFieldAccess(JNIEnv *env, jobject obj);
	//void Function_staticMethodCall (JNIEnv *env, jobject obj);
	jintArray Function_reverse2(JNIEnv *pEnv,jobject pObj, jintArray intArr);
	void Function_modifyInstanceVariable(JNIEnv *env, jobject thisObj);
#ifdef __cplusplus
}
#endif
#endif /* FUNCTION_H */

#include <stdio.h>
#include <jni.h>
#include <debug.h>
#include "NativeFunction.h"

#define ERROR_MESSAGE_LENGTH 1024*5    //Error message length
#define JNI_EXCEPTION_CLASS_NAME "java/lang/IllegalArgumentException"
#define THROW_JAVA_EXCEPTION(_INFO_) \
   ThrowJNIException(__FILE__,__LINE__, _INFO_); 

char  g_azErrorMessage[ERROR_MESSAGE_LENGTH] = {0};

jdouble sum(JNIEnv *env, jobject thiz, jint a, jdouble b) {
	double result = a + b;
	printf("%d + %f = %f", a, b, result);
	return result;
}
jboolean Function_passBooleanReturnBoolean(JNIEnv *pEnv, jobject pObj, jboolean pBooleanP){
	printf( "%d in  %d bytes", pBooleanP, sizeof(jboolean));
	return (!pBooleanP);
}
jint Function_passIntReturnInt(JNIEnv *pEnv, jobject pObj, jint pIntP) {
	printf( "%d in %d bytes", pIntP, sizeof(jint));
	return pIntP + 1;
}
jdouble Function_passDoubleReturnDouble(JNIEnv *pEnv, jobject pObj, jdouble pDoubleP) {
	printf( "%f in %d bytes", pDoubleP, sizeof(jdouble));
	return pDoubleP + 0.5;
}

jbyte Function_passByteReturnByte(JNIEnv *pEnv, jobject pObj, jbyte pByteP){
	printf( "%d in %d bytes", pByteP, sizeof(jbyte));
	return pByteP;
}
jchar Function_passCharReturnChar(JNIEnv *pEnv, jobject pObj, jchar pCharP){
	printf( "%d in %d bytes", pCharP, sizeof(jchar));
	return pCharP;
}
jshort Function_passShortReturnShort(JNIEnv *pEnv, jobject pObj, jshort pShortP){
	printf( "%d in %d bytes", pShortP, sizeof(jshort));
	return pShortP;
}
jlong Function_passLongReturnLong(JNIEnv *pEnv, jobject pObj, jlong pLongP){
	printf( "%lld in %d bytes", pLongP, sizeof(jlong));
	return pLongP;
}
jfloat passFloatReturnFloat(JNIEnv *pEnv, jobject pObj, jfloat pFloatP){
	printf( "%f in %d bytes", pFloatP, sizeof(jfloat));
	return pFloatP;
}

jstring  Function_passStrReturnStr(JNIEnv *env, jobject thisObj, jstring inJNIStr) {
	const char *inCStr = (*env)->GetStringUTFChars(env, inJNIStr, NULL);
	if (inCStr == NULL) return NULL;
	
	char outCStr[128] = "";
	//strncpy (outCStr, inCStr, sizeof(outCStr));
	int len = (*env)->GetStringLength(env, inJNIStr);;
	for (int k = 0; k < len; k++)
		outCStr[k] = inCStr[len-k-1];
	(*env)->ReleaseStringUTFChars(env, inJNIStr, inCStr);  // release inCStr
	return (*env)->NewStringUTF(env, outCStr);
}


void Function_staticFieldAccess(JNIEnv *env, jobject obj){
	static jfieldID fid_s = NULL; /* cached field ID  */
	jint si;
	jclass cls = (*env)->GetObjectClass(env, obj);
	//jclass cls = (*env)->FindClass(env,"com/example/samplejni/SampleJNI");
	if (fid_s == NULL){
		fid_s = (*env)->GetStaticFieldID(env, cls, "si", "I");
		if (fid_s == NULL) {
			return; /* field not found */
		}
	}
	si = (*env)->GetStaticIntField(env, cls, fid_s);
	printf(" ======StaticFieldAccess.si = %d\n", si);
	(*env)->SetStaticIntField(env, cls, fid_s, 2*si);
}


void Function_staticMethodCall (JNIEnv *env, jobject obj){
	//jthrowable exc;
	jclass cls = (*env)->GetObjectClass(env, obj);
	//jclass cls = (*env)->FindClass(env,"com/example/samplejni/SampleJNI");
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "callback", "()Ljava/lang/String;");
	//exc=(*env)->ExceptionOccurred(env);
	/*if(exc){

		THROW_JAVA_EXCEPTION("Error allocating memory.");
	}*/
	if (mid == NULL) {
		return; /* method not found */
	}
	printf("==========In C\n");
	(*env)->CallStaticVoidMethod(env, cls, mid);
}


jintArray Function_reverse2(JNIEnv *pEnv, jobject pObj, jintArray intArr){
	jint *ia = (*pEnv)->GetIntArrayElements(pEnv,intArr, 0);
	if (ia == 0) {//NULL
			return 0; /* exception occurred */
	}
	jsize len = (*pEnv)->GetArrayLength(pEnv,intArr);
	jint *newia = (jint*)malloc(len * sizeof(jint));
	for (int k = 0; k < len; k++)
		newia[k] = ia[len-k-1];
	(*pEnv)->ReleaseIntArrayElements(pEnv, intArr, ia, 0);
	jintArray newintArr = (*pEnv)->NewIntArray(pEnv, len);
	(*pEnv)->SetIntArrayRegion(pEnv, newintArr, 0, len, newia);
	return newintArr;
}


void Function_modifyInstanceVariable(JNIEnv *env, jobject thisObj){
	jclass thisClass = (*env)->GetObjectClass(env, thisObj);
	//jclass thisClass = (*env)->FindClass(env,"com/example/samplejni/SampleJNI");
	
	// modify integer "number" field
	jfieldID fidNumber = (*env)->GetFieldID(env, thisClass, "number", "I");
	if (NULL == fidNumber) return;
	jint number = (*env)->GetIntField(env, thisObj, fidNumber);
	number = 99;
	(*env)->SetIntField(env, thisObj, fidNumber, number);
	
	// modify string "message" field
	jfieldID fidMessage = (*env)->GetFieldID(env, thisClass, "message", "Ljava/lang/String;");
	if (NULL == fidMessage) return;
	jstring message = (*env)->GetObjectField(env, thisObj, fidMessage);
	const char *cStr = (*env)->GetStringUTFChars(env, message, NULL);
	if (NULL == cStr) return;
	(*env)->ReleaseStringUTFChars(env, message, cStr);
	message = (*env)->NewStringUTF(env, "Hello from C");
	if (NULL == message) return;
	(*env)->SetObjectField(env, thisObj, fidMessage, message);
}


 void ThrowJNIException(const char* pzFile, int iLine,const char* pzMessage) {
    //Check for null parameter
    if(pzFile != NULL && pzMessage != NULL && iLine != 0)
       sprintf(g_azErrorMessage,"JNIException ! \n \
       File :  %s \n \
       Line number : %d \n \
       Reason for Exception: %s ",pzFile,iLine,pzMessage);
    jclass    tClass        = NULL;
    //Findout the exception handling class
    JNIEnv *env;
    int status;
    int isAttached = 0;

    if ((status = (*gJavaVM)->GetEnv(gJavaVM, (void**)&env, JNI_VERSION_1_6)) < 0) {
        if ((status = (*gJavaVM)->AttachCurrentThread(gJavaVM, &env, NULL)) < 0) {
		
            return;
        }
       isAttached = 1;
    }
    
	if( env == NULL) {
	 //  if (isAttached) (*gJavaVM)->DetachCurrentThread(gJavaVM);
       printf("Invalid null pointer in ThrowJNIException " );
       return;
    }
	//(*env)->ExceptionDescribe(env);
	(*env)->ExceptionClear(env);

    tClass = (*env)->FindClass(env,JNI_EXCEPTION_CLASS_NAME);
    if (tClass == 0) {
      printf("Not found java/lang/IllegalArgumentException");
      return;
    }
    //Throw the excption with error info
    (*env)->ThrowNew(env,tClass,g_azErrorMessage );
    (*env)->DeleteLocalRef(env,tClass);

    if (isAttached) (*gJavaVM)->DetachCurrentThread(gJavaVM);

 }




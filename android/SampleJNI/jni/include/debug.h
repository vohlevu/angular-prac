#include <android/log.h>
//#include <utils/Log.h>

#define LOG_TAG "SampleJNI.cpp"
#define printf(...) __android_log_print(ANDROID_LOG_DEBUG, "NATIVE", __VA_ARGS__);
//#define printf(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "<<<<< %s (%s, %d) >>>>>", __FUNCTION__, __FILE__, __LINE__);

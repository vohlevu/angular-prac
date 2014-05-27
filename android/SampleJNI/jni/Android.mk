LOCAL_PATH := $(call my-dir)
####################################
include $(CLEAR_VARS)
MY_SRC_DIR = src
MY_INCLUDES_DIR = $(LOCAL_PATH)/include
######################################
LOCAL_MODULE    := libSampleJNI
LOCAL_SRC_FILES := \
				$(MY_SRC_DIR)/NativeFunction.c \
				$(MY_SRC_DIR)/RegisterNative.cpp
				
LOCAL_C_INCLUDES += \
					$(MY_INCLUDES_DIR)
#					$(JNI_H_INCLUDE)

# All of the shared libraries we link against.
LOCAL_SHARED_LIBRARIES := \
							liblog
#							libutils
# No static libraries.
LOCAL_STATIC_LIBRARIES := 
#dynamic libraries
				
# Compiler flags.
LOCAL_CFLAGS += -Wno-write-strings -std=c99 -std=gnu99 -Wimplicit-function-declaration

LOCAL_MODULE_TAGS := eng							
LOCAL_LDLIBS := -llog -landroid 

LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)
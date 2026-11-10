LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ConnectMediator_static

LOCAL_MODULE_FILENAME := libConnectMediator

LOCAL_SRC_FILES := \
src/Common/CommandList.cpp \
src/Common/GlobalVar.cpp \

LOCAL_EXPORT_C_INCLUDES := \
$(LOCAL_PATH)/src/Common \
$(LOCAL_PATH)/src/Controller/Command \
$(LOCAL_PATH)/src/Controller/Command/cs \
$(LOCAL_PATH)/src/Controller/Command/ga \
$(LOCAL_PATH)/src/Controller/Command/ls \
$(LOCAL_PATH)/src/Model/ModelData \
$(LOCAL_PATH)/src/Model/Proxy \
$(LOCAL_PATH)/src/Model/vo/cs \
$(LOCAL_PATH)/src/Model/vo/ga \
$(LOCAL_PATH)/src/Model/vo/ls \

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/src/Common \
$(LOCAL_PATH)/src/Controller/Command \
$(LOCAL_PATH)/src/Controller/Command/cs \
$(LOCAL_PATH)/src/Controller/Command/ga \
$(LOCAL_PATH)/src/Controller/Command/ls \
$(LOCAL_PATH)/src/Model/ModelData \
$(LOCAL_PATH)/src/Model/Proxy \
$(LOCAL_PATH)/src/Model/vo/cs \
$(LOCAL_PATH)/src/Model/vo/ga \
$(LOCAL_PATH)/src/Model/vo/ls \
$(LOCAL_PATH)/../../BaseLayer/Component/Common \
$(LOCAL_PATH)/../../BaseLayer/Services/Socket \

include $(BUILD_STATIC_LIBRARY)
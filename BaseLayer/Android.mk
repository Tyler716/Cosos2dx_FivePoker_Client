LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := baselayer_static

LOCAL_MODULE_FILENAME := libbaselayer

LOCAL_SRC_FILES := \
Component/Net/GameSocketClient.cpp \
Component/Net/SocketClient.cpp \
Component/Patterns/Command/Command.cpp \
Component/Patterns/Facade/Facade.cpp \
Component/Patterns/Mediator/Mediator.cpp \
Component/Patterns/Observer/Notification.cpp \
Component/Patterns/Observer/Notifier.cpp \
Component/Patterns/Proxy/Proxy.cpp \
Core/Controller/Controller.cpp \
Core/Model/Model.cpp \
Core/View/View.cpp \
Services/Socket/SocketProxy.cpp

LOCAL_EXPORT_C_INCLUDES := \
$(LOCAL_PATH)/Component/Common \
$(LOCAL_PATH)/Component/Interfaces \
$(LOCAL_PATH)/Component/Net \
$(LOCAL_PATH)/Component/Patterns/Command \
$(LOCAL_PATH)/Component/Patterns/Facade \
$(LOCAL_PATH)/Component/Patterns/Iterator \
$(LOCAL_PATH)/Component/Patterns/Mediator \
$(LOCAL_PATH)/Component/Patterns/Observer \
$(LOCAL_PATH)/Component/Patterns/Proxy \
$(LOCAL_PATH)/Core/Controller \
$(LOCAL_PATH)/Core/Model \
$(LOCAL_PATH)/Core/View \
$(LOCAL_PATH)/Services/Socket \
$(LOCAL_PATH)/Tools \
$(LOCAL_PATH)/Tools/Thread

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/Component/Common \
$(LOCAL_PATH)/Component/Interfaces \
$(LOCAL_PATH)/Component/Net \
$(LOCAL_PATH)/Component/Patterns/Command \
$(LOCAL_PATH)/Component/Patterns/Facade \
$(LOCAL_PATH)/Component/Patterns/Iterator \
$(LOCAL_PATH)/Component/Patterns/Mediator \
$(LOCAL_PATH)/Component/Patterns/Observer \
$(LOCAL_PATH)/Component/Patterns/Proxy \
$(LOCAL_PATH)/Core/Controller \
$(LOCAL_PATH)/Core/Model \
$(LOCAL_PATH)/Core/View \
$(LOCAL_PATH)/Services/Socket \
$(LOCAL_PATH)/Tools \
$(LOCAL_PATH)/Tools/Thread

include $(BUILD_STATIC_LIBRARY)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -ldl
#LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -licuuc

#APP_CPPFLAGS := -fexceptions
#APP_CPPFLAGS += -frtti

#LOCAL_CFLAGS := -fshort-wchar
#LOCAL_CFLAGS += -DView_Test

LOCAL_SRC_FILES := \
jniMain/main.cpp \
../../../src/Game/Game.cpp \
../../../src/Game/UI_cocos2dx/FivePoker/AppDelegate.cpp \
../../../src/Game/UI_cocos2dx/FivePoker/GameScene.cpp \
../../../src/Game/UI_cocos2dx/FivePoker/NGLayer.cpp \
../../../src/Game/UI_cocos2dx/FivePoker/DGLayer.cpp \
../../../src/Game/UI_cocos2dx/FivePoker/LoginLayer.cpp \
../../../src/Game/UI_cocos2dx/FivePoker/TouchableSprite.cpp \
../../../src/Game/UI_cocos2dx/FivePoker/CursorTextField.cpp \

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../src/Game \
$(LOCAL_PATH)/../../../src/Game/UI_cocos2dx/FivePoker \
$(LOCAL_PATH)/../../../src/Model/vo \

LOCAL_STATIC_LIBRARIES := baselayer_static
LOCAL_STATIC_LIBRARIES += ConnectMediator_static

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_lua_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,BaseLayer)
$(call import-module,GameLayer/ConnectMediator)
$(call import-module,CocosDenshion/android)
$(call import-module,cocos2dx)

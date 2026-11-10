#include "AppDelegate.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>

#include "Game.h"

#define  LOG_TAG    "JNIMsg"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace GameLayer::Game;

extern "C"
{

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JniHelper::setJavaVM(vm);

    return JNI_VERSION_1_4;
}

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
{
    if (!CCDirector::sharedDirector()->getOpenGLView())
    {
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "before new game!");

        Game game;
        game.initialize();
        game.setWindowSize(w, h);
        game.exec();

        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Game exec done!");
        //game->destroyGame();

        //delete game;
        // set the design resolution screen size, if you want to use Design Resoulution scaled to current screen, please uncomment next line.
        // view->setDesignResolutionSize(480, 320);
    }
    else
    {
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "nativeInit draw!");

        ccDrawInit();
        ccGLInvalidateStateCache();

        CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
        CCTextureCache::reloadAllTextures();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVNET_COME_TO_FOREGROUND, NULL);
        CCDirector::sharedDirector()->setGLDefaultValues();
    }
}
}   //extern "C"

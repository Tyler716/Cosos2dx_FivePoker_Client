#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#ifndef View_Test
//#include "CCLuaEngine.h"
#include "Facade.h"
#endif
#include "LoginLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID


using namespace CocosDenshion;
#ifndef View_Test
using namespace BaseLayer::Patterns;
#endif

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
    /*
#ifndef View_Test
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "~AppDelegate()");
    
     CCScene *pScene = (CCScene *)Facade::getInstance().retrieveMediator("NGMediator").getViewComponent();
    if(pScene)
    {
            __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "SAFE_RELEASE GameScene!");
        CC_SAFE_RELEASE(pScene);
    }
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#endif
    */
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(&CCEGLView::sharedOpenGLView());

    // enable High Resource Mode(2x, such as iphone4) and maintains low resource on other devices.
//     pDirector->enableRetinaDisplay(true);

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    //CCScene *pLoginScene = ProgressLayer::scene();
   // CCScene * pLoginScene = LoginLayer::scene();

    //TODO:pScene¥áµ¹Mediator¦sviewComponent
    CCScene *pScene = GameScene::create();
    //pScene->retain();
#ifndef View_Test
    Facade::getInstance().retrieveMediator("NGMediator").setViewComponent((void *)pScene);
#endif  //View_Test

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "applicationDidEnterBackground");
#endif
    //CCDirector::sharedDirector()->stopAnimation();
    CCDirector::sharedDirector()->pause();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "applicationWillEnterForeground");
#endif
    
    //CCDirector::sharedDirector()->startAnimation();
    CCDirector::sharedDirector()->resume();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
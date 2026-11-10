#include "GameScene.h"
#include "CommandList.h"
#include "LoginLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include "platform/android/jni/SystemInfoJni.h"
#include <android/log.h>
#include <dlfcn.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

//using namespace BaseLayer::Patterns;
//USING_NS_CC;

LoginLayer::LoginLayer()
{
    m_pLoginMessage = NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ucnv_convert = NULL;
    m_pDL = dlopen("/system/lib/libicuuc.so", RTLD_LAZY);
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
}

LoginLayer::~LoginLayer()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(m_pDL)
    {
        dlclose(m_pDL);
        m_pDL = NULL;
    }
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
}

// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
bool LoginLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //load uconv
    std::string strVer[5];
    strVer[0] = std::string("_3_8");
    strVer[1] = std::string("_4_2");
    strVer[2] = std::string("_44");
    strVer[3] = std::string("_46");
    strVer[4] = std::string("_48");
    std::string strConvert = std::string("ucnv_convert");
    std::string strFull;

    for(int i = 0; i < 6; i++)
    {
        if(i == 0)
        {
            strFull = strConvert;
        }
        else
        {
            strFull = strConvert + strVer[i-1];
        }

        if(!ucnv_convert)
        {
            __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "try %s", strFull.data());
            ucnv_convert = (int (*)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*))dlsym(m_pDL, strFull.data());
        }
        else
        {
            break;
        }
    }
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    CCLayer::setKeypadEnabled(true);

    CCSize size = CCDirector::sharedDirector()->getWinSize();

    m_pBackgroundLogin = CCSprite::create("PIC_LOGIN_BG.jpg");
    m_pBackgroundLogin->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(m_pBackgroundLogin, 0);

    CCMenuItemImage *pLoginItem = CCMenuItemImage::create(
                                    "BTN_LOGIN_1.png",
                                    "BTN_LOGIN_3.png",
                                    this,
                                    menu_selector(LoginLayer::onBtnLogin));
    pLoginItem->setAnchorPoint(ccp(0, 0));
    pLoginItem->setPosition(ccp(230, 217));

    CCMenuItemImage *pCancelItem = CCMenuItemImage::create(
                                    "BTN_CANCEL_1.png",
                                    "BTN_CANCEL_3.png",
                                    this,
                                    menu_selector(LoginLayer::onBtnCancel));
    pCancelItem->setAnchorPoint(ccp(0, 0));
    pCancelItem->setPosition(ccp(418, 217));

    CCMenuItemImage *pForgotPWItem = CCMenuItemImage::create(
                                    "BTN_FORGOT_PW_1.png",
                                    "BTN_FORGOT_PW_3.png",
                                    this,
                                    menu_selector(LoginLayer::onBtnForgotPW));
    pForgotPWItem->setAnchorPoint(ccp(0, 0));
    pForgotPWItem->setPosition(ccp(510, 275));

    m_pLoginMenu = CCMenu::create(pCancelItem, NULL);
    m_pLoginMenu->setPosition(CCPointZero);
    this->addChild(m_pLoginMenu, 1);

    m_pCancelMenu = CCMenu::create(pForgotPWItem, NULL);
    m_pCancelMenu->setPosition(CCPointZero);
    this->addChild(m_pCancelMenu, 1);

    m_pForgetPWMenu = CCMenu::create(pLoginItem, NULL);
    m_pForgetPWMenu->setPosition(CCPointZero);
    this->addChild(m_pForgetPWMenu, 1);

    m_pAccountText = CursorTextField::textFieldWithPlaceHolder("input account", "Arial", 20);
    m_pAccountText->setPosition(ccp(339, 353));
    m_pAccountText->setAnchorPoint(ccp(0, 0));
    m_pAccountText->setDesignedSize(CCSizeMake(225, 20));
    m_pAccountText->setColor(ccc3(0, 0, 0));
    m_pAccountText->setLimitNum(MAX_ACCOUNT);
    this->addChild(m_pAccountText, 1);
    m_pPasswordText = CursorTextField::textFieldWithPlaceHolder("input password", "Arial", 20);
    m_pPasswordText->setPosition(ccp(339, 305));
    m_pPasswordText->setAnchorPoint(ccp(0, 0));
    m_pPasswordText->setDesignedSize(CCSizeMake(225, 20));
    m_pPasswordText->setColor(ccc3(0, 0, 0));
    m_pPasswordText->setIsPsw(true);
    m_pPasswordText->setLimitNum(MAX_PASSWORD);
    this->addChild(m_pPasswordText, 1);

    m_pLoginMessage = CCLabelTTF::create("", "Arial", 24);
    m_pLoginMessage->setColor(ccc3(255, 0, 0));
    m_pLoginMessage->setPosition(ccp(400, 190));
    m_pLoginMessage->setVisible(false);
    this->addChild(m_pLoginMessage, 1);

    m_pVersion = CCLabelTTF::create("v0.0.0.1", "Arial", 20);
    m_pVersion->setColor(ccc3(255, 255, 255));
    m_pVersion->setPosition(ccp(740, 20));
    this->addChild(m_pVersion, 1);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;

    if (JniHelper::getStaticMethodInfo(t,
                    "com/istrong/FivePoker/FivePoker",
                    "getGameVersion",
                    "()Ljava/lang/String;"))
    {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        CCString *ret = new CCString(JniHelper::jstring2string(str).c_str());
        ret->autorelease();
        t.env->DeleteLocalRef(str);

        m_pVersion->setString(ret->m_sString.c_str());
    }
#endif
    return true;
}

void LoginLayer::onEnter()
{
    CCLayer::onEnter();

    m_pFacade = &Facade::getInstance();
}

void LoginLayer::keyBackClicked()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "LoginLayer::keyBackClicked");
#endif
    CCDirector::sharedDirector()->end();
}
/*
CCScene *LoginLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LoginLayer *layer = LoginLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
*/
void LoginLayer::onBtnLogin(CCObject *pSender)
{
    std::string strAccu(m_pAccountText->getString());
    std::string strPW(m_pPasswordText->getString());

    int iAccuLen = strAccu.length();
    int iPWLen = strPW.length();

    if(iAccuLen > 0 && iPWLen > 0)
    {
        m_pLoginMessage->setVisible(false);
#if 1
#ifndef View_Test
        CS_LB_USER_RELOGIN_GS_R_Vo vo;

	    //TODO android修改寬字元
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        wcscpy(vo.IP, L"210.61.12.16");
#else
        wcscpy(vo.IP, L"210.61.12.16");
#endif	//CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        vo.PORT = 8802;
        vo.GSGrpNo = 0;
        vo.GSGrpUniID = 1;
        vo.GameMode = GAME_GRP_MODE_AUTO_LV1;
        vo.GameType = GAME_TYPE_FIVE_PK;
        vo.GameWindowID = 0;
        vo.PLUniID = 238;
        vo.SeatNo = 1;

        /******只連GameServer測試****/
        m_pFacade->sendNotification(CentralServerEvent::CS_LB_USER_RELOGIN_GS_Event::Name, &vo);
#else
        //updateLoginFailCallBack();
#endif  //View_Test
#else
        m_pFacade->sendNotification(LoginFormSendSignal);
#endif
    }
    else    //TODO: login輸入不合法
    {
        m_pLoginMessage->setVisible(true);
        if(iAccuLen == 0 && iPWLen == 0)
        {
            m_pLoginMessage->setString("Please check your account and password...");
        }
        else if(iAccuLen == 0)
        {
            m_pLoginMessage->setString("Please check your account...");
        }
        else
        {
            m_pLoginMessage->setString("Please check your password...");
        }
    }
}

CCScene* LoginLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LoginLayer *layer = LoginLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void LoginLayer::onBtnCancel(CCObject *pSender)
{
    CCDirector::sharedDirector()->end();
}

void LoginLayer::onBtnForgotPW(CCObject *pSender)
{
}

void LoginLayer::updateLoginFailCallBack(int iState)
{
#ifndef View_Test
    m_pLoginMessage->setVisible(true);
    char tmpString[100];
    sprintf(tmpString, "Login Fail! Please check it...ErrorCode:%i", iState);
    m_pLoginMessage->setString(tmpString);
#else
    ((CCLayerMultiplex*)m_pParent)->switchTo(GameView::VIEW_NG);
#endif
}

void LoginLayer::connectResultCallBack(bool bResult)
{
    if(bResult) //連線OK
    {
        if(m_pFacade->hasProxy("LoginServerProxy"))
        {
            std::map<std::string, IData *> *datamap = (std::map<std::string, IData *> *)m_pFacade->retrieveProxy("LoginServerProxy").getData();
            ConnectionModelData *modeldata = (ConnectionModelData *)(datamap->find("ConnectionModelData")->second);

            std::cout<<"connect state:"<<modeldata->connectState<<std::endl;

            if(modeldata->connectState == ConnectionModelData::CONNECT_SUCCESS)
            {
                std::string strAccu(m_pAccountText->getString());
                std::string strPW(m_pPasswordText->getString());

                wchar_t tmpAccu[MAX_ACCOUNT];
                wchar_t tmpPwd[MAX_PASSWORD];


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "LoginLayer connectSuccessCallBack");
            
                if(ucnv_convert)
                {
                    //wcstombs(ip, vo->IP, wcslen(vo->IP));
                    int err_code = 0;
                    int ilen = ucnv_convert(sizeof(wchar_t) == 4 ? "UTF-32LE" : "UTF-16LE", "utf-8",
                                            (char *)tmpAccu, MAX_ACCOUNT, 
                                            (const char *)strAccu.data(), strAccu.length(), 
                                            &err_code);
                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "wcslen(tmpAccu):%i, strAccu:%s tmpAccu:%s, len:%i, error:%i", wcslen(tmpAccu), strAccu.data(), tmpAccu, ilen, err_code);

                    ilen = ucnv_convert(sizeof(wchar_t) == 4 ? "UTF-32LE" : "UTF-16LE", "utf-8",
                                            (char *)tmpPwd, MAX_PASSWORD, 
                                            (const char *)strPW.data(), strPW.length(), 
                                            &err_code);
                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "wcslen(tmpPwd):%i, strPW:%s tmpPwd:%s, len:%i, error:%i", wcslen(tmpPwd), strPW.data(), tmpPwd, ilen, err_code);
                }
                else
                {
                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "ucnv_convert fail!");
                }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                mbstowcs(tmpAccu, strAccu.data(), MAX_ACCOUNT);
                mbstowcs(tmpPwd, strPW.data(), MAX_PASSWORD);
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

                //送出login封包
                LoginRequestVo loginrequestVo(tmpAccu, tmpPwd);
                m_pFacade->sendNotification(LoginServerConnect, &loginrequestVo);
            }
            else
            {
                connectResultCallBack(false);
            }
        }
        else
        {
            //error
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "LoginLayer can't get loginproxy!");
#endif
        }
    }
    else
    {
        m_pLoginMessage->setVisible(true);
        m_pLoginMessage->setString("Connect Fail! Please check it...");
    }
}
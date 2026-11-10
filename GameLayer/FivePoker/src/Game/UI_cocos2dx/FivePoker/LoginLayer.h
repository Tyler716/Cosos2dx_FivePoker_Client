#ifndef _LOGINLAYER_H_
#define _LOGINLAYER_H_

#include "cocos2d.h"
#include "Facade.h"
#include "../../../Model/ModelData/FivePokerModelData.h"
#include "CursorTextField.h"

class LoginLayer : public CCLayer
{
public:
    LoginLayer();

    virtual ~LoginLayer();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    virtual void onEnter();

    virtual void keyBackClicked();

    LAYER_CREATE_FUNC(LoginLayer);

    //static CCScene* scene();

    void updateLoginFailCallBack(int iState);
    void connectResultCallBack(bool bResult);

    static CCScene* scene();

private:
    void onBtnLogin(CCObject *pSender);
    void onBtnCancel(CCObject *pSender);
    void onBtnForgotPW(CCObject *pSender);

private:
    //­I´º
    CCSprite *m_pBackgroundLogin;
    CCMenu *m_pLoginMenu, *m_pCancelMenu, *m_pForgetPWMenu;
    CCLabelTTF *m_pLoginMessage, *m_pVersion;

    IFacade *m_pFacade;

    CursorTextField *m_pAccountText, *m_pPasswordText;

    void *m_pDL;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    int (*ucnv_convert)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*);
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
};
#endif //_LOGINLAYER_H_
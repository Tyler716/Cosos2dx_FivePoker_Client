#ifndef _NGLAYE_H_
#define _NGLAYE_H_

#include "cocos2d.h"
#include "AnimationAction.hpp"
#include "TouchableSprite.h"
#include "../../../Model/ModelData/FivePokerModelData.h"
#include "Facade.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#endif

using namespace BaseLayer::Patterns;
USING_NS_CC;

class NGLayer : public CCLayer
{
public:
    NGLayer();

    virtual ~NGLayer();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    virtual void onEnter();

    virtual void keyBackClicked();

    // implement the "static node()" method manually
    LAYER_CREATE_FUNC(NGLayer);

    //
    void updateUIdata();

    void updateBetFailCallBack(GCliMsgBetFail_Vo *BetVo);
    void updateDealCardCallBack(GCliMsgDeal_Vo *dealVo);
    void updateDrawCardCallBack(GCliMsgDraw_Vo *drawVo);
    void updatePayTableCallBack(int GameScore[5][11]);
    void updateMultipleCallBack(int iMultiple);

protected:
    cocos2d::CCArray *_targets;
    cocos2d::CCArray *_projectiles;
    int _projectilesDestroyed;

    FivePokerModelData *m_ModelData;
    IProxy *gameProxy;

private:
    void InitButton();
    void InitLabel();
    void InitPayTable();
    void InitAnimation();
    void onBtnLeave(CCObject *pSender);
    void onBtnHelp(CCObject *pSender);
    void onBtnBet(CCObject *pSender);
    void onBtnMaxBet(CCObject *pSender);
    void onBtnHold(CCObject *pSender);
    void onBtnMultipleDown(CCObject *pSender);
    void onBtnMultipleUp(CCObject *pSender);
    void onBtnDeal(CCObject *pSender);

private:
    void updateBet(int iBet);
    void updateCredit(int iCredit, int iMutiple);
    void updateWinPoint();
    bool CheckBet(int iBet);

    //翻牌動作
    void onShowNGCard(CCNode *sender,void *vo);
    void onResetCardComplete(CCNode *sender,void *vo);

    void scheduleOnceTimer(float fTime);
    void changeView(int iView);

    void ButtonState(int iState);
    int getCardIndex(int iPokerHand);

private:
    IFacade *m_pFacade;
    //背景
    CCSprite *m_pBackgroundNG;
    CCSprite *m_pNo, *m_pBetRatio;

    //畫面上的數字
    CCLabelAtlas *m_pWinPointLabel, *m_pBetLabel, *m_pCreditLabel, *m_pMultipleLabel;
    CCLabelAtlas *m_pPayTableLabel[5][10];
    CCLabelAtlas *m_pTableNumLabel, *m_pRatioNumLabel;

    //目前押注,倍數, 金額, 倍率, 桌號
    int m_iBet, m_iMultiple, m_iCredit, m_iBetRatioL, m_iBetRatioR, m_iTableNum, m_iWinPoint;

    int m_iPayTableNum[5][10];

    //動畫: 押注或牌, 贏牌的閃爍, 賠率表的閃爍
    AnimationAction *m_pHintAnimation, *m_pWinPayAnimation, *m_pBetPayTableAnimation;
    AnimationAction *m_pWinCardAnimation[5];

    //牌面的物件
    CCSprite *m_pSpriteBackCard[5];
    CCSprite *m_pSpriteCard[5], *m_pSpriteHeld[5];

    //按鈕
    CCMenu  *m_pDealMenu, *m_pBetMenu, *m_pMaxBetMenu, *m_pMultipleUp, *m_pMultipleDown;
    CCMenu  *m_pHoldMenu[5];

    CCTexture2D *m_CardTexture[54];

    int m_iHold[5], m_iHoldCardNum, m_iSelectIndex;
    bool m_bIsDealWin;
};

#endif //_NGLAYER_H_

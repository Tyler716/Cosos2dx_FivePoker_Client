#ifndef _DGLAYE_H_
#define _DGLAYE_H_

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

class DGLayer : public cocos2d::CCLayer
{
public:
    DGLayer();

    virtual ~DGLayer();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    virtual void onEnter();

    virtual void keyBackClicked();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer

    //
   // void addTarget();

    //
    //void spriteMoveFinished(CCNode* sender);

    //
    void gameLogic(float dt);

    //touch callback function
    //void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

    // implement the "static node()" method manually
    LAYER_CREATE_FUNC(DGLayer);

    //
    void updateUIdata();

    void updateDGCardCallBack(GCliMsgDoubleGameData_Vo *DGVo);
    void updateDGResultCardCallBack(GCliMsgDoubleGameResult_Vo *DGResultVo);
    void updateCollectCallBack();

    void onTouchesCardBegan(int iTag);

protected:
    cocos2d::CCArray *_targets;
    cocos2d::CCArray *_projectiles;
    int _projectilesDestroyed;

    FivePokerModelData *m_ModelData;
    IProxy *gameProxy;

private:
    void InitButton();
    void InitLabel();
    void InitAnimation();
    // a selector callback
    void onBtnLeave(CCObject *pSender);
    void onBtnKeep(CCObject *pSender);
    void onBtnHelp(CCObject *pSender);
    void onBtnDG(CCObject *pSender);
    void onBtnCollect(CCObject *pSender);

private:
    void updateBet(int iBet);
    void updateCredit(int iCredit, int iMutiple);
    void updateWinPoint();
    bool CheckBet(int iBet);

    void RunNumber(float dt);

    //翻牌動作
    void onShowDGCard(CCNode *sender,void *vo);
    void onResetCardComplete(CCNode *sender,void *vo);

    void scheduleOnceTimer(float fTime);
    void changeView(int iView);

    void ButtonState(int iState);
    int getCardIndex(int iPokerHand);

private:
    IFacade *m_pFacade;
    //背景
    CCSprite *m_pBackgroundDG;
    CCSprite *m_pNo, *m_pBetRatio;

    //畫面上的數字
    CCLabelAtlas *m_pWinPointLabel, *m_pBetLabel, *m_pCreditLabel, *m_pMultipleLabel;
    CCLabelAtlas *m_pTableNumLabel, *m_pRatioNumLabel;
    CCLabelAtlas *m_pDGBetLabel, *m_pDGWillWinLabel;

    //目前押注,倍數, 金額, 倍率, 桌號
    int m_iBet, m_iMultiple, m_iCredit, m_iBetRatioL, m_iBetRatioR, m_iTableNum, m_iWinPoint;
    int m_iWinReduceNum;  //跑表單位
    bool m_bWaitToChangeView;

    //動畫: 押注或牌, 贏牌的閃爍, 賠率表的閃爍
    AnimationAction *m_pDGHintAnimation;
    AnimationAction *m_pDGBtnAnimation, *m_pDGWinnerAnimation;

    //賠率表的物件
    //CCSprite *m_pPayTableSprit;

    //動畫使用的事件
    //CCAnimate *m_pAction;

    //牌面的物件
    TouchableSprite *m_pSpriteBackCard[5];
    CCSprite *m_pSpriteCard[5];

    //DG的牌物件
    CCSprite *m_pSpriteDealer, *m_pSpritePlayer[4], *m_pSpriteCardEffect[4];

    //按鈕
    CCMenu  *m_pHoldMenu[5];
    CCMenu  *m_pDGMenu, *m_pCollectMenu;

    CCTexture2D *m_CardTexture[54];

    int m_iHold[5], m_iSelectIndex;
};

#endif //_DGLAYER_H_

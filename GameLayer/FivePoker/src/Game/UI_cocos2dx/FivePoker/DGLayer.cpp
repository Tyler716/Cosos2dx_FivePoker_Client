#include "DGLayer.h"
#include "extensions\CCControlExtension\CCControlButton.h"
#include "..\..\..\Model\Vo\GSLMsg_Vo.h"
#include "CommandList.h"
#include "GlobalVar.h"
#include <cmath>
#include <time.h>  //for time()
#include <stdlib.h>  //for rand()
#include "AppDelegate.h"
#include "GameScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#define runNumSpeed 0 //0~1

//USING_NS_CC;
#define rand_number(low,high) rand()%((high)-(low)+1)+(low);

#define reverseCardFrame    0.25

DGLayer::DGLayer()
    : m_pWinPointLabel(NULL)
    , m_pBetLabel(NULL)
    , m_pCreditLabel(NULL)
    , m_pMultipleLabel(NULL)
    , m_ModelData(NULL)
    , gameProxy(NULL)
    , m_pDGHintAnimation(NULL)
    , m_iMultiple(1)
    , m_iBet(1)
    , m_iCredit(99999900)
    , m_iWinPoint(0)
    , m_iBetRatioL(1)
    , m_iBetRatioR(1)
    , m_iTableNum(1)
    , m_iSelectIndex(0)
    , m_iWinReduceNum(0)
    , m_bWaitToChangeView(false)
{

}

DGLayer::~DGLayer()
{
    SAFE_DELETE(m_pDGHintAnimation);
    SAFE_DELETE(m_pDGBtnAnimation);
    SAFE_DELETE(m_pDGWinnerAnimation);

#ifdef View_Test
    SAFE_DELETE(m_ModelData);
#endif
}

// on "init" you need to initialize your instance
bool DGLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCLayer::setKeypadEnabled(true);

#ifndef View_Test
    m_pFacade = &Facade::getInstance();
    if(m_pFacade)
    {
        if(m_pFacade->hasProxy("GameProxy"))
        {
            gameProxy = &m_pFacade->retrieveProxy("GameProxy");

            std::map<std::string, IData *> *Game_datamap = (std::map<std::string, IData *> *)gameProxy->getData();

            m_ModelData = (FivePokerModelData *)Game_datamap->find("FivePokerModelData")->second;

            if(!m_ModelData)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
#else
    if(!m_ModelData)
    {
        m_ModelData = new FivePokerModelData();
        m_ModelData->m_BetIndex = 1;
        m_ModelData->m_BetRatio = 1;
        m_ModelData->m_PreState = GameState::STATE_DGSELECT;
        m_ModelData->m_View = GameView::VIEW_DG;
        m_ModelData->m_MaxBet = 5;
        m_ModelData->m_MinBet = 1;
        m_ModelData->m_PointRatioL = 1;
        m_ModelData->m_PointRatioR = 1;
        m_ModelData->m_TableNo = 1;
        m_ModelData->m_Multiple = 1;
        m_ModelData->m_uTotalPoints = 5000000;
        m_ModelData->m_GameRatio[0] = 1;
        m_ModelData->m_GameRatio[1] = 10;
        m_ModelData->m_GameRatio[2] = 100;
        m_ModelData->m_GameRatio[3] = 1000;
    }
#endif  //View_Test

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    InitButton();
    InitLabel();

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    //CCLabelTTF* pLabel = CCLabelTTF::create("FivePoker", "Arial", 24);
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    //pLabel->setPosition( ccp(size.width / 2, size.height - 50) );

    // add the label as a child to this layer
    //this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    m_pBackgroundDG = CCSprite::create("PIC_DG_BackGround.png");
    //m_pBackgroundDG->setVisible(false);

    // position the sprite on the center of the screen
    m_pBackgroundDG->setPosition(ccp(size.width/2, size.height/2));

    // add the sprite as a child to this layer
    this->addChild(m_pBackgroundDG, 0);

    m_pBetRatio = CCSprite::create("PIC_RATIO_BG.png");
    m_pBetRatio->setAnchorPoint(ccp(0, 0));
    m_pBetRatio->setPosition(ccp(103, 571));
    this->addChild(m_pBetRatio, 1);

    m_pNo = CCSprite::create("PIC_NO.png");
    m_pNo->setAnchorPoint(ccp(0, 0));
    m_pNo->setPosition(ccp(8, 577));
    this->addChild(m_pNo, 1);

    InitAnimation();

    //牌面內容
    std::string szcardFileName;
    int num = 0;
    int suit = 0;
    for(int i = 0; i < 52; i++)
    {
        szcardFileName = std::string("PIC_POKER_");
        num = i%13;
        suit = i/13;

        switch(num)
        {
        case 0:
            {
                szcardFileName.append("Ace of ");
                break;
            }

        case 10:
            {
                szcardFileName.append("J of ");
                break;
            }

        case 11:
            {
                szcardFileName.append("Q of ");
                break;
            }
            
        case 12:
            {
                szcardFileName.append("K of ");
                break;
            }

        default:
            {
                char tmpNum[5];
                sprintf(tmpNum, "%i", num+1);
                szcardFileName.append(tmpNum);
                szcardFileName.append(" of ");
                break;
            }
        }

        switch(suit)
        {
        case 0: //spade
            {
                szcardFileName.append("spades.png");
                break;
            }

        case 1: //heart
            {
                szcardFileName.append("heart.png");
                break;
            }

        case 2: //diamond
            {
                szcardFileName.append("diamond.png");
                break;
            }

        case 3: //club
            {
                szcardFileName.append("club.png");
                break;
            }
        }
        m_CardTexture[i] = CCTextureCache::sharedTextureCache()->addImage(szcardFileName.data());
    }
    m_CardTexture[52] = CCTextureCache::sharedTextureCache()->addImage("PIC_POKER_JOKER-A.png");
    m_CardTexture[53] = CCTextureCache::sharedTextureCache()->addImage("PIC_POKER_JOKER-B.png");

    for(int i = 0; i < 5; i++)
    {
        //牌背
        m_pSpriteBackCard[i] = TouchableSprite::touchSpriteWithFile("PIC_POKER_BG.png", this, callfuncI_selector(DGLayer::onTouchesCardBegan));
        m_pSpriteBackCard[i]->setTag(i);
        m_pSpriteBackCard[i]->setPosition(ccp(100+i*150, 240));
        this->addChild(m_pSpriteBackCard[i], 1);

        //牌面
        m_pSpriteCard[i] = CCSprite::create(m_CardTexture[0]);
        m_pSpriteCard[i]->setPosition(ccp(100+i*150, 240));
        this->addChild(m_pSpriteCard[i], 1);
        m_pSpriteCard[i]->setVisible(false);
    }

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    //registerWithTouchDispatcher();

    m_pSpriteDealer = CCSprite::create("PIC_DG_DEALER.png");
    m_pSpriteDealer->setAnchorPoint(ccp(0, 0));
    m_pSpriteDealer->setPosition(ccp(58, 130));
    m_pSpriteDealer->setVisible(false);
    this->addChild(m_pSpriteDealer, 1);

    for(int i = 0; i < 4; i++)
    {
        //DG的player
        m_pSpritePlayer[i] = CCSprite::create("PIC_DG_PLAYER.png");
        m_pSpritePlayer[i]->setAnchorPoint(ccp(0, 0));
        m_pSpritePlayer[i]->setPosition(ccp(58+(i+1)*150, 130));
        this->addChild(m_pSpritePlayer[i], 1);
        m_pSpritePlayer[i]->setVisible(false);

        m_pSpriteCardEffect[i] = CCSprite::create("PIC_DG_POKER_SlideIn.png");
        m_pSpriteCardEffect[i]->setPosition(ccp(100+(i+1)*150, 240));
        this->addChild(m_pSpriteCardEffect[i], 2);
        m_pSpriteCardEffect[i]->setVisible(false);
    }

    return true;
}

void DGLayer::onEnter()
{
#ifndef View_Test
    updateUIdata();
#endif  //View_Test
    CCLayer::onEnter();
}

void DGLayer::keyBackClicked()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "DGLayer::keyBackClicked");
#endif
    CCDirector::sharedDirector()->end();
}

void DGLayer::onBtnHelp(CCObject *pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#endif
}
/*
void DGLayer::onBtnKeep(CCObject *pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#endif
}
*/

void DGLayer::onBtnLeave(CCObject *pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void DGLayer::InitButton()
{
    /**
     * system button
     */
    //CCControlButton *pHelplButton = CCControlButton::create(CCScale9Sprite::create("BTN_HELP_1.png"));
    //pHelplButton->setPosition();
     
    CCMenuItemImage *pHelpItem = CCMenuItemImage::create(
                                        "BTN_HELP_1.png",
                                        "BTN_HELP_3.png",
                                        this,
                                        menu_selector(DGLayer::onBtnHelp));
    pHelpItem->setAnchorPoint(ccp(0, 0));
    pHelpItem->setPosition(ccp(676, 550));
    /*
    CCMenuItemImage *pKeepItem = CCMenuItemImage::create(
                                        "BTN_KEEP_1.png",
                                        "BTN_KEEP_3.png",
                                        this,
                                        menu_selector(DGLayer::onBtnKeep));
    pKeepItem->setAnchorPoint(ccp(0, 0));
    pKeepItem->setPosition(ccp(705, 549));
  */
    
    CCMenuItemImage *pLeaveItem = CCMenuItemImage::create(
                                        "BTN_LEAVE_1.png",
                                        "BTN_LEAVE_3.png",
                                        this,
                                        menu_selector(DGLayer::onBtnLeave));
    pLeaveItem->setAnchorPoint(ccp(0, 0));
    pLeaveItem->setPosition(ccp(730, 550));
  
    /**
     * Game button
     */
    CCMenuItemImage *pDGItem = CCMenuItemImage::create(
                                        "BTN_DOUBLEUP_2.png",
                                        "BTN_DOUBLEUP_2.png",
                                        this,
                                        menu_selector(DGLayer::onBtnDG));
    pDGItem->setAnchorPoint(ccp(0, 0));
    pDGItem->setPosition(ccp(257, 20));

    CCMenuItemImage *pCollectItem = CCMenuItemImage::create(
                                        "BTN_COLLECT_2.png",
                                        "BTN_COLLECT_2.png",
                                        this,
                                        menu_selector(DGLayer::onBtnCollect));
    pCollectItem->setAnchorPoint(ccp(0, 0));
    pCollectItem->setPosition(ccp(427, 20));

    // create menu, it's an autorelease object
    CCMenu* pLeaveMenu = CCMenu::create(pLeaveItem, NULL);
    pLeaveMenu->setPosition(CCPointZero);
    this->addChild(pLeaveMenu, 1);
    /*
    CCMenu* pKeepMenu = CCMenu::create(pKeepItem, NULL);
    pKeepMenu->setPosition(CCPointZero);
    this->addChild(pKeepMenu, 1);
    */

    CCMenu* pHelpMenu = CCMenu::create(pHelpItem, NULL);
    pHelpMenu->setPosition(CCPointZero);
    this->addChild(pHelpMenu, 1);

    m_pDGMenu  = CCMenu::create(pDGItem, NULL);
    m_pDGMenu->setPosition(CCPointZero);
    m_pDGMenu->setVisible(false);
    this->addChild(m_pDGMenu, 1);
    m_pCollectMenu = CCMenu::create(pCollectItem, NULL);
    m_pCollectMenu->setPosition(CCPointZero);
    m_pCollectMenu->setVisible(false);
    this->addChild(m_pCollectMenu, 1);
}

void DGLayer::InitLabel()
{
    /**
     * 顯示數字
     */
    m_pWinPointLabel = CCLabelAtlas::create("0.00", "NUM_SCORE_ALL.png", 12, 12, '.');
    m_pWinPointLabel->setAnchorPoint(ccp(0.5, 0.5)); //造成置中的效果
    m_pWinPointLabel->setPosition(155, 79);
    m_pWinPointLabel->setScaleX((float)0.7);
    this->addChild(m_pWinPointLabel, 1);

    char szData[20];
#ifndef View_Test
    m_iBet = (m_ModelData->m_GameRatio[m_ModelData->m_Multiple-1])*(m_ModelData->m_BetRatio)*(m_ModelData->m_BetIndex);
#endif  //View_Test
    sprintf(szData, "%i.00", m_iBet);
    m_pBetLabel = CCLabelAtlas::create(szData, "NUM_SCORE_ALL.png", 12, 12, '.');
    m_pBetLabel->setAnchorPoint(ccp(0.5, 0.5));
    m_pBetLabel->setPosition(155, 48);
    m_pBetLabel->setScaleX((float)0.7);
    this->addChild(m_pBetLabel, 1);

    memset(szData, 0, sizeof(szData));
#ifndef View_Test
    m_iCredit = m_ModelData->m_uTotalPoints/100;
#endif  //View_Test
    sprintf(szData, "%i.00", m_iCredit);
    m_pCreditLabel = CCLabelAtlas::create(szData, "NUM_SCORE_ALL.png", 12, 12, '.');
    m_pCreditLabel->setAnchorPoint(ccp(0.5, 0.5));
    m_pCreditLabel->setPosition(155, 19);
    m_pCreditLabel->setScaleX((float)0.7);
    this->addChild(m_pCreditLabel, 1);

    char szMultiple[10];
#ifndef View_Test
    sprintf(szMultiple, "2%i", (m_ModelData->m_GameRatio[m_ModelData->m_Multiple-1])*(m_ModelData->m_BetRatio));
#else
    int iMultiple = pow((float)10, m_iMultiple-1);
    sprintf(szMultiple, "2%i", iMultiple);
#endif  //View_Test
    m_pMultipleLabel = CCLabelAtlas::create(szMultiple, "NUM_COINVALUE_ALL.png", 21, 16, '0');
    m_pMultipleLabel->setAnchorPoint(ccp(0.5, 0.5));
    m_pMultipleLabel->setPosition(700, 32);
    m_pMultipleLabel->setScaleX((float)1);
    this->addChild(m_pMultipleLabel, 1);

    char szTableNum[10];
#ifndef View_Test
    m_iTableNum = m_ModelData->m_TableNo;
#endif  //View_Test
    sprintf(szTableNum, "%i", m_iTableNum);
    m_pTableNumLabel = CCLabelAtlas::create(szTableNum, "NUM_NO_ALL.png", 16, 26, '0');
    m_pTableNumLabel->setAnchorPoint(ccp(0, 0));
    m_pTableNumLabel->setPosition(47, 573);
    this->addChild(m_pTableNumLabel, 1);

    char szRatioNum[10];
#ifndef View_Test
    m_iBetRatioL = m_ModelData->m_PointRatioL;
    m_iBetRatioR = m_ModelData->m_PointRatioR;
#endif  //View_Test
    sprintf(szRatioNum, "%i/%i", m_iBetRatioL, m_iBetRatioR);
    m_pRatioNumLabel = CCLabelAtlas::create(szRatioNum, "NUM_RATIO_ALL.png", 13, 19, '/');
    m_pRatioNumLabel->setAnchorPoint(ccp(0.5, 0.5));
    m_pRatioNumLabel->setPosition(134, 587);

    int iRatioLen = strlen(szRatioNum);
    if(iRatioLen == 4)
    {
        m_pRatioNumLabel->setScaleX(0.8);
    }
    else if(iRatioLen == 5)
    {
        m_pRatioNumLabel->setScaleX(0.6);
    }
    this->addChild(m_pRatioNumLabel, 2);

    m_pDGBetLabel = CCLabelAtlas::create("0.00", "NUM_DG_MONEY_ALL.png", 19, 26, '.');
    m_pDGBetLabel->setAnchorPoint(ccp(0.5, 0.5)); //造成置中的效果
    m_pDGBetLabel->setPosition(415, 497);
    m_pDGBetLabel->setScaleX((float)0.7);
    this->addChild(m_pDGBetLabel, 1);

    m_pDGWillWinLabel = CCLabelAtlas::create("0.00", "NUM_DG_MONEY_ALL.png", 19, 26, '.');
    m_pDGWillWinLabel->setAnchorPoint(ccp(0.5, 0.5)); //造成置中的效果
    m_pDGWillWinLabel->setPosition(415, 469);
    m_pDGWillWinLabel->setScaleX((float)0.7);
    this->addChild(m_pDGWillWinLabel, 1);
}

void DGLayer::InitAnimation()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    PicInfo info;

    //DG提示文字動畫
    info.iHeight = 50;
    info.iWidth = 481;
    info.Hor_Num = 5;
    info.Ver_Num = 11;
    info.Total_Num = 55;
    info.x = 60;
    info.y = 390;
    m_pDGHintAnimation = new AnimationAction("ANI_DG_Cue_ALL(repeat).png", info);
    m_pDGHintAnimation->getAnimation()->setAnchorPoint(ccp(0, 0));
    this->addChild(m_pDGHintAnimation->getAnimation(), 1);
    m_pDGHintAnimation->getAnimation()->setVisible(false);
    //m_pDGHintAnimation->runAction();
    

    //DG按鈕動畫
    info.iHeight = 61;
    info.iWidth = 121;
    info.Hor_Num = 7;
    info.Ver_Num = 5;
    info.Total_Num = 31;
    info.x = 257;
    info.y = 20;
    m_pDGBtnAnimation  = new AnimationAction("ANI_DG_ButtonShine_ALL(repeat).png", info, 0.04);
    this->addChild(m_pDGBtnAnimation->getAnimation(), 1);
    m_pDGBtnAnimation->getAnimation()->setAnchorPoint(ccp(0, 0));
    m_pDGBtnAnimation->getAnimation()->setVisible(false);

    //DG玩家贏動畫
    info.iHeight = 50;
    info.iWidth = 286;
    info.Hor_Num = 7;
    info.Ver_Num = 10;
    info.Total_Num = 66;
    info.x = size.width/2-info.iWidth/2;
    info.y = 230;
    m_pDGWinnerAnimation = new AnimationAction("ANI_WINNER_ALL.png", info, 0.08, 1);
    this->addChild(m_pDGWinnerAnimation->getAnimation(), 2);
    m_pDGWinnerAnimation->getAnimation()->setAnchorPoint(ccp(0, 0));
    m_pDGWinnerAnimation->getAnimation()->setVisible(false);
}

void DGLayer::updateUIdata()
{
    char szData[20];
    m_iBet = (m_ModelData->m_GameRatio[m_ModelData->m_Multiple-1])*(m_ModelData->m_BetRatio)*(m_ModelData->m_BetIndex);
    sprintf(szData, "%i.00", m_iBet);
    m_pBetLabel->setString(szData);

    memset(szData, 0, sizeof(szData));
    m_iCredit = m_ModelData->m_CurrentCredit/100;
    sprintf(szData, "%i.00", m_iCredit);
    m_pCreditLabel->setString(szData);

    m_iWinPoint = m_ModelData->m_iWinPoint/100;

    char szMultiple[10];
    sprintf(szMultiple, "2%i", (m_ModelData->m_GameRatio[m_ModelData->m_Multiple-1])*(m_ModelData->m_BetRatio));

#ifndef View_Test
    if(m_ModelData->m_Multiple == 4)
    {
        m_pMultipleLabel->setScaleX((float)0.8);
    }
    else
    {
        m_pMultipleLabel->setScaleX((float)1);
    }
#else
    if(m_iMultiple == 4)
    {
        m_pMultipleLabel->setScaleX((float)0.8);
    }
    else
    {
        m_pMultipleLabel->setScaleX((float)1);
    }
#endif
    m_pMultipleLabel->setString(szMultiple);

    char szTableNum[10];
    m_iTableNum = m_ModelData->m_TableNo;
    sprintf(szTableNum, "%i", m_iTableNum);
    m_pTableNumLabel->setString(szTableNum);

    char szRatioNum[10];
    m_iBetRatioL = m_ModelData->m_PointRatioL;
    m_iBetRatioR = m_ModelData->m_PointRatioR;
    sprintf(szRatioNum, "%i/%i", m_iBetRatioL, m_iBetRatioR);
    m_pRatioNumLabel->setString(szRatioNum);

    switch(m_ModelData->m_State)
    {
    case GameState::STATE_DGSELECT:
        {
            for(int i = 0; i < 5; i++)
            {
                m_pSpriteBackCard[i]->setVisible(false);
                m_pSpriteCard[i]->setTexture(m_CardTexture[getCardIndex(m_ModelData->m_PokerValue[i])]);
                m_pSpriteCard[i]->setVisible(true);
            }

            ButtonState(m_ModelData->m_State);
        }
        break;
    }

    updateWinPoint();
}

/**
 * 進比倍按鈕按下
 **/
void DGLayer::onBtnDG(CCObject *pSender)
{
    memset(m_iHold, 0, sizeof(m_iHold));
#ifndef View_Test
    //send GSLMSG_PLAYER_DOUBLE_GAME 3004
    m_pFacade->sendNotification(GameSLMsg::GSLMSG_PLAYER_DOUBLE_GAME::Name);
#else
    static GCliMsgDoubleGameData_Vo vo;
    vo.PokerHand = 5;
    //重新翻牌
    updateDGCardCallBack(&vo);
#endif  //View_Test
}

/**
 * 取分按鈕按下
 **/
void DGLayer::onBtnCollect(CCObject *pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "onBtnCollect, m_iWinReduceNum=%i", m_iWinReduceNum);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    wchar_t szLog[1024];
    wsprintf(szLog, L"updateCollectCallBack, m_iWinReduceNum=%i\r\n", m_iWinReduceNum);
    OutputDebugString(szLog);
#endif
    if(m_iWinReduceNum == 0)    //還沒開始跑表
    {
        m_pDGMenu->setVisible(false);
        m_pDGBtnAnimation->getAnimation()->setVisible(false);
        m_pDGBtnAnimation->stopAction();
        //m_pCollectMenu->setVisible(false);

        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_COLLECT.wav");

        m_iWinReduceNum = 1;
        schedule(schedule_selector(DGLayer::RunNumber), 0.1);

#ifndef View_Test
        m_ModelData->m_CanDoubleGame = false;
        m_ModelData->m_PreState = m_ModelData->m_State;
        m_ModelData->m_State = GameState::STATE_BALANCE;
        //GSLMSG_GET_POINT 3006 and waitCallBack
        m_pFacade->sendNotification(GameSLMsg::GSLMSG_GET_POINT::Name);
#else
        m_iCredit += m_iWinPoint;
        m_iWinPoint = 0;
        updateCollectCallBack();
#endif  //View_Test
    }
    else if(m_iWinPoint > 0)
    {
        m_iWinReduceNum = m_iWinPoint;
    }
}

void DGLayer::RunNumber(float dt)
{
    //遞減到0
    char szTmpData[20];
    static int iAcceleration = 0;

    if(m_iWinPoint >= m_iWinReduceNum)
    {
        iAcceleration++;
        m_iWinPoint = m_iWinPoint - m_iWinReduceNum;
        m_iCredit = m_iCredit + m_iWinReduceNum;
            
        sprintf(szTmpData, "%i.00", m_iWinPoint);
        m_pWinPointLabel->setString(szTmpData);

        sprintf(szTmpData, "%i.00", m_iCredit);
        m_pCreditLabel->setString(szTmpData);

        if(m_iWinPoint == 0)    //跑表跑完了
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "RunNumber, m_iWinPoint==0, m_bWaitToChangeView:%i, m_iWinReduceNum=%i", m_bWaitToChangeView, m_iWinReduceNum);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            wchar_t szLog[1024];
            wsprintf(szLog, L"RunNumber, m_iWinPoint==0, m_bWaitToChangeView:%i, m_iWinReduceNum=%i\r\n", m_bWaitToChangeView, m_iWinReduceNum);
            OutputDebugString(szLog);
#endif
            m_pCollectMenu->setVisible(false);
            iAcceleration = 0;
            if(m_bWaitToChangeView)
            {
                updateCollectCallBack();
            }
            m_iWinReduceNum = 0;
            unschedule(schedule_selector(DGLayer::RunNumber));
        }
        else if((m_iWinPoint % (m_iWinReduceNum*10) == 0 && m_iWinReduceNum == 1) || m_iWinReduceNum > 1) //個位數跑表先跑一輪後
        {
            double dExponent = (int)log10((double)m_iWinPoint);    //取目前贏分指數

            do
            {
                m_iWinReduceNum = rand_number((int)pow(10, (dExponent-1)+((runNumSpeed>=0 && runNumSpeed<=1)?runNumSpeed:0)), (int)pow(10, dExponent));
            }
            while(m_iWinReduceNum > m_iWinPoint || m_iWinReduceNum == 0);

            m_iWinReduceNum += iAcceleration;

            if(m_iWinReduceNum > m_iWinPoint)
            {
                m_iWinReduceNum = m_iWinPoint;
            }
        }
    }
    else
    {
        m_iWinReduceNum = m_iWinPoint;
    }
}

/**
 * 由mediator呼叫
 * 
 **/
void DGLayer::updateCollectCallBack()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "updateCollectCallBack, m_iWinPoint=%i", m_iWinPoint);
#endif
    if(m_iWinPoint == 0)    //跑表跑完
    {
        m_bWaitToChangeView = false;
        //算分以及changeView
        updateCredit(m_iCredit, m_iMultiple);
        changeView(GameView::VIEW_NG);
    }
    else    //跑表中
    {
        m_bWaitToChangeView = true;
    }
}

/**
 * 由mediator呼叫
 * 
 **/
void DGLayer::updateDGCardCallBack(GCliMsgDoubleGameData_Vo *DGVo)
{
    for(int i = 0; i < 5; i++)
    {
        m_pSpriteCard[i]->setVisible(false);
    }

    if(m_iSelectIndex != 0)
    {
        m_pSpritePlayer[m_iSelectIndex-1]->setVisible(false);
        m_pSpriteCardEffect[m_iSelectIndex-1]->setVisible(false);
        m_pDGWinnerAnimation->getAnimation()->setVisible(false);
        m_pDGWinnerAnimation->stopAction();

        m_iSelectIndex = 0;
    }

    ButtonState(GameState::STATE_DGCHOOSE);

    m_pSpriteCard[0]->setTexture(m_CardTexture[getCardIndex(DGVo->PokerHand)]);

    m_pSpriteBackCard[0]->runAction(
        CCSequence::actions(
            CCCallFuncND::actionWithTarget(this, 
                callfuncND_selector(DGLayer::onShowDGCard), 
                (void *)DGVo), 
            NULL));
}

/**
 * 由mediator呼叫
 * 
 **/
void DGLayer::updateDGResultCardCallBack(GCliMsgDoubleGameResult_Vo *DGResultVo)
{
    m_ModelData->m_iWinPoint = DGResultVo->WinLosePoint;
    m_iWinPoint = DGResultVo->WinLosePoint/100;
    if(DGResultVo->WinLosePoint > 0)
    {
        m_ModelData->m_PreState = m_ModelData->m_State;
        m_ModelData->m_State = STATE_DGWIN;
    }
    else if(DGResultVo->WinLosePoint == 0)
    {
        m_ModelData->m_PreState = m_ModelData->m_State;
        m_ModelData->m_State = STATE_DGOVER;
    }

    memcpy(m_ModelData->m_PokerValue, DGResultVo->PokerHand, sizeof(POKERHAND));

    //PokerHand 比倍用的5張牌 0:電腦牌 1:玩家選的牌的值 2~4:本來蓋住的牌的值

    for(int i = 0; i < 5; i++)
    {
        if(i == m_iSelectIndex)
        {
            m_pSpriteCard[m_iSelectIndex]->setTexture(m_CardTexture[getCardIndex(DGResultVo->PokerHand[1])]);
        }
        else
        {
            if(i == 0 || i > m_iSelectIndex)
            {
                m_pSpriteCard[i]->setTexture(m_CardTexture[getCardIndex(DGResultVo->PokerHand[i])]);
            }
            else //i < m_iSelectIndex
            {
                m_pSpriteCard[i]->setTexture(m_CardTexture[getCardIndex(DGResultVo->PokerHand[i+1])]);
            }
        }
    }

    m_pSpriteBackCard[m_iSelectIndex]->setFlipX(false);
    m_pSpriteBackCard[m_iSelectIndex]->runAction(
        CCSequence::actions(
            CCOrbitCamera::actionWithDuration(
                reverseCardFrame, 1, 0, 0, -180, 0, 0),
            CCCallFuncND::actionWithTarget(this, 
                callfuncND_selector(DGLayer::onShowDGCard), 
                    (void *)DGResultVo), 
            NULL));
}

int DGLayer::getCardIndex(int iPokerHand)
{
    int iIndex = 0;
    int iNum = iPokerHand & 0x00ff;
    int iSuit = iPokerHand & 0xff00;

    if(iNum == 14)
    {
        iNum = 1;
    }
    
    if(iNum == 15)
    {
        iNum = 54;
    }
    else if(iNum == 16)
    {
        iNum = 55;
    }
    else
    {
        iIndex += iNum;

        if (iSuit == 0x0200)//紅心
        {
            iIndex += 13;
        }
        else if (iSuit == 0x0400)//方塊
        {
            iIndex += 13 * 2;
        }
        else if (iSuit == 0x0800)//梅花
        {
            iIndex += 13 * 3;
        }
    }

    return iIndex - 1;
}

/**
 * DG牌的動作
 **/
void DGLayer::onShowDGCard(CCNode *sender,void *vo)
{
    GCliMsgDoubleGameData_Vo    *DGVo = NULL;
    GCliMsgDoubleGameResult_Vo  *DGResultVo = NULL;

    static int iCard = -1;
    
    //全部蓋牌, 且翻第一張
    if(m_ModelData->m_State == STATE_DGSELECT)
    {
        DGVo = (GCliMsgDoubleGameData_Vo *)vo;

        switch(iCard)
        {
        case -1:    //蓋牌
            {
                iCard += 1;

                for(int i = 0; i < 5; i++)
                {
                    m_pSpriteBackCard[i]->setVisible(true);
                    m_pSpriteCard[i]->setVisible((bool)m_iHold[i]);

                    //if(m_iHold[i] == 0)
                    {
                        m_pSpriteBackCard[i]->setFlipX(true);

                        //翻最後一張就呼叫callback function
                        if(i < 4) //前四張
                        {
                            m_pSpriteBackCard[i]->runAction(
                                CCSequence::actions(
                                    CCOrbitCamera::actionWithDuration(
                                        reverseCardFrame, 1, 0, 0, 180, 0, 0),
                                    NULL));
                        }
                        else if(i == 4)
                        {
                            m_pSpriteBackCard[i]->runAction(
                                CCSequence::actions(
                                    CCOrbitCamera::actionWithDuration(
                                        reverseCardFrame, 1, 0, 0, 180, 0, 0),
                                    CCCallFuncND::actionWithTarget(this, 
                                        callfuncND_selector(DGLayer::onShowDGCard), 
                                        vo), 
                                    NULL));
                        }
                    }
                }
                break;
            }

        case 0: //第一張的翻牌動作
            {
                iCard += 1;

                //m_State = STATE_DGCHOOSE;

                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_OPEN.wav");

                m_pSpriteBackCard[0]->setFlipX(false);
                //翻開第一張牌, 翻完後呼叫callback func
                m_pSpriteBackCard[0]->runAction(
                    CCSequence::actions(
                        CCOrbitCamera::actionWithDuration(
                            reverseCardFrame, 1, 0, 0, -180, 0, 0),
                        CCCallFuncND::actionWithTarget(this, 
                            callfuncND_selector(DGLayer::onShowDGCard), 
                            vo), 
                        NULL));
                break;
            }

        case 1: //第一張翻牌後顯示牌面
            {
                m_ModelData->m_PreState = m_ModelData->m_State;
                m_ModelData->m_State = GameState::STATE_DGCHOOSE;
                
                m_pSpriteBackCard[0]->setVisible(false);
                m_pSpriteCard[0]->setVisible(true);
                break;
            }
        }
    }
    else if(m_ModelData->m_State == GameState::STATE_DGWIN || m_ModelData->m_State == GameState::STATE_DGOVER)
    {
        DGResultVo = (GCliMsgDoubleGameResult_Vo *)vo;

        switch(iCard)
        {
        case 1:     //翻開選的那張, 之後也翻其他張
            {
                m_pSpriteBackCard[m_iSelectIndex]->setFlipX(false);
                m_pSpriteBackCard[m_iSelectIndex]->setVisible(false);
                m_pSpriteCard[m_iSelectIndex]->setVisible(true);
                m_pSpritePlayer[m_iSelectIndex-1]->setVisible(true);

                char szWinPoint[20];
                sprintf(szWinPoint, "%i.00", DGResultVo->WinLosePoint/100);
                m_pWinPointLabel->setString(szWinPoint);
                m_pDGBetLabel->setString(szWinPoint);
                sprintf(szWinPoint, "%i.00", DGResultVo->WinLosePoint/100*2);
                //m_pWinPointLabel->setString(szWinPoint);
                m_pDGWillWinLabel->setString(szWinPoint);

                iCard += 1;

                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_OPEN.wav");
                
                m_pSpriteBackCard[iCard-1]->runAction(
                    CCSequence::actions(
                        CCOrbitCamera::actionWithDuration(
                            reverseCardFrame, 1, 0, 0, -180, 0, 0),
                        CCCallFuncND::actionWithTarget(this, 
                            callfuncND_selector(DGLayer::onShowDGCard), 
                            vo), 
                        NULL));
                break;
            }
        case 2:
        case 3:
        case 4:
            {
                if((iCard-1) != m_iSelectIndex)
                {
                    m_pSpriteBackCard[iCard-1]->setFlipX(false);
                    m_pSpriteBackCard[iCard-1]->setVisible(false);
                    m_pSpriteCard[iCard-1]->setVisible(true);

                    iCard += 1;

                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_OPEN.wav");
                
                    m_pSpriteBackCard[iCard-1]->runAction(
                        CCSequence::actions(
                            CCOrbitCamera::actionWithDuration(
                                reverseCardFrame, 1, 0, 0, -180, 0, 0),
                            CCCallFuncND::actionWithTarget(this, 
                                callfuncND_selector(DGLayer::onShowDGCard), 
                                vo), 
                            NULL));
                }
                else if((iCard-1) == m_iSelectIndex)
                {
                    iCard += 1;

                    m_pSpriteBackCard[iCard-1]->runAction(
                        CCSequence::actions(
                            CCOrbitCamera::actionWithDuration(
                                reverseCardFrame, 1, 0, 0, -180, 0, 0),
                            CCCallFuncND::actionWithTarget(this, 
                                callfuncND_selector(DGLayer::onShowDGCard), 
                                vo), 
                            NULL));
                }
                break;
            }

        case 5:
            {
                if((iCard-1) != m_iSelectIndex)
                {
                    m_pSpriteBackCard[iCard-1]->setVisible(false);
                    m_pSpriteCard[iCard-1]->setVisible(true);
                }

                iCard = -1;

                if(DGResultVo->WinLosePoint == 0)
                {
                    m_pSpriteCardEffect[m_iSelectIndex-1]->setVisible(false);

                    //m_View = VIEW_NG;
                   //m_State = STATE_BALANCE;
#ifndef View_Test
                    GSLWaitSync_Vo syncVo;
                    syncVo.nSyncSignal = SYNCSIGNAL_GAME_BALANCE;
                    m_ModelData->m_PreState = m_ModelData->m_State;
                    m_ModelData->m_State = GameState::STATE_BALANCE;
                    m_pFacade->sendNotification(GameClientServiceEvent::SLMSG_WAIT_SYNC_Event::Name, &syncVo);
#else
                    m_ModelData->m_PreState = m_ModelData->m_State;
                    m_ModelData->m_State = GameState::STATE_BALANCE;
#endif
                    scheduleOnce(schedule_selector(DGLayer::scheduleOnceTimer), 2);
                }
                else
                {
                    //閃winner
                    m_pDGWinnerAnimation->getAnimation()->setVisible(true);
                    m_pDGWinnerAnimation->runAction();
                    m_ModelData->m_PreState = m_ModelData->m_State;
                    m_ModelData->m_State = GameState::STATE_DGSELECT;
                    ButtonState(m_ModelData->m_State);
                }
                break;
            }
        }
    }
}

/**
 * DG點牌的事件
 **/
void DGLayer::onTouchesCardBegan(int iTag)
{
    if(m_iSelectIndex == 0) //判斷避免連點兩次牌
    {
        //pTouch->
        if(iTag > 0 && iTag < 5)
        {
            CCLog("onTouchesCardBegan: %i", iTag);

            m_iSelectIndex = iTag;

            m_pSpriteCardEffect[m_iSelectIndex-1]->setVisible(true);

            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_HOLD.wav");

    #ifndef View_Test
            //發送封包
            GSLDoubleGame_Vo vo;
            vo.SelectIndex = m_iSelectIndex;
            m_pFacade->sendNotification(GameSLMsg::GSLMSG_DOUBLE_GAME::Name, &vo);
    #else
            static GCliMsgDoubleGameResult_Vo vo = {0};
            vo.PokerHand[0] = 5;
            vo.PokerHand[1] = 6;
            vo.PokerHand[2] = 7;
            vo.PokerHand[3] = 8;
            vo.PokerHand[4] = 2;
            if(m_iSelectIndex != 4)
            {
                vo.WinLosePoint = m_iWinPoint*2;
            }
            else
            {
                vo.WinLosePoint = 0;
            }
            updateDGResultCardCallBack(&vo);
    #endif  //View_Test
        }
    }
}

/**
 * 更新畫面押注
 **/
void DGLayer::updateBet(int iBet)
{
    char szBet[10];

    int iMultiple = pow((float)10, m_iMultiple-1);
    sprintf(szBet, "%i.00", iBet*iMultiple);

    m_pBetLabel->setString(szBet);
    //m_pBetLabel->visit();
}

/**
 * 更新畫面credit
 **/
void DGLayer::updateCredit(int iCredit, int iMultiple)
{
    char szCredit[20];

    sprintf(szCredit, "%i.00", iCredit);
/*
    int iFirst = iCredit/iMultiple;
    int iSecond = iCredit%iMultiple;

#ifndef View_Test
#else
    if(iSecond != 0)
    {
        sprintf(szCredit, "%i.%i", iFirst, iSecond);
    }
    else
    {
        sprintf(szCredit, "%i.00", iFirst);
    }
#endif //View_Test*/
    
    m_pCreditLabel->setString(szCredit);
    //m_pCreditLabel->visit();
}

/**
 * 更新畫面贏分, 包含NG或DG
 **/
void DGLayer::updateWinPoint()
{
    char szWinPoint[20];
#ifndef View_Test
    sprintf(szWinPoint, "%i.00", m_ModelData->m_iWinPoint/100);
#else
    sprintf(szWinPoint, "%i.00", m_iWinPoint/100);
#endif
    m_pWinPointLabel->setString(szWinPoint);
    m_pDGBetLabel->setString(szWinPoint);
#ifndef View_Test
    sprintf(szWinPoint, "%i.00", m_ModelData->m_iWinPoint/100*2);
#else
    sprintf(szWinPoint, "%i.00", m_iWinPoint/100*2);
#endif
    m_pDGWillWinLabel->setString(szWinPoint);


    CCBlink *pBlinkAction = CCBlink::create(2.75, 1);
    CCRepeatForever *pActionForever = CCRepeatForever::create(pBlinkAction);
    pActionForever->setTag(0);
}

/**
 * 檢查押注是否合法
 **/
bool DGLayer::CheckBet(int iBet)
{
    int iMultiple = pow((float)10, m_iMultiple-1);

    if(iBet*iMultiple <= m_iCredit)
    {
        return true;
    }

    return false;
}

void DGLayer::scheduleOnceTimer(float fTime)
{
    changeView(GameView::VIEW_NG);
}

//TODO: 待優化
void DGLayer::changeView(int iView)
{
    m_ModelData->m_PreView = m_ModelData->m_View;
    m_ModelData->m_View = iView;
    ((CCLayerMultiplex*)m_pParent)->switchTo(iView);
}

/**
 * 按鈕狀態
 **/
void DGLayer::ButtonState(int iState)
{
    switch(iState)
    {
    case GameState::STATE_DGCHOOSE:
        {
            m_pDGMenu->setVisible(false);
            m_pDGBtnAnimation->getAnimation()->setVisible(false);
            m_pDGBtnAnimation->stopAction();
            m_pCollectMenu->setVisible(false);
            break;
        }

    case GameState::STATE_DGSELECT:
    case GameState::STATE_DGWIN:
        {
            m_pDGMenu->setVisible(true);
            m_pDGBtnAnimation->getAnimation()->setVisible(true);
            m_pDGBtnAnimation->runAction();
            m_pCollectMenu->setVisible(true);
            break;
        }
    }
}
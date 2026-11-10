#include "NGLayer.h"
#include "extensions\CCControlExtension\CCControlButton.h"
#include "..\..\..\Model\Vo\GSLMsg_Vo.h"
#include "CommandList.h"
#include "GlobalVar.h"
#include <cmath>
#include "AppDelegate.h"
#include "GameScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <Windows.h>
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

//USING_NS_CC;

#define reverseCardFrame    0.2

NGLayer::NGLayer()
    : m_pWinPointLabel(NULL)
    , m_pBetLabel(NULL)
    , m_pCreditLabel(NULL)
    , m_pMultipleLabel(NULL)
    , m_ModelData(NULL)
    , gameProxy(NULL)
    , m_pBetPayTableAnimation(NULL)
    , m_pHintAnimation(NULL)
    , m_iMultiple(1)
    , m_iBet(1)
    , m_iCredit(999999)
    , m_iBetRatioL(1)
    , m_iBetRatioR(1)
    , m_iTableNum(1)
    , m_iHoldCardNum(0)
    , m_iSelectIndex(0)
    , m_bIsDealWin(false)
{
    for(int i = 0; i < 5; i++)
    {
        m_pWinCardAnimation[i] = NULL;
    }
}

NGLayer::~NGLayer()
{
    SAFE_DELETE(m_pBetPayTableAnimation);
    SAFE_DELETE(m_pHintAnimation);
    SAFE_DELETE(m_pWinPayAnimation);

#ifdef View_Test
    SAFE_DELETE(m_ModelData);
#endif

    for(int i = 0; i < 5; i++)
    {
        SAFE_DELETE(m_pWinCardAnimation[i]);
    }
}

// on "init" you need to initialize your instance
bool NGLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCLayer::setKeypadEnabled(true);

    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SND_5PK_JOKER_BET.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SND_5PK_JOKER_HOLD.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SND_5PK_JOKER_DEAL.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SND_5PK_JOKER_DEALTEE.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SND_5PK_JOKER_COLLECT.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SND_5PK_JOKER_OPEN.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SND_5PK_JOKER_TRANSFOR.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SND_5PK_JOKER_WIN_BIG.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SND_5PK_JOKER_WIN_SMALL.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SND_5PK_JOKER_WIN_TOPAWARD.wav");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "NGLayer::init");
#endif

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
            else
            {
                memcpy(m_iPayTableNum[0], m_ModelData->m_GameScore_1+1, sizeof(m_iPayTableNum[0]));
                memcpy(m_iPayTableNum[1], m_ModelData->m_GameScore_2+1, sizeof(m_iPayTableNum[1]));
                memcpy(m_iPayTableNum[2], m_ModelData->m_GameScore_3+1, sizeof(m_iPayTableNum[2]));
                memcpy(m_iPayTableNum[3], m_ModelData->m_GameScore_4+1, sizeof(m_iPayTableNum[3]));
                memcpy(m_iPayTableNum[4], m_ModelData->m_GameScore_5+1, sizeof(m_iPayTableNum[4]));
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
        m_ModelData->m_PreState = GameState::STATE_OVER;
        m_ModelData->m_View = GameView::VIEW_NG;
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
    int iPay[10] = {1, 2, 3, 5, 7, 10, 40, 100, 400, 800};
    for(int i = 0; i < 10; i++)
    {
        m_iPayTableNum[0][i] = m_iBet*iPay[i]*m_iMultiple;
        m_iPayTableNum[1][i] = m_iBet*iPay[i]*m_iMultiple*2;
        m_iPayTableNum[2][i] = m_iBet*iPay[i]*m_iMultiple*3;
        m_iPayTableNum[3][i] = m_iBet*iPay[i]*m_iMultiple*4;
        m_iPayTableNum[4][i] = m_iBet*iPay[i]*m_iMultiple*5;
    }
#endif  //View_Test

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    InitButton();
    InitLabel();
    InitPayTable();

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
    m_pBackgroundNG = CCSprite::create("PIC_BackGround.png");

    // position the sprite on the center of the screen
    m_pBackgroundNG->setPosition(ccp(size.width/2, size.height/2));

    // add the sprite as a child to this layer
    this->addChild(m_pBackgroundNG, 0);

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
        m_pSpriteBackCard[i] = CCSprite::spriteWithFile("PIC_POKER_BG.png");
        m_pSpriteBackCard[i]->setTag(i);
        m_pSpriteBackCard[i]->setPosition(ccp(100+i*150, 240));
        this->addChild(m_pSpriteBackCard[i], 1);

        //牌面
        m_pSpriteCard[i] = CCSprite::create(m_CardTexture[0]);
        m_pSpriteCard[i]->setPosition(ccp(100+i*150, 240));
        this->addChild(m_pSpriteCard[i], 1);
        m_pSpriteCard[i]->setVisible(false);

        //Held
        m_pSpriteHeld[i] = CCSprite::spriteWithFile("PIC_HELD.png");
        m_pSpriteHeld[i]->setPosition(ccp(100+i*150, 245));
        this->addChild(m_pSpriteHeld[i], 2);
        m_pSpriteHeld[i]->setVisible(false);
    }

    return true;
}

void NGLayer::onEnter()
{
#ifndef View_Test
    if(m_ModelData->m_PreView == GameView::VIEW_LOGIN)
    {
        updateUIdata();
    }
    updateWinPoint();

    m_pBetPayTableAnimation->getAnimation()->setVisible(true);
    m_pBetPayTableAnimation->runAction();

    if(m_ModelData->m_BetIndex >= 1)
    {
        m_pBetPayTableAnimation->moveAnimationPos(188 + 64*(m_ModelData->m_BetIndex-1), 354);
    }
#endif  //View_Test

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "NGLayer::onEnter");
#endif
    CCLayer::onEnter();
}

void NGLayer::onBtnHelp(CCObject *pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#endif
}
/*
void NGLayer::onBtnKeep(CCObject *pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#endif
}
*/
void NGLayer::onBtnLeave(CCObject *pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void NGLayer::keyBackClicked()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "NGLayer::keyBackClicked");
#endif
    CCDirector::sharedDirector()->end();
}

void NGLayer::InitButton()
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
                                        menu_selector(NGLayer::onBtnHelp));
    pHelpItem->setAnchorPoint(ccp(0, 0));
    pHelpItem->setPosition(ccp(665, 550));
    /*
    CCMenuItemImage *pKeepItem = CCMenuItemImage::create(
                                        "BTN_KEEP_1.png",
                                        "BTN_KEEP_3.png",
                                        this,
                                        menu_selector(NGLayer::onBtnKeep));
    pKeepItem->setAnchorPoint(ccp(0, 0));
    pKeepItem->setPosition(ccp(739, 549));
    */
  
    
    CCMenuItemImage *pLeaveItem = CCMenuItemImage::create(
                                        "BTN_LEAVE_1.png",
                                        "BTN_LEAVE_3.png",
                                        this,
                                        menu_selector(NGLayer::onBtnLeave));
    pLeaveItem->setAnchorPoint(ccp(0, 0));
    pLeaveItem->setPosition(ccp(739, 550));
  
    /**
     * Game button
     */
    CCMenuItemImage *pBetItem = CCMenuItemImage::create(
                                        "BTN_BET.png",
                                        "BTN_BET_3.png",
                                        this,
                                        menu_selector(NGLayer::onBtnBet));
    pBetItem->setAnchorPoint(ccp(0, 0));
    pBetItem->setPosition(ccp(248, 19));

    CCMenuItemImage *pDealItem = CCMenuItemImage::create(
                                        "BTN_DEAL.png",
                                        "BTN_DEAL_3.png",
                                        this,
                                        menu_selector(NGLayer::onBtnDeal));
    pDealItem->setAnchorPoint(ccp(0, 0));
    pDealItem->setPosition(ccp(345, 22));

    CCMenuItemImage *pMaxBetItem = CCMenuItemImage::create(
                                        "BTN_MAXBET.png",
                                        "BTN_MAXBET_3.png",
                                        this,
                                        menu_selector(NGLayer::onBtnMaxBet));
    pMaxBetItem->setAnchorPoint(ccp(0, 0));
    pMaxBetItem->setPosition(ccp(476, 19));

    CCMenuItemImage *pMultipleDownItem = CCMenuItemImage::create(
                                        "BTN_COINVALUE_DOWN_1.png",
                                        "BTN_COINVALUE_DOWN_3.png",
                                        this,
                                        menu_selector(NGLayer::onBtnMultipleDown));
    pMultipleDownItem->setAnchorPoint(ccp(0, 0));
    pMultipleDownItem->setPosition(ccp(616, 9));

    CCMenuItemImage *pMultipleUpItem = CCMenuItemImage::create(
                                        "BTN_COINVALUE_UP_1.png",
                                        "BTN_COINVALUE_UP_3.png",
                                        this,
                                        menu_selector(NGLayer::onBtnMultipleUp));
    pMultipleUpItem->setAnchorPoint(ccp(0, 0));
    pMultipleUpItem->setPosition(ccp(741, 9));

    CCMenuItemImage *pHoldMenu[5];
    for(int i = 0; i < 5; i++)
    {
        pHoldMenu[i] = CCMenuItemImage::create(
                        "BTN_HOLD_1.png",
                        "BTN_HOLD_2.png",
                        this,
                        menu_selector(NGLayer::onBtnHold));
        pHoldMenu[i]->setAnchorPoint(ccp(0, 0));
        pHoldMenu[i]->setPosition(ccp(40+i*150, 110));
        pHoldMenu[i]->setTag(i);

        m_pHoldMenu[i] = CCMenu::create(pHoldMenu[i], NULL);
        m_pHoldMenu[i]->setPosition(CCPointZero);
        m_pHoldMenu[i]->setVisible(false);
        this->addChild(m_pHoldMenu[i], 1);
    }

    /**********************NG button****************************/
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

    m_pBetMenu = CCMenu::create(pBetItem, NULL);
    m_pBetMenu->setPosition(CCPointZero);
    this->addChild(m_pBetMenu, 1);

    m_pDealMenu = CCMenu::create(pDealItem, NULL);
    m_pDealMenu->setPosition(CCPointZero);
    this->addChild(m_pDealMenu, 1);

    m_pMaxBetMenu = CCMenu::create(pMaxBetItem, NULL);
    m_pMaxBetMenu->setPosition(CCPointZero);
    this->addChild(m_pMaxBetMenu, 1);

    m_pMultipleDown = CCMenu::create(pMultipleDownItem, NULL);
    m_pMultipleDown->setPosition(CCPointZero);
    this->addChild(m_pMultipleDown, 1);

    m_pMultipleUp = CCMenu::create(pMultipleUpItem, NULL);
    m_pMultipleUp->setPosition(CCPointZero);
    this->addChild(m_pMultipleUp, 1);
}

void NGLayer::InitLabel()
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
    //m_iCredit = m_ModelData->m_uTotalPoints/100;
#endif  //View_Test
    //sprintf(szData, "%i.00", m_iCredit);
    m_pCreditLabel = CCLabelAtlas::create("50000.00", "NUM_SCORE_ALL.png", 12, 12, '.');
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
}

void NGLayer::InitPayTable()
{
    //m_iPayTableNum[][]
    char szPayNum[10];
    int i = 0;
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            //sprintf(szPayNum, "%i", m_iPayTableNum[i][j]);
            m_pPayTableLabel[i][j] = CCLabelAtlas::create("4000000", "NUM_BOARD_ALL.png", 14, 20, '0');
            m_pPayTableLabel[i][j]->setAnchorPoint(ccp(1, 0.5));
            m_pPayTableLabel[i][j]->setPosition(250+i*64, 387+j*19);
            m_pPayTableLabel[i][j]->setScaleX(0.6);
            this->addChild(m_pPayTableLabel[i][j], 1);
            //m_CCPayTableLayer->addChild(m_pPayTableLabel[i][j], 2);
        }
    }
}

void NGLayer::InitAnimation()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    //提示文字動畫
    PicInfo info;
    info.iHeight = 50;
    info.iWidth = 369;
    info.Hor_Num = 5;
    info.Ver_Num = 11;
    info.Total_Num = 55;
    info.x = size.width/2-185;
    info.y = 95;
    m_pHintAnimation = new AnimationAction("ANI_BetOrDeal_ALL.png", info);
    m_pHintAnimation->getAnimation()->setAnchorPoint(ccp(0, 0));
    this->addChild(m_pHintAnimation->getAnimation(), 1);
    m_pHintAnimation->runAction();

    //賠率表中獎動畫
    info.iHeight = 23;
    info.iWidth = 471;
    info.Hor_Num = 5;
    info.Ver_Num = 16;
    info.Total_Num = 78;
    info.x = 55;
    info.y = 375;

    m_pWinPayAnimation = new AnimationAction("ANI_BOARD(WIN_LIGHT)_ALL.png", info, 0.06);
    m_pWinPayAnimation->getAnimation()->setAnchorPoint(ccp(0, 0));
    m_pWinPayAnimation->setOpacity(180);    //0~255
    m_pWinPayAnimation->getAnimation()->setVisible(false);
    this->addChild(m_pWinPayAnimation->getAnimation(), 1);
    //m_pWinPayAnimation->runAction();

    //賠率表押注動畫
    info.iHeight = 235;
    info.iWidth = 64;
    info.Hor_Num = 5;
    info.Ver_Num = 1;
    info.Total_Num = 5;
    info.x = 188;
    info.y = 354;

    m_pBetPayTableAnimation  = new AnimationAction("ANI_BOARD(BET_LEVEL).png", info, 0.02, 1);
    this->addChild(m_pBetPayTableAnimation->getAnimation(), 1);
    m_pBetPayTableAnimation->getAnimation()->setAnchorPoint(ccp(0, 0));
    if(m_iBet == 0)
    {
        m_pBetPayTableAnimation->getAnimation()->setVisible(false);
    }
    else if(m_iBet > 0)
    {
        m_pBetPayTableAnimation->getAnimation()->setVisible(true);
        m_pBetPayTableAnimation->setFullFrame(5, 1);
    }

    //贏牌的牌閃爍動畫
    info.iHeight = 180;
    info.iWidth = 130;
    info.Hor_Num = 6;
    info.Ver_Num = 1;
    info.Total_Num = 6;

    for(int i = 0; i < 5; i++)
    {
        info.x = 100+i*150;
        info.y = 240;
        m_pWinCardAnimation[i] = new AnimationAction(
            "ANI_POKER_LIGHT_ALL(repeat).png",
            info);

        this->addChild(m_pWinCardAnimation[i]->getAnimation(), 1);
        m_pWinCardAnimation[i]->getAnimation()->setVisible(false);
    }
}

void NGLayer::updateUIdata()
{
    if(!m_pHintAnimation->getAnimation()->isVisible())
    {
        m_pHintAnimation->runAction();
        m_pHintAnimation->getAnimation()->setVisible(true);
    }

    char szData[20];
    m_iBet = (m_ModelData->m_GameRatio[m_ModelData->m_Multiple-1])*(m_ModelData->m_BetRatio)*(m_ModelData->m_BetIndex);
    sprintf(szData, "%i.00", m_iBet);
    m_pBetLabel->setString(szData);

    memset(szData, 0, sizeof(szData));
    m_iCredit = m_ModelData->m_uTotalPoints/100;
    sprintf(szData, "%i.00", m_iCredit);
    m_pCreditLabel->setString(szData);

    char szMultiple[10];
    sprintf(szMultiple, "2%i", (m_ModelData->m_GameRatio[m_ModelData->m_Multiple-1])*(m_ModelData->m_BetRatio));
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

    memcpy(m_iPayTableNum[0], m_ModelData->m_GameScore_1+1, sizeof(m_iPayTableNum[0]));
    memcpy(m_iPayTableNum[1], m_ModelData->m_GameScore_2+1, sizeof(m_iPayTableNum[1]));
    memcpy(m_iPayTableNum[2], m_ModelData->m_GameScore_3+1, sizeof(m_iPayTableNum[2]));
    memcpy(m_iPayTableNum[3], m_ModelData->m_GameScore_4+1, sizeof(m_iPayTableNum[3]));
    memcpy(m_iPayTableNum[4], m_ModelData->m_GameScore_5+1, sizeof(m_iPayTableNum[4]));

    char szPayNum[10];
    int i = 0;

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            sprintf(szPayNum, "%i", m_iPayTableNum[i][j]);
            m_pPayTableLabel[i][j]->setString(szPayNum);
        }
    }

    if(m_ModelData->m_State == STATE_READY)
    {
        ButtonState(m_ModelData->m_State);
    }
}

/**
 * 押注按鈕按下
 **/
void NGLayer::onBtnBet(CCObject* pSender)
{
#ifndef View_Test
    m_ModelData->m_BetIndex += 1;
    if(m_ModelData->m_BetIndex > m_ModelData->m_MaxBet)
    {
        m_ModelData->m_BetIndex = m_ModelData->m_BetIndex - 5;
    }
    m_iBet = (m_ModelData->m_GameRatio[m_ModelData->m_Multiple-1])*(m_ModelData->m_BetRatio)*(m_ModelData->m_BetIndex);
    updateBet(m_iBet);
#else
    m_ModelData->m_BetIndex += 1;
    if(m_ModelData->m_BetIndex > 5)
    {
        m_ModelData->m_BetIndex = m_ModelData->m_BetIndex - 5;
    }
    updateBet(m_ModelData->m_BetIndex);
#endif  //View_Test

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_BET.wav");

    m_pBetPayTableAnimation->getAnimation()->setVisible(true);
    m_pBetPayTableAnimation->runAction();

    if(m_ModelData->m_BetIndex >= 1)
    {
        m_pBetPayTableAnimation->moveAnimationPos(188 + 64*(m_ModelData->m_BetIndex-1), 354);
    }
}

/**
 * 最大押注按鈕按下
 **/
void NGLayer::onBtnMaxBet(CCObject *pSender)
{
#ifndef View_Test
    m_ModelData->m_BetIndex = m_ModelData->m_MaxBet;
    m_iBet = (m_ModelData->m_GameRatio[m_ModelData->m_Multiple-1])*(m_ModelData->m_BetRatio)*(m_ModelData->m_BetIndex);
#else
    m_ModelData->m_BetIndex = 5;
#endif  //View_Test

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_BET.wav");

    updateBet(m_iBet);
    m_pBetPayTableAnimation->getAnimation()->setVisible(true);
    m_pBetPayTableAnimation->runAction();

    if(m_ModelData->m_BetIndex >= 1)
    {
        m_pBetPayTableAnimation->moveAnimationPos(188 + 64*(m_ModelData->m_BetIndex-1), 354);
    }
}

/**
 * 發牌按鈕按下
 **/
void NGLayer::onBtnDeal(CCObject *pSender)
{
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_DEAL.wav");

    //m_HintAction->set
    if(m_ModelData->m_State == GameState::STATE_READY || m_ModelData->m_State == GameState::STATE_OVER) //第一把
    {
        memset(m_iHold, 0, sizeof(m_iHold));

#ifndef View_Test
        m_ModelData->m_PreState = m_ModelData->m_State;
        m_ModelData->m_State = GameState::STATE_DEAL;

        ButtonState(m_ModelData->m_State);

        //send GSLMSG_BET 3002
        GSLBet_Vo vo;
        vo.Bet = m_ModelData->m_BetIndex;
        m_pFacade->sendNotification(GameSLMsg::GSLMSG_BET::Name, &vo);
#else
        /************test result***************/
        if(m_iCredit < m_iBet)
        {
            static GCliMsgBetFail_Vo betVo;
            betVo.ErrorCode = 1;

            updateBetFailCallBack( &betVo);
        }
        else
        {
            //m_pHintAnimation->stopAction();
            //m_pHintAnimation->getAnimation()->setVisible(false);

            static GCliMsgDeal_Vo dealVo;

            dealVo.PokerHand[0] = 1;
            dealVo.PokerHand[1] = 2;
            dealVo.PokerHand[2] = 3;
            dealVo.PokerHand[3] = 4;
            dealVo.PokerHand[4] = 5;
            dealVo.PokerType = 0;
            dealVo.Hold[0] = 0;
            dealVo.Hold[1] = 1;
            dealVo.Hold[2] = 0;
            dealVo.Hold[3] = 1;
            dealVo.Hold[4] = 0;
            dealVo.UserPoint = m_iCredit - m_iBet*pow((float)10, m_iMultiple-1);

            static int pokerType = 0;

            if(pokerType < 9)
            {
                pokerType++;
            }
            else
            {
                pokerType = 0;
            }
            m_ModelData->m_PreState = m_ModelData->m_State;
            m_ModelData->m_State = GameState::STATE_DEAL;

            dealVo.PokerType = pokerType;
            updateDealCardCallBack(&dealVo);
        }
        /************test result***************/
#endif  //View_Test
    }
    else if(m_ModelData->m_State == GameState::STATE_DEAL)  //狀態切為draw
    {
        if(m_bIsDealWin)
        {
            m_bIsDealWin = false;
            m_pWinPayAnimation->getAnimation()->setVisible(false);
            m_pWinPayAnimation->stopAction();
        }
#ifndef View_Test
        //送出封包 GSLMSG_DRAW 3003
        GSLDraw_Vo vo = {0};
        memcpy(vo.Hold, m_iHold, sizeof(vo.Hold));
        m_pFacade->sendNotification(GameSLMsg::GSLMSG_DRAW::Name, &vo);
#else
        static GCliMsgDraw_Vo vo;

        vo.PokerHand[0] = 7;
        vo.PokerHand[1] = 2;
        vo.PokerHand[2] = 11;
        vo.PokerHand[3] = 4;
        vo.PokerHand[4] = 36;
        vo.WinLosePoint = 100;
        vo.PokerType = 0;

        static int pokerType = 0;
        
        if(pokerType != 0)
        {
            pokerType++;
            //vo->WinLosePoint = pokerType;
        }
        else
        {
            vo.WinLosePoint = 0;
            pokerType = 0;
        }
        vo.PokerType = pokerType;
        
        updateDrawCardCallBack(&vo);
#endif  //View_Test
    }
}

/**
 * 扣倍數按鈕按下
 **/
void NGLayer::onBtnMultipleDown(CCObject *pSender)
{
    //if(CheckBet(m_iBet))
    {
        if(m_iMultiple > 1)
        {
            m_iMultiple -= 1;
        }
        else if(m_iMultiple == 1)
        {
            m_iMultiple = 4;
        }
#ifndef View_Test
        //send GSLMSG_SET_MULTIPLE 3007
        GSLSetMultiple_Vo vo;
        vo.MultipleIndex = m_iMultiple;
        m_pFacade->sendNotification(GameSLMsg::GSLMSG_SET_MULTIPLE::Name, &vo);
#else
        //updateMultipleCallBack(m_iMultiple);

        int iPay[10] = {1, 2, 3, 5, 7, 10, 40, 100, 400, 800};
        int iPayTableNum[5][11];
        for(int i = 1; i < 11; i++)
        {
            iPayTableNum[0][i] = iPay[i-1]*pow((float)10, m_iMultiple-1);
            iPayTableNum[1][i] = iPay[i-1]*pow((float)10, m_iMultiple-1)*2;
            iPayTableNum[2][i] = iPay[i-1]*pow((float)10, m_iMultiple-1)*3;
            iPayTableNum[3][i] = iPay[i-1]*pow((float)10, m_iMultiple-1)*4;
            iPayTableNum[4][i] = iPay[i-1]*pow((float)10, m_iMultiple-1)*5;
        }
        updatePayTableCallBack(iPayTableNum);
#endif  //View_Test
    }
}

/**
 * 加倍數按鈕按下
 **/
void NGLayer::onBtnMultipleUp(CCObject *pSender)
{
    if(m_iMultiple < 4)
    {
        m_iMultiple += 1;
    }
    else if(m_iMultiple == 4)
    {
        m_iMultiple = 1;
    }
#ifndef View_Test
    //send GSLMSG_SET_MULTIPLE 3007
    GSLSetMultiple_Vo vo;
    vo.MultipleIndex = m_iMultiple;
    m_pFacade->sendNotification(GameSLMsg::GSLMSG_SET_MULTIPLE::Name, &vo);
#else
    updateMultipleCallBack(m_iMultiple);

    
    int iPay[10] = {1, 2, 3, 5, 7, 10, 40, 100, 400, 800};
    int iPayTableNum[5][11];
    for(int i = 1; i < 11; i++)
    {
        iPayTableNum[0][i] = iPay[i-1]*pow((float)10, m_iMultiple-1);
        iPayTableNum[1][i] = iPay[i-1]*pow((float)10, m_iMultiple-1)*2;
        iPayTableNum[2][i] = iPay[i-1]*pow((float)10, m_iMultiple-1)*3;
        iPayTableNum[3][i] = iPay[i-1]*pow((float)10, m_iMultiple-1)*4;
        iPayTableNum[4][i] = iPay[i-1]*pow((float)10, m_iMultiple-1)*5;
    }
    updatePayTableCallBack(iPayTableNum);
#endif  //View_Test
}

/**
 * Hold牌按鈕按下
 **/
void NGLayer::onBtnHold(CCObject *pSender)
{
    CCMenuItemImage *pImage = (CCMenuItemImage*)pSender;
    int i = pImage->getTag();
    m_iHold[i] = !m_iHold[i];
    m_pSpriteHeld[i]->setVisible((bool)m_iHold[i]);

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_HOLD.wav");
}

/**
 * 由mediator呼叫
 * 押注失敗回呼
 **/
void NGLayer::updateBetFailCallBack(GCliMsgBetFail_Vo *BetVo)
{
    CCLog("Bet Fail, ErrorCode:%i", BetVo->ErrorCode);
}

/**
 * 由mediator呼叫
 * 
 **/
void NGLayer::updateDealCardCallBack(GCliMsgDeal_Vo *dealVo)
{
    if(m_ModelData->m_PreState == GameState::STATE_OVER && 
       m_ModelData->m_State == GameState::STATE_DEAL)   //第一把
    {
        m_iCredit = dealVo->UserPoint/100;
        updateCredit(m_iCredit, m_iMultiple);

        if(m_pHintAnimation->getAnimation()->isVisible())
        {
            m_pHintAnimation->stopAction();
            m_pHintAnimation->getAnimation()->setVisible(false);
        }

        for(int i = 0; i < 5; i++)
        {
            m_pSpriteCard[i]->setTexture(m_CardTexture[getCardIndex(dealVo->PokerHand[i])]);
        }

        memcpy((void *)m_iHold, (void *)dealVo->Hold, sizeof(m_iHold));

        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_OPEN.wav");

        m_pSpriteBackCard[0]->runAction(
            CCSequence::actions(
                CCOrbitCamera::actionWithDuration(
                    reverseCardFrame, 1, 0, 0, -180, 0, 0),
                CCCallFuncND::actionWithTarget(this, 
                    callfuncND_selector(NGLayer::onShowNGCard), 
                    (void *)dealVo), 
                NULL));
    }
    else if(m_ModelData->m_PreState == GameState::STATE_READY &&
            m_ModelData->m_State == GameState::STATE_DEAL) //玩過第一局之後, 下一局前須再重蓋牌
    {
        m_iCredit = dealVo->UserPoint/100;
        updateCredit(m_iCredit, m_iMultiple);

        m_ModelData->m_PreState = m_ModelData->m_State;
        m_ModelData->m_State = GameState::STATE_DEAL;

        if(m_pHintAnimation->getAnimation()->isVisible())
        {
            m_pHintAnimation->stopAction();
            m_pHintAnimation->getAnimation()->setVisible(false);
        }

        for(int i = 0; i < 5; i++)
        {
            m_pSpriteCard[i]->setVisible(false);
            m_pSpriteBackCard[i]->setVisible(true);
            m_pSpriteCard[i]->setTexture(m_CardTexture[getCardIndex(dealVo->PokerHand[i])]);
        }

        memcpy((void *)m_iHold, (void *)dealVo->Hold, sizeof(m_iHold));

        m_pSpriteBackCard[0]->runAction(
            CCSequence::actions(
            CCCallFuncND::actionWithTarget(this, callfuncND_selector(NGLayer::onShowNGCard), (void *)dealVo), NULL));
            
        //m_pWinPayAnimation->getAnimation()->setPositionY(375+dealVo->PokerType*19);
    }
    else    //狀態錯誤
    {
    }
}

/**
 * 由mediator呼叫
 * 
 **/
void NGLayer::updateDrawCardCallBack(GCliMsgDraw_Vo *drawVo)
{
    if(m_ModelData->m_State == GameState::STATE_DEAL)
    {
        m_ModelData->m_PreState = m_ModelData->m_State;
        m_ModelData->m_State = GameState::STATE_DRAW;

        ButtonState(m_ModelData->m_State);

        //m_pWinPayAnimation->getAnimation()->setVisible(false);
        //m_pWinPayAnimation->stopAction();

        m_ModelData->m_iWinPoint = drawVo->WinLosePoint;

        for(int i = 0; i < 5; i++)
        {
            m_pSpriteCard[i]->setVisible(false);
            m_pSpriteCard[i]->setTexture(m_CardTexture[getCardIndex(drawVo->PokerHand[i])]);
            m_pSpriteHeld[i]->setVisible(false);
            m_pHoldMenu[i]->setVisible(false);
            m_iHoldCardNum += m_iHold[i];
        }

        m_pSpriteBackCard[0]->runAction(
            CCSequence::actions(
                CCCallFuncND::actionWithTarget(this, 
                    callfuncND_selector(NGLayer::onShowNGCard), 
                    (void *)drawVo), 
                NULL));
/*
        if(drawVo->WinLosePoint == 0)
        {
            m_State = STATE_OVER;
        }*/
    }
    else    //狀態錯誤
    {
    }
}

/**
 * 由mediator呼叫
 * 
 **/
//void NGLayer::updateMultipleCallBack(int iMultipleIndex)
void NGLayer::updateMultipleCallBack(int iMultiple)
{
    char szMultiple[10];
    m_iMultiple = iMultiple;
    int iMultipleTmp = pow((float)10, m_iMultiple-1);
    sprintf(szMultiple, "2%i", iMultipleTmp);

    if(m_iMultiple == 4)
    {
        m_pMultipleLabel->setScaleX((float)0.8);
    }
    else
    {
        m_pMultipleLabel->setScaleX((float)1);
    }
    m_pMultipleLabel->setString(szMultiple);
    //m_pMultipleLabel->visit();
    m_iBet = (m_ModelData->m_GameRatio[m_ModelData->m_Multiple-1])*(m_ModelData->m_BetRatio)*(m_ModelData->m_BetIndex);
    updateBet(m_iBet);
}

/**
 * 由mediator呼叫
 * 
 **/
void NGLayer::updatePayTableCallBack(int GameScore[5][11])
{
    memcpy(m_iPayTableNum[0], GameScore[0]+1, sizeof(m_iPayTableNum[0]));
    memcpy(m_iPayTableNum[1], GameScore[1]+1, sizeof(m_iPayTableNum[1]));
    memcpy(m_iPayTableNum[2], GameScore[2]+1, sizeof(m_iPayTableNum[2]));
    memcpy(m_iPayTableNum[3], GameScore[3]+1, sizeof(m_iPayTableNum[3]));
    memcpy(m_iPayTableNum[4], GameScore[4]+1, sizeof(m_iPayTableNum[4]));

    char szPayNum[10];
    int i = 0;
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            sprintf(szPayNum, "%i", m_iPayTableNum[i][j]);
            m_pPayTableLabel[i][j]->setString(szPayNum);
        }
    }
}

int NGLayer::getCardIndex(int iPokerHand)
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
 * NG牌的動作
 **/
void NGLayer::onShowNGCard(CCNode *sender,void *vo)
{
    GCliMsgDeal_Vo *dealVo;
    GCliMsgDraw_Vo *drawVo;

    if(m_ModelData->m_State == GameState::STATE_DEAL)
    {
        dealVo = (GCliMsgDeal_Vo *)vo;
    }
    else if(m_ModelData->m_State == GameState::STATE_DRAW)
    {
        drawVo = (GCliMsgDraw_Vo *)vo;
    }

    static int iCard = 1;

    int iReverse = 1;   //draw是否翻牌, 沒hold的牌才需要翻

    if(iCard >= 0 && m_ModelData->m_State == GameState::STATE_DRAW)
    {
        iReverse = (m_iHold[iCard] == 0?1:0);
    }

    switch(iCard)
    {
    case -1: //重設牌面
        {
            iCard += 1;

            for(int i = 0; i < 5; i++)
            {
                if(m_ModelData->m_State == GameState::STATE_DRAW)
                {
                    m_pSpriteBackCard[i]->setVisible(!m_iHold[i]);
                    m_pSpriteCard[i]->setVisible((bool)m_iHold[i]);
                }
                else if(m_ModelData->m_State == GameState::STATE_DEAL)
                {
                    m_pSpriteBackCard[i]->setVisible(true);
                    m_pSpriteCard[i]->setVisible(false);
                }

                if(m_iHold[i] == 0 || m_ModelData->m_State == GameState::STATE_DEAL)
                {
                    
                    if(m_ModelData->m_State == GameState::STATE_DRAW || m_ModelData->m_State == GameState::STATE_READY || m_ModelData->m_State == GameState::STATE_DEAL)
                    {
                        m_pSpriteBackCard[i]->setFlipX(true);
                    }

                    /*if(i < 4)
                    {*/
                        m_pSpriteBackCard[i]->runAction(
                            CCSequence::actions(
                                CCOrbitCamera::actionWithDuration(
                                    reverseCardFrame, 1, 0, 0, 180, 0, 0),
                                CCCallFuncND::actionWithTarget(this, 
                                    callfuncND_selector(NGLayer::onResetCardComplete), 
                                    vo),
                                NULL));
                    //}
                    /*
                    else if(i == 4)
                    {
                        m_pSpriteBackCard[i]->runAction(
                            CCSequence::actions(
                                CCOrbitCamera::actionWithDuration(
                                    reverseCardFrame, 1, 0, 0, 180, 0, 0),
                                CCCallFuncND::actionWithTarget(this, 
                                    callfuncND_selector(NGLayer::onShowNGCard), 
                                    vo), 
                                NULL));
                    }*/
                }
                else if(i == 4 && m_iHoldCardNum == 5)  //全hold住不翻牌
                {
                    m_pSpriteBackCard[i]->runAction(
                        CCSequence::actions(
                            CCCallFuncND::actionWithTarget(this, 
                                callfuncND_selector(NGLayer::onResetCardComplete), 
                                vo),
                            NULL));
                }
            }

            break;
        }

    case 0:
        {
            iCard += 1;
            m_pSpriteBackCard[iCard-1]->setFlipX(false);
            
            if(iReverse)
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_OPEN.wav");
                m_pSpriteBackCard[iCard-1]->setVisible(true);
                m_pSpriteCard[iCard-1]->setVisible(false);

                m_pSpriteBackCard[0]->runAction(
                    CCSequence::actions(
                        CCOrbitCamera::actionWithDuration(
                            reverseCardFrame, 1, 0, 0, -180, 0, 0),
                        CCCallFuncND::actionWithTarget(this, 
                            callfuncND_selector(NGLayer::onShowNGCard), 
                            vo), 
                        NULL));

                break;
            }
            else
            {
                if(m_ModelData->m_State == GameState::STATE_DRAW)
                {
                    iReverse = (m_iHold[iCard] == 0?1:0);
                }
            }
        }

    case 1: //翻第一張
        {
            m_pSpriteBackCard[iCard-1]->setVisible(false);
            m_pSpriteCard[iCard-1]->setVisible(true);

            iCard += 1;
            m_pSpriteBackCard[iCard-1]->setFlipX(false);

            if(iReverse)
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_OPEN.wav");
                m_pSpriteBackCard[iCard-1]->runAction(
                    CCSequence::actions(
                        CCOrbitCamera::actionWithDuration(
                            reverseCardFrame, 1, 0, 0, -180, 0, 0),
                        CCCallFuncND::actionWithTarget(this, 
                            callfuncND_selector(NGLayer::onShowNGCard), 
                            vo), 
                        NULL));

                break;
            }
            else
            {
                if(m_ModelData->m_State == GameState::STATE_DRAW)
                {
                    iReverse = (m_iHold[iCard] == 0?1:0);
                }
            }
        }

    case 2: //翻第二張
        {
            m_pSpriteBackCard[iCard-1]->setVisible(false);
            m_pSpriteCard[iCard-1]->setVisible(true);

            iCard += 1;
            m_pSpriteBackCard[iCard-1]->setFlipX(false);

            if(iReverse)
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_OPEN.wav");
                m_pSpriteBackCard[iCard-1]->runAction(
                    CCSequence::actions(
                        CCOrbitCamera::actionWithDuration(
                            reverseCardFrame, 1, 0, 0, -180, 0, 0),
                        CCCallFuncND::actionWithTarget(this, 
                            callfuncND_selector(NGLayer::onShowNGCard), 
                            vo), 
                        NULL));
                break;
            }
            else
            {
                if(m_ModelData->m_State == GameState::STATE_DRAW)
                {
                    iReverse = (m_iHold[iCard] == 0?1:0);
                }
            }
        }

    case 3: //翻第三張
        {
            m_pSpriteBackCard[iCard-1]->setVisible(false);
            m_pSpriteCard[iCard-1]->setVisible(true);

            iCard += 1;
            m_pSpriteBackCard[iCard-1]->setFlipX(false);

            if(iReverse)
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_OPEN.wav");
                m_pSpriteBackCard[iCard-1]->runAction(
                    CCSequence::actions(
                        CCOrbitCamera::actionWithDuration(
                            reverseCardFrame, 1, 0, 0, -180, 0, 0),
                        CCCallFuncND::actionWithTarget(this, 
                            callfuncND_selector(NGLayer::onShowNGCard), 
                            vo), 
                        NULL));

                break;
            }
            else
            {
                if(m_ModelData->m_State == GameState::STATE_DRAW)
                {
                    iReverse = (m_iHold[iCard] == 0?1:0);
                }
            }
        }

    case 4: //翻第四張
        {
            m_pSpriteBackCard[iCard-1]->setVisible(false);
            m_pSpriteCard[iCard-1]->setVisible(true);

            iCard += 1;
            m_pSpriteBackCard[iCard-1]->setFlipX(false);

            if(iReverse)
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_OPEN.wav");
                m_pSpriteBackCard[iCard-1]->runAction(
                    CCSequence::actions(
                        CCOrbitCamera::actionWithDuration(
                            reverseCardFrame, 1, 0, 0, -180, 0, 0),
                        CCCallFuncND::actionWithTarget(this, 
                            callfuncND_selector(NGLayer::onShowNGCard), 
                            vo), 
                        NULL));

                break;
            }
            else
            {
                if(m_ModelData->m_State == GameState::STATE_DRAW)
                {
                    iReverse = (m_iHold[iCard] == 0?1:0);
                }
            }
        }

    case 5: //翻第五張
        {
            m_pSpriteBackCard[iCard-1]->setVisible(false);
            m_pSpriteCard[iCard-1]->setVisible(true);

            iCard = -1;

            if(m_ModelData->m_State == GameState::STATE_DRAW)
            {
                if(drawVo->WinLosePoint == 0 /*|| (drawVo->WinLosePoint > 0 && !drawVo->CanDoubleGame)*/)
                {
#ifndef View_Test
                    m_ModelData->m_PreState = m_ModelData->m_State;
                    m_ModelData->m_State = GameState::STATE_BALANCE;
                    GSLWaitSync_Vo syncVo;
                    syncVo.nSyncSignal = SYNCSIGNAL_GAME_BALANCE;
                    m_pFacade->sendNotification(GameClientServiceEvent::SLMSG_WAIT_SYNC_Event::Name, &syncVo);
#else
                    m_ModelData->m_PreState = m_ModelData->m_State;
                    m_ModelData->m_State = GameState::STATE_READY;
                    ButtonState(m_ModelData->m_State);
#endif
                }
                else
                {
                    updateWinPoint();
                }
            }

            if(m_ModelData->m_State == GameState::STATE_DEAL)
            {
                //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_OPEN.wav");

                m_pDealMenu->setVisible(true);

                //顯示贏的賠率表
                if(dealVo->PokerType > 0)
                {
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_DEALTEE.wav");

                    m_bIsDealWin = true;
                    m_pWinPayAnimation->getAnimation()->setVisible(true);
                    m_pWinPayAnimation->getAnimation()->setPositionY(375+(dealVo->PokerType-1)*19);
                    m_pWinPayAnimation->runAction();
                }
                
                //m_pWinPayAnimation->getAnimation()->setPositionY(375+0*19);
                //m_pWinPayAnimation->runAction();

                for(int i = 0; i < 5; i++)
                {
                    m_pHoldMenu[i]->setVisible(true);
                    m_pSpriteHeld[i]->setVisible((bool)m_iHold[i]);
                }
            }
            else if(m_ModelData->m_State == GameState::STATE_DRAW)
            {
                //顯示贏的賠率表
                if(drawVo->PokerType > 0)
                {
                    m_pWinPayAnimation->getAnimation()->setVisible(true);
                    m_pWinPayAnimation->getAnimation()->setPositionY(375+(drawVo->PokerType-1)*19);
                    m_pWinPayAnimation->runAction();

                    if(drawVo->PokerType >= HAND_STRAIGHT_FLUSH)
                    {
                        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_WIN_TOPAWARD.wav");
                    }
                    else if(drawVo->PokerType >= HAND_FULL_HOUSE)
                    {
                        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_WIN_BIG.wav");
                    }
                    else
                    {
                        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_WIN_SMALL.wav");
                    }
                }
                //m_pWinPayAnimation->getAnimation()->setPositionY(375+0*19);

                for(int i = 0; i < 5; i++)
                {
                    m_pWinCardAnimation[i]->getAnimation()->setVisible(true);
                    m_pWinCardAnimation[i]->runAction();
                }

                memcpy(m_ModelData->m_PokerValue, drawVo->PokerHand, sizeof(POKERHAND));

                scheduleOnce(schedule_selector(NGLayer::scheduleOnceTimer), 3);

                m_ModelData->m_CanDoubleGame = false;
            }
        }
    }
}

/**
 * 蓋牌完成的事件
 **/
void NGLayer::onResetCardComplete(CCNode *sender,void *vo)
{
    static int iReverCardNum = 0;

    //已蓋牌的數目
    iReverCardNum++;

    if(iReverCardNum == (5-m_iHoldCardNum) || m_iHoldCardNum == 5) //所有牌蓋完
    {
        m_iHoldCardNum = 0;
        iReverCardNum = 0;

        onShowNGCard(sender, vo);
    }
}

/**
 * 更新畫面押注
 **/
void NGLayer::updateBet(int iBet)
{
    char szBet[10];

    //int iMultiple = pow((float)10, m_iMultiple-1);
    sprintf(szBet, "%i.00", iBet);

    m_pBetLabel->setString(szBet);
}

/**
 * 更新畫面credit
 **/
void NGLayer::updateCredit(int iCredit, int iMultiple)
{
    char szCredit[20];
    sprintf(szCredit, "%i.00", iCredit);
    m_pCreditLabel->setString(szCredit);
}

/**
 * 更新畫面贏分, 包含NG或DG
 **/
void NGLayer::updateWinPoint()
{
    char szWinPoint[20];
    sprintf(szWinPoint, "%i.00", m_ModelData->m_iWinPoint/100);
    m_pWinPointLabel->setString(szWinPoint);
}

/**
 * 檢查押注是否合法
 **/
bool NGLayer::CheckBet(int iBet)
{
    int iMultiple = pow((float)10, m_iMultiple-1);

    if(iBet*iMultiple <= m_iCredit)
    {
        return true;
    }

    return false;
}

void NGLayer::scheduleOnceTimer(float fTime)
{
    for(int i = 0; i < 5; i++)
    {
        m_pWinCardAnimation[i]->getAnimation()->setVisible(false);
        m_pWinCardAnimation[i]->stopAction();
    }
    m_pWinPayAnimation->getAnimation()->setVisible(false);
    m_pWinPayAnimation->stopAction();

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SND_5PK_JOKER_TRANSFOR.wav");

    m_ModelData->m_CurrentCredit = m_iCredit*100;

    changeView(GameView::VIEW_DG);
}

//TODO: 待優化
void NGLayer::changeView(int iView)
{
    m_ModelData->m_PreView = m_ModelData->m_View;
    m_ModelData->m_View = iView;
    m_ModelData->m_PreState = m_ModelData->m_State;
    m_ModelData->m_State = GameState::STATE_DGSELECT;
    ((CCLayerMultiplex*)m_pParent)->switchTo(iView);
}

/**
 * 按鈕狀態
 **/
void NGLayer::ButtonState(int iState)
{
    switch(iState)
    {
    case GameState::STATE_OVER:
    case GameState::STATE_READY:
    //case STATE_ROUNDEND:
        {
            m_pDealMenu->setVisible(true);
            m_pBetMenu->setVisible(true);
            m_pMaxBetMenu->setVisible(true);
            m_pMultipleDown->setVisible(true);
            m_pMultipleUp->setVisible(true);

            break;
        }
    case GameState::STATE_DEAL:
    case GameState::STATE_DRAW:
        {
            m_pDealMenu->setVisible(false);
            m_pBetMenu->setVisible(false);
            m_pMaxBetMenu->setVisible(false);
            m_pMultipleDown->setVisible(false);
            m_pMultipleUp->setVisible(false);

            break;
        }
    }
}
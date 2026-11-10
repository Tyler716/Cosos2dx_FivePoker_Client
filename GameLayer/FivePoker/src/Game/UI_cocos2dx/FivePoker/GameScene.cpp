#include "CommandList.h"
#include "GlobalVar.h"
#include "GameScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <Windows.h>
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

GameScene::~GameScene()
{
    CC_SAFE_RELEASE(m_pNGLayer);
    CC_SAFE_RELEASE(m_pDGLayer);
    //CC_SAFE_RELEASE(m_pLoginLayer);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(m_pFacade)
    {
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "GameScene delete facade...");
        SAFE_DELETE(m_pFacade);
    }
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
}

bool GameScene::init()
{
    if(CCScene::init())
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "GameScene init...");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#ifndef View_Test
        m_pFacade = &Facade::getInstance();
        if(m_pFacade)
        {
            if(m_pFacade->hasProxy("GameProxy"))
            {
                IProxy *gameProxy = &m_pFacade->retrieveProxy("GameProxy");

                std::map<std::string, IData *> *Game_datamap = (std::map<std::string, IData *> *)gameProxy->getData();

                m_ModelData = (FivePokerModelData *)Game_datamap->find("FivePokerModelData")->second;

                if(!m_ModelData)
                {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "GameScene get model data fail!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
                    return false;
                }
            }
            else
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "GameScene can't get GameProxy...");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
                return false;
            }
        }
        else
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "GameScene can't get facade...");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            return false;
        }
#ifdef DEBUG_CONSOLE
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        OutputDebugString(L"before create recvTimer!\r\n")
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#endif  //DEBUG_CONSOLE
        //schedule(schedule_selector(GameScene::recvTimer), 0);
        scheduleUpdate();
#endif  //View_Test
        m_pNGLayer = NGLayer::create();
        m_pNGLayer->retain();
        m_pDGLayer = DGLayer::create();
        m_pDGLayer->retain();
        
        m_pLoginLayer = LoginLayer::create();
        //m_pLoginLayer->retain();

        CCLayerMultiplex* layer = CCLayerMultiplex::layerWithLayers(m_pLoginLayer, m_pNGLayer, m_pDGLayer, NULL);
        //CCLayerMultiplex* layer = CCLayerMultiplex::layerWithLayers(m_pNGLayer, m_pDGLayer, NULL);
        addChild(layer, 0, 0); 
    }
    else
    {
        return false;
    }

    return true;
}

//由scheduleupdate觸發
void GameScene::update(float fDelta)
{
    //TODO: 加上critical section or semephore or mutex
    if(g_GameMsg.GetLength() > 0)
    {
        Message *pMsg = g_GameMsg.Remove();

        if(pMsg)
        {
            g_GameMsg.SetLock();

            switch(pMsg->m_iCmdID)
            {
                case GameDefineMsg::GAMEMSG_LOGIN_FAIL::id: //登入失敗
                {
                    m_pLoginLayer->updateLoginFailCallBack((int)(*(int *)pMsg->m_pData));
                    break;
                }
                
                case GameDefineMsg::GAMEMSG_NETWORK_OFF::id:    //網路關閉
                {
                    m_pLoginLayer->connectResultCallBack(false);
                    break;
                }

                case GameDefineMsg::GAMEMSG_CONNECT_LS_OK::id:  //連線OK
                {
                    m_pLoginLayer->connectResultCallBack(true);
                    break;
                }

                case GameCliMsg::GCLIMSG_FIRST::id:  //遊戲開始
                {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "FivePokerScene get GCLIMSG_FIRST, state:%i", m_ModelData->m_State);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#ifdef DEBUG_CONSOLE
                    OutputDebugString(L"FivePokerScene get GCLIMSG_FIRST!\r\n")
#endif  //DEBUG_CONSOLE
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

                    //if(m_ModelData->m_View == VIEW_NG)
                    {
                        if(m_ModelData->m_State == STATE_BALANCE)
                        {
#ifndef View_Test
                            //TODO:是否增加確認鈕
                            GSLGameContinue_Vo vo;
                            vo.BtnResult = 1;
                            m_ModelData->m_PreState = m_ModelData->m_State;
                            m_ModelData->m_State = STATE_ROUNDEND;
                            m_pFacade->sendNotification(GameClientServiceEvent::SLMSG_GAME_CONTINUE_Event::Name, &vo);
#endif  //View_Test
                        }
                        else if(m_ModelData->m_State == STATE_ROUNDEND)
                        {
                            //先判斷是否贏再決定要不要呼叫
                            m_ModelData->m_PreState = m_ModelData->m_State;
                            m_ModelData->m_State = STATE_READY;

                            if(m_ModelData->m_iWinPoint)
                            {
                                m_ModelData->m_iWinPoint = 0;
                                m_ModelData->m_CurrentCredit += m_ModelData->m_iWinPoint;
                                m_pDGLayer->updateCollectCallBack();
                            }
                            m_pNGLayer->updateUIdata();
                        }
                    }
                    break;
                }

                case GameCliMsg::GCLIMSG_DEAL::id:  //第一次開牌資料
                {
#ifdef DEBUG_CONSOLE
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                    OutputDebugString(L"FivePokerScene get GCLIMSG_DEAL!\r\n")
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#endif  //DEBUG_CONSOLE
                    m_pNGLayer->updateDealCardCallBack((GCliMsgDeal_Vo *)pMsg->m_pData);

                    break;
                }

                case GameCliMsg::GCLIMSG_DRAW::id:  //第二次開牌資料
                {
#ifdef DEBUG_CONSOLE
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                    OutputDebugString(L"FivePokerScene get GCLIMSG_DRAW!\r\n")
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#endif  //DEBUG_CONSOLE
                    m_pNGLayer->updateDrawCardCallBack((GCliMsgDraw_Vo *)pMsg->m_pData);
                    break;
                }

                case GameCliMsg::GCLIMSG_GAME_OVER::id:  //遊戲結束
                {
#ifdef DEBUG_CONSOLE
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                    OutputDebugString(L"FivePokerScene get GCLIMSG_GAME_OVER!\r\n")
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#endif  //DEBUG_CONSOLE
                    GSLWaitSync_Vo syncVo;
                    syncVo.nSyncSignal = SYNCSIGNAL_GAME_BALANCE;
                    m_pFacade->sendNotification(GameClientServiceEvent::SLMSG_WAIT_SYNC_Event::Name, &syncVo);
                    break;
                }

                case GameCliMsg::GCLIMSG_SET_MULTIPLE_RESULT::id:  //設定倍率的結果
                {
#ifdef DEBUG_CONSOLE
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                    OutputDebugString(L"FivePokerScene get GCLIMSG_SET_MULTIPLE_RESULT!\r\n")
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#endif  //DEBUG_CONSOLE
                    CGGCliSetMultipleResult_Vo *vo = (CGGCliSetMultipleResult_Vo *)pMsg->m_pData;
                    m_pNGLayer->updateMultipleCallBack(vo->MultipleIndex);

                    int iGameScore[5][11];
                    memcpy(iGameScore[0], vo->GameScore_1, sizeof(iGameScore[0]));
                    memcpy(iGameScore[1], vo->GameScore_2, sizeof(iGameScore[1]));
                    memcpy(iGameScore[2], vo->GameScore_3, sizeof(iGameScore[2]));
                    memcpy(iGameScore[3], vo->GameScore_4, sizeof(iGameScore[3]));
                    memcpy(iGameScore[4], vo->GameScore_5, sizeof(iGameScore[4]));
                    m_pNGLayer->updatePayTableCallBack(iGameScore);

                    break;
                }

                case GameCliMsg::GCLIMSG_BET_FAIL::id:
                {
#ifdef DEBUG_CONSOLE
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                    OutputDebugString(L"FivePokerScene get GCLIMSG_BET_FAIL!\r\n")
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#endif  //DEBUG_CONSOLE
                    GCliMsgBetFail_Vo *vo = (GCliMsgBetFail_Vo *)pMsg->m_pData;
                    m_pNGLayer->updateBetFailCallBack(vo);
                    break;
                }

                case GameCliMsg::GCLIMSG_DOUBLE_GAME_DATA::id:
                {
#ifdef DEBUG_CONSOLE
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                    OutputDebugString(L"FivePokerScene get GCLIMSG_DOUBLE_GAME_DATA!\r\n")
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#endif  //DEBUG_CONSOLE
                    GCliMsgDoubleGameData_Vo *vo = (GCliMsgDoubleGameData_Vo *)pMsg->m_pData;
                    //重新翻牌
                    m_pDGLayer->updateDGCardCallBack(vo);
                    break;
                }

                case GameCliMsg::GCLIMSG_DOUBLE_GAME_RESULT::id:
                {
#ifdef DEBUG_CONSOLE
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                    OutputDebugString(L"FivePokerScene get GCLIMSG_DOUBLE_GAME_DATA!\r\n")
#endif  //(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#endif  //DEBUG_CONSOLE
                    GCliMsgDoubleGameResult_Vo *vo = (GCliMsgDoubleGameResult_Vo *)pMsg->m_pData;
                    m_pDGLayer->updateDGResultCardCallBack(vo);
                    break;
                }
            }

            delete pMsg;

            g_GameMsg.SetUnLock();
        }
    }
}
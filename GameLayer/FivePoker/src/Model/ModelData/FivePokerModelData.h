#ifndef _FIVEPOKERMODELDATA_H_
#define _FIVEPOKERMODELDATA_H_

#include "IData.h"
#include "Type.h"
#include "../vo/GameMsg.h"
#include "../vo/GCLIMSG_Vo.h"

using BaseLayer::Common::IData;

struct FivePokerModelData : public IData
{
    FivePokerModelData()
    {
        m_GameStatus = GAME_STATUS_NONE;
        m_TableNo = 0;
        m_BettingScores = 0;
        m_Multiple = 1;
        m_MaxBet = 5;
        m_MinBet = 1;
        m_BetRatio = 1;
        m_BetIndex = 1;
        memset(m_GameRatio, 0, sizeof(m_GameRatio));
        memset(m_GameScore_1, 0, sizeof(m_GameScore_1));
        memset(m_GameScore_2, 0, sizeof(m_GameScore_2));
        memset(m_GameScore_3, 0, sizeof(m_GameScore_3));
        memset(m_GameScore_4, 0, sizeof(m_GameScore_4));
        memset(m_GameScore_5, 0, sizeof(m_GameScore_5));
        m_PointRatioL = 0;
        m_PointRatioR = 0;
        m_uTotalPoints = 0;
        m_uUserIndex = 0;

        m_CanCoutinue = false;
        m_PokerType = WinResult::HAND_ZITCH;
        m_CanDoubleGame = false;
        m_bFirstLoading = false;

        m_iWinPoint = 0;
        m_PreView = GameView::VIEW_LOGIN;
        m_View = GameView::VIEW_LOGIN;
        m_State = GameState::STATE_OVER;
        m_PreState = GameState::STATE_OVER;
        m_CurrentCredit = 0;
    }

    Int m_GameStatus;
    Int m_TableNo;              //桌號
    Int m_BettingScores;        //下注分數
    Int m_Multiple;             //下注倍率(索引值)
    Int m_MaxBet;               //最高下注分
    Int m_MinBet;               //最低下注分
    Int m_BetRatio;             //倍率
    Int m_BetIndex;             //押注索引
    Int m_GameRatio[4];         //遊戲基本注額
    Int m_GameScore_1[11];      //遊戲得分表 - 1分
    Int m_GameScore_2[11];      //遊戲得分表 - 2分
    Int m_GameScore_3[11];      //遊戲得分表 - 3分
    Int m_GameScore_4[11];      //遊戲得分表 - 4分
    Int m_GameScore_5[11];      //遊戲得分表 - 5分
    Int m_PointRatioL;          //左比率
    Int m_PointRatioR;          //右比率
    Int m_CurrentCredit;        //目前未結算畫面credit

    UInt m_uTotalPoints;          //目前剩餘的點數 1:100
    UInt m_uUserIndex;            //哪位玩家的資訊

    POKERHAND m_PokerValue;
    Int m_PokerCount;

    Int m_iWinPoint;            //贏分
    Int m_PreState, m_State, m_PreView, m_View;

    //遊戲旗標
    bool m_CanCoutinue;         //是否可繼續(結算時使用)
    Int m_PokerType;            //撲克牌的獎項資料
    bool m_CanDoubleGame;//是否可以玩雙倍遊戲
    bool m_bFirstLoading;
    Int m_iGameScene;   //遊戲幕


    //比倍專用
    Int m_UserSelet;
    POKERHAND m_CardsValue;
    bool m_IsWin;

    enum
    {
        GAME_STATUS_NONE = 0,           //無
        GAME_STATUS_BET = 1,            //下注階段
        GAME_STATUS_DEAL = 2,           //保留階段
        GAME_STATUS_DRAW = 3,           //開牌階段
        GAME_STATUS_DOUBLE_GAME = 4     //雙倍遊戲階段
    };

    enum
    {
        GAME_SCENE,             //Gme幕
        GAMEBALANCE_SCENE,      //GameBalance幕
        GROUPBALANCE_SCENE      //GroupBlance幕
    };
};

#endif //_FIVEPOKERMODELDATA_H_

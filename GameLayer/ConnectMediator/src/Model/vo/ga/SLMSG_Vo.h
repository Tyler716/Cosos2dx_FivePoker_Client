#ifndef _SLMSG_VO_H_
#define _SLMSG_VO_H_

#include "Type.h"

#define MAX_USER_NAME 17
#define MAX_NICK_NAME 17

//SLMSG_USER_INFO
struct GSLUserInfo_Vo
{
    Int u32Size;
    Int u32UserIndexArraySize;
    Int u32UserIndexArray[1];
};

//SLMSG_WAIT_SYNC
struct GSLWaitSync_Vo
{
    int nSyncSignal;                    //回應哪一種同步訊號
};

//SLMSG_GAME_CONTINUE
struct GSLGameContinue_Vo
{
	Int BtnResult;					//按鈕的結果
										//RESULT_OK = 繼續遊戲, RESULT_CANCEL = 離開遊戲, RESULT_HOLD = 暫停遊戲
};

//CLIMSG_USER_INFO
struct GCliUserInfo_Vo                    //相關函式 QueryCliUserInfoSize、FillCliUserInfo
{
    UInt uSize;                     //structure size
    UInt uUserIndex;                //哪位玩家的資訊
    Int u32State;                       //玩家狀態
    Char szUserName[MAX_USER_NAME + 1 ];    //名字、名稱、帳號
    Char szNickName[MAX_NICK_NAME + 1 ];    //暱稱
    Char PicURL[64]; //  64bytes              //< 照片連結網址
    Int64 uEntryPoints;              //原有點數
    Int64 uTotalPoints;              //目前剩餘的點數
    UInt uPlayCount;                //總場次
    UInt uFlags;                    //旗標

    enum
    {
        PLAYER_STATUS_NONE = 0,         //空座位
        PLAYER_STATUS_WAIT = 1,         //在等候區
        PLAYER_STATUS_ON_GAME = 2       //在遊戲區(包含斷線玩家)
    };
};

//CLIMSG_WAIT_SYNC
struct GCliWaitSync_Vo
{
    UInt    uSize;                  //structure size
    Int     nSyncSignal;            //等待哪一種訊號
    Int     Result;                 //等待成功與否
};


//CLIMSG_GAME_CONTINUE
struct GCliGameContinue_Vo
{
	UInt uUserIndex;				//玩家索引值, 當此值為 MEN_IN_GRP 時代表所有玩家均設定了, 否則為單一玩家
	Int BtnResult;					//按鈕的結果
										//RESULT_OK = 繼續遊戲, RESULT_CANCEL = 離開遊戲
	Int  not_enough_user_;				//char not_enough_user_;
};

//場景(幕)
struct GCliWSScene_Vo : GCliWaitSync_Vo
{
    UInt    uMenInGrp;              //該局的人數
    Int     uUserIndex;             //該玩家在局裡的索引值
};

//Game 麻將使用
struct GCliWSGameBase_Vo : GCliWSScene_Vo
{
    UInt    uBasePoints;            //基本扣點
    UInt    uGamePoints;            //遊戲點數
    UInt    uCountdownTime;         //倒數時間
    UInt    uGameCount;             //第幾局遊戲(已經玩了幾場)
};

//GameBalance   結算
struct GCliWSGameBalanceBase_Vo : GCliWSScene_Vo
{
    UInt    uCountdownTime;         //倒數時間
    UInt    uWinner;                //單場勝利者的玩家索引值,若此值為 m_uMenInGrp 則表示和局 動態入局的 這個沒有用
    Int     nCanContinue;           //0 or 1: 是否可以繼續遊戲, 可用來讓 Client 端顯示按鈕
    Int     nCanExit;               //0 or 1: 是否可以離開遊戲, 可用來讓 Client 端顯示按鈕
    UInt    uBasePoints;            //基本扣點
    UInt    uGamePoints;            //遊戲點數
    UInt    uNeedPoints;            //至少需要多少點數才能夠繼續遊戲
    Int     nGameFlags;             //旗標
};

//GroupBalance
struct GCliWSGroupBalanceBase : GCliWSScene_Vo
{
    UInt    uWinner;                //總勝利者(依據得點最多者)
};

#endif //_SLMSG_VO_H_

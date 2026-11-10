#ifndef _GAME_MSG_H_
#define _GAME_MSG_H_

#include "Common.h"

//#define WaitSync    "WaitSync"

/**
 * 送到server的MSG
 */
struct GameSLMsg
{
    struct GSLMSG_FIRST
    {
        static const std::string Name;
        static const int id   = 3001;
    };

    //玩家下注->開牌
    struct GSLMSG_BET
    {
        static const std::string Name;
        static const int id   = 3002;
    };

    //玩家保留->開牌
    struct GSLMSG_DRAW
    {
        static const std::string Name;
        static const int id   = 3003;
    };

    //玩家要玩比倍遊戲
    struct GSLMSG_PLAYER_DOUBLE_GAME
    {
        static const std::string Name;
        static const int id   = 3004;
    };

    //比倍
    struct GSLMSG_DOUBLE_GAME
    {
        static const std::string Name;
        static const int id   = 3005;
    };

    //得分
    struct GSLMSG_GET_POINT
    {
        static const std::string Name;
        static const int id   = 3006;
    };

    //設定倍率
    struct GSLMSG_SET_MULTIPLE
    {
        static const std::string Name;
        static const int id   = 3007;
    };

    //玩家離開遊戲
    struct GSLMSG_EXIT_GAME
    {
        static const std::string Name;
        static const int id   = 3008;
    };

    //玩家每局記錄,共50局
    struct GSLMSG_RECORD_INFO
    {
        static const std::string Name;
        static const int id   = 3011;
    };

    //通知 SERVER 廣播訊息
    struct GSLMSG_TALK
    {
        static const std::string Name;
        static const int id   = 3014;
    };

    //是否可以保留該桌
    struct GSLMSG_CAN_KEEP_TABLE
    {
        static const std::string Name;
        static const int id   = 3015;
    };

    //玩小遊戲
    struct GSLMSG_PLAY_MINI_GAME
    {
        static const std::string Name;
        static const int id   = 3016;
    };

    static void registerEvent(std::string Name, int id)
    {
        GameSLMsg_map.insert(std::pair<std::string, int>(Name, id));
    }
};

/**
 * Client端會收到的Msg
 */
struct GameCliMsg
{
    //遊戲開始
    struct GCLIMSG_FIRST
    {
        static const std::string Name;
        static const int id   = 6001;
    };

    //下注失敗
    struct GCLIMSG_BET_FAIL
    {
        static const std::string Name;
        static const int id   = 6002;
    };

    //第一次開牌資料
    struct GCLIMSG_DEAL
    {
        static const std::string Name;
        static const int id   = 6003;
    };

    //第二次開牌資料
    struct GCLIMSG_DRAW
    {
        static const std::string Name;
        static const int id   = 6004;
    };

    //比倍遊戲資料
    struct GCLIMSG_DOUBLE_GAME_DATA
    {
        static const std::string Name;
        static const int id   = 6005;
    };

    //比倍遊戲結果
    struct GCLIMSG_DOUBLE_GAME_RESULT
    {
        static const std::string Name;
        static const int id   = 6006;
    };

    //遊戲結束
    struct GCLIMSG_GAME_OVER
    {
        static const std::string Name;
        static const int id   = 6007;
    };

    //設定倍率的結果
    struct GCLIMSG_SET_MULTIPLE_RESULT
    {
        static const std::string Name;
        static const int id   = 6008;
    };

    //離開遊戲
    struct GCLIMSG_EXIT_GAME
    {
        static const std::string Name;
        static const int id   = 6009;
    };

    //玩家每局記錄,共50局
    struct GCLIMSG_RECORD_INFO
    {
        static const std::string Name;
        static const int id   = 6010;
    };

    //玩家中獎閃燈訊息
    struct GCLIMSG_SHINE_LIGHT
    {
        static const std::string Name;
        static const int id   = 6013;
    };

    //通知遊戲廣播訊息
    struct GCLIMSG_TALK
    {
        static const std::string Name;
        static const int id   = 6014;
    };

    //是否可以保留該桌
    struct GCLIMSG_CAN_KEEP_TABLE
    {
        static const std::string Name;
        static const int id   = 6015;
    };

    //小遊戲結果
    struct GCLIMSG_MINI_GAME_RESULT
    {
        static const std::string Name;
        static const int id   = 6016;
    };

    static void registerEvent(std::string Name, int id)
    {
        GameCliMsg_map.insert(std::pair<std::string, int>(Name, id));
    }
};

//遊戲內自訂訊息 900開頭
struct GameDefineMsg
{
    struct GAMEMSG_LOGIN_FAIL
    {
        static const std::string Name;
        static const int id = 900;
    };

    struct GAMEMSG_NETWORK_OFF
    {
        static const std::string Name;
        static const int id = 901;
    };

    struct GAMEMSG_CONNECT_LS_OK
    {
        static const std::string Name;
        static const int id = 902;
    };

    static void registerEvent(std::string Name, int id)
    {
        GameCliMsg_map.insert(std::pair<std::string, int>(Name, id));
    }
};

enum WinResult
{
    HAND_ZITCH = 0,             // 散牌
    HAND_ONE_PAIR = 1,          // 一對
    HAND_TWO_PAIR = 2,          // 兩對
    HAND_THREE_OF_A_KIND = 3,   // 三條
    HAND_STRAIGHT = 4,          // 順子
    HAND_FLUSH = 5,             // 同花
    HAND_FULL_HOUSE = 6,        // 葫蘆
    HAND_FOUR_OF_A_KIND = 7,    // 鐵支
    HAND_STRAIGHT_FLUSH = 8,    // 同花順
    HAND_FIVE_OF_A_KIND = 9,    // 五條(梅)
    HAND_ROYAL_FLUSH = 10       // 同花大順
};

enum GameState
{
    STATE_READY,
    STATE_DEAL,
    STATE_DRAW,
    STATE_WIN,
    STATE_BALANCE,
    STATE_ROUNDEND,
    STATE_OVER,
    STATE_DGSELECT,
    STATE_DGCHOOSE,
    STATE_DGWIN,
    STATE_DGOVER
};

enum GameView
{
    VIEW_LOGIN,
    VIEW_NG,
    VIEW_DG
};

#endif //_GAME_MSG_H_

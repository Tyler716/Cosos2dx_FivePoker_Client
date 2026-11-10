#ifndef _COMMANDLIST_H_
#define _COMMANDLIST_H_

#include "Common.h"
#include "ConnectionModelData.h"
#include "../Model/vo/cs/CS_LB_USER_RELOGIN_GS_R_Vo.h"
#include "../Model/vo/ls/LoginRequestVo.h"
#include <string>
/*
 * 遊戲內部使用的命令
 */
#define LoginFormSendSignal                     "LoginFormSendSignal"
#define JoinGroupSignal                         "JoinGroupSignal"
#define AskOnlineMemberCountSignal              "AskOnlineMemberCountSignal"
#define LoginServerConnect                      "LoginServerConnect"
#define CentralServerConnect                    "CentralServerConnect"
#define GameServerConnect                       "GameServerConnect"

/**
 * 同步用ID
 */
static const Int SYNCSIGNAL_GAME = 2048;//等待至遊戲幕
static const Int SYNCSIGNAL_GAME_BALANCE = 2049;//等待至遊戲結算幕
static const Int SYNCSIGNAL_GROUP_BALANCE = 2050;//等待至局結算幕
static const Int SYNCSIGNAL_GROUP_OVER = 2051;//等待該局結束
static const Int SYNCSIGNAL_SCENE_START = 2052;//該幕開始

/*
 * Login server event
 */
struct LoginServiceEvent
{
    struct LS_LB_CHK_USER_Event
    {
        static const std::string Name;
        static const int id   = 384;
    };

    static void registerEvent(std::string Name, int id)
    {
        LoginServerEvent_map.insert(std::pair<std::string, int>(Name, id));
    }
};
//const std::string LoginServiceEvent::LS_LB_CHK_USER_Event::Name = "LS_LB_CHK_USER";

/*
 * 遊戲客端服務事件
 * Game Client Event
 */

struct GameClientServiceEvent
{
    struct GS_GA_RELOGIN_USER_Event
    {
        static const std::string Name;
        static const int id   = 256;
    };

    // 玩家與 Game server 之間的 tick
    struct GS_GA_TICK_Event
    {
        static const std::string Name;
        static const int id   = 257;
    };

    // 遊戲視窗註冊載體識別碼
    struct LB_GA_REGISTER_LOADER_Event
    {
        static const std::string Name;
        static const int id   = 1001;
    };

    // 轉登入 gamer server
    struct LB_GA_RELOGIN_GS_Event
    {
        static const std::string Name;
        static const int id   = 1002;
    };

    // 轉登入 gamer server 的結果
    struct LB_GA_RELOGIN_RESULT_Event
    {
        static const std::string Name;
        static const int id   = 1003;
    };

    // 關閉遊戲視窗
    struct LB_GA_CLOSE_GAME_CLIENT_Event
    {
        static const std::string Name;
        static const int id   = 9001;
    };

    //S->C
    //該局的所有玩家均加入戰局了
    struct CLIMSG_GROUP_SETTED_Event
    {
        static const std::string Name;
        static const int id   = 2048;
    };

    //SLMSG_USER_INFO 訊息的傳回值
    struct CLIMSG_USER_INFO_Event
    {
        static const std::string Name;
        static const int id   = 2049;
    };

    //SLMSG_GAME_CONTINUE 訊息的傳回值,傳回除了自己以外
    struct CLIMSG_GAME_CONTINUE_Event
    {
        static const std::string Name;
        static const int id   = 2051;
    };

    //SLMSG_SYN 訊息的傳回值
    struct CLIMSG_WAIT_SYNC_Event
    {
        static const std::string Name;
        static const int id   = 2054;
    };

    //遊戲伺服器訊息
    struct CLIMSG_SERVER_MSG
    {
        static const std::string Name;
        static const int id   = 2058;
    };

    //玩家加入局(告知其他玩家誰加入)
    struct CLIMSG_USER_JOIN_GAME
    {
        static const std::string Name;
        static const int id   = 2062;
    };

    //玩家離開局(告知其他玩家誰離開)
    struct CLIMSG_USER_LEAVE_GAME
    {
        static const std::string Name;
        static const int id   = 2063;
    };

    //新的解散局封包,請回大廳
    struct CLIMSG_DISBAND_GAME
    {
        static const std::string Name;
        static const int id   = 2065;
    };

    //C->S
    //取得某玩家的資訊
    struct SLMSG_USER_INFO_Event
    {
        static const std::string Name;
        static const int id   = 4001;
    };

    //一局遊戲結束,回應是否繼續遊戲
    struct SLMSG_GAME_CONTINUE_Event
    {
        static const std::string Name;
        static const int id   = 4002;
    };

    struct SLMSG_WAIT_SYNC_Event
    {
        static const std::string Name;
        static const int id   = 4005;
    };

    static void registerEvent(std::string Name, int id)
    {
        GameServerEvent_map.insert(std::pair<std::string, int>(Name, id));
    }
};

/*
 * Central server event
 */
struct CentralServerEvent
{
    /******************** 玩家登入 ********************/
    // Login server relogin 到 central server 進行卡號及密碼檢查
    struct CS_LB_CHK_USER_Event
    {
        static const std::string Name;
        static const int id   = 1001;
    };

    // 取得在連線伺服器下的線上人數
    struct CS_LB_GET_MEN_IN_NET_Event
    {
        static const std::string Name;
        static const int id   = 1002;
    };

    // 取得玩家清單
    struct CS_LB_GET_USR_LIST_Event
    {
        static const std::string Name;
        static const int id   = 1003;
    };

    // 查詢某位玩家是否在線
    struct CS_LB_SEARCH_USER_Event
    {
        static const std::string Name;
        static const int id   = 1004;
    };

    // 取得遊戲清單
    struct CS_LB_GET_GAME_STATE_Event
    {
        static const std::string Name;
        static const int id   = 1005;
    };

    /******************** 遊戲操作 ********************/
    // 產生一個新局
    struct CS_LB_USER_CREATE_BUILTIN_GRP_Event
    {
        static const std::string Name;
        static const int id   = 1101;
    };

    // 產生一個可延伸容量的新局
    struct CS_LB_USER_CREATE_EXTEND_GRP_Event
    {
        static const std::string Name;
        static const int id   = 1102;
    };

    // 加入一個局
    struct CS_LB_USER_JOIN_BUILTIN_GRP_Event
    {
        static const std::string Name;
        static const int id   = 1103;
    };

    // 加入一個可延伸容量的局
    struct CS_LB_USER_JOIN_EXTEND_GRP_Event
    {
        static const std::string Name;
        static const int id   = 1104;
    };

    // 玩家取消加入局
    struct CS_LB_USER_CANCEL_JOIN_GRP_Event
    {
        static const std::string Name;
        static const int id   = 1105;
    };

    // Relogin 到 game server
    struct CS_LB_USER_RELOGIN_GS_Event
    {
        static const std::string Name;
        static const int id   = 1106;
    };

    // 玩家合法地離開局
    struct CS_LB_FINISH_GRP_OK_Event
    {
        static const std::string Name;
        static const int id   = 1107;
    };

    // 玩家不合法地離開局
    struct CS_LB_FINISH_GRP_NOT_OK_Event
    {
        static const std::string Name;
        static const int id   = 1108;
    };

    // 玩家合法地從仍在執行中的局離開
    struct CS_LB_USER_LEAVE_OK_Event
    {
        static const std::string Name;
        static const int id   = 1109;
    };

    // 玩家從遊戲中斷線
    struct CS_LB_CLIENT_DISCONNECTED_Event
    {
        static const std::string Name;
        static const int id   = 1110;
    };

    // 取得指定桌位的玩家資訊
    struct CS_LB_QUERY_TABLE_PL_INFO_Event
    {
        static const std::string Name;
        static const int id   = 1111;
    };

    // 玩家斷線重連的資訊
    struct CS_LB_USER_KEEP_TABLE_INFO_Event
    {
        static const std::string Name;
        static const int id   = 1112;
    };

    // 取得目前大廳的保留局
    struct CS_LB_GET_HOLD_TABLE_INFO_Event
    {
        static const std::string Name;
        static const int id   = 1113;
    };

    // 解散目前大廳的保留局
    struct CS_LB_SET_UNHOLD_TABLE_Event
    {
        static const std::string Name;
        static const int id   = 1114;
    };

    /******************** 公告訊息 ********************/
    // 系統公告
    struct CS_LB_POST_SYS_MSG_Event
    {
        static const std::string Name;
        static const int id   = 1201;
    };

    // 公告訊息
    struct CS_LB_BROADCAST_MSG_Event
    {
        static const std::string Name;
        static const int id   = 1202;
    };

    // 遊戲中彩金訊息
    struct CS_LB_BROADCAST_JPMSG_Event
    {
        static const std::string Name;
        static const int id   = 1203;
    };

    // 取得彩金資訊
    struct CS_LB_QUERY_JPBONUS_Event
    {
        static const std::string Name;
        static const int id   = 1204;
    };

    /******************** 通訊測聯 ********************/
    // 啟動或停止Tick
    struct CS_LB_BOOT_TICK_Event
    {
        static const std::string Name;
        static const int id   = 1301;
    };

    // 玩家與 central server 之間的 tick
    struct CS_LB_TICK_Event
    {
        static const std::string Name;
        static const int id   = 1302;
    };

    // Ping
    struct CS_LB_PING_Event
    {
        static const std::string Name;
        static const int id   = 1303;
    };

    /******************** 玩家資訊 ********************/
    // 查詢玩家資訊
    struct CS_LB_QRY_USER_PROFILE_INFO_Event
    {
        static const std::string Name;
        static const int id   = 1401;
    };

    // 設定玩家暱稱
    struct CS_LB_SET_GAME_NICKNAME_Event
    {
        static const std::string Name;
        static const int id   = 1402;
    };

    // 查詢玩家是否可以上傳相片
    struct CS_LB_QUERY_USER_UPLOAD_PIC_ALLOWED_Event
    {
        static const std::string Name;
        static const int id   = 1403;
    };

    // 更新玩家照片
    struct CS_LB_UPDATE_USER_PIC_Event
    {
        static const std::string Name;
        static const int id   = 1404;
    };


    /******************** 聊天對話 ********************/
    // 聊天室的對話
    struct CS_LB_CHAT_ROOM_TALK_Event
    {
        static const std::string Name;
        static const int id   = 1501;
    };

    // 密語的對話
    struct CS_LB_CHAT_WHISPER_Event
    {
        static const std::string Name;
        static const int id   = 1502;
    };

    /******************** 帳務交易 ********************/
    // 取得玩家交易手續費率
    struct CS_LB_QUERY_CHARGE_RATE_Event
    {
        static const std::string Name;
        static const int id   = 1601;
    };

    // 轉移玩家有價幣點數
    struct CS_LB_TRANSFER_V_POINTS_Event
    {
        static const std::string Name;
        static const int id   = 1602;
    };

    // 轉移玩家免費幣點數
    struct CS_LB_TRANSFER_F_POINTS_Event
    {
        static const std::string Name;
        static const int id   = 1603;
    };


    /******************** 頻道聊天 ********************/
    // 頻道列表
    struct CS_LB_CHANNEL_LIST_Event
    {
        static const std::string Name;
        static const int id   = 1701;
    };

    // 玩家加入頻道
    struct CS_LB_CHANNEL_USER_JOIN_Event
    {
        static const std::string Name;
        static const int id   = 1702;
    };

    // 頻道內的玩家列表
    struct CS_LB_CHANNEL_USER_LIST_Event
    {
        static const std::string Name;
        static const int id   = 1703;
    };

    // 頻道內發出訊息
    struct CS_LB_CHANNEL_USER_SEND_MESSAGE_Event
    {
        static const std::string Name;
        static const int id   = 1704;
    };

    // 玩家離開頻道
    struct CS_LB_CHANNEL_USER_LEAVE_Event
    {
        static const std::string Name;
        static const int id   = 1705;
    };

    // 全頻道發出訊
    struct CS_LB_USER_SEND_MESSAGE_ALL_CHANNELS_Event
    {
        static const std::string Name;
        static const int id   = 1706;
    };


    /******************** 社群交友 ********************/
    // 取得好友的清單
    struct CS_LB_GET_FRIEND_LIST_Event
    {
        static const std::string Name;
        static const int id   = 1801;
    };

    // 加入好友
    struct CS_LB_ADD_FRIEND_Event
    {
        static const std::string Name;
        static const int id   = 1802;
    };

    // 詢問是否允許對方加入好友
    struct CS_LB_ASK_ADD_FRIEND_Event
    {
        static const std::string Name;
        static const int id   = 1803;
    };

    // 刪除好友
    struct CS_LB_DEL_FRIEND_Event
    {
        static const std::string Name;
        static const int id   = 1804;
    };

    // 取得壞人的清單
    struct CS_LB_GET_BADGUY_LIST_Event
    {
        static const std::string Name;
        static const int id   = 1805;
    };

    // 加入壞人
    struct CS_LB_ADD_BADGUY_Event
    {
        static const std::string Name;
        static const int id   = 1806;
    };

    // 刪除壞人
    struct CS_LB_DEL_BADGUY_Event
    {
        static const std::string Name;
        static const int id   = 1807;
    };

    /******************** 即時視訊 ********************/
    // 要求者進行視訊連結
    struct CS_LB_CHAT_DEMAND_ON_VIDEO_Event
    {
        static const std::string Name;
        static const int id   = 1901;
    };

    // 接收者進行視訊連結
    struct CS_LB_CHAT_REPLY_ON_VIDEO_Event
    {
        static const std::string Name;
        static const int id   = 1902;
    };

    // 進行雙方視訊連結的連線
    struct CS_LB_CHAT_CONNECT_ON_VIDEOS_Event
    {
        static const std::string Name;
        static const int id   = 1903;
    };

    // 進行雙方視訊連結的斷線
    struct CS_LB_CHAT_DISCONNECT_ON_VIDEOS_Event
    {
        static const std::string Name;
        static const int id   = 1904;
    };


    /******************** 座位清單 ********************/
    // 詢問指定遊戲的座位清單
    struct CS_LB_ASK_GAME_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   = 2000;
    };

    // 取得 HOLDEM 遊戲類型的座位清單
    struct CS_LB_GET_HOLDEM_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   = 2001;
    };

    // 取得 BLACKJACK 遊戲類型的座位清單
    struct CS_LB_GET_BLACKJACK_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   = 2002;
    };

    // 取得 BACARRA 遊戲類型的座位清單
    struct CS_LB_GET_BACARRA_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   = 2003;
    };

    // 取得 SEVEN_POKER 遊戲類型的座位清單
    struct CS_LB_GET_SEVEN_POKER_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   = 2004;
    };

    // 取得 BEAST_BAR 遊戲類型的座位清單
    struct CS_LB_GET_BEAST_BAR_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   =  2005;
    };

    // 取得 SICBO 遊戲類型的座位清單
    struct CS_LB_GET_SICBO_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   = 2006;
    };

    // 取得 BINGO_PLANET 遊戲類型的座位清單
    struct CS_LB_GET_BINGO_PLANET_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   = 2007;
    };

    // 取得 THIRTEEN 遊戲類型的座位清單
    struct CS_LB_GET_THIRTEEN_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   = 2008;
    };

    // 取得 TW4MJ16 遊戲類型的座位清單
    struct CS_LB_GET_TW4MJ16_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   = 2009;
    };

    // 取得 TW4MJ16 遊戲類型的座位清單
    struct CS_LB_GET_FRUIT_BAR_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   = 2010;
    };

    // 取得SMARIO遊戲類型的各座位清單
    struct CS_LB_GET_SMARIO_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   = 2011;
    };

    // 取得FIVE_POKER遊戲類型的各座位清單
    struct CS_LB_GET_FIVE_POKER_GRP_LIST_Event
    {
        static const std::string Name;
        static const int id   = 2012;
    };

    /******************** 其他雜項 ********************/
    // 取得BACARRA的路資訊
    struct CS_LB_BACARRA_ROAD_INFO_Event
    {
        static const std::string Name;
        static const int id   = 9001;
    };

    // 取得骰寶的歷史記錄
    struct CS_LB_SICBO_ROAD_INFO_Event
    {
        static const std::string Name;
        static const int id   = 9002;
    };

    static void registerEvent(std::string Name, int id)
    {
        CentralServerEvent_map.insert(std::pair<std::string, int>(Name, id));
    }
};

#endif //_COMMANDLIST_H_

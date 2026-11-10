#include "CommandList.h"

/*
 * Login server event
 */
const std::string LoginServiceEvent::LS_LB_CHK_USER_Event::Name = "LS_LB_CHK_USER";
const std::string GameClientServiceEvent::GS_GA_RELOGIN_USER_Event::Name = "GS_GA_RELOGIN_USER";
const std::string GameClientServiceEvent::GS_GA_TICK_Event::Name = "GS_GA_TICK";
const std::string GameClientServiceEvent::LB_GA_REGISTER_LOADER_Event::Name = "LB_GA_REGISTER_LOADER";
const std::string GameClientServiceEvent::LB_GA_RELOGIN_GS_Event::Name = "LB_GA_RELOGIN_GS";
const std::string GameClientServiceEvent::LB_GA_RELOGIN_RESULT_Event::Name = "LB_GA_RELOGIN_RESULT";
const std::string GameClientServiceEvent::LB_GA_CLOSE_GAME_CLIENT_Event::Name = "LB_GA_CLOSE_GAME_CLIENT";
const std::string GameClientServiceEvent::CLIMSG_GROUP_SETTED_Event::Name = "CLIMSG_GROUP_SETTED";
const std::string GameClientServiceEvent::CLIMSG_USER_INFO_Event::Name = "CLIMSG_USER_INFO";
const std::string GameClientServiceEvent::CLIMSG_GAME_CONTINUE_Event::Name = "CLIMSG_GAME_CONTINUE";
const std::string GameClientServiceEvent::CLIMSG_WAIT_SYNC_Event::Name = "CLIMSG_WAIT_SYNC";
const std::string GameClientServiceEvent::CLIMSG_SERVER_MSG::Name = "CLIMSG_SERVER_MSG";
const std::string GameClientServiceEvent::CLIMSG_USER_JOIN_GAME::Name = "CLIMSG_USER_JOIN_GAME";
const std::string GameClientServiceEvent::CLIMSG_USER_LEAVE_GAME::Name = "CLIMSG_USER_LEAVE_GAME";
const std::string GameClientServiceEvent::CLIMSG_DISBAND_GAME::Name = "CLIMSG_DISBAND_GAME";
const std::string GameClientServiceEvent::SLMSG_USER_INFO_Event::Name = "SLMSG_USER_INFO";
const std::string GameClientServiceEvent::SLMSG_GAME_CONTINUE_Event::Name = "SLMSG_GAME_CONTINUE";
const std::string GameClientServiceEvent::SLMSG_WAIT_SYNC_Event::Name = "SLMSG_WAIT_SYNC";

/******************** 玩家登入 ********************/
const std::string CentralServerEvent::CS_LB_CHK_USER_Event::Name = "CS_LB_CHK_USER";
const std::string CentralServerEvent::CS_LB_GET_MEN_IN_NET_Event::Name = "CS_LB_GET_MEN_IN_NET";
const std::string CentralServerEvent::CS_LB_GET_USR_LIST_Event::Name = "CS_LB_GET_USR_LIST";
const std::string CentralServerEvent::CS_LB_SEARCH_USER_Event::Name = "CS_LB_SEARCH_USER";
const std::string CentralServerEvent::CS_LB_GET_GAME_STATE_Event::Name = "CS_LB_GET_GAME_STATE";

/******************** 遊戲操作 ********************/
const std::string CentralServerEvent::CS_LB_USER_CREATE_BUILTIN_GRP_Event::Name = "CS_LB_USER_CREATE_BUILTIN_GRP";
const std::string CentralServerEvent::CS_LB_USER_CREATE_EXTEND_GRP_Event::Name = "CS_LB_USER_CREATE_EXTEND_GRP";
const std::string CentralServerEvent::CS_LB_USER_JOIN_BUILTIN_GRP_Event::Name = "CS_LB_USER_JOIN_BUILTIN_GRP";
const std::string CentralServerEvent::CS_LB_USER_JOIN_EXTEND_GRP_Event::Name = "CS_LB_USER_JOIN_EXTEND_GRP";
const std::string CentralServerEvent::CS_LB_USER_CANCEL_JOIN_GRP_Event::Name = "CS_LB_USER_CANCEL_JOIN_GRP";
const std::string CentralServerEvent::CS_LB_USER_RELOGIN_GS_Event::Name = "CS_LB_USER_RELOGIN_GS";
const std::string CentralServerEvent::CS_LB_FINISH_GRP_OK_Event::Name = "CS_LB_FINISH_GRP_OK";
const std::string CentralServerEvent::CS_LB_FINISH_GRP_NOT_OK_Event::Name = "CS_LB_FINISH_GRP_NOT_OK";
const std::string CentralServerEvent::CS_LB_USER_LEAVE_OK_Event::Name = "CS_LB_USER_LEAVE_OK";
const std::string CentralServerEvent::CS_LB_CLIENT_DISCONNECTED_Event::Name = "CS_LB_CLIENT_DISCONNECTED";
const std::string CentralServerEvent::CS_LB_QUERY_TABLE_PL_INFO_Event::Name = "CS_LB_QUERY_TABLE_PL_INFO";
const std::string CentralServerEvent::CS_LB_USER_KEEP_TABLE_INFO_Event::Name = "CS_LB_USER_KEEP_TABLE_INFO";
const std::string CentralServerEvent::CS_LB_GET_HOLD_TABLE_INFO_Event::Name = "CS_LB_GET_HOLD_TABLE_INFO";
const std::string CentralServerEvent::CS_LB_SET_UNHOLD_TABLE_Event::Name = "CS_LB_SET_UNHOLD_TABLE";
/******************** 公告訊息 ********************/
const std::string CentralServerEvent::CS_LB_POST_SYS_MSG_Event::Name = "CS_LB_POST_SYS_MSG";
const std::string CentralServerEvent::CS_LB_BROADCAST_MSG_Event::Name = "CS_LB_BROADCAST_MSG";
const std::string CentralServerEvent::CS_LB_BROADCAST_JPMSG_Event::Name = "CS_LB_BROADCAST_JPMSG";
const std::string CentralServerEvent::CS_LB_QUERY_JPBONUS_Event::Name = "CS_LB_QUERY_JPBONUS";
/******************** 通訊測聯 ********************/
const std::string CentralServerEvent::CS_LB_BOOT_TICK_Event::Name = "CS_LB_BOOT_TICK";
const std::string CentralServerEvent::CS_LB_TICK_Event::Name = "CS_LB_TICK";
const std::string CentralServerEvent::CS_LB_PING_Event::Name = "CS_LB_PING";
/******************** 玩家資訊 ********************/
const std::string CentralServerEvent::CS_LB_QRY_USER_PROFILE_INFO_Event::Name = "CS_LB_QRY_USER_PROFILE_INFO";
const std::string CentralServerEvent::CS_LB_SET_GAME_NICKNAME_Event::Name = "CS_LB_SET_GAME_NICKNAME";
const std::string CentralServerEvent::CS_LB_QUERY_USER_UPLOAD_PIC_ALLOWED_Event::Name = "CS_LB_QUERY_USER_UPLOAD_PIC_ALLOWED";
const std::string CentralServerEvent::CS_LB_UPDATE_USER_PIC_Event::Name = "CS_LB_UPDATE_USER_PIC";
/******************** 聊天對話 ********************/
const std::string CentralServerEvent::CS_LB_CHAT_ROOM_TALK_Event::Name = "CS_LB_CHAT_ROOM_TALK";
const std::string CentralServerEvent::CS_LB_CHAT_WHISPER_Event::Name = "CS_LB_CHAT_WHISPER";
/******************** 帳務交易 ********************/
const std::string CentralServerEvent::CS_LB_QUERY_CHARGE_RATE_Event::Name = "CS_LB_QUERY_CHARGE_RATE";
const std::string CentralServerEvent::CS_LB_TRANSFER_V_POINTS_Event::Name = "CS_LB_TRANSFER_V_POINTS";
const std::string CentralServerEvent::CS_LB_TRANSFER_F_POINTS_Event::Name = "CS_LB_TRANSFER_F_POINTS";
/******************** 頻道聊天 ********************/
const std::string CentralServerEvent::CS_LB_CHANNEL_LIST_Event::Name = "CS_LB_CHANNEL_LIST";
const std::string CentralServerEvent::CS_LB_CHANNEL_USER_JOIN_Event::Name = "CS_LB_CHANNEL_USER_JOIN";
const std::string CentralServerEvent::CS_LB_CHANNEL_USER_LIST_Event::Name = "CS_LB_CHANNEL_USER_LIST";
const std::string CentralServerEvent::CS_LB_CHANNEL_USER_SEND_MESSAGE_Event::Name = "S_LB_CHANNEL_USER_SEND_MESSAGE";
const std::string CentralServerEvent::CS_LB_CHANNEL_USER_LEAVE_Event::Name = "CS_LB_CHANNEL_USER_LEAVE";
const std::string CentralServerEvent::CS_LB_USER_SEND_MESSAGE_ALL_CHANNELS_Event::Name = "CS_LB_USER_SEND_MESSAGE_ALL_CHANNELS";
/******************** 社群交友 ********************/
const std::string CentralServerEvent::CS_LB_GET_FRIEND_LIST_Event::Name = "CS_LB_GET_FRIEND_LIST";
const std::string CentralServerEvent::CS_LB_ADD_FRIEND_Event::Name = "CS_LB_ADD_FRIEND";
const std::string CentralServerEvent::CS_LB_ASK_ADD_FRIEND_Event::Name = "CS_LB_ASK_ADD_FRIEND";
const std::string CentralServerEvent::CS_LB_DEL_FRIEND_Event::Name = "CS_LB_DEL_FRIEND";
const std::string CentralServerEvent::CS_LB_GET_BADGUY_LIST_Event::Name = "CS_LB_GET_BADGUY_LIST";
const std::string CentralServerEvent::CS_LB_ADD_BADGUY_Event::Name = "CS_LB_ADD_BADGUY";
const std::string CentralServerEvent::CS_LB_DEL_BADGUY_Event::Name = "CS_LB_DEL_BADGUY";
/******************** 即時視訊 ********************/
const std::string CentralServerEvent::CS_LB_CHAT_DEMAND_ON_VIDEO_Event::Name = "CS_LB_CHAT_DEMAND_ON_VIDEO";
const std::string CentralServerEvent::CS_LB_CHAT_REPLY_ON_VIDEO_Event::Name = "CS_LB_CHAT_REPLY_ON_VIDEO";
const std::string CentralServerEvent::CS_LB_CHAT_CONNECT_ON_VIDEOS_Event::Name = "CS_LB_CHAT_CONNECT_ON_VIDEOS";
const std::string CentralServerEvent::CS_LB_CHAT_DISCONNECT_ON_VIDEOS_Event::Name = "CS_LB_CHAT_DISCONNECT_ON_VIDEOS";
/******************** 座位清單 ********************/
const std::string CentralServerEvent::CS_LB_ASK_GAME_GRP_LIST_Event::Name = "CS_LB_ASK_GAME_GRP_LIST";
const std::string CentralServerEvent::CS_LB_GET_HOLDEM_GRP_LIST_Event::Name = "CS_LB_GET_HOLDEM_GRP_LIST";
const std::string CentralServerEvent::CS_LB_GET_BLACKJACK_GRP_LIST_Event::Name = "CS_LB_GET_BLACKJACK_GRP_LIST";
const std::string CentralServerEvent::CS_LB_GET_BACARRA_GRP_LIST_Event::Name = "CS_LB_GET_BACARRA_GRP_LIST";
const std::string CentralServerEvent::CS_LB_GET_SEVEN_POKER_GRP_LIST_Event::Name = "CS_LB_GET_SEVEN_POKER_GRP_LIST";
const std::string CentralServerEvent::CS_LB_GET_BEAST_BAR_GRP_LIST_Event::Name = "CS_LB_GET_BEAST_BAR_GRP_LIST";
const std::string CentralServerEvent::CS_LB_GET_SICBO_GRP_LIST_Event::Name = "CS_LB_GET_SICBO_GRP_LIST";
const std::string CentralServerEvent::CS_LB_GET_BINGO_PLANET_GRP_LIST_Event::Name = "CS_LB_GET_BINGO_PLANET_GRP_LIST";
const std::string CentralServerEvent::CS_LB_GET_THIRTEEN_GRP_LIST_Event::Name = "CS_LB_GET_THIRTEEN_GRP_LIST";
const std::string CentralServerEvent::CS_LB_GET_TW4MJ16_GRP_LIST_Event::Name = "CS_LB_GET_TW4MJ16_GRP_LIST";
const std::string CentralServerEvent::CS_LB_GET_FRUIT_BAR_GRP_LIST_Event::Name = "CS_LB_GET_FRUIT_BAR_GRP_LIST";
const std::string CentralServerEvent::CS_LB_GET_SMARIO_GRP_LIST_Event::Name = "S_LB_GET_SMARIO_GRP_LIST";
const std::string CentralServerEvent::CS_LB_GET_FIVE_POKER_GRP_LIST_Event::Name = "CS_LB_GET_FIVE_POKER_GRP_LIST";
/******************** 其他雜項 ********************/
const std::string CentralServerEvent::CS_LB_BACARRA_ROAD_INFO_Event::Name = "CS_LB_BACARRA_ROAD_INFO";
const std::string CentralServerEvent::CS_LB_SICBO_ROAD_INFO_Event::Name = "CS_LB_SICBO_ROAD_INFO";
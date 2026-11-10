#ifndef _LOBBYSTARTUPCOMMAND_HPP_
#define _LOBBYSTARTUPCOMMAND_HPP_

#include "../Controller/Command/cs/AcceptPlayerLoginCommand.hpp"
#include "../Controller/Command/cs/ConnectToCentralServerCommand.hpp"
#include "../Controller/Command/cs/CentralServerTickCommand.hpp"
#include "../Controller/Command/ga/ConnectToGameServerCommand.hpp"

#include "../Controller/Command/cs/AcceptGameGroupListCommand.hpp"
#include "../Controller/Command/cs/AcceptGameStateCommand.hpp"
#include "../Controller/Command/cs/AcceptKeepTableInfoCommand.hpp"
#include "../Controller/Command/cs/AcceptOnlineMemberCountCommand.hpp"
#include "../Controller/Command/cs/AcceptUserListCommand.hpp"
#include "../Controller/Command/cs/AddBadGuyCommand.hpp"
#include "../Controller/Command/cs/AddFriendCommand.hpp"
#include "../Controller/Command/cs/AddLoaderToStageCommand.hpp"
#include "../Controller/Command/cs/AllChannelMessageReceivedCommand.hpp"
#include "../Controller/Command/cs/AskGameGroupListCommand.hpp"
#include "../Controller/Command/cs/AskOnlineMemberCountCommand.hpp"
#include "../Controller/Command/cs/AskUserListCommand.hpp"
#include "../Controller/Command/cs/BadGuyAddedCommand.hpp"
#include "../Controller/Command/cs/BadGuyDeletedCommand.hpp"
#include "../Controller/Command/cs/BadGuyListReceivedCommand.hpp"
#include "../Controller/Command/cs/BroadcastJPMessageArrivedCommand.hpp"
#include "../Controller/Command/cs/BroadcastMessageArrivedCommand.hpp"
#include "../Controller/Command/cs/CancelJoinedGroupCommand.hpp"
#include "../Controller/Command/cs/CentralServerTickEnabledCommand.hpp"
#include "../Controller/Command/cs/ChannelListArrivedCommand.hpp"
#include "../Controller/Command/cs/ChannelMessageReceivedCommand.hpp"
#include "../Controller/Command/cs/ChannelUserLeaveCommand.hpp"
#include "../Controller/Command/cs/ChannelUserListArrivedCommand.hpp"
#include "../Controller/Command/cs/DeleteBadGuyCommand.hpp"
#include "../Controller/Command/cs/DeleteFriendCommand.hpp"
#include "../Controller/Command/cs/DoGameLoadErrorCommand.hpp"
#include "../Controller/Command/cs/FriendAddedCommand.hpp"
#include "../Controller/Command/cs/FriendDeletedCommand.hpp"
#include "../Controller/Command/cs/FriendListReceivedCommand.hpp"
#include "../Controller/Command/cs/FriendshipAskCommand.hpp"
#include "../Controller/Command/cs/FriendshipAskResultCommand.hpp"
#include "../Controller/Command/cs/GameDisconnectedCommand.hpp"
#include "../Controller/Command/cs/GameGroupFinishedCommand.hpp"
#include "../Controller/Command/cs/GetBadGuyListCommand.hpp"
#include "../Controller/Command/cs/GetFriendListCommand.hpp"
#include "../Controller/Command/cs/JoinChannelCommand.hpp"
#include "../Controller/Command/cs/JoinChannelResultCommand.hpp"
#include "../Controller/Command/cs/JoinedGroupCancelledCommand.hpp"
#include "../Controller/Command/cs/JoinGroupCommand.hpp"
#include "../Controller/Command/cs/JoinGroupResultCommand.hpp"
#include "../Controller/Command/cs/JPBonusReceivedCommand.hpp"
#include "../Controller/Command/cs/LoadGameCommand.hpp"
#include "../Controller/Command/cs/LoginToCentralServerCommand.hpp"
#include "../Controller/Command/cs/PingQualityCommand.hpp"
#include "../Controller/Command/cs/QueryChargeRateCommand.hpp"
#include "../Controller/Command/cs/QueryChargeRateResultCommand.hpp"
#include "../Controller/Command/cs/QueryJPBonusCommand.hpp"
#include "../Controller/Command/cs/ReceivePingCommand.hpp"
#include "../Controller/Command/cs/SendAllChannelMessageCommand.hpp"
#include "../Controller/Command/cs/SendChannelMessageCommand.hpp"
#include "../Controller/Command/cs/SendPingCommand.hpp"
#include "../Controller/Command/cs/SendTalkCommand.hpp"
#include "../Controller/Command/cs/SendWhisperCommand.hpp"
#include "../Controller/Command/cs/SerachUserCommand.hpp"
#include "../Controller/Command/cs/SerachUserResultCommand.hpp"
#include "../Controller/Command/cs/SetUserNickNameCommand.hpp"
#include "../Controller/Command/cs/SetUserNickNameResultCommand.hpp"
#include "../Controller/Command/cs/SetUserPictureCommand.hpp"
#include "../Controller/Command/cs/SetUserPictureResultCommand.hpp"
#include "../Controller/Command/cs/SystemMessageArrivedCommand.hpp"
#include "../Controller/Command/cs/TalkReceivedCommand.hpp"
#include "../Controller/Command/cs/TransferFreePointCommand.hpp"
#include "../Controller/Command/cs/TransferFreePointResultCommand.hpp"
#include "../Controller/Command/cs/TransferValuablePointCommand.hpp"
#include "../Controller/Command/cs/TransferValuablePointResultCommand.hpp"
#include "../Controller/Command/cs/UserProfileReceivedCommand.hpp"
#include "../Controller/Command/cs/WhisperReceivedCommand.hpp"
#include "../Controller/Command/ga/AcceptGameClientClosedCommand.hpp"
#include "../Controller/Command/ga/AcceptGameServerLoginResultCommand.hpp"
#include "../Controller/Command/ga/LoginToGameServerCommand.hpp"
#include "../Controller/Command/ga/GameServerTickCommand.hpp"
#include "../Controller/Command/ga/RegisterLoaderCommand.hpp"
#include "../Controller/Command/ls/LoginToLoginServerCommand.hpp"
#include "../Controller/Command/ls/ConnectToLoginServerCommand.hpp"
#include "../../Common/CommandList.h"

namespace GameLayer
{
    namespace Lobby
    {
        using BaseLayer::Patterns::Command;
        using BaseLayer::Patterns::Facade;
        using BaseLayer::Interfaces::INotification;
        using BaseLayer::Interfaces::IFacade;
        using BaseLayer::Interfaces::ICommand;

        using namespace std;

        class LobbyStartUpCommand : public Command
        {
        public:
            LobbyStartUpCommand()
                : Command()
            {

            }

            virtual void execute(INotification const &note)
            {
                //note.getBody();

                bool bFirstInit = Facade::hasCore();

                IFacade &facade = Facade::getInstance();

                //TODO:new的command須刪除
                if(bFirstInit)
                {
                    std::cout<<"get LobbyStartUpCommand"<<std::endl;

                    /******************註冊event**********************/
                    //LoginServerEvent
                    LoginServiceEvent::registerEvent(
                        LoginServiceEvent::LS_LB_CHK_USER_Event::Name,
                        LoginServiceEvent::LS_LB_CHK_USER_Event::id);

                    //GameClient
                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::GS_GA_RELOGIN_USER_Event::Name,
                        GameClientServiceEvent::GS_GA_RELOGIN_USER_Event::id);

                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::GS_GA_TICK_Event::Name,
                        GameClientServiceEvent::GS_GA_TICK_Event::id);

                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::LB_GA_CLOSE_GAME_CLIENT_Event::Name,
                        GameClientServiceEvent::LB_GA_CLOSE_GAME_CLIENT_Event::id);

                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::LB_GA_REGISTER_LOADER_Event::Name,
                        GameClientServiceEvent::LB_GA_REGISTER_LOADER_Event::id);
/*
                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::LB_GA_RELOGIN_GS_Event::Name,
                        GameClientServiceEvent::LB_GA_RELOGIN_GS_Event::id);
*/
                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::LB_GA_RELOGIN_RESULT_Event::Name,
                        GameClientServiceEvent::LB_GA_RELOGIN_RESULT_Event::id);

                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::CLIMSG_GROUP_SETTED_Event::Name,
                        GameClientServiceEvent::CLIMSG_GROUP_SETTED_Event::id);

                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::CLIMSG_USER_INFO_Event::Name,
                        GameClientServiceEvent::CLIMSG_USER_INFO_Event::id);

                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::CLIMSG_GAME_CONTINUE_Event::Name,
                        GameClientServiceEvent::CLIMSG_GAME_CONTINUE_Event::id);

                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::CLIMSG_WAIT_SYNC_Event::Name,
                        GameClientServiceEvent::CLIMSG_WAIT_SYNC_Event::id);

                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::CLIMSG_SERVER_MSG::Name,
                        GameClientServiceEvent::CLIMSG_SERVER_MSG::id);

                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::CLIMSG_USER_JOIN_GAME::Name,
                        GameClientServiceEvent::CLIMSG_USER_JOIN_GAME::id);

                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::CLIMSG_USER_LEAVE_GAME::Name,
                        GameClientServiceEvent::CLIMSG_USER_LEAVE_GAME::id);

                    GameClientServiceEvent::registerEvent(
                        GameClientServiceEvent::CLIMSG_DISBAND_GAME::Name,
                        GameClientServiceEvent::CLIMSG_DISBAND_GAME::id);

                    //Central server
                    /******************** 玩家登入 ********************/
                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CHK_USER_Event::Name,
                        CentralServerEvent::CS_LB_CHK_USER_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_GAME_STATE_Event::Name,
                        CentralServerEvent::CS_LB_GET_GAME_STATE_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_MEN_IN_NET_Event::Name,
                        CentralServerEvent::CS_LB_GET_MEN_IN_NET_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_USR_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_USR_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_SEARCH_USER_Event::Name,
                        CentralServerEvent::CS_LB_SEARCH_USER_Event::id);

                    /******************** 遊戲操作 ********************/
                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_USER_CREATE_BUILTIN_GRP_Event::Name,
                        CentralServerEvent::CS_LB_USER_CREATE_BUILTIN_GRP_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_USER_CREATE_EXTEND_GRP_Event::Name,
                        CentralServerEvent::CS_LB_USER_CREATE_EXTEND_GRP_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_USER_JOIN_BUILTIN_GRP_Event::Name,
                        CentralServerEvent::CS_LB_USER_JOIN_BUILTIN_GRP_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_USER_JOIN_EXTEND_GRP_Event::Name,
                        CentralServerEvent::CS_LB_USER_JOIN_EXTEND_GRP_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_USER_CANCEL_JOIN_GRP_Event::Name,
                        CentralServerEvent::CS_LB_USER_CANCEL_JOIN_GRP_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_USER_RELOGIN_GS_Event::Name,
                        CentralServerEvent::CS_LB_USER_RELOGIN_GS_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_FINISH_GRP_OK_Event::Name,
                        CentralServerEvent::CS_LB_FINISH_GRP_OK_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_FINISH_GRP_NOT_OK_Event::Name,
                        CentralServerEvent::CS_LB_FINISH_GRP_NOT_OK_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_USER_LEAVE_OK_Event::Name,
                        CentralServerEvent::CS_LB_USER_LEAVE_OK_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CLIENT_DISCONNECTED_Event::Name,
                        CentralServerEvent::CS_LB_CLIENT_DISCONNECTED_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_QUERY_TABLE_PL_INFO_Event::Name,
                        CentralServerEvent::CS_LB_QUERY_TABLE_PL_INFO_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_USER_KEEP_TABLE_INFO_Event::Name,
                        CentralServerEvent::CS_LB_USER_KEEP_TABLE_INFO_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_HOLD_TABLE_INFO_Event::Name,
                        CentralServerEvent::CS_LB_GET_HOLD_TABLE_INFO_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_SET_UNHOLD_TABLE_Event::Name,
                        CentralServerEvent::CS_LB_SET_UNHOLD_TABLE_Event::id);

                    /******************** 公告訊息 ********************/
                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_POST_SYS_MSG_Event::Name,
                        CentralServerEvent::CS_LB_POST_SYS_MSG_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_BROADCAST_MSG_Event::Name,
                        CentralServerEvent::CS_LB_BROADCAST_MSG_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_BROADCAST_JPMSG_Event::Name,
                        CentralServerEvent::CS_LB_BROADCAST_JPMSG_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_QUERY_JPBONUS_Event::Name,
                        CentralServerEvent::CS_LB_QUERY_JPBONUS_Event::id);

                    /******************** 通訊測聯 ********************/
                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_BOOT_TICK_Event::Name,
                        CentralServerEvent::CS_LB_BOOT_TICK_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_TICK_Event::Name,
                        CentralServerEvent::CS_LB_TICK_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_PING_Event::Name,
                        CentralServerEvent::CS_LB_PING_Event::id);

                    /******************** 玩家資訊 ********************/
                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_QRY_USER_PROFILE_INFO_Event::Name,
                        CentralServerEvent::CS_LB_QRY_USER_PROFILE_INFO_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_SET_GAME_NICKNAME_Event::Name,
                        CentralServerEvent::CS_LB_SET_GAME_NICKNAME_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_QUERY_USER_UPLOAD_PIC_ALLOWED_Event::Name,
                        CentralServerEvent::CS_LB_QUERY_USER_UPLOAD_PIC_ALLOWED_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_UPDATE_USER_PIC_Event::Name,
                        CentralServerEvent::CS_LB_UPDATE_USER_PIC_Event::id);

                    /******************** 聊天對話 ********************/
                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CHAT_ROOM_TALK_Event::Name,
                        CentralServerEvent::CS_LB_CHAT_ROOM_TALK_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CHAT_WHISPER_Event::Name,
                        CentralServerEvent::CS_LB_CHAT_WHISPER_Event::id);

                    /******************** 帳務交易 ********************/
                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_QUERY_CHARGE_RATE_Event::Name,
                        CentralServerEvent::CS_LB_QUERY_CHARGE_RATE_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_TRANSFER_V_POINTS_Event::Name,
                        CentralServerEvent::CS_LB_TRANSFER_V_POINTS_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_TRANSFER_F_POINTS_Event::Name,
                        CentralServerEvent::CS_LB_TRANSFER_F_POINTS_Event::id);

                    /******************** 頻道聊天 ********************/
                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CHANNEL_LIST_Event::Name,
                        CentralServerEvent::CS_LB_CHANNEL_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CHANNEL_USER_JOIN_Event::Name,
                        CentralServerEvent::CS_LB_CHANNEL_USER_JOIN_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CHANNEL_USER_LIST_Event::Name,
                        CentralServerEvent::CS_LB_CHANNEL_USER_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CHANNEL_USER_SEND_MESSAGE_Event::Name,
                        CentralServerEvent::CS_LB_CHANNEL_USER_SEND_MESSAGE_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CHANNEL_USER_LEAVE_Event::Name,
                        CentralServerEvent::CS_LB_CHANNEL_USER_LEAVE_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_USER_SEND_MESSAGE_ALL_CHANNELS_Event::Name,
                        CentralServerEvent::CS_LB_USER_SEND_MESSAGE_ALL_CHANNELS_Event::id);

                    /******************** 社群交友 ********************/
                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_FRIEND_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_FRIEND_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_ADD_FRIEND_Event::Name,
                        CentralServerEvent::CS_LB_ADD_FRIEND_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_ASK_ADD_FRIEND_Event::Name,
                        CentralServerEvent::CS_LB_ASK_ADD_FRIEND_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_DEL_FRIEND_Event::Name,
                        CentralServerEvent::CS_LB_DEL_FRIEND_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_BADGUY_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_BADGUY_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_ADD_BADGUY_Event::Name,
                        CentralServerEvent::CS_LB_ADD_BADGUY_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_DEL_BADGUY_Event::Name,
                        CentralServerEvent::CS_LB_DEL_BADGUY_Event::id);

                    /******************** 即時視訊 ********************/
                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CHAT_DEMAND_ON_VIDEO_Event::Name,
                        CentralServerEvent::CS_LB_CHAT_DEMAND_ON_VIDEO_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CHAT_REPLY_ON_VIDEO_Event::Name,
                        CentralServerEvent::CS_LB_CHAT_REPLY_ON_VIDEO_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CHAT_CONNECT_ON_VIDEOS_Event::Name,
                        CentralServerEvent::CS_LB_CHAT_CONNECT_ON_VIDEOS_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_CHAT_DISCONNECT_ON_VIDEOS_Event::Name,
                        CentralServerEvent::CS_LB_CHAT_DISCONNECT_ON_VIDEOS_Event::id);

                    /******************** 座位清單 ********************/
                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_ASK_GAME_GRP_LIST_Event::Name,
                        CentralServerEvent::CS_LB_ASK_GAME_GRP_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_HOLDEM_GRP_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_HOLDEM_GRP_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_BLACKJACK_GRP_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_BLACKJACK_GRP_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_BACARRA_GRP_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_BACARRA_GRP_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_SEVEN_POKER_GRP_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_SEVEN_POKER_GRP_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_BEAST_BAR_GRP_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_BEAST_BAR_GRP_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_SICBO_GRP_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_SICBO_GRP_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_BINGO_PLANET_GRP_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_BINGO_PLANET_GRP_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_THIRTEEN_GRP_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_THIRTEEN_GRP_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_TW4MJ16_GRP_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_TW4MJ16_GRP_LIST_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_GET_FRUIT_BAR_GRP_LIST_Event::Name,
                        CentralServerEvent::CS_LB_GET_FRUIT_BAR_GRP_LIST_Event::id);

                    /******************** 其他雜項 ********************/
                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_BACARRA_ROAD_INFO_Event::Name,
                        CentralServerEvent::CS_LB_BACARRA_ROAD_INFO_Event::id);

                    CentralServerEvent::registerEvent(
                        CentralServerEvent::CS_LB_SICBO_ROAD_INFO_Event::Name,
                        CentralServerEvent::CS_LB_SICBO_ROAD_INFO_Event::id);


                    /**************遊戲內部使用事件********************/
                    /**
                     *命令名稱：LoginFormSendSignal
                     *觸發訊號：無
                     *處理命令：ConnectToLoginServerCommand
                     */
                    facade.registerCommand(
                        LoginFormSendSignal,
                        (ICommand *)new ConnectToLoginServerCommand());

                    /**
                     *命令名稱：JoinGroupSignal
                     *觸發訊號：無
                     *處理命令：LoadGameCommand
                     */
                    facade.registerCommand(
                        JoinGroupSignal,
                        (ICommand *)new LoadGameCommand());

                    /**請求目前連線伺服器下的現有人數的訊號
                     *命令名稱：AskOnlineMemberCountSignal
                     *觸發訊號：無
                     *處理命令：AskOnlineMemberCountCommand
                     */
                    facade.registerCommand(
                        AskOnlineMemberCountSignal,
                        (ICommand *)new AskOnlineMemberCountCommand());

                    // Login server 事件處理
                    /**
                     *命令名稱：LoginServerConnect
                     *觸發訊號：無，由 ConnectToLoginServerCommand 發送 CONNECT
                     *處理命令：LoginToLoginServerCommand
                     */
                    facade.registerCommand(
                        LoginServerConnect,
                        (ICommand *)new LoginToLoginServerCommand());

                    /**
                     *命令名稱：LS_LB_CHK_USER
                     *觸發訊號：無，由 LoginToLoginServerCommand 發送 LS_LB_CHK_USER
                     *處理命令：ConnectToCentralServerCommand
                     */
                    //LoginServer event
                    facade.registerCommand(
                            LoginServiceEvent::LS_LB_CHK_USER_Event::Name,
                        (ICommand *)new ConnectToCentralServerCommand());

                    // Central server事件處理
                    /**
                     *命令名稱：CentralServerConnect
                     *觸發訊號：無，由 ConnectToCentralServerCommand 發送 CONNECT
                     *處理命令：LoginToCentralServerCommand
                     */
                    facade.registerCommand(
                        CentralServerConnect,
                        (ICommand *)new LoginToCentralServerCommand());

                    /******************** 玩家登入 ********************/
                    /**
                     *命令名稱：CS_LB_CHK_USER
                     *觸發訊號：無，由 LoginToCentralServerCommand 發送 CS_LB_CHK_USER_S
                     *處理命令：AcceptPlayerLoginCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_CHK_USER_Event::Name,
                        (ICommand *)new AcceptPlayerLoginCommand());

                    /**
                     *命令名稱：CS_LB_GET_MEN_IN_NET
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：AcceptOnlineMemberCountCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_MEN_IN_NET_Event::Name,
                        (ICommand *)new AcceptOnlineMemberCountCommand());

                    /**
                     *命令名稱：CS_LB_GET_USR_LIST
                     *觸發訊號：AskUserListSignal 對應 AskUserListCommand
                     *處理命令：AcceptUserListCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_USR_LIST_Event::Name,
                        (ICommand *)new AcceptUserListCommand());

                    /**
                     *命令名稱：CS_LB_SEARCH_USER
                     *觸發訊號：SearchUserSignal 對應 SerachUserCommand
                     *處理命令：SerachUserResultCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_SEARCH_USER_Event::Name,
                        (ICommand *)new SerachUserResultCommand());

                    /**
                     *命令名稱：CS_LB_GET_GAME_STATE
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：AcceptGameStateCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_GAME_STATE_Event::Name,
                        (ICommand *)new AcceptGameStateCommand());

                    /******************** 遊戲操作 ********************/
                    /**
                     *命令名稱：CS_LB_USER_CREATE_BUILTIN_GRP
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：JoinGroupResultCommand
                     */
                    //ICommand *tmpJoinGroupResultCommand = (ICommand *)new JoinGroupResultCommand();

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_USER_CREATE_BUILTIN_GRP_Event::Name,
                        (ICommand *)new JoinGroupResultCommand());

                    //TODO: CS_LB_USER_CREATE_EXTEND_GRP(暫不實作)

                    /**
                     *命令名稱：CS_LB_USER_JOIN_BUILTIN_GRP
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：JoinGroupResultCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_USER_JOIN_BUILTIN_GRP_Event::Name,
                        (ICommand *)new JoinGroupResultCommand());

                    //TODO: CS_LB_USER_JOIN_EXTEND_GRP(暫不實作)

                    /**
                     *命令名稱：CS_LB_USER_CANCEL_JOIN_GRP
                     *觸發訊號：CancelJoinedGroupSignal 對應 CancelJoinedGroupCommand
                     *處理命令：JoinedGroupCancelledCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_USER_CANCEL_JOIN_GRP_Event::Name,
                        (ICommand *)new JoinedGroupCancelledCommand());

                    /**
                     *命令名稱：CS_LB_USER_RELOGIN_GS
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：ConnectToGameServerCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_USER_RELOGIN_GS_Event::Name,
                        (ICommand *)new ConnectToGameServerCommand());

                    /**
                     *命令名稱：GameServerConnect
                     *觸發訊號：無，由 ConnectToGameServerCommand 發送 CONNECT
                     *處理命令：LoginToGameServerCommand
                     */
                    facade.registerCommand(
                        GameServerConnect,
                        (ICommand *)new LoginToGameServerCommand());

                    /**
                     *命令名稱：CS_LB_FINISH_GRP_OK
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：GameGroupFinishedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_FINISH_GRP_OK_Event::Name,
                        (ICommand *)new GameGroupFinishedCommand());

                    //TODO: CS_LB_FINISH_GRP_NOT_OK
                    //TODO: CS_LB_USER_LEAVE_OK

                    /**
                     *命令名稱：CS_LB_CLIENT_DISCONNECTED
                     *觸發訊號：
                     *處理命令：GameDisconnectedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_CLIENT_DISCONNECTED_Event::Name,
                        (ICommand *)new GameDisconnectedCommand());

                    //TODO: CS_LB_QUERY_TABLE_PL_INFO

                    /**
                     *命令名稱：CS_LB_USER_KEEP_TABLE_INFO
                     *觸發訊號：
                     *處理命令：AcceptKeepTableInfoCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_USER_KEEP_TABLE_INFO_Event::Name,
                        (ICommand *)new AcceptKeepTableInfoCommand());

                    //TODO: CS_LB_GET_HOLD_TABLE_INFO
                    //TODO: CS_LB_SET_UNHOLD_TABLE

                    /******************** 公告訊息 ********************/
                    /**
                     *命令名稱：CS_LB_POST_SYS_MSG
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：SystemMessageArrivedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_POST_SYS_MSG_Event::Name,
                        (ICommand *)new SystemMessageArrivedCommand());

                    /**
                     *命令名稱：CS_LB_BROADCAST_MSG
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：BroadcastMessageArrivedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_BROADCAST_MSG_Event::Name,
                        (ICommand *)new BroadcastMessageArrivedCommand());

                    /**
                     *命令名稱：CS_LB_BROADCAST_MSG
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：BroadcastMessageArrivedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_BROADCAST_MSG_Event::Name,
                        (ICommand *)new BroadcastMessageArrivedCommand());

                    /**
                     *命令名稱：CS_LB_QUERY_JPBONUS
                     *觸發訊號：QueryJPBonusSignal 對應 QueryJPBonusCommand
                     *處理命令：JPBonusReceivedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_QUERY_JPBONUS_Event::Name,
                        (ICommand *)new JPBonusReceivedCommand());

                    /******************** 通訊測聯 ********************/
                    /**
                     *命令名稱：CS_LB_BOOT_TICK
                     *觸發訊號：
                     *處理命令：CentralServerTickEnabledCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_BOOT_TICK_Event::Name,
                        (ICommand *)new CentralServerTickEnabledCommand());

                    // CS_LB_TICK
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_TICK_Event::Name,
                        (ICommand *)new CentralServerTickCommand());

                    /**
                     *命令名稱：CS_LB_PING
                     *觸發訊號：
                     *處理命令：ReceivePingCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_PING_Event::Name,
                        (ICommand *)new ReceivePingCommand());

                    /******************** 玩家資訊 ********************/
                    /**
                     *命令名稱：CS_LB_QRY_USER_PROFILE_INFO
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：UserProfileReceivedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_QRY_USER_PROFILE_INFO_Event::Name,
                        (ICommand *)new UserProfileReceivedCommand());

                    /**
                     *命令名稱：CS_LB_SET_GAME_NICKNAME
                     *觸發訊號：SetUserNickNameSignal 對應 SetUserNickNameCommand
                     *處理命令：SetUserNickNameResultCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_SET_GAME_NICKNAME_Event::Name,
                        (ICommand *)new SetUserNickNameResultCommand());

                    //TODO: CS_LB_QUERY_USER_UPLOAD_PIC_ALLOWED

                    /**
                     *命令名稱：CS_LB_UPDATE_USER_PIC
                     *觸發訊號：SetUserPictureLinkSignal 對應 SetUserPictureCommand
                     *處理命令：SetUserPictureResultCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_UPDATE_USER_PIC_Event::Name,
                        (ICommand *)new SetUserPictureResultCommand());

                    /******************** 聊天對話 ********************/
                    /**
                     *命令名稱：CS_LB_CHAT_ROOM_TALK
                     *觸發訊號：SendTalkSignal 對應 SendTalkCommand
                     *處理命令：TalkReceivedCommand
                     */
                    facade.registerCommand(
                            CentralServerEvent::CS_LB_CHAT_ROOM_TALK_Event::Name,
                        (ICommand *)new TalkReceivedCommand());

                    /**
                     *命令名稱：CS_LB_CHAT_WHISPER
                     *觸發訊號：SendWhisperSignal 對應 SendWhisperCommand
                     *處理命令：WhisperReceivedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_CHAT_WHISPER_Event::Name,
                        (ICommand *)new WhisperReceivedCommand());

                    /******************** 帳務交易 ********************/
                    /**
                     *命令名稱：CS_LB_QUERY_CHARGE_RATE
                     *觸發訊號：QueryChargeRateSignal 對應 QueryChargeRateCommand
                     *處理命令：QueryChargeRateResultCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_QUERY_CHARGE_RATE_Event::Name,
                        (ICommand *)new QueryChargeRateResultCommand());

                    /**
                     *命令名稱：CS_LB_TRANSFER_V_POINTS
                     *觸發訊號：TransferValuablePointSignal 對應 TransferValuablePointCommand
                     *處理命令：TransferValuablePointResultCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_TRANSFER_V_POINTS_Event::Name,
                        (ICommand *)new TransferValuablePointResultCommand());

                    /**
                     *命令名稱：CS_LB_TRANSFER_F_POINTS
                     *觸發訊號：TransferFreePointSignal 對應 TransferFreePointCommand
                     *處理命令：TransferFreePointResultCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_TRANSFER_F_POINTS_Event::Name,
                        (ICommand *)new TransferFreePointResultCommand());

                    /******************** 頻道聊天 ********************/
                    /**
                     *命令名稱：CS_LB_CHANNEL_LIST
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：ChannelListArrivedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_CHANNEL_LIST_Event::Name,
                        (ICommand *)new ChannelListArrivedCommand());

                    /**
                     *命令名稱：CS_LB_CHANNEL_USER_JOIN
                     *觸發訊號：JoinChannelSignal 對應 JoinChannelCommand
                     *處理命令：JoinChannelResultCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_CHANNEL_USER_JOIN_Event::Name,
                        (ICommand *)new JoinChannelResultCommand());

                    /**
                     *命令名稱：CS_LB_CHANNEL_USER_LIST
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：ChannelUserListArrivedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_CHANNEL_USER_LIST_Event::Name,
                        (ICommand *)new ChannelUserListArrivedCommand());

                    /**
                     *命令名稱：CS_LB_CHANNEL_USER_SEND_MESSAGE
                     *觸發訊號：SendChannelMessageSignal 對應 SendChannelMessageCommand
                     *處理命令：ChannelMessageReceivedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_CHANNEL_USER_SEND_MESSAGE_Event::Name,
                        (ICommand *)new ChannelMessageReceivedCommand());

                    /**
                     *命令名稱：CS_LB_CHANNEL_USER_LEAVE
                     *觸發訊號：無，由 central server 主動發出
                     *處理命令：ChannelUserLeaveCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_CHANNEL_USER_LEAVE_Event::Name,
                        (ICommand *)new ChannelUserLeaveCommand());

                    /**
                     *命令名稱：CS_LB_USER_SEND_MESSAGE_ALL_CHANNELS
                     *觸發訊號：SendAllChannelMessageSignal 對應 SendAllChannelMessageCommand
                     *處理命令：AllChannelMessageReceivedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_USER_SEND_MESSAGE_ALL_CHANNELS_Event::Name,
                        (ICommand *)new AllChannelMessageReceivedCommand());

                    /******************** 社群交友 ********************/
                    /**
                     *命令名稱：CS_LB_GET_FRIEND_LIST
                     *觸發訊號：GetFriendListSignal 對應 GetFriendListCommand
                     *處理命令：FriendListReceivedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_FRIEND_LIST_Event::Name,
                        (ICommand *)new FriendListReceivedCommand());

                    /**
                     *命令名稱：CS_LB_ADD_FRIEND
                     *觸發訊號：AddFriendSignal 對應 AddFriendCommand
                     *處理命令：FriendAddedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_ADD_FRIEND_Event::Name,
                        (ICommand *)new FriendAddedCommand());

                    /**
                     *命令名稱：CS_LB_ASK_ADD_FRIEND
                     *觸發訊號：FriendshipAskSignal 對應 FriendshipAskCommand
                     *處理命令：FriendshipAskResultCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_ASK_ADD_FRIEND_Event::Name,
                        (ICommand *)new FriendshipAskResultCommand());

                    /**
                     *命令名稱：CS_LB_DEL_FRIEND
                     *觸發訊號：DeleteFriendSignal 對應 DeleteFriendCommand
                     *處理命令：FriendDeletedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_DEL_FRIEND_Event::Name,
                        (ICommand *)new FriendDeletedCommand());

                    /**
                     *命令名稱：CS_LB_GET_BADGUY_LIST
                     *觸發訊號：GetBadGuyListSignal 對應 GetBadGuyListCommand
                     *處理命令：BadGuyListReceivedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_BADGUY_LIST_Event::Name,
                        (ICommand *)new BadGuyListReceivedCommand());

                    /**
                     *命令名稱：CS_LB_ADD_BADGUY
                     *觸發訊號：AddBadGuySignal 對應 AddBadGuyCommand
                     *處理命令：BadGuyAddedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_ADD_BADGUY_Event::Name,
                        (ICommand *)new BadGuyAddedCommand());

                    /**
                     *命令名稱：CS_LB_DEL_BADGUY
                     *觸發訊號：DeleteBadGuySignal 對應 DeleteBadGuyCommand
                     *處理命令：BadGuyDeletedCommand
                     */
                    facade.registerCommand(
                        CentralServerEvent::CS_LB_DEL_BADGUY_Event::Name,
                        (ICommand *)new BadGuyDeletedCommand());

                    /******************** 即時視訊 ********************/
                    //TODO: CS_LB_CHAT_DEMAND_ON_VIDEO
                    //TODO: CS_LB_CHAT_REPLY_ON_VIDEO
                    //TODO: CS_LB_CHAT_CONNECT_ON_VIDEOS
                    //TODO: CS_LB_CHAT_DISCONNECT_ON_VIDEOS

                    /******************** 座位清單 ********************/
                    /**
                     *命令名稱：CS_LB_ASK_GAME_GRP_LIST
                     *觸發訊號：AskGameGroupListSignal 對應 AskGameGroupListCommand
                     *處理命令：無，只有客端發送封包
                     */

                    /**
                     *命令名稱：CS_LB_GET_HOLDEM_GRP_LIST,CS_LB_GET_BLACKJACK_GRP_LIST,
                     *CS_LB_GET_BACARRA_GRP_LIST,CS_LB_GET_SEVEN_POKER_GRP_LIST,
                     *CS_LB_GET_BEAST_BAR_GRP_LIST, CS_LB_GET_SICBO_GRP_LIST
                     *CS_LB_GET_BINGO_PLANET_GRP_LIST, CS_LB_GET_THIRTEEN_GRP_LIST
                     *CS_LB_GET_TW4MJ16_GRP_LIST, CS_LB_GET_FRUIT_BAR_GRP_LIST
                     *觸發訊號：
                     *處理命令：AcceptGameGroupListCommand
                     */
                    //ICommand *acceptgamegrouplistCmd = (ICommand *)new AcceptGameGroupListCommand();

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_HOLDEM_GRP_LIST_Event::Name,
                        //acceptgamegrouplistCmd);
                        (ICommand *)new AcceptGameGroupListCommand());

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_BLACKJACK_GRP_LIST_Event::Name,
                        //acceptgamegrouplistCmd);
                        (ICommand *)new AcceptGameGroupListCommand());

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_BACARRA_GRP_LIST_Event::Name,
                        //acceptgamegrouplistCmd);
                        (ICommand *)new AcceptGameGroupListCommand());

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_SEVEN_POKER_GRP_LIST_Event::Name,
                        //acceptgamegrouplistCmd);
                        (ICommand *)new AcceptGameGroupListCommand());

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_BEAST_BAR_GRP_LIST_Event::Name,
                        //acceptgamegrouplistCmd);
                        (ICommand *)new AcceptGameGroupListCommand());

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_SICBO_GRP_LIST_Event::Name,
                        //acceptgamegrouplistCmd);
                        (ICommand *)new AcceptGameGroupListCommand());

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_BINGO_PLANET_GRP_LIST_Event::Name,
                        //acceptgamegrouplistCmd);
                        (ICommand *)new AcceptGameGroupListCommand());

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_THIRTEEN_GRP_LIST_Event::Name,
                        //acceptgamegrouplistCmd);
                        (ICommand *)new AcceptGameGroupListCommand());

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_TW4MJ16_GRP_LIST_Event::Name,
                        //acceptgamegrouplistCmd);
                        (ICommand *)new AcceptGameGroupListCommand());

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_FRUIT_BAR_GRP_LIST_Event::Name,
                        //acceptgamegrouplistCmd);
                        (ICommand *)new AcceptGameGroupListCommand());

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_SMARIO_GRP_LIST_Event::Name,
                        //acceptgamegrouplistCmd);
                        (ICommand *)new AcceptGameGroupListCommand());

                    facade.registerCommand(
                        CentralServerEvent::CS_LB_GET_FIVE_POKER_GRP_LIST_Event::Name,
                        //acceptgamegrouplistCmd);
                        (ICommand *)new AcceptGameGroupListCommand());

                    /******************** 其他雜項 ********************/
                    //TODO: CS_LB_BACARRA_ROAD_INFO
                    //TODO: CS_LB_SICBO_ROAD_INFO

                    // game client 事件處理對應。
                    /**
                     *命令名稱：GS_GA_RELOGIN_USER_Event
                     *觸發訊號：
                     *處理命令：
                     */

                    facade.registerCommand(
                        GameClientServiceEvent::GS_GA_RELOGIN_USER_Event::Name,
                        (ICommand *)new AcceptGameServerLoginResultCommand());

                    /**
                     *命令名稱：GS_GA_TICK_Event
                     *觸發訊號：
                     *處理命令：GameServerTickCommand
                     */
                    facade.registerCommand(
                        GameClientServiceEvent::GS_GA_TICK_Event::Name,
                        (ICommand *)new GameServerTickCommand());
                    /**
                     *命令名稱：LB_GA_REGISTER_LOADER
                     *觸發訊號：
                     *處理命令：RegisterLoaderCommand
                     */
                    facade.registerCommand(
                        GameClientServiceEvent::LB_GA_REGISTER_LOADER_Event::Name,
                        (ICommand *)new RegisterLoaderCommand());

                    /**
                     *命令名稱：LB_GA_RELOGIN_RESULT
                     *觸發訊號：
                     *處理命令：AcceptGameServerLoginResultCommand
                     */
                    //TODO:待確認
                    /*
                    facade.registerCommand(
                        GameClientServiceEvent::LB_GA_RELOGIN_RESULT_Event::Name,
                        (ICommand *)new AcceptGameServerLoginResultCommand());
                     */
                    /**
                     *命令名稱：LB_GA_CLOSE_GAME_CLIENT
                     *觸發訊號：
                     *處理命令：AcceptGameClientClosedCommand
                     */
                    facade.registerCommand(
                        GameClientServiceEvent::LB_GA_CLOSE_GAME_CLIENT_Event::Name,
                        (ICommand *)new AcceptGameClientClosedCommand());

                    /**
                     *命令名稱：
                     *觸發訊號：
                     *處理命令：
                     */


                    //TODO:regist lobby proxy
                    //facade.registerProxy();

                    //TODO:regist lobby mediator
                }
            }
        };
    }
}

#endif //_LOBBYSTARTUPCOMMAND_HPP_

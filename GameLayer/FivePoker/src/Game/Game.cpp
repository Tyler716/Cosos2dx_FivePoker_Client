#include "Game.h"
#include "../Model/Proxy/GameProxy.hpp"
#include "../View/Mediator/NGMediator.hpp"
#include "../View/Mediator/DGMediator.hpp"
#include "../View/Mediator/LoginMediator.hpp"
#include "../Controller/Command/WaitSyncCommand.hpp"
#include "../Controller/Command/SendMsgCommand.hpp"
#include "LobbyStartUpCommand.hpp"
#include "GameServerProxy.hpp"
#include "CCEGLView.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

using GameLayer::Game::Game;
using GameLayer::Game::NGMediator;
using GameLayer::Game::GameProxy;
using GameLayer::Game::WaitSyncCommand;
using GameLayer::Lobby::LobbyStartUpCommand;
using namespace BaseLayer::Patterns;
using namespace BaseLayer::Interfaces;
using namespace GameLayer::Lobby;

const std::string GameSLMsg::GSLMSG_FIRST::Name = "GSLMSG_FIRST";
const std::string GameSLMsg::GSLMSG_BET::Name = "GSLMSG_BET";
const std::string GameSLMsg::GSLMSG_DRAW::Name = "GSLMSG_DRAW";
const std::string GameSLMsg::GSLMSG_PLAYER_DOUBLE_GAME::Name = "GSLMSG_PLAYER_DOUBLE_GAME";
const std::string GameSLMsg::GSLMSG_DOUBLE_GAME::Name = "GSLMSG_DOUBLE_GAME";
const std::string GameSLMsg::GSLMSG_GET_POINT::Name = "GSLMSG_GET_POINT";
const std::string GameSLMsg::GSLMSG_SET_MULTIPLE::Name = "GSLMSG_SET_MULTIPLE";
const std::string GameSLMsg::GSLMSG_EXIT_GAME::Name = "GSLMSG_EXIT_GAME";
const std::string GameSLMsg::GSLMSG_RECORD_INFO::Name = "GSLMSG_RECORD_INFO";
const std::string GameSLMsg::GSLMSG_TALK::Name = "GSLMSG_TALK";
const std::string GameSLMsg::GSLMSG_CAN_KEEP_TABLE::Name = "GSLMSG_CAN_KEEP_TABLE";
const std::string GameSLMsg::GSLMSG_PLAY_MINI_GAME::Name = "GSLMSG_PLAY_MINI_GAME";

const std::string GameCliMsg::GCLIMSG_FIRST::Name = "GCLIMSG_FIRST";
const std::string GameCliMsg::GCLIMSG_BET_FAIL::Name = "GCLIMSG_BET_FAIL";
const std::string GameCliMsg::GCLIMSG_DEAL::Name = "GCLIMSG_DEAL";
const std::string GameCliMsg::GCLIMSG_DRAW::Name = "GCLIMSG_DRAW";
const std::string GameCliMsg::GCLIMSG_DOUBLE_GAME_DATA::Name = "GCLIMSG_DOUBLE_GAME_DATA";
const std::string GameCliMsg::GCLIMSG_DOUBLE_GAME_RESULT::Name = "GCLIMSG_DOUBLE_GAME_RESULT";
const std::string GameCliMsg::GCLIMSG_GAME_OVER::Name = "GCLIMSG_GAME_OVER";
const std::string GameCliMsg::GCLIMSG_SET_MULTIPLE_RESULT::Name = "GCLIMSG_SET_MULTIPLE_RESULT";
const std::string GameCliMsg::GCLIMSG_EXIT_GAME::Name = "GCLIMSG_EXIT_GAME";
const std::string GameCliMsg::GCLIMSG_RECORD_INFO::Name = "GCLIMSG_RECORD_INFO";
const std::string GameCliMsg::GCLIMSG_SHINE_LIGHT::Name = "GCLIMSG_SHINE_LIGHT";
const std::string GameCliMsg::GCLIMSG_TALK::Name = "GCLIMSG_TALK";
const std::string GameCliMsg::GCLIMSG_CAN_KEEP_TABLE::Name = "GCLIMSG_CAN_KEEP_TABLE";
const std::string GameCliMsg::GCLIMSG_MINI_GAME_RESULT::Name = "GCLIMSG_MINI_GAME_RESULT";

const std::string GameDefineMsg::GAMEMSG_LOGIN_FAIL::Name = "GAMEMSG_LOGIN_FAIL";
const std::string GameDefineMsg::GAMEMSG_NETWORK_OFF::Name = "GAMEMSG_NETWORK_OFF";
const std::string GameDefineMsg::GAMEMSG_CONNECT_LS_OK::Name = "GAMEMSG_CONNECT_LS_OK";

USING_NS_CC;

Game::Game()
{
    facade = &Facade::getInstance();
	
	m_iWidth = 0;
	m_iHeight = 0;
}

Game::~Game()
{
}

bool Game::initialize()
{
#ifndef View_Test
    //註冊proxy
    facade->registerProxy((IProxy *)new GameProxy("GameProxy"));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "registerProxy GameProxy");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

    //註冊command
    facade->registerCommand("LobbyStartUp",
        (ICommand *)new LobbyStartUpCommand());

    //facade->registerCommand(WaitSync, (ICommand *)new WaitSyncCommand());
    //ICommand *SyncCmd = (ICommand *)new WaitSyncCommand();

    facade->registerCommand(
        GameClientServiceEvent::CLIMSG_GROUP_SETTED_Event::Name,
        (ICommand *)new WaitSyncCommand());

    facade->registerCommand(
        GameClientServiceEvent::CLIMSG_USER_INFO_Event::Name,
        (ICommand *)new WaitSyncCommand());

    facade->registerCommand(
        GameClientServiceEvent::CLIMSG_GAME_CONTINUE_Event::Name,
        (ICommand *)new WaitSyncCommand());

    facade->registerCommand(
        GameClientServiceEvent::CLIMSG_WAIT_SYNC_Event::Name,
        (ICommand *)new WaitSyncCommand());

    facade->registerCommand(
        GameClientServiceEvent::CLIMSG_SERVER_MSG::Name,
        (ICommand *)new WaitSyncCommand());

    facade->registerCommand(
        GameClientServiceEvent::CLIMSG_USER_JOIN_GAME::Name,
        (ICommand *)new WaitSyncCommand());

    //註冊Msg
    GameSLMsg::registerEvent(GameSLMsg::GSLMSG_FIRST::Name, GameSLMsg::GSLMSG_FIRST::id);
    GameSLMsg::registerEvent(GameSLMsg::GSLMSG_BET::Name, GameSLMsg::GSLMSG_BET::id);
    GameSLMsg::registerEvent(GameSLMsg::GSLMSG_DRAW::Name, GameSLMsg::GSLMSG_DRAW::id);
    GameSLMsg::registerEvent(GameSLMsg::GSLMSG_PLAYER_DOUBLE_GAME::Name, GameSLMsg::GSLMSG_PLAYER_DOUBLE_GAME::id);
    GameSLMsg::registerEvent(GameSLMsg::GSLMSG_DOUBLE_GAME::Name, GameSLMsg::GSLMSG_DOUBLE_GAME::id);
    GameSLMsg::registerEvent(GameSLMsg::GSLMSG_GET_POINT::Name, GameSLMsg::GSLMSG_GET_POINT::id);
    GameSLMsg::registerEvent(GameSLMsg::GSLMSG_SET_MULTIPLE::Name, GameSLMsg::GSLMSG_SET_MULTIPLE::id);
    GameSLMsg::registerEvent(GameSLMsg::GSLMSG_EXIT_GAME::Name, GameSLMsg::GSLMSG_EXIT_GAME::id);
    GameSLMsg::registerEvent(GameSLMsg::GSLMSG_RECORD_INFO::Name, GameSLMsg::GSLMSG_RECORD_INFO::id);
    GameSLMsg::registerEvent(GameSLMsg::GSLMSG_TALK::Name, GameSLMsg::GSLMSG_TALK::id);
    GameSLMsg::registerEvent(GameSLMsg::GSLMSG_CAN_KEEP_TABLE::Name, GameSLMsg::GSLMSG_CAN_KEEP_TABLE::id);
    GameSLMsg::registerEvent(GameSLMsg::GSLMSG_PLAY_MINI_GAME::Name, GameSLMsg::GSLMSG_PLAY_MINI_GAME::id);
    GameSLMsg::registerEvent(GameClientServiceEvent::SLMSG_WAIT_SYNC_Event::Name, GameClientServiceEvent::SLMSG_WAIT_SYNC_Event::id);
    GameSLMsg::registerEvent(GameClientServiceEvent::SLMSG_GAME_CONTINUE_Event::Name, GameClientServiceEvent::SLMSG_GAME_CONTINUE_Event::id);

    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_FIRST::Name, GameCliMsg::GCLIMSG_FIRST::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_BET_FAIL::Name, GameCliMsg::GCLIMSG_BET_FAIL::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_DEAL::Name, GameCliMsg::GCLIMSG_DEAL::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_DRAW::Name, GameCliMsg::GCLIMSG_DRAW::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_DOUBLE_GAME_DATA::Name, GameCliMsg::GCLIMSG_DOUBLE_GAME_DATA::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_DOUBLE_GAME_RESULT::Name, GameCliMsg::GCLIMSG_DOUBLE_GAME_RESULT::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_GAME_OVER::Name, GameCliMsg::GCLIMSG_GAME_OVER::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_SET_MULTIPLE_RESULT::Name, GameCliMsg::GCLIMSG_SET_MULTIPLE_RESULT::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_EXIT_GAME::Name, GameCliMsg::GCLIMSG_EXIT_GAME::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_RECORD_INFO::Name, GameCliMsg::GCLIMSG_RECORD_INFO::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_SHINE_LIGHT::Name, GameCliMsg::GCLIMSG_SHINE_LIGHT::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_TALK::Name, GameCliMsg::GCLIMSG_TALK::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_CAN_KEEP_TABLE::Name, GameCliMsg::GCLIMSG_CAN_KEEP_TABLE::id);
    GameCliMsg::registerEvent(GameCliMsg::GCLIMSG_MINI_GAME_RESULT::Name, GameCliMsg::GCLIMSG_MINI_GAME_RESULT::id);

    GameDefineMsg::registerEvent(GameDefineMsg::GAMEMSG_LOGIN_FAIL::Name, GameDefineMsg::GAMEMSG_LOGIN_FAIL::id);
    GameDefineMsg::registerEvent(GameDefineMsg::GAMEMSG_NETWORK_OFF::Name, GameDefineMsg::GAMEMSG_NETWORK_OFF::id);
    GameDefineMsg::registerEvent(GameDefineMsg::GAMEMSG_CONNECT_LS_OK::Name, GameDefineMsg::GAMEMSG_CONNECT_LS_OK::id);
#endif  //View_Test
    return false;
}

void Game::exec()
{
#ifndef View_Test
    //發送command連接
    facade->sendNotification("LobbyStartUp");

    facade->registerCommand(
        GameSLMsg::GSLMSG_SET_MULTIPLE::Name,
        (ICommand *)new SendMsgCommand());

    facade->registerCommand(
        GameSLMsg::GSLMSG_BET::Name,
        (ICommand *)new SendMsgCommand());

    facade->registerCommand(
        GameSLMsg::GSLMSG_DRAW::Name,
        (ICommand *)new SendMsgCommand());

    facade->registerCommand(
        GameSLMsg::GSLMSG_PLAYER_DOUBLE_GAME::Name,
        (ICommand *)new SendMsgCommand());

    facade->registerCommand(
        GameSLMsg::GSLMSG_DOUBLE_GAME::Name,
        (ICommand *)new SendMsgCommand());

    facade->registerCommand(
        GameSLMsg::GSLMSG_GET_POINT::Name,
        (ICommand *)new SendMsgCommand());

    facade->registerCommand(
        GameSLMsg::GSLMSG_EXIT_GAME::Name,
        (ICommand *)new SendMsgCommand());

    facade->registerCommand(
        GameClientServiceEvent::SLMSG_WAIT_SYNC_Event::Name,
        (ICommand *)new SendMsgCommand());

    facade->registerCommand(
        GameClientServiceEvent::SLMSG_GAME_CONTINUE_Event::Name,
        (ICommand *)new SendMsgCommand());
        
    //註冊mediator
    facade->registerMediator((IMediator *)new NGMediator("NGMediator"));
    facade->registerMediator((IMediator *)new DGMediator("DGMediator"));
    facade->registerMediator((IMediator *)new LoginMediator("LoginMediator"));
#endif  //View_Test
    /*******畫面開發測試**********/
    // create the application instance
    CCEGLView *view = &CCEGLView::sharedOpenGLView();
    view->setViewName("FivePoker");
#if !defined(_WIN32) && !defined(_WIN64)
    view->setFrameSize(m_iWidth, m_iHeight);
    // set the design resolution screen size, if you want to use Design Resoulution scaled to current screen, please uncomment next line.
    view->setDesignResolutionSize(800, 600);

    AppDelegate *pAppDelegate = new AppDelegate();
#else
    view->setFrameSize(800, 600);

    AppDelegate app;
#endif  //!defined(_WIN32) && !defined(_WIN64)

    int ret = CCApplication::sharedApplication().run();
    /**************************/
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    __android_log_print(ANDROID_LOG_INFO, "JNIMsg", "sharedApplication run end!");
#endif  //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

}

void Game::setWindowSize(int iWidth, int iHeight)
{
    m_iWidth = iWidth;
	m_iHeight = iHeight;
}

//PC版使用
void Game::destroyGame()
{
#ifndef View_Test
    //CCScene *pScene = (CCScene *)facade->retrieveMediator("NGMediator").getViewComponent();
    //pScene->release();
#endif  //View_Test
    delete facade;
}


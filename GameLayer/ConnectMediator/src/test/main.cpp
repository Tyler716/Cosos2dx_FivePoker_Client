#include "../../../../BaseLayer/Component/Patterns/Facade/Facade.h"
#include "../../../../BaseLayer/Services/Socket/ConnectionModelData.h"
#include "../Controller/Command/LobbyStartUpCommand.hpp"
#include "../Model/Proxy/LoginServerProxy.hpp"
#include "../Model/vo/ls/LoginRequestVo.h"
#include <iostream>

using namespace BaseLayer::Patterns;
using namespace BaseLayer::Interfaces;
using namespace GameLayer::Lobby;

int main(int argc, char *argv[])
{

    IFacade *facade = &Facade::getInstance();

    facade->registerCommand("LobbyStartUp", (ICommand *)new LobbyStartUpCommand());

    facade->sendNotification("LobbyStartUp");

    facade->sendNotification(LoginFormSendSignal);

    //if connecting fail, 下面的操作會有問題
    if(facade->hasProxy("LoginServerProxy"))
    {
        std::map<std::string, IData *> *datamap = (std::map<std::string, IData *> *)facade->retrieveProxy("LoginServerProxy").getData();
        ConnectionModelData *modeldata = (ConnectionModelData *)(datamap->find("ConnectionModelData")->second);

        std::cout<<"connect state:"<<modeldata->connectState<<std::endl;

        if(modeldata->connectState == ConnectionModelData::CONNECT_SUCCESS)
        {
            LoginRequestVo loginrequestVo(L"is001", L"mmoo");

            std::cout<<"Send Login notification!"<<std::endl;

            facade->sendNotification(LoginServerConnect, &loginrequestVo);

            std::cin.get();

            delete facade->removeProxy("LoginServerProxy");
            delete facade->removeProxy("GameServerProxy");
            delete facade->removeProxy("CentralServerProxy");
        }
    }

    delete facade;

    return 0;
}

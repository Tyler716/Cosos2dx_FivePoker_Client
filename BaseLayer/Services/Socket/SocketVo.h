#ifndef _SOCKETVO_H_
#define _SOCKETVO_H_

struct SocketVo
{
    SocketVo()
        : ip(""),
          port(0),
          autoLogin(false)
    {
    };
    virtual ~SocketVo(){};

    std::string ip;
    int port;
    bool autoLogin;
};

#endif //_SOCKETVO_H_

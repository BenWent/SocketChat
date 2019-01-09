#ifndef CHATTCPSERVER_H
#define CHATTCPSERVER_H

#include <string>
#include <map>
#include <netinet/in.h>
#include <pthread.h>

#include "CTCPServerObserver.h"

class ChatTCPServerObserver : public CTCPServerObserver
{
public:
    ChatTCPServerObserver();

    virtual ~ChatTCPServerObserver();

public:
    /**
		服务端回调函数
    */
    virtual void ServerFunction(int nConnectedSocket);

    /**
		线程运行函数	
    */
    static void* RunExecutiveFunction(void* pContext);

private:
    // ip与socket描述符映射表
    static std:: map<std:: string, int> ip_socketOfClientMap;
};

#endif

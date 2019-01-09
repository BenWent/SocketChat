#ifndef CTCPSERVER_H
#define CTCPSERVER_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

#include "CTCPServerObserver.h"

using namespace std;

//服务端Socket父类
class CTCPServer
{
public:
    CTCPServer(CTCPServerObserver* pObserver, int nServerPort, int nLengthOfQueueOfListen = 100, const char *strBoundIP = NULL);

    virtual ~CTCPServer();

public:
    int Run();

private:
	// 服务端程序运行的端口号
    int m_nServerPort;
	// 服务端程序绑定的ip地址
    char* m_strBoundIP;
    int m_nLengthOfQueueOfListen;
	
	// 服务器端功能具体实现类
    CTCPServerObserver* m_pObserver;
};

#endif


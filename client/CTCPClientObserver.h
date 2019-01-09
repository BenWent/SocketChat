#ifndef CTCPCLIENTOBSERVER_H
#define CTCPCLIENTOBSERVER_H

#include <netinet/in.h>
#include <vector>

class CTCPClientObserver
{
public:
    CTCPClientObserver()
    {
    }

    virtual ~CTCPClientObserver()
    {
    }

public:
	// 客户端socket要执行的功能的顶级接口
    virtual void ClientFunction(int nConnectedSocket, sockaddr_in socketAddr, std:: vector<std:: string>* friendIPVector) = 0;
};

#endif

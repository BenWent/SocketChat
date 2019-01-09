#ifndef CHATTCPCLIENT_H
#define CHATTCPCLIENT_H

#include "CTCPClientObserver.h"

class ChatTCPClientObserver : public CTCPClientObserver
{
public:
    ChatTCPClientObserver(); 
    virtual ~ChatTCPClientObserver();
    
public:
    virtual void ClientFunction(int nConnectedSocket, sockaddr_in socketAddr, std:: vector<std:: string>* friendIPVector);
};

#endif

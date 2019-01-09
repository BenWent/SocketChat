#ifndef CTCPSERVEROBSERVER_H
#define CTCPSERVEROBSERVER_H


class CTCPServerObserver
{
public:
    CTCPServerObserver()
    {
    }

    virtual ~CTCPServerObserver()
    {
    }

public:
	// 定义服务端要通过Socket实现功能的顶级接口
    virtual void ServerFunction(int nListenSocket) = 0;
};

#endif

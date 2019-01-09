#include "CTCPServer.h"
#include "CTCPServerObserver.h"

CTCPServer:: CTCPServer(CTCPServerObserver* pObserver, int nServerPort, int nLengthOfQueueOfListen, const char *strBoundIP)
{
	m_pObserver = pObserver;
	m_nServerPort = nServerPort;
	m_nLengthOfQueueOfListen = nLengthOfQueueOfListen;

	if(NULL == strBoundIP)
	{
	    m_strBoundIP = NULL;
	}
	else
	{
	    int length = strlen(strBoundIP);
	    m_strBoundIP = new char[length + 1];
	    memcpy(m_strBoundIP, strBoundIP, length + 1);
	}
}

CTCPServer:: ~CTCPServer()
{
	if(m_strBoundIP != NULL)
	{
		// 释放空间
	    delete [] m_strBoundIP;
	}
}

int CTCPServer:: Run()
    {
		// 指明socket使用ipv4协议(AF_INET)，且数据流使用字节流(SOCK_STREAM)传输,
		// protocol为0时，会自动选择相应的数据流传输方式
		int nListenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
		if(-1 == nListenSocket)
		{
		    std::cout << "socket error" << std::endl;

		    return -1;
		}

		// ipv4 套接口地址结构体
		sockaddr_in ServerAddress;
		// 将各个字段设为0
		memset(&ServerAddress, 0, sizeof(sockaddr_in));
		// 设置 协议簇字段 为ipv4协议
		ServerAddress.sin_family = AF_INET;

		// 设置 ip地址 字段
		if(NULL == m_strBoundIP)
		{
			// 表明服务器程序可以在任意网络接口上接受客户连接
		    ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else
		{
		    if(::inet_pton(AF_INET, m_strBoundIP, &ServerAddress.sin_addr) != 1)
		    {
				std::cout << "inet_pton error" << std::endl;
				::close(nListenSocket);

				return -1;
		    }
		}
		// 设置 端口 字段
		ServerAddress.sin_port = htons(m_nServerPort);

		// 将 socket 与 套接口地址结构(ip:port) 绑定到一起
		if(::bind(nListenSocket, (sockaddr *)&ServerAddress, sizeof(sockaddr_in)) == -1)
		{
		    std::cout << "bind error" << std::endl;
		    ::close(nListenSocket);

		    return -1;
		}

		// 监听nListenSocket被动地等待客户端连接到该socket，m_nLengthOfQueueOfListen
		// 指定了内核应该为相应套接口排队的最大连接个数
		if(::listen(nListenSocket, m_nLengthOfQueueOfListen) == -1)
		{
		    std::cout << "listen error" << std::endl;
		    ::close(nListenSocket);

		    return -1;
		}

		// 回调服务器代理接口
		if(NULL != m_pObserver)
		{
			m_pObserver -> ServerFunction(nListenSocket);
		}

		::close(nListenSocket);

		return 0;
    }

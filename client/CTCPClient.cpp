#include "CTCPClient.h"
#include "Config.h"

CTCPClient:: CTCPClient(CTCPClientObserver* pOberver, std:: vector<std:: string>* friendIPVector)
{
	m_pOberver = pOberver;
	m_friendIPVector = friendIPVector;
	
	// 从配置文件中读取服务端ip和port
	Config config("config.txt");

	std:: string serverIP;
	serverIP = config.Read("serverIP", serverIP);
	m_strServerIP = new char[strlen(serverIP.c_str()) + 1];
	strcpy(m_strServerIP, serverIP.c_str());

	m_nServerPort = config.Read("serverPort", m_nServerPort);
}


CTCPClient:: ~CTCPClient()
{

}

int CTCPClient:: Run()
{
	// 建立客户端Socket,使用ipv4协议通信，使用scoket字节流传输数据
	int nClientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == nClientSocket)
	{
	    std::cout << "socket error" << std::endl;

	    return -1;
	}

	// IPV4套接口地址结构
	sockaddr_in ServerAddress;
	memset(&ServerAddress, 0, sizeof(sockaddr_in));

	// 设置 协议簇字段
	ServerAddress.sin_family = AF_INET;

	// 设置 服务端ip地址字段
	if(::inet_pton(AF_INET, m_strServerIP, &ServerAddress.sin_addr) != 1)
	{
	    std::cout << "inet_pton error" << std::endl;
	    ::close(nClientSocket);

	    return -1;
	}

	// 设置 服务端端口字段
	ServerAddress.sin_port = htons(m_nServerPort);

	// 客户端调用该函数向服务端发出连接请求
	if(::connect(nClientSocket, (sockaddr*)&ServerAddress, sizeof(ServerAddress)) == -1)
	{
	    std::cout << "connect error" << std::endl;
	    ::close(nClientSocket);

	    return -1;
	}

	// 回调客户端想通过socket执行的功能
	if(NULL != m_pOberver)
	{
		m_pOberver -> ClientFunction(nClientSocket, ServerAddress, m_friendIPVector);
	}
	// ::close(nClientSocket);

	return 0;
}

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h> // net_ntoa函数
#include <cstring>
#include <sstream>
#include <pthread.h>

#include "ChatTCPServerObserver.h"

// 线程通信数据
struct ServerDataStruct
{
	// 客户端端口socket
	int nConnectedSocket;
	// 服务端监听socket
	int nListenSocket;
	// 客户端ipv4地址结构
	sockaddr_in clientAddress;
};


/**
	服务端线程运行函数
*/
void* ChatTCPServerObserver:: RunExecutiveFunction(void* pContext)
{
	struct ServerDataStruct* sds = (struct ServerDataStruct*) pContext;
	// 发送消息方的ip
	std:: string senderIP = inet_ntoa(sds -> clientAddress.sin_addr);

	// 客户端是否发送了退出消息
	bool isExit = false;

	std:: string text;
	// 接收客户端消息的缓冲区
	char buff[1024];
	// 从socket流中读取字段的长度
	int len = 0;
	// 接收方ip、消息下标、内容
	std:: string receiverIP, count, content;
	// 协议符号的位置
	int dotPosition, colonPosition, hashtagPosition;
	std:: stringstream ss;

	while(!isExit)
	{
		len = ::read(sds -> nConnectedSocket, buff,  sizeof(buff));

		if(len > 0)
		{
			text.assign(&buff[0], &buff[len]);

			while(text != "")
			{
				// 清空
				ss.clear();
				ss.str("");

				dotPosition = text.find("\\.");
				colonPosition = text.find("\\:");
				hashtagPosition = text.find("\\#");
				
				count = text.substr(0, dotPosition);
				// 收信方ip
				receiverIP = text.substr(dotPosition + 2, colonPosition - dotPosition - 2);
				content = text.substr(colonPosition + 2, hashtagPosition - colonPosition - 2);

				// 将消息发送给对应的客户端
				if(ip_socketOfClientMap.find(receiverIP) != ip_socketOfClientMap.end())
				{ // 通信对方在线
					std:: string sendContent = content;

					// 客服端与服务端的通信协议为：1\.ip\:content\#
					// 协议 \\d 表示告知对方我已离线，并要求对方清空聊天记录
					if("\\q" == sendContent)
						sendContent = "\\d"; // 告知对方我已离线并清空聊天记录
					ss << count << "\\." << senderIP << "\\:" << sendContent << "\\#";

					sendContent = ss.str();	

					::write(ip_socketOfClientMap[receiverIP], sendContent.c_str(), sendContent.length());
				}

				if("\\q" == content) // 客户端发送了退出消息，将维护的对应客户端socket关闭
				{
					for(std:: map<std:: string, int>:: iterator iter = ip_socketOfClientMap.begin(); 
							iter != ip_socketOfClientMap.end(); iter++)
					{
						if(iter -> first == senderIP)
						{
							ip_socketOfClientMap.erase(iter);
							::close(iter -> second); // 关闭socket

							isExit = true; 
							break;
						}
					}
				}

				text = text.substr(hashtagPosition + 2);
			} // while(text != "")
			
		} // if(len > 0)

	} //  while(!isExit)
	
	delete sds;
}

std:: map<std:: string, int> ChatTCPServerObserver:: ip_socketOfClientMap;

ChatTCPServerObserver:: ChatTCPServerObserver()
{

}

ChatTCPServerObserver:: ~ChatTCPServerObserver()
{

}

void ChatTCPServerObserver:: ServerFunction(int nListenSocket)
{
	sockaddr_in clientAddress;
	socklen_t lengthOfClientAddress = sizeof(sockaddr_in);

	while(true)
	{
		// 得到请求连接服务器的客户端socket描述符
		int nConnectedSocket = ::accept(nListenSocket, (sockaddr*)&clientAddress, &lengthOfClientAddress);
		
		pthread_t tid; 
		if(-1 != nConnectedSocket) // 为每个客户端开启一个线程
		{
			std:: string senderIP = inet_ntoa(clientAddress.sin_addr);
			ip_socketOfClientMap[senderIP] = nConnectedSocket;

			struct ServerDataStruct* sds = new ServerDataStruct;
			sds -> nConnectedSocket = nConnectedSocket;
			sds -> nListenSocket = nListenSocket;
			sds -> clientAddress = clientAddress; 			

			// 开启线程
			pthread_create(&tid, NULL, ChatTCPServerObserver:: RunExecutiveFunction
				,(void*) sds);
			pthread_detach(tid);
		}
	}

}

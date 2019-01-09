#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <ctime>


#include "ChatTCPClientObserver.h"
#include "Utils.h"

/**
	客户端发送消息线程与接收消息线程通信数据结构
*/
struct ClientDataStruct
{
	// socket地址
	int nConnectedSocket;
	// ipv4地址结构
	sockaddr_in socketAddr;
	// 好友ip集合
	std:: vector<std:: string>* friendIPVector;
	// 消息条数
	int count;
	//文本计时器
	std:: map<int, time_t> textClockMap;
	// 发送消息的历史记录
	std:: map<std:: string, std:: map<int, std:: string>* >  sendMessageHistoryMap;
	//收到消息的历史记录
	std:: map<std:: string, std:: map<int, std:: string>* >  receiveMessageHistoryMap;
};


/**
	刷新聊天屏幕
*/
void* refreshScreen(std:: map<std:: string, std:: map<int ,std:: string>* >  sendMessageHistoryMap,
		std:: map<std:: string, std:: map<int ,std:: string>* >&  receiveMessageHistoryMap)
{

	system("clear");
	std:: cout << "\t\t--------请输入消息(\\q退出聊天，\\r撤回消息)--------" 
		<< std:: endl << std:: endl;
	// 是否应该输出区别各个好友的分隔线
	bool needOutputDivisor = false;
	std:: string offlineIP = "";

	// 将接收消息重新打印到屏幕
	for(std:: map<std:: string, std:: map<int, std:: string>* >:: iterator iter1 = 
		receiveMessageHistoryMap.begin(); iter1 != receiveMessageHistoryMap.end(); iter1++)
	{
		std:: map<int, std:: string>* _map = iter1 -> second;
		for(std:: map<int, std:: string>:: iterator iter2 = _map -> begin(); 
			iter2 != _map -> end(); iter2++)
		{
			if("\\d" == iter2 -> second)
			{
				offlineIP = iter1 -> first;
				std:: cout << iter1 -> first << " 已经离线，将清除消息记录！ "  << std:: endl;
			}
			else
			{
				std:: cout << iter1 -> first << " >> " << iter2 -> first << ". " << iter2 -> second 
				<< std:: endl;
			}

			needOutputDivisor = true;
		} 
		if(needOutputDivisor)
			std:: cout << "----------------------------" << std:: endl;
		needOutputDivisor = false;
	}

	// 将发送消息重新打印到屏幕
	for(std:: map<std:: string, std:: map<int, std:: string>* >:: iterator iter1 = 
		sendMessageHistoryMap.begin(); iter1 != sendMessageHistoryMap.end(); iter1++)
	{
		std:: map<int, std:: string>* _map = iter1 -> second;
		for(std:: map<int, std:: string>:: iterator iter2 = _map -> begin(); 
			iter2 != _map -> end(); iter2++)
		{
			std:: cout << "\t\t\t\t" << iter2 -> first 
				<< ". " << iter2 -> second << " >> "  << iter1 -> first << std:: endl;

			needOutputDivisor = true;
		} 
		if(needOutputDivisor)
			std:: cout << "\t\t\t\t----------------------------" << std:: endl;  

		needOutputDivisor = false;
	}

	// 清除聊天记录
	if("" != offlineIP)
	{
		for(std:: map<std:: string, std:: map<int, std:: string>* >:: iterator iter1 = 
			receiveMessageHistoryMap.begin(); iter1 != receiveMessageHistoryMap.end(); iter1++)
		{
			if(offlineIP == iter1 -> first)
			{
				receiveMessageHistoryMap.erase(iter1);
				delete iter1 -> second;

				offlineIP = "";
				break;
			}
		}
	}
}

/**
	客户端发送消息线程运行函数
*/
void*  RunSenderFunction(void* pContext)
{
	// 字符串拼接器
	std:: ostringstream oss;

	struct ClientDataStruct* cds = (struct ClientDataStruct*)pContext;
	// 消息文本
	std:: string text, content;

	while(text != "\\q")
	{
		// 清空字符串拼接器中的数据
		oss.str("");
		// 输入待发送的信息
		getline(std:: cin, text);

		if("\\r" != text || "\\q" == text)
		{ // 聊天
			cds -> count += 1;

			// 将群发给好友的信息，拼接成文本字符串，交给服务器去解析转发
			for(short i = 0; i < cds -> friendIPVector -> size(); i++)
			{
				// 好友ip
				std:: string ip = (*(cds -> friendIPVector))[i];

				// 添加发送消息历史记录
				if(cds -> sendMessageHistoryMap.find(ip) == cds -> sendMessageHistoryMap.end())
				{
					cds -> sendMessageHistoryMap[ip] = new std:: map<int, std:: string>;
				}
				(*(cds -> sendMessageHistoryMap[ip]))[cds -> count] = text;
				// 客服端与服务端的通信协议为：1\.ip\:content\#
				oss << cds -> count << "\\." << ip << "\\:" << text << "\\#";
			}
			
			// 第 count 条消息发出去的时间
			cds -> textClockMap[cds -> count] = time(0);
		}
		else
		{ // 撤回消息
			if(1 > cds -> count)
			{
				std:: cout << "没有消息可以撤回" << std:: endl;

				continue;
			}
			else
				std:: cout << "请输入撤回第几条消息（" << 1 << " - " << cds -> count << "）:";

			char indexString[4];
			int index = 0;

			while(1 > index)
			{  // 为了代码的健壮性，必须确保客户端输入的是整数
				std:: cin >> indexString;
				index = Utils:: isIntegerNum(indexString);
			}
			// 接收输入index产生的多余的 回车换行
			std:: cin.get();

			if(index > cds -> count)
			{
				std:: cout << "没有消息 " << index << " 的历史记录不能撤回" << std:: endl;

				continue;
			}

			// 消息超过六十秒后不能撤回
			if((time(0) - (cds -> textClockMap[index])) > 60)
			{
				std:: cout << "消息发出时间已经超过1分钟，不能撤回" << std:: endl;

				continue;
			}

			for(short i = 0; i < cds -> friendIPVector -> size(); i++)
			{ // 对一个好友都发送撤回消息
				std:: string ip = (*(cds -> friendIPVector))[i];

				// 撤回消息后，更改发送消息历史记录
				for(std:: map<int, std:: string>:: iterator iter = cds -> sendMessageHistoryMap[ip] -> begin(); 
					iter != cds -> sendMessageHistoryMap[ip] -> end(); iter++)
				{
					if(iter -> first == index) // 删除一条历史记录并修改以前各个消息记录的下标
					{
						int size = cds -> sendMessageHistoryMap[ip] -> size();
						for(;index < size;  iter++, index++)
						{
							// 消息下标左移
							(*(cds -> sendMessageHistoryMap[ip]))[iter -> first] = (*(cds ->sendMessageHistoryMap[ip]))[iter -> first + 1];

							// 消息时间计数器左移
							cds -> textClockMap[iter -> first] = cds -> textClockMap[iter -> first + 1];
						}	

						cds -> sendMessageHistoryMap[ip] -> erase(iter); 
						(cds -> textClockMap).erase(iter -> first);
					}
				}
				// 客服端与服务端的通信协议为：1\.ip\:content\#
				oss << index << "\\." << ip << "\\:\\r" << "\\#";
			}
			// 总消息数减一
			cds -> count = cds -> count - 1;

		}// 撤回消息

		// 发送消息到服务器
		content = oss.str();
		::write(cds -> nConnectedSocket, content.c_str(), content.length());

		if("\\q" != text) // 除了发送退出消息，每次发送呢消息都要刷新屏幕
			refreshScreen(cds -> sendMessageHistoryMap, cds -> receiveMessageHistoryMap);

	} // while(text != "\\q")

	// 释放及发送消息历史记录所占用的资源
	for (std:: map<std:: string, std:: map<int, std:: string>* > 
			::iterator iter1 = cds -> sendMessageHistoryMap.begin(); 
			iter1 != cds -> sendMessageHistoryMap.end(); iter1++)
	{
		delete iter1 -> second;
	}

	// 释放及发送消息历史记录所占用的资源
	for (std:: map<std:: string, std:: map<int, std:: string>* > 
			::iterator iter1 = cds -> receiveMessageHistoryMap.begin(); 
			iter1 != cds -> receiveMessageHistoryMap.end(); iter1++)
	{
		delete iter1 -> second;
	}

	// 客户端发送退出消息后强制退出程序
	exit(0);
}

/**
	客户端接收线程运行函数
*/
void* RunReceiverFunction(void* pContext)
{
	struct ClientDataStruct* cds = (struct ClientDataStruct*)pContext;

	// 读服务器消息缓冲区
	char buff[1024];
	// 读取的服务器消息的长度
	int len = 0;
	
	// 协议符号位置
	int dotPosition, colonPosition, hashtagPosition;
	// 发送者ip
	std:: string receiverIP;
	// 消息的下标
	int index;
	// 接收到的服务器的文本
	std:: string content, text, count;

	std:: stringstream ss;

	while(true)
	{	
		// 从服务端读取消息，将其放入到缓冲区中
		len = ::read(cds -> nConnectedSocket, buff, sizeof(buff));

		if(len > 0)
			text.assign(&buff[0], &buff[len]); // 将收到的字符信息转换为字符串

		while("" != text)
		{
			// 清空转换器
			ss.str("");
			ss.clear();	

			// 获取协议符号位置
			dotPosition = text.find("\\.");
			colonPosition = text.find("\\:");
			hashtagPosition = text.find("\\#");

			count = text.substr(0, dotPosition);
			ss << count;
			ss >> index;
			// 收信方ip
			receiverIP = text.substr(dotPosition + 2, colonPosition - dotPosition - 2);
			content = text.substr(colonPosition + 2, hashtagPosition - colonPosition - 2);

			if(cds -> receiveMessageHistoryMap.find(receiverIP) 
					== cds -> receiveMessageHistoryMap.end())
			{	// 历史记录中还没有收录ip对应的客户端发送过来的消息
				cds -> receiveMessageHistoryMap[receiverIP] = new std:: map<int, std:: string>;
			}

			if("\\r" == content)
			{// 撤回消息
				for(std:: map<int, std:: string>:: iterator iter = cds -> receiveMessageHistoryMap[receiverIP] -> begin(); 
					iter != cds -> receiveMessageHistoryMap[receiverIP] -> end(); iter++)
				{
					if(iter -> first == index) // 删除一条历史记录并修改以前各个消息记录的下标
					{
						int size = cds -> receiveMessageHistoryMap[receiverIP] -> size();
						for(;index < size;  iter++, index++)
							// 消息下标左移
							(*(cds -> receiveMessageHistoryMap[receiverIP]))[iter -> first] 
								= (*(cds ->receiveMessageHistoryMap[receiverIP]))[iter -> first + 1];

						cds -> receiveMessageHistoryMap[receiverIP] -> erase(iter); 
					}
				}
			} // if(content == "\\r")
			else 
			{ // 普通消息
				(*(cds -> receiveMessageHistoryMap[receiverIP]))[index] = content;
			}
			// 刷新屏幕
			refreshScreen(cds -> sendMessageHistoryMap, cds -> receiveMessageHistoryMap);

			text = text.substr(hashtagPosition + 2);
		} // while("" != text)

	} // while(content != "\\q")
}

ChatTCPClientObserver:: ChatTCPClientObserver()
{
}

ChatTCPClientObserver:: ~ChatTCPClientObserver()
{
}

void ChatTCPClientObserver:: ClientFunction(int nConnectedSocket, sockaddr_in socketAddr, std:: vector<std:: string>* friendIPVector)
{
	struct ClientDataStruct* cds = new ClientDataStruct;
	cds -> nConnectedSocket = nConnectedSocket;
	cds -> socketAddr = socketAddr;
	cds -> friendIPVector = friendIPVector;
	cds -> count = 0;

	
	// 发送消息线程
	pthread_t tid1; 
	pthread_create(&tid1, NULL, RunSenderFunction, (void*)cds);

	// 刷新屏幕
	refreshScreen(cds -> sendMessageHistoryMap, cds -> receiveMessageHistoryMap);

	//接收消息线程
	pthread_t tid2; 
	pthread_create(&tid2, NULL, RunReceiverFunction, (void*)cds);
	
	pthread_join(tid1, 0);
	pthread_join(tid2, 0);

	delete cds;
}

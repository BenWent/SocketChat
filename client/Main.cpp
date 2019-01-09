#include <iostream>
#include <vector>
#include <string>

#include "CPluginController.h"
#include "Config.h"
#include "Utils.h"
#include "ChatTCPClientObserver.h"
#include "CTCPClient.h"

using namespace std;

int main()
{

	CPluginController pluginController;
	Config config("config.txt");
	int pluginID;
	pluginID = config.Read("pluginID", pluginID);
	pluginController.ProcessRequest(pluginID);


	// 给多少ip地址发送消息
	int friendsNum = 0;

	// 确保用户输入的用户数量是 正整数且大于1
	while(friendsNum < 1)
	{
		cout << "请输入要通信的用户数(>=1):";
		char friendsNumString[4];
		cin >> friendsNumString;

		friendsNum = Utils:: isIntegerNum(friendsNumString);
		cin.ignore(1024, '\n');
	}

	// 统计用户输入的对方ip
	vector<string> friendIPVector;
	// 统计用户输入的个合法ip数量
	int ipCount = 1;
	while(ipCount <= friendsNum)
	{
		cout << "请输入第 " << ipCount << " 个ip:";
		string ip;
		getline(cin, ip);

		const char* ipArray = ip.c_str();
		if(Utils:: isIPAddressValid(ipArray))
		{
			ipCount++;
			friendIPVector.push_back(ip);
		}
		else
		{
			cout << "你输入的ip:" << ip << " 不合法，请重新输入" << endl;
		}
	}

	ChatTCPClientObserver observer;
    CTCPClient client(&observer, &friendIPVector);
    
    client.Run();

	return 0;
}

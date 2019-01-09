#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "IUIPlugin.h"

using namespace std;

const int PLUGIN_ID = 2;


void* printCharacter(void* param)
{
	bool* isExit = (bool*)param;

	char twinkleChars[] = ">>>>>";
	char* p = twinkleChars;

	setbuf(stdout, NULL);
	while(!(*isExit))
	{
		while('\0' != *p && !(*isExit))
		{
			usleep(300000);
			cout << *p << " ";

			p++;
		}

		while(twinkleChars != p && !(*isExit))
		{
			cout << "\b\b";
			cout << " ";
			cout << "\b";

			p--;
		}
	}

	system("clear");
}

void* enterCharactr(void* param)
{
	bool* isExit = (bool*)param;

	while(cin.get() != '\n');
	*isExit = true;
}

extern "C" class UIPlugin1 : public IUIPlugin
{
public:
	UIPlugin1()
	{
	}

	virtual ~UIPlugin1()
	{
	}

	virtual void UI()
	{
		system("clear");

		cout << "\t\t\t------------------------------------" << endl;
		cout << "\t\t\t----\t\t\t\t" << "----" << endl;
		cout << "\t\t\t----\t\t\t\t" << "----" << endl;
		cout << "\t\t\t\t    " ;

		// 将标准输出流的缓存置为空
		setbuf(stdout, NULL);

		char title[] = "简单的聊天程序";
		for(short i = 0; title[i] != '\0'; i++)
		{
			putchar(title[i]);
			usleep(50000);
		}
		cout << endl;

		cout << "\t\t\t----\t\t\t\t" << "----" << endl;
		cout << "\t\t\t----\t\t\t\t" << "----" << endl;
		cout << "\t\t\t------------------------------------" << endl  << endl;
		cout << "\t\t\t\t";

		char start[] ="按 Enter 开始聊天 ";
		for(short i = 0; start[i] != '\0'; i++)
		{
			putchar(start[i]);
			usleep(50000);
		}

		bool isExit = false;

		pthread_t t1;
		pthread_create(&t1, NULL, printCharacter, (void*)&isExit);

		pthread_t t2;
		pthread_create(&t2, NULL, enterCharactr, (void*)&isExit);

		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
	}

	virtual void GetHelp()
	{
		cout << "Plugin ID " << PLUGIN_ID << " : This plugin will show you a simple UI." << endl;
	}

	virtual int GetID(void)
	{
		return PLUGIN_ID;
	}
};

extern "C" void CreateObj(IUIPlugin **ppPlugin)
{
	static UIPlugin1 plugin;
	*ppPlugin = &plugin;
}

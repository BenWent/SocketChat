#include <iostream>
#include <stdlib.h>

#include "IUIPlugin.h"

using namespace std;

const int PLUGIN_ID = 1;

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
		cout << "\t\t\t\t    简单的聊天程序" << endl;
		cout << "\t\t\t----\t\t\t\t" << "----" << endl;
		cout << "\t\t\t----\t\t\t\t" << "----" << endl;
		cout << "\t\t\t------------------------------------" << endl  << endl;
		cout << "\t\t\t\t按 Enter 开始聊天 >>>";
		while(cin.get() != '\n');

		system("clear");
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

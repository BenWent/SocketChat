#include <dirent.h>
#include <string.h>
#include <iostream>
#include <sstream>

#include "CPluginEnumerator.h"
#include "Config.h"

CPluginEnumerator::CPluginEnumerator()
{
}

CPluginEnumerator::~CPluginEnumerator()
{
}

bool CPluginEnumerator::GetPluginNames(vector<string>& vstrPluginNames)
{
	// 从配置文件中读取插件的存放位置
	Config config("./config.txt");
	std:: string pluginPosition;
	pluginPosition = config.Read("pluginPosition", pluginPosition);
	pluginPosition = "./" + pluginPosition;

	// 打开插件所在目录
    DIR *dir = opendir(pluginPosition.c_str());
    if(NULL == dir) // 打开插件目录失败
		return false;
    
    for(;;)
    { 
		struct dirent *pentry = readdir(dir);
		if(pentry == 0)
		    break;

		if(strcmp(pentry->d_name, ".") == 0)
		    continue;

		if(strcmp(pentry->d_name, "..") == 0)
		    continue;

		string str = pluginPosition + "/";
		// 拼接得到插件名的相对路径
		str += pentry->d_name;

		vstrPluginNames.push_back(str);
    }

    closedir(dir);

    return true;
}

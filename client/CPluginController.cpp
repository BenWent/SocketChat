#include <dlfcn.h>
#include <iostream>

#include "CPluginController.h"
#include "CPluginEnumerator.h"
#include "IUIPlugin.h"

CPluginController::CPluginController()
{
	InitializeController();
}

CPluginController::~CPluginController()
{
	UninitializeController();
}

bool CPluginController:: InitializeController()
{
	// 插件名容器
	std:: vector<std:: string> vstrPluginNames;

	// 将所有插件名装入内存
	CPluginEnumerator enumerator;
	if(!enumerator.GetPluginNames(vstrPluginNames))
		return false;

	for(short i = vstrPluginNames.size() - 1 ; i >= 0; i--)
	{ // 遍历插件名
		typedef int (*PLUGIN_CREATE)(IUIPlugin**);
		PLUGIN_CREATE CreateProc; 

		// 插件顶级接口
		IUIPlugin *pPlugin = NULL;

		// 以懒加载的方式加载插件
		void* hinstLib = dlopen(vstrPluginNames[i].c_str(), RTLD_LAZY); 
		
		if(NULL == hinstLib)
		{ // 打开动态链接库失败
			std:: cout << " open error" << dlerror() << std::endl;
		}
		else
		{
			m_vhForPlugin.push_back(hinstLib);
			// 得到动态加载库中的 CreateObj函数的句柄
			CreateProc = (PLUGIN_CREATE)dlsym(hinstLib, "CreateObj"); 

			if(NULL != CreateProc) 
			{
				(CreateProc)(&pPlugin);

				if(NULL != pPlugin)
				{
					m_vpPlugin.push_back(pPlugin);
				}
			} // NULL != CreateProc
		}
	}

	return true;
}

bool CPluginController::ProcessRequest(int id)
{
	for(short i = 0; i < m_vpPlugin.size(); i++)
	{
		if(m_vpPlugin[i] -> GetID() == id)
		{
			m_vpPlugin[i] -> UI();

			break;
		}
	}

	return true;
}

bool CPluginController:: ProcessHelp(int id)
{

	for(short i = 0; i < m_vpPlugin.size(); i++)
	{
		if(m_vpPlugin[i] -> GetID() == id)
		{
			m_vpPlugin[i] -> GetHelp();

			break;
		}
	}

	return true;
}

bool CPluginController::UninitializeController()
{
	for(short i = 0; i < m_vhForPlugin.size(); i++)
	{
		dlclose(m_vhForPlugin[i]);
	}

	return true;
}

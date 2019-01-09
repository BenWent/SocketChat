#ifndef CPLUGINCONTROLLER_H
#define CPLUGINCONTROLLER_H

#include <vector>

class IUIPlugin;

/**
	插件控制器类
*/
class CPluginController
{
public:
	CPluginController();
	virtual ~CPluginController();

public:
	/**
		处理帮助请求
	*/
	bool ProcessHelp(int id);
	/**
		处理功能调用请求
		@Parameters:
			id：功能编号
	*/
	bool ProcessRequest(int id);

private:
	/**
		初始化插件控制器
	*/
	bool InitializeController();
	/**
		释放插件控制器占用的资源
	*/
	bool UninitializeController();

private:
	// 插件容器
	std::vector<void* > m_vhForPlugin;
	// 插件功能容器
	std::vector<IUIPlugin* > m_vpPlugin;
};

#endif

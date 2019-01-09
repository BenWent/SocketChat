#pragma once

/**
	插件功能的顶级接口
*/
extern "C" class IUIPlugin
{
public:
	IUIPlugin();
	virtual ~IUIPlugin();

	/**
		插件帮助信息
	*/
	virtual void GetHelp() = 0;
	/**
		用户显示的UI
	*/
	virtual void UI() = 0;
	/**
		获得插件的唯一标识符
	*/
	virtual int GetID() = 0;
};

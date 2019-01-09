#ifndef UTILS_H
#define UTILS_H

#include <string>

class Utils
{
public:
	/**
		@Summary: 判断一个字符串是否是整数类型的字符串
		@Parameters:
			temp: 待检测的字符串
		@Return:如果字符串代表的是一个整数，返回该字符串转换成的整数，如果不是一个整数字符串返回-1
	*/
	static int isIntegerNum(char* temp);

	/**
		@Summary:判断IPv4地址是否有效 
		@Parameters:
			srcIP:待判断的字符串是否是有效的ip地址

		@Return:有效的IPv4地址返回true，无效的IPv4地址返回false
		例如： 
			1.输入IP为XXX.XXX.XXX.XXX格式
			2.字符串两端含有空格认为是合法IP
			3.字符串中间含有空格认为是不合法IP
			4.类似于 01.1.1.1， 1.02.3.4  IP子段以0开头为不合法IP
			5.子段为单个0 认为是合法IP，0.0.0.0也算合法I
	*/
	static bool isIPAddressValid(const char* srcIP);

};


#endif

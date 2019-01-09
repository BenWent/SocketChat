#include <sstream>
#include <iostream> 
#include <fstream>
#include <cstring>

#include "Utils.h"

int Utils:: isIntegerNum(char* temp)
{
	if(temp == NULL)
		return -1;
	
	std:: stringstream ss;
	ss << temp;

	int	integerNum = 0;
	if(ss >> integerNum) // 判断输入的是否整数
	{
		char c;
		if(!(ss >> c)) // 判断输入的不是字符
		{
			return integerNum;
		}
	}
	return -1;
}

bool Utils::  isIPAddressValid(const char* srcIP)
{
    // ip字符串不能为空
    if (!srcIP) 
        return false; 
 
    int i = 0, j = strlen(srcIP) - 1; 
    //去除首尾空格(取出从i-1到j+1之间的字符):  
    while (srcIP[i++] == ' ');  
    while (srcIP[j--] == ' ');  
    
    char destIP[100]; 
    int index = 0;  
    for (char k=i-1, temp='\0'; k<=j+1; k++)  
    {  
        temp = *(srcIP + k);
        // 合法的ip地址只有两种种字符:数字字符和'.'
        if(!(('0' <= temp && temp <= '9') || temp == '.'))
            return false;

        destIP[index++] = temp;
    }       
    destIP[index] = '\0';  
      
    char *p = destIP;
    // ip字符串的字段的首字符
    char firstCharacter;
    // ip字符串中'.'的个数
    int dotNum=0;
  
    while (*p != '\0')  
    {  
        firstCharacter = *p;
  
        // 为每一子段的数值，应在0到255之间  
        int sum = 0;  
        while (*p != '.' && *p != '\0')  
        {  
          sum = sum * 10 + *p  - 48;
          p++;  
        }  

        if (*p == '.') 
        {   
            // 判断"."前后是否是数字
            if (
                    (p != destIP) && 
                    (*(p - 1) >= '0' && *(p - 1) <= '9') && 
                    (*(p + 1) >= '0'&&*(p + 1) <= '9')
                )
                dotNum++;   
            else  
                return false;  
        }

        if((sum > 255) || (sum > 0 && firstCharacter == '0') || dotNum > 3) 
            return false;
  
        if (*p != '\0') 
            p++;  
    }  
    if (dotNum != 3) 
        return false;

    return true;
}

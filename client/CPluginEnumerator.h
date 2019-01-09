#ifndef CPLUGINENUMERATOR_H
#define CPLUGINENUMERATOR_H

#include <vector>
#include <string>

using namespace std;

/**
	加载插件类
*/
class CPluginEnumerator
{
public:
    CPluginEnumerator();
    virtual ~CPluginEnumerator();

    bool GetPluginNames(vector<string>& vstrPluginNames);
};

#endif

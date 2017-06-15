#include "RegPool.h"


RegPool::RegPool()
{
	for(int i=0;i<18;i++)
	{
		string name = (i<10) ? "t" : "s";
		int index = (i<10) i : i -10;
		pool["$"+name+string(index)] = false;
			
	}
}

const string RegPool::getReg()
{
	for(map<string,bool>::iterator& ite=pool.begin(); ite != pool.end();ite++)
	{
		if(false == ite->second)
		{
			pool[ite->first]= true;
			return ite->first;
		}
		
		assert(false);
		return "";
	}
}

void RegPool::freeReg(const string& regKey)
{
	pool[regKey] = false;
}
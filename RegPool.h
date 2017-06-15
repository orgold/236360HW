#ifndef _REGPOOL_H_
#define _REGPOOL_H_

#include <map>
#include <string>
using std::string;
using std::map;

class RegPool {
map<string,bool> pool;

public:
	RegPool();
	const string getReg();
	void freeReg(const string&);
};

#endif
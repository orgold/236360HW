#include "hw4_table_utils.h"
#include <assert.h>
void SymbolTable::insertFunction(
	const string& name,
	const vector<TYPE>& paramTypes,
	TYPE retType,
	const vector<string>& paramNames)
{
	if (functionMap.find(name) != functionMap.end())
		throw AlreadyDefinedException(name);
	FunctionData funcData(retType,paramTypes);
	functionMap[name] = funcData;
	insertScope();
	for(int i = 0; i < paramTypes.size(); i++ ){
		if (functionMap.find(paramNames[i]) != functionMap.end())
			throw AlreadyDefinedException(paramNames[i]);
		scopeStack.back().insertParam(
			paramNames[i],
			paramTypes[i],
			-1*(i+1)
		);
	}
}

void SymbolTable::insertScope()
{
	int currentOffset = 
		(scopeStack.size() == 0) ? 0 : scopeStack.back().getCurrentOffset();
	scopeStack.push_back(ScopeTable(currentOffset));
}


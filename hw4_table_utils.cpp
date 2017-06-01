#include "hw4_table_utils.h"
#include <assert.h>
void SymbolTable::insertFunction(
	const string& name,
	const vector<TYPE>& paramTypes,
	TYPE retType,
	const vector<string>& paramNames)
{
	checkNameNotDefinedAsFunction(name);
	
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

void SymbolTable::removeScope()
{
	assert(scopeStack.size() != 0);
	scopeStack.pop_back();
}

void SymbolTable::insertVar(const string& name, TYPE type)
{
	//check that it won't shadow
	checkNameNotDefinedAsFunction(name);
	checkNameNotDefinedAsVar(name);
	assert(scopeStack.size() != 0);
	scopeStack.back().insertVar(name, type);
}

void SymbolTable::checkNameNotDefinedAsFunction(const string& name)
{
	if (functionMap.find(name) != functionMap.end())
		throw AlreadyDefinedException(name);
}

void SymbolTable::checkNameNotDefinedAsVar(const string& name)
{
	for(auto& sTable : scopeStack)
	{
		if(sTable->isVarInScope(name))
			throw AlreadyDefinedException(name);
	}
}
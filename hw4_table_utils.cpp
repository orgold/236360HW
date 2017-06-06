#include "hw4_table_utils.h"

void SymbolTable::insertFunction(
	const string& name,
	const vector<TYPE>& paramTypes,
	TYPE retType,
	const vector<string>& paramNames)
{
	checkNameNotDefinedAsFunction(name);
	
	FunctionData funcData={retType,paramTypes};
	functionMap[name] = funcData;
	funcsByOrderArrival.push_back(name);
	insertScope();
	for(int i = 0; i < paramTypes.size(); i++ ){
		if (functionMap.find(paramNames[i]) != functionMap.end()){
			std::cout<< paramNames[i] << "thrown for funcName" << std::endl;
			throw AlreadyDefinedException(paramNames[i]);
		}
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
	endScope();
	scopeStack.back().sendToPrint();
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
	for(std::vector<ScopeTable>::iterator sTable = scopeStack.begin() ; sTable!= scopeStack.end();sTable++)
	{
		if(sTable->isVarInScope(name))
			throw AlreadyDefinedException(name);
	}
}
SymbolTable::SymbolTable()
{
	functionMap["print"] = {VOID,{STRING}};
	functionMap["printi"] = {VOID,{INT}};
}
TYPE SymbolTable::getType(string name,TYPE& typi)
{
	for(std::vector<ScopeTable>::iterator sTable = scopeStack.begin() ; sTable!= scopeStack.end();sTable++)
	{
		if(sTable->isVarInScope(name))
			return sTable->getType(name);
	}
	throw errorUndefException(name);
}
SymbolTable::~SymbolTable()
{
	
	
	for (std::vector<string>::iterator ite=funcsByOrderArrival.begin();ite!=funcsByOrderArrival.end();ite++)
	{
		vector<const char*> arrangeForPrint(functionMap[*ite].paramTypeList.size());
		std::transform(
			functionMap[*ite].paramTypeList.begin(),
			functionMap[*ite].paramTypeList.end(),
			arrangeForPrint.begin(),
			sendToPrint
		);
		printID(
			ite->c_str(),
			0,
			(makeFunctionType(functionMap[*ite].returnType,arrangeForPrint)).c_str()
		);
	}
		
}

int main()
{
	try {
	SymbolTable st;
	
	vector<TYPE> vecParmList ={INT,BOOL,BYTE};
	vector <string> vecStrList={"i","boo","byt"};
	st.insertFunction("foo",vecParmList,INT,vecStrList);
	st.insertVar("x",INT);
	
	st.insertScope();
	st.insertVar("y",INT);
	st.insertVar("z",INT);
	st.removeScope();
	st.insertVar("y",INT);
	st.removeScope();
	st.insertFunction("foo2",vecParmList,INT,vecStrList);
	st.insertVar("x",INT);
	

}
catch(AlreadyDefinedException & e)
{
	std::cout<<"exe!!!!!!! for "<<e.id<<std::endl;
}
}
#include "hw4_table_utils.h"


const char* toCString(TYPE type)
{
	switch(type){
		case(INT_T):
			return "INT";
		case(BOOL_T):
			return "BOOL";
		case(BYTE_T):
			return "BYTE";
		case(STRING_T):
			return  "STRING";
		case(VOID_T):
			return "VOID";
	}
}

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
			throw errorDefException(paramNames[i]);
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
		throw errorDefException(name);
}

void SymbolTable::checkNameNotDefinedAsVar(const string& name)
{
	for(std::vector<ScopeTable>::iterator sTable = scopeStack.begin() ; sTable!= scopeStack.end();sTable++)
	{
		if(sTable->isVarInScope(name))
			throw errorDefException(name);
	}
}
SymbolTable::SymbolTable()
{
	struct FunctionData fooData;
	//add print
	fooData.returnType = VOID_T;
	fooData.paramTypeList.push_back(STRING_T);
	
	functionMap["print"] = fooData;
	
	//add printi
	fooData.paramTypeList.push_back(INT_T);
	functionMap["printi"] = fooData;
}

TYPE SymbolTable::getType(string name)
{
	for(std::vector<ScopeTable>::iterator sTable = scopeStack.begin() ; sTable!= scopeStack.end();sTable++)
	{
		if(sTable->isVarInScope(name))
			return sTable->getType(name);
	}
	throw errorUndefException(name);
}


TYPE SymbolTable::checkFuncTypeAndArgs(string id, TYPE* typeList,int size)
{
	if(functionMap.find(id)==functionMap.end()) throw errorUndefFuncException(id);
	FunctionData savedFuncData = functionMap[id];
	if(size != savedFuncData.paramTypeList.size())  throw errorPrototypeMismatchException(id,savedFuncData.paramTypeList);
	for(int i=0;i<size;i++)
	{
		if(typeList[i]!=savedFuncData.paramTypeList[size - 1- i]) 
			throw errorPrototypeMismatchException(id,savedFuncData.paramTypeList);
	}
	return savedFuncData.returnType;
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
			toCString
		);
		printID(
			ite->c_str(),
			0,
			(makeFunctionType(toCString(functionMap[*ite].returnType),arrangeForPrint)).c_str()
		);
	}
		
}

void SymbolTable::coverInsertFunction(const string& name,TYPE* typeList,int paramNum, TYPE retType, char** paramNames)
{
	vector<TYPE> types;
	vector<string> names;
	for(int i=paramNum ; i >0; i--){
		types.push_back(typeList[i-1]);
		names.push_back(paramNames[i-1]);
	}
	insertFunction(name,types,retType, names);
}

/*
int main()
{
	try {
		SymbolTable st;
		
		vector<TYPE> vecParmList ={INT_T,BOOL_T,BYTE_T};
		vector <string> vecStrList={"i","boo","byt"};
		st.insertFunction("foo",vecParmList,INT_T,vecStrList);
		st.insertVar("x",INT_T);
		
		st.insertScope();
		st.insertVar("y",INT_T);
		st.insertVar("z",INT_T);
		st.removeScope();
		st.insertVar("y",INT_T);
		st.removeScope();
		st.insertFunction("foo2",vecParmList,INT_T,vecStrList);
		st.insertVar("x",INT_T);
		

	}
	catch(AlreadyDefinedException & e)
	{
		std::cout<<"exe!!!!!!! for "<<e.id<<std::endl;
	}
}
*/
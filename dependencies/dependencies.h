//////////////////////////////////////////////////////////////////////////////////////////////
// dependencies.h - Support file and directory operations						            //
// ver 1.1																			        //				
// Platform:      Dell Intel Core i3, Win10									     		    //
// Application:   Object Oriented Design - Project #1, Spring 2019                          //
// Author:        Arva Kagdi                                                                //
//////////////////////////////////////////////////////////////////////////////////////////////

/*
* Module Operations:
* ==================
* This module checkes the dependency for each file
* It creates a dependency table and a info table where it checks for the dependency
* It also looks for the type  of node wether it is a class or function
*
* Required files :
* ==================
* ConfigureParser.h, ConfigureParser.cpp
* Parser.h, Parser.cpp
* ActionsAndRules.h, ActionsAndRules.cpp
* AbstrSynTree.h, AbstrSynTree.cpp
* ScopeStack.h, ScopeStack.cpp
* Utilities.h, Utilities.cpp
*
* Build commands
* ===================
* - devenv OODProject2.sln
*
* Maintenance History:
* ===================
* ver 1.1 : 3/20/2019
* ver 1.0 : 2/14/2019
* first release
*/


#pragma once
#include <vector>
#include <map>
#include "../CppParser/Parser/ConfigureParser.h"
#include "../CppParser/Parser/Parser.h"
#include "../CppParser/Parser/ActionsAndRules.h"
#include "../CppParser/AbstractSyntaxTree/AbstrSynTree.h"
#include "../CppParser/ScopeStack/ScopeStack.h"
#include "../CppParser/Utilities/Utilities.h"
#define Util StringHelper

//<----creating class dependencies---->//
class dependencies
{
public:
	dependencies();
	~dependencies();
	enum TypeInfo { none, classes, function, end, singleLineFn };
	std::map<std::string, std::vector<std::string>> summonParser(std::vector<std::string> files);
	void analyzeDepT(std::map < std::string, std::vector<std::string>> & depT);
	void DepTable(CodeAnalysis::ASTNode* pGlobalScope, std::string fileSpec, std::map<std::string, std::vector<std::string>>& dependencyMap);
	void newInfoTable(CodeAnalysis::ASTNode * pGlobalScope, std::string file);
	TypeInfo findTypeInfo(CodeAnalysis::ASTNode* pGlobalScope);
	std::map<std::string, std::map<std::size_t, dependencies::TypeInfo>> returnTypeInfoTable() { return typeInfoTable_; }

private:
	std::map<std::string, std::map<std::size_t, dependencies::TypeInfo>> typeInfoTable_;
};


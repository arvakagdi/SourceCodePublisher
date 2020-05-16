//////////////////////////////////////////////////////////////////////////////////////////////
// dependency.cpp - checks the dependency of each file                                      //
// ver 1.1																			        //				
// Platform:       Dell Intel Core i3, Win10                                                //
// Application:    Object Oriented Design - Project #1, Spring 2019                         //
// Author:         Arva Kagdi                                                               //
//////////////////////////////////////////////////////////////////////////////////////////////

#include "dependencies.h"

using namespace CodeAnalysis;
using namespace Utilities1;

dependencies::dependencies()
{
}


dependencies::~dependencies()
{
}

std::map<std::string, std::vector<std::string>> dependencies::summonParser(std::vector<std::string> files)
{
	std::string fileSpec;
	std::map<std::string, std::vector<std::string>> dependencyMap;

	//-----< Parses through the files >------//
	for (size_t i = 0; i < files.size(); ++i)  
	{
		fileSpec = FileSystem::Path::getFullFileSpec(files[i]);
		std::string msg = "Processing file" + fileSpec;
		Util::title(msg);

		ConfigParseForCodeAnal configure;
		Parser* pParser = configure.Build();

		std::string name;

		try
		{
			if (pParser)
			{
				name = FileSystem::Path::getName(files[i]);
				if (!configure.Attach(fileSpec))
				{
					std::cout << "\n  could not open file " << name << std::endl;
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return std::map<std::string, std::vector<std::string>>();
			}

			//-----< Saves current package name to  repository >------//
			Repository* pRepo = Repository::getInstance();
			pRepo->package() = name;

	
			//-----< Parses through the package untill empty >------//
			while (pParser->next())
			{
				pParser->parse();
			}
			std::cout << "\n";

			//-----< AST operation is performed here. It computes the complelxity for  every node and records in AST >------//
			ASTNode* pGlobalScope = pRepo->getGlobalScope();

			DepTable(pGlobalScope, fileSpec,dependencyMap);
			newInfoTable(pGlobalScope, fileSpec);

			analyzeDepT(dependencyMap);

			complexityEval(pGlobalScope);
		
		//-----< DIsplays element >------//
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
			std::cout << "\n  exception caught at line " << __LINE__ << " ";
			std::cout << "\n  in package \"" << name << "\"";
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	return dependencyMap;
}

//-----< Analayzing dependency table >------//
void dependencies::analyzeDepT(std::map<std::string, std::vector<std::string>>& depTmap)
{
	if (depTmap.size() == 0)
		return;
	auto iter = depTmap.begin();
	while (iter != depTmap.end()) {
		if (iter->second.size() != 0) {
			for (size_t i = 0; i < iter->second.size(); i++) {
				auto found = iter->second[i].find_last_of("/");
				auto fquote = iter->second[i].find_last_of("\"");
				if (found == iter->second[i].npos) {
					auto quote1 = iter->second[i].find_first_of("\"");
					iter->second[i] = iter->second[i].substr(quote1 + 1, fquote - quote1 - 1);
				}
				iter->second[i] = iter->second[i].substr(found + 1, fquote - found - 1);
			}
		}
		iter++;
	}
}

//-----< We create the dependency table here >------//
void dependencies::DepTable(CodeAnalysis::ASTNode * pGlobalScope, std::string fileSpec, std::map<std::string, std::vector<std::string>>& dependencyMap)
{
	auto iter = pGlobalScope->statements_.begin();
	std::vector<std::string> dep;
	while (iter != pGlobalScope->statements_.end()) {
		dep.push_back((*iter)->ToString());
		auto iter2 = dependencyMap.find(fileSpec);
		if (iter2 == dependencyMap.end()) {
			dependencyMap.insert(std::pair<std::string, std::vector<std::string>>(fileSpec, dep));
		}
		else {
			iter2->second.push_back((*iter)->ToString());
		}
		iter++;
	}
}

//-----< An info table is crreated >------//
void dependencies::newInfoTable(CodeAnalysis::ASTNode * pGlobalScope, std::string file)
{

	static size_t indentLevel = 0;
	TypeInfo type = findTypeInfo(pGlobalScope);
	std::map<size_t, TypeInfo> temp;
	auto name = file;
	if (pGlobalScope->package_ != "" && pGlobalScope->type_ != "control" && pGlobalScope->type_ != "anonymous") {
		temp.insert(std::pair<size_t, TypeInfo>(pGlobalScope->startLineCount_, type));
		if (pGlobalScope->type_ == "class" || pGlobalScope->type_ == "struct") {
			temp.insert(std::pair<size_t, TypeInfo>(pGlobalScope->endLineCount_ - 1, end));
		}
		else {
			temp.insert(std::pair<size_t, TypeInfo>(pGlobalScope->endLineCount_, end));
		}
		auto find = typeInfoTable_.find(file);
		if (find == typeInfoTable_.end()) {
			typeInfoTable_.insert(std::pair<std::string, std::map<size_t, TypeInfo>>(name, temp));
		}
		else {
			find->second.insert(std::pair<size_t, TypeInfo>(pGlobalScope->startLineCount_, type));
			if (pGlobalScope->type_ == "class" || pGlobalScope->type_ == "struct")
				find->second.insert(std::pair<size_t, TypeInfo>(pGlobalScope->endLineCount_ - 1, end));
			else
				find->second.insert(std::pair<size_t, TypeInfo>(pGlobalScope->endLineCount_, end));
		}
	}
	auto iter = pGlobalScope->children_.begin();
	while (iter != pGlobalScope->children_.end()) {
		newInfoTable(*iter, file);
		++iter;
	}
	--indentLevel;
}

//-----< Check the type of node >------//
dependencies::TypeInfo dependencies::findTypeInfo(CodeAnalysis::ASTNode * pGlobalScope)
{
	if (pGlobalScope->type_ == "namespace")
		return none;
	if (pGlobalScope->type_ == "class")
		return classes;
	if (pGlobalScope->type_ == "function" && pGlobalScope->endLineCount_ - pGlobalScope->startLineCount_ == 0)
		return singleLineFn;
	if (pGlobalScope->type_ == "function")
		return function;
	if (pGlobalScope->type_ == "interface")
		return function;
	if (pGlobalScope->type_ == "struct")
		return classes;
	return none;
}

//-----<Test Stub for dependencies >------//
#ifdef TEST_DEPENDENCY

int main()
{
	std::vector<std::string> files;
	files.pushback("../CppParser/FileSystem/FileSystem.h");
	files.pushback("../CppParser/Parser/ConfigureParser.h");
	files.pushback("../CppParser/Parser/Toker.h");
	files.pushback("../CppParser/Parser/Semi.h");
	files.pushback("../CppParser/Parser/Utilities.h");
	files.pushback("../CppParser/Parser/Logger.h");
	files.pushback("../CppParser/Parser/ScopeStack.h");
	Dependency ob;
	ob.summonparser(files);

	return 0;
}

#endif


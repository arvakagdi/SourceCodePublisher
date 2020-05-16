
 //////////////////////////////////////////////////////////////////////////////////////////////
 // converter.h -  Includes all the functions required for conversion of  files to HTML      //
 // ver 1.1																			         //				
 // Platform:      Dell Intel Core i3, Win10									             //
 // Application:   Object Oriented Design - Project #1, Spring 2019                          //
 // Author:        Arva Kagdi                                                                //
 //////////////////////////////////////////////////////////////////////////////////////////////

 /*
 * Module Operations:
 * ==================
 * This package contains a converter class named : converter
 * Converts files to HTML and change special characters "<" and ">" to their equivalent exscapes
 * The text of the code is prepended by "<pre>" and postpended by "</pre>" to  render white spaces
 * The function code_convert converts the source code into HTML.
 * These files are then placed in a vector of string
 * char_change function is used to repace the special characters and remove white spaces
 *
 * Fuction new are :
 * ===================
 * dependenciesFetch, HandlingComments, DivTagsAdded, SwitchFn.
 *
 * Required files :
 * ===================
 * FileSystem.h, FileSystem.cpp
 * display.h, display.cpp
 * dependency.h, dependency.cpp
 *
 * Build commands
 * ===================
 * - devenv OODProject2.sln
 *
 * Maintenance History:
 * ====================
 * ver 1.1 : 3/20/2019
 * ver 1.0 : 2/14/2019
 * first release
 */


#pragma once
#include<vector>
#include<iostream>
#include "../DirectoryNavigator/FileSystem/FileSystem.h"
#include "../display/display.h"
#include <fstream>
#include<string>
#include <algorithm>
#include <map>
#include"../dependencies/dependencies.h"

using TypeInfoT = std::map<std::string, std::map<std::size_t, dependencies::TypeInfo>>;
namespace HTML
{
	//--------< creating a class class convert >--------//
	class converter
	{
	public:

		converter();
		~converter();

		std::string code_convert(std::string filesToProcess_, std::map<std::string, std::vector<std::string>> depT, const TypeInfoT &typeTable);
		void FetchDependencies(std::string &filesToProcess, const std::map<std::string, std::vector<std::string>> &depTable, std::ofstream &webpageOutput);
		void HideShowComment(std::vector<std::string> &filedata, const TypeInfoT &typeTable);
		void AddDivTags(std::string &file, std::vector<std::string> &filedata, const TypeInfoT &typeTable);
		void FunctSwitch(size_t &i, std::vector<std::string> &filedata, std::map<std::size_t, dependencies::TypeInfo>::const_iterator &iter2, size_t lineNo);
		void HideShowClass(size_t &i, std::vector<std::string> &filedata, size_t &lineNo);
		void HideShowFunction(size_t &i, std::vector<std::string> &filedata, size_t &lineNo);
		void SingleLineFOpen(size_t &i, std::vector<std::string> &filedata, size_t &lineNo);
		void SingleLineFClose(size_t &i, std::vector<std::string> &filedata, size_t &lineNo);
	};
}


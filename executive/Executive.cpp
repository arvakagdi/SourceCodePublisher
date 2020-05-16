/////////////////////////////////////////////////////////////////////////////////
// Executive.h -  Executive package for the source code publisher              //
// ver 1.1																       //	
// Platform:      Dell Intel Core i3, Win10									   //
// Application:   Object Oriented Design - Project #1, Spring 2019             //
// Author:        Arva Kagdi                                                   //
// Source:        Jim Fawcett,Syracuse University                              //
/////////////////////////////////////////////////////////////////////////////////
/*
*
* Module Operations:
* ==================
* The Executive  package consists of the functions used in Executive.h
* It carries out the process of calling display and process to convert and display the HTML files with hide and show options.
* It shows the dependencies of each file
* It fulfills all the  demostration requirements
*
* Required files:
* ==================
* -Executive.h
*/

#include "Executive.h"
#include "../CppParser/Parser/ConfigureParser.h"
#include "../CppParser/Parser/Parser.h"
#include "../CppParser/Parser/ActionsAndRules.h"
#include "../CppParser/AbstractSyntaxTree/AbstrSynTree.h"
#include "../CppParser/ScopeStack/ScopeStack.h"
#include<regex>


using namespace Utilities1;
using namespace FileSystem;
using namespace HTML;
using namespace CodeAnalysis;


Executive::Executive()
{
}

Executive::~Executive()
{
}

//----< This function displays list of files >----//
void Executive::displayFiles(std::vector<std::string> Files) {
	for (auto file : Files)
		std::cout << "\n" << file;
	std::cout << "\n";
}

std::string customUsage()
{
	std::string usage;
	usage += "\n  Command Line: path [/option]* [/pattern]*";
	usage += "\n    path is relative or absolute path where processing begins";
	usage += "\n    [/option]* are one or more options of the form:";
	usage += "\n      /s - walk directory recursively";
	//usage += "\n      /h - hide empty directories";
	//usage += "\n      /a - on stopping, show all files in current directory";
	usage += "\n    [pattern]* are one or more pattern strings of the form:";
	usage += "\n      *.h *.cpp *.cs *.txt or *.*";
	usage += "\n";
	return usage;

}

//----< This function displays the requirenment 3 >----//
void Executive::Dis_Requirenment()
{
	std::string file, path;

	std::cout << "\n//---------------------------------------< Demonstrating Requirement 4>-----------------------------------------//";

	file = FileSystem::Path::getFullFileSpec("../Executive/Executive.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 1. Executive:- Path: " << path;

	file = FileSystem::Path::getFullFileSpec("../Converter/Converter.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 2. Converter:- Path: " << path;

	file = FileSystem::Path::getFullFileSpec("../Display/Display.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 3. Display:- Path: " << path;

	file = FileSystem::Path::getFullFileSpec("../Dependency/Dependency.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 4. Dependency:- Path: " << path;

	file = FileSystem::Path::getFullFileSpec("../DirExplorer-Naive/Loader/Loader.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 5. Loader:- Path: " << path;
	std::cout << "\n ---passed---\n\n";
	
}

//----< This function processes command line arguments and applies regex >----//
std::vector<std::string> initialization(int argc,char** argv) {
	ProcessCmdLine pcl(argc, argv);
	pcl.usage(customUsage());

	std::cout << "\n//---------------------------------------< Demonstrating Requirement 3>-----------------------------------------//";
	preface("Command Line: ");
	pcl.showCmdLine();
	pcl.showRegexes();
	std::cout << "\n";

	if (pcl.parseError())
	{
		pcl.usage();
		std::cout << "\n\n";
		_getche();
		return std::vector<std::string>();
	}

	DirExplorerN de(pcl.path());
	for (auto patt : pcl.patterns())
	{
		de.addPattern(patt);
	}

	if (pcl.hasOption('s'))
	{
		de.recurse();
	}

	de.search();

	std::vector<std::string> filesRetreived = de.fileToProcess(); 
	std::vector<std::string> CppToHtmlFiles;

	std::cout << "\n//---------------------------------------< Demonstrating Requirement 5 and 6>-----------------------------------------//";
	std::cout << "\n List of files before applying regex";
	Executive obj;
	obj.displayFiles(filesRetreived);

	
	for (auto regeX : pcl.regexes()) {     //----< Matching the pattern with the files >----//
		std::regex s(regeX);
		for (auto file : filesRetreived) {
			auto fileName = Path::getName(file);
			if (regex_match(fileName, s))
				CppToHtmlFiles.push_back(file);
		}
	}
	return CppToHtmlFiles;
}


int main(int argc, char** argv) {


	dependencies object;

	Executive obj;
	obj.Dis_Requirenment();

	std::vector<std::string> CppToHtmlFiles;
	CppToHtmlFiles = initialization(argc,argv);
	
	std::cout << "\n List of file after applying regex";

	if (CppToHtmlFiles.size() == 0) {
		std::cout << "\n No files found";
		_getche();
		return -1;
	}
	obj.displayFiles(CppToHtmlFiles);
	std::cout << "\n ";
	std::cout << "\n//---------------------------------------< Demonstrating Requirement 7 and 8>-----------------------------------------//";
	std::cout << "Converter - The function CppToHtml converts all .cpp and .h file to HTML \n";
	std::cout << "Dependency - summonParser is responsible for parsing the files and storing the dependency in a map\n";
	std::cout << "Converter - DivTagsAdded and Converter - HandlingFunctions are the functions that handle divtags and functions.\n\n";
	HTML::converter conv; //----< converter object created >----//
	std::vector<std::string> htmlFiles;  
	
	
	std::map<std::string, std::vector<std::string>> depT;
	depT = object.summonParser(CppToHtmlFiles);
	std::map<std::string, std::map<std::size_t, dependencies::TypeInfo>> typeInfo = object.returnTypeInfoTable();

	for (auto file : CppToHtmlFiles) {
		htmlFiles.push_back(conv.code_convert(file, depT, typeInfo));
	}
	
	obj.displayFiles(CppToHtmlFiles);
	
	//std::string dirPath = "../ConvertedPages";
	std::vector<std::string> convertedFiles = FileSystem::Directory::getFiles();
	std::string x = FileSystem::Directory::getCurrentDirectory();

	std::cout << "\n Opening the converted files one by one";
	display d;					 //----< Created an object for class display to display the files>----//
	d.showWebPage(htmlFiles);    //----<Displaying the filess>----//

	
	_getche();
	return 0;
}
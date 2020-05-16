///////////////////////////////////////////////////////////////////////////////////
// Executive.h -  Executive package for the source code publisher                //
// ver 1.1																         //				
// Platform:      Dell Intel Core i3, Win10									     //
// Application:   Object Oriented Design - Project #1, Spring 2019               //
// Author:        Arva Kagdi                                                     //
// Source:        Jim Fawcett,Syracuse University                                //
///////////////////////////////////////////////////////////////////////////////////
/*
/*
* Module Operations:
* ==================
* Executive is the main entry point that links all the other packages. It includes display and converter
* First the converter is called to convert all the source files to HTML. 
* We implement hiding and showing comments, functions, classes
* Dependency table is created and we now have dependencies on of all files
* We only look for the .cpp and .h extensions
* To find particular file names within a specified range, Regex is used
* The converted files are stored into ConvertedWebpages folder
* Now to display all these converter files, we need to cal display function which displays 6 files one by one
* The HTML files are popped in Mozilla firefox. After one file is displayed, the browser needs to be closed for the child to pop up
* This way all 6 converted files are displayed
* We also demonstrate all the requirenments for this project in the Executive package
*
* Required files :
* ==================
* converter.h, converter.cpp
* Display.h, Display.cpp
* CodeUtilities.h, CodeUtilities.cpp
* DirExplorerN.h, DirExplorerN.cpp
* Utilities.h, Utilities.cpp 
* ConfigureParser.h,  ConfigureParser.cpp
* Parser.h, Parser.cpp
* ActionsAndRules.h, ActionsAndRules.cpp
* AbstrSynTree.h, AbstrSynTree.cpp
* ScopeStack.h, ScopeStack.cpp
* dependencies.h, dependencies.cpp
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
#include "../converter/converter.h"
#include "../display/display.h"
#include "../DirectoryNavigator/Utilities/CodeUtilities/CodeUtilities.h"
#include "../DirectoryNavigator/DirExplorer-Naive/DirExplorerN.h"
#include "../CppParser/Utilities/Utilities.h"
#include "../CppParser/Parser/ConfigureParser.h"
#include "../CppParser/Parser/Parser.h"
#include "../CppParser/Parser/ActionsAndRules.h"
#include "../CppParser/AbstractSyntaxTree/AbstrSynTree.h"
#include "../CppParser/ScopeStack/ScopeStack.h"
#include "../dependencies/dependencies.h"
#include<conio.h>
#include<vector>
#define Util StringHelper

//----< Created class Executive >----//
class Executive
{
public:
	Executive();
	~Executive();
	//----< function prototypes >----//
	void Dis_Requirenment();
	void displayFiles(std::vector<std::string> Files);
};
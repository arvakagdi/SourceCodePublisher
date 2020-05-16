//////////////////////////////////////////////////////////////////////////////////////////////
// converter.cpp - converts the files to HTML                                               //
// ver 1.1																			        //				
// Platform:       Dell Intel Core i3, Win10                                                //
// Application:    Object Oriented Design - Project #2, Spring 2019                         //
// Author:         Arva Kagdi                                                               //
//////////////////////////////////////////////////////////////////////////////////////////////

#include "converter.h"

namespace HTML {
	converter::converter()
	{
	}

	converter::~converter()
	{
	}

	//--------< This function will convert cpp files to its HTML equivalent >--------//
	std::string converter::code_convert(std::string filesToProcess_,std::map<std::string, std::vector<std::string>> depT, const TypeInfoT &typeTable)
	{
		std::string fileName = FileSystem::Path::getName(filesToProcess_);
		std::ifstream in(filesToProcess_);
		std::vector<std::string> fileContent;

		auto checkIter = depT.find(fileName);

		for (std::string line; std::getline(in, line);) {

			//---< searches for character "<" and replaces with its exscape sequence "&lt"  >---//
			while (line.find("<") != -1)
				line.replace(line.find("<"), 1, "&lt;");
			//---< searches for character ">" and replaces with its exscape sequence "&gt"  >---//
			while (line.find(">") != -1)
				line.replace(line.find(">"), 1, "&gt;");
			fileContent.push_back(line);
		}

		HideShowComment(fileContent, typeTable);
		AddDivTags(filesToProcess_, fileContent, typeTable);

		std::string dirPath = "..//ConvertedPages";
		std::string webPath = dirPath + "//" + fileName + ".html";

		//-------< adding HTML tags (prepends and postpends) >--------//
		std::ofstream display(webPath);
		display << "<!Document html>\n";
		display << "<html>\n";
		display << "<head>\n";
		display << "<link rel = \"stylesheet\" href = \"style.css\">\n";
		display << "<script src=\"showHideClass.js\"></script>\n";
		display << "</head>\n";
		display << "<body>\n";
		display << "<button onclick = \"showHideClass(\'classes\')\">Show or Hide Class Contents</button>\n";
		display << "<button onclick = \"showHideClass(\'functions\')\">Show or Hide Function Contents</button>\n";
		display << "<button onclick = \"showHideClass(\'comments\')\">Show or Hide Comments</button><br>";
		FetchDependencies(filesToProcess_, depT, display);
		display << "<pre>\n";
		for (auto content : fileContent)
			display << content << "\n";
		display << "</pre>\n";
		display << "</body\n>";
		display << "</html\n>";

		display.close();
		return webPath;
	}
	
	//-------< In this function we check and fetch the dependencies for each file and display it >--------//
	void converter::FetchDependencies(std::string & filesToProcess, const std::map<std::string, std::vector<std::string>>& depTable, std::ofstream & display)
	{
		auto iter = depTable.find(filesToProcess);
		if (iter == depTable.end()) {
			display << "No dependency found for this file";
			return;
		}
		else {
			auto iter2 = iter->second;
			if (iter2.size() == 0)
				display << "No dependency found for this file";
			else {
				for (size_t i = 0; i < iter2.size(); i++) {
					display << "<a href=\"" + iter2[i] + ".html \">" + iter2[i] + "</a>" + "&nbsp&nbsp&nbsp&nbsp";
				}
			}
		}
	}
	
	//-------< This function is used to hide and show the comments >--------//
	void converter::HideShowComment(std::vector<std::string>& filedata, const TypeInfoT & typeTable)
	{

		bool isComment = false;
		size_t found;
		for (size_t i = 0; i < filedata.size(); i++) {
			found = filedata[i].find("//");
			if (found != filedata[i].npos) {
				filedata[i].replace(found, 2, "<div class=\"comments\">//");
				filedata[i].replace(filedata[i].size(), filedata[i].size(), "</div>");
			}
			found = filedata[i].find("/*");
			if (found != filedata[i].npos) {
				isComment = true;
				filedata[i].replace(found, 2, "<div class=\"comments\">/*");
			}
			if (isComment) {
				size_t muLineC = filedata[i].find("*/");
				if (muLineC != filedata[i].npos) {
					filedata[i].replace(muLineC, 2, "*/</div>");
					isComment = false;
				}
			}
		}
	}

	//-------< This function is used to add the DivTags >--------//
	void converter::AddDivTags(std::string & file, std::vector<std::string>& filedata, const TypeInfoT & typeTable)
	{
		auto iter = typeTable.find(file);
		if (iter != typeTable.end()) {
			auto iter2 = iter->second.begin();
			size_t i;
			while (iter2 != iter->second.end())
			{
				size_t lineNo = iter2->first;
				FunctSwitch(i, filedata, iter2, lineNo);
				iter2++;
			}
		}
	}
	

	//-------< This function is used for running the switch functionality >--------//
	void converter::FunctSwitch(size_t & i, std::vector<std::string>& filedata, std::map<std::size_t, dependencies::TypeInfo>::const_iterator & iter2, size_t lineNo)
	{
		switch (iter2->second) {
		case dependencies::TypeInfo::classes:
			HideShowClass(i, filedata, lineNo);
			break;
		case dependencies::TypeInfo::function:
			HideShowFunction(i, filedata, lineNo);
			break;
		case dependencies::TypeInfo::singleLineFn:
			SingleLineFOpen(i, filedata, lineNo);
			SingleLineFClose(i, filedata, lineNo);
			break;
		case dependencies::TypeInfo::end:
			i = filedata[lineNo - 1].find("\t};");
			if (i != filedata[lineNo - 1].npos) {
				filedata[lineNo - 1].replace(i, 3, "\t};</div>"); break;
			}
			i = filedata[lineNo - 1].find("};");
			if (i != filedata[lineNo - 1].npos) {
				filedata[lineNo - 1].replace(i, 2, "};</div>"); break;
			}
			i = filedata[lineNo - 1].find("\t}");
			if (i != filedata[lineNo - 1].npos) {
				filedata[lineNo - 1].replace(i, 2, "\t}</div>"); break;
			}
			i = filedata[lineNo - 1].find("}");
			if (i != filedata[lineNo - 1].npos) {
				filedata[lineNo - 1].replace(i, 1, "}</div>"); break;
			}
			i = filedata[lineNo - 1].find("\t*/");
			if (i != filedata[lineNo - 1].npos) {
				filedata[lineNo - 1].replace(i, 3, "\t*/</div>"); break;
			}
			i = filedata[lineNo - 1].find("*/");
			if (i != filedata[lineNo - 1].npos) {
				filedata[lineNo - 1].replace(i, 2, "*/</div>"); break;
			}break;
		default: break;
		}
	}

	//-------< This function is used to hide and show the classes >--------//
	void converter::HideShowClass(size_t & i, std::vector<std::string>& filedata, size_t & lineNo)
	{
		i = filedata[lineNo - 1].find("\t{");
		if (i != filedata[lineNo - 1].npos) {
			filedata[lineNo - 1].replace(i, 2, "<div class=\"classes\">\t{");
		}
		else {
			i = filedata[lineNo - 1].find("{");
			if (i != filedata[lineNo - 1].npos)
				filedata[lineNo - 1].replace(i, 1, "<div class=\"classes\">{");
		}
	}
	
	//-------< This function is used to hide and show the functions >--------//
	void converter::HideShowFunction(size_t & i, std::vector<std::string>& filedata, size_t & lineNo)
	{
		i = filedata[lineNo - 1].find("\t{");
		if (i != filedata[lineNo - 1].npos) {
			filedata[lineNo - 1].replace(i, 2, "<div class=\"functions\">\t{");
		}
		else {
			i = filedata[lineNo - 1].find("{");
			if (i != filedata[lineNo - 1].npos)
				filedata[lineNo - 1].replace(i, 1, "<div class=\"functions\">{");
		}
	}
	

	//-------< This function takes care of the single line function open  >--------//
	void converter::SingleLineFOpen(size_t & i, std::vector<std::string>& filedata, size_t & lineNo)
	{

		i = filedata[lineNo - 1].find("\t{");
		if (i != filedata[lineNo - 1].npos) {
			filedata[lineNo - 1].replace(i, 2, "<div class=\"functions\">\t{");
		}
		else {
			i = filedata[lineNo - 1].find("{");
			if (i != filedata[lineNo - 1].npos) {
				filedata[lineNo - 1].replace(i, 1, "<div class=\"functions\">{");
			}
		}
	}

	//-------< This function takes care of the single line function close >--------//
	void converter::SingleLineFClose(size_t & i, std::vector<std::string>& filedata, size_t & lineNo)
	{
		i = filedata[lineNo - 1].find("\t}");
		if (i != filedata[lineNo - 1].npos) {
			filedata[lineNo - 1].replace(i, 2, "\t}</div>");
		}
		else {
			i = filedata[lineNo - 1].find("}");
			if (i != filedata[lineNo - 1].npos) {
				filedata[lineNo - 1].replace(i, 1, "}</div>");
			}
		}
	}
	
}

//-------< Test Stub for converter >--------//
#ifdef TEST_converter

int main()
{
	std::cout << "\tTesting independent package implementation from test stub." << "\n";
	std::cout << "\t**********************************************************" << "\n\n\n";
	std::vector<std::string> htmlFiles;
	std::vector<std::string> filesRetreived;
	filesRetreived.push_back("./converter.cpp");
	HTML::converter conv;
	for (auto file : filesRetreived) {
		htmlFiles.push_back(conv.cppToHtml(file));
	}
	std::cout << "Printing converted file names" << "\n";
	for (auto fileName : htmlFiles)
		std::cout << fileName << "\n";
	getchar();
	return 0;
}

#endif
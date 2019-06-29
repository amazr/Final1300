#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
using namespace std;

unordered_map<string, int> int_vars;
vector<string> datatypes = { "int", "dec", "chr", "str" };
vector<string> operators = {"+", "-", "*", "/", "++", "--", "+=", "-=", "%"};
vector<string> keywords = { "display" };


struct typelocation {
	string type;
	int location;	//This is the location of the LAST char in the type name
};

struct line {
	string lineStr;
	string strLiteral;
	int lineNum;
};

//This will display all characters to the right of "display:" to the terminal
void displayFunction(line thisLine, int wordLocation) {
	
	if (thisLine.strLiteral == "") {
		cout << endl << "Warning on line " << thisLine.lineNum << " -> Empty display function" << endl;
		return;
	}

	cout << thisLine.strLiteral;
	
}

//This function will check if a keyword was used and handle that keywords function
void checkForKeywords(line thisLine) {
	for (auto word : keywords) {
		size_t found = thisLine.lineStr.find(word);
		if (found != string::npos) {
			if (word == keywords.at(0)) {
				displayFunction(thisLine, found);
			}
		}
	}
}

//This will create a string literal if a line has one. This is to exempt it from the clear whitespace rule
line createStrLiteral(line thisLine) {
	bool inLiteral = false;
	bool gettingVar = false;
	int varCounter = 0;
	vector<string> vars;
	string varName = "";


	for (int i = 0; i < thisLine.lineStr.size(); i++) {

		if (thisLine.lineStr[i] == '"') {
			if (inLiteral) {
				inLiteral = false;
			}
			else {
				inLiteral = true;
			}
		}
		if (inLiteral && thisLine.lineStr[i] != '\"') {
			thisLine.strLiteral += thisLine.lineStr[i];
		}

		//Ending line statement is a '.'
		if (!inLiteral && thisLine.lineStr[i] == '.') {
			thisLine.strLiteral += "\n";
		}

		//Vars appended to strings must be surrounded by ':'
		if (!inLiteral && thisLine.lineStr[i] == ':') {
			if (!gettingVar) {
				gettingVar = true;
				varCounter++;

			}
			else {
				gettingVar = false;
				varName.erase(varName.begin());
				vars.push_back(varName);
				varName = "";
				if (int_vars.find(vars.at(varCounter - 1)) != int_vars.end()) {
					thisLine.strLiteral += to_string(int_vars[vars.at(varCounter - 1)]);;

				}
			}
		}

		if (gettingVar) {
			varName += thisLine.lineStr[i];
		}

	}
	
	return thisLine;
}

//this function removes whitespace from a line and then returns the line
line removeWhitespace(line thisLine) {
	
	//IMPORTANT: If this is not called, all string literals will have NO whitespace. It is important to use line. strLiteral when reffering to literals
	thisLine = createStrLiteral(thisLine);

	for (int i = 0; i < thisLine.lineStr.size(); i++) {
		if (thisLine.lineStr[i] == ' ') {
			thisLine.lineStr.erase(thisLine.lineStr.begin() + i);
		}
	}

	return thisLine;
}

//this function returns typelocation struct with a new data type and a new location for its ending character
typelocation searchForType(line thisLine) {

	typelocation newType;
	for (auto word : datatypes) {
		size_t found = thisLine.lineStr.find(word);
		if (found != string::npos) {
			newType.type = word;
			newType.location = found + (word.size() - 1);
			return newType;
		}
	}

	newType.location = -1;
	return newType;
}


//This function gets a new variable name and stores it in the proper unordered map of that variables declared type
void createNewVar(line thisLine, typelocation varType) {

	size_t found = thisLine.lineStr.find("=");
	string varName = "";
	string varValueStr = "";

	//Runs if a variable is created with an equal sign and a value
	if (found != string::npos) {
		//this finds the variable name
		for (int i = varType.location + 1; i < found; i++) {
			varName += thisLine.lineStr[i];
		}
		//this finds the variable value
		for (int i = found + 1; i < thisLine.lineStr.size(); i++) {
			varValueStr += thisLine.lineStr[i];
		}
	}
	//Runs if variable value is not declared at this time
	else {
		for (int i = varType.location + 1; i < thisLine.lineStr.size(); i++) {
			varName += thisLine.lineStr[i];
		}
	}

	//Integer variable creation
	if (varType.type == datatypes[0]) {
		try {
			if (varValueStr == "") {
				int_vars[varName] = 0;	//If var is not declared set it to 0
			}
			else {
				int_vars[varName] = stoi(varValueStr);
			}
		}
		catch (invalid_argument) {
			cout << "Incorrect value: " << varValueStr << " entered on line: " << thisLine.lineNum << endl;
		}
		catch (out_of_range) {
			cout << "Value out of range on line: " << thisLine.lineNum << endl;
		}
	}
}

//this function will interpret each line
void readLine(line thisLine) {
	typelocation newType;

	//This will also handle the spacing within string literals
	thisLine = removeWhitespace(thisLine);

	//Find new datatype
	newType = searchForType(thisLine);
	if (newType.location != -1) {
		createNewVar(thisLine, newType);
	}

	checkForKeywords(thisLine);
}

//this function opens the file to be read
void openFile(string fileName) {
	ifstream ifs(fileName);
	
	line thisLine;
	thisLine.lineNum = 1;
	while (getline(ifs, thisLine.lineStr)) {
		readLine(thisLine);
		thisLine.lineNum++;
	}
}

int main() {

	string fileName = "";
	cout << "Welcome to UNNAMED LANGUAGE v1" << endl << "Enter file name to run" << endl;
	cin >> fileName;
	openFile(fileName);

	//cout << endl << endl << "All vars listed below" << endl;
	//for (auto var : int_vars) {
	//	cout << "var name: " << var.first << " = " << var.second << endl;
	//}

	return 0;
}
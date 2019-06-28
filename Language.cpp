#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
using namespace std;

unordered_map<string, int> int_vars;
vector<string> datatypes = { "int", "dec", "chr", "str" };
vector<string> operands = {};
vector<string> keywords = {};

struct typelocation {
	string type;
	int location;
};

struct line {
	string lineStr;
	int lineNum;
};


//this function removes whitespace from a line and then returns the line
line removeWhitespace(line thisLine) {
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
			//cout << " ... keyword was found in: " << found;		//debuggin line
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

	//Integer variable creation
	if (varType.type == datatypes[0]) {
		try {
			int_vars[varName] = stoi(varValueStr);
		}
		catch (invalid_argument) {
			cout << "Incorrect datatype entered on line: " << thisLine.lineNum << endl;
		}
		catch (out_of_range) {
			cout << "Value out of range on line: " << thisLine.lineNum << endl;
		}
	}
}

//this function will interpret each line
void readLine(line thisLine) {
	typelocation newType;
	thisLine = removeWhitespace(thisLine);

	//cout << thisLine.lineStr;											//debugging line
	
	//Find new datatype
	newType = searchForType(thisLine);
	if (newType.location != -1) {
		createNewVar(thisLine, newType);
	}

	//cout << endl;											//debugging line
}

//this function opens the file to be read
void openFile(string fileName) {
	ifstream ifs(fileName);
	
	line thisLine;
	thisLine.lineNum = 1;
	while (getline(ifs, thisLine.lineStr)) {
		thisLine.lineNum++;
		readLine(thisLine);
	}
}

int main() {

	openFile("ide.txt");

	cout << endl << endl << "All vars listed below" << endl;
	for (auto var : int_vars) {
		cout << "var name: " << var.first << " = " << var.second << endl;
	}

	return 0;
}
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


//this function removes whitespace from a line and then returns the line
string removeWhitespace(string line) {
	for (int i = 0; i < line.size(); i++) {
		if (line[i] == ' ') {
			line.erase(line.begin() + i);
		}
	}
	return line;
}

//this function returns typelocation struct with a new data type and a new location for its ending character
typelocation searchForType(string line) {
	typelocation newType;
	for (auto word : datatypes) {
		size_t found = line.find(word);
		if (found != string::npos) {
			cout << " ... keyword was found in: " << found;		//debuggin line
			newType.type = word;
			newType.location = found + (word.size() - 1);
			return newType;
		}
	}

	newType.location = -1;
	return newType;

}

//this function will interpret each line
void readLine(string line) {
	typelocation newType;
	line = removeWhitespace(line); 

	cout << line;											//debugging line
	
	//Find new datatype
	newType = searchForType(line);
	if (newType.location != -1) {
		cout << " ... end location for type is: " << newType.location;
	}

	cout << endl;											//debugging line
}

//this function opens the file to be read
void openFile(string fileName) {
	ifstream ifs(fileName);
	
	string line;
	while (getline(ifs, line)) {
		readLine(line);
	}
}

int main() {

	openFile("ide.txt");

	return 0;
}
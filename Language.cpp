#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
using namespace std;

//Big daddy unordered map with its little pseudo-children under it
unordered_map<string, string> var_types;
	unordered_map<string, int> int_vars;
	unordered_map<string, float> dec_vars;
	unordered_map<string, char> chr_vars;
	unordered_map<string, string> str_vars;
	unordered_map<string, bool> bol_vars;

//Vectors for various things
vector<string> datatypes = { "int", "dec", "chr", "str", "bol" };
vector<string> operators = {"+", "-", "*", "/", "%", "++", "--", "+=", "-="};
int numOfBasicOperators = 5;
vector<string> keywords = { "display" , "if" };
vector<string> evalWords = { "=" , "!=" , "and" , "or" };
vector<string> warnings;
string warnStr;

//this is a struct for datatypes
struct typelocation {
	string type;
	int location;	//This is the location of the LAST char in the type name
};

//This is the data that is available for each individual line
struct line {
	string lineStr;
	string strLiteral = "";
	int lineNum;
};

bool doesVarExist(string varName) {
	for (auto name : var_types) {
		if (name.first == varName) {
			return true;
		}
	}
	return false;
}

//This will return an error if a var is not found in the map... call this always for errors
string fetchVarType(string varName) {
	auto found = var_types.find(varName);
	if (found != var_types.end()) {
		return var_types[varName];
	}
	else {
		warnings.push_back(warnStr + " :" + varName + ": was not found");
		return "";
	}
}

//Eventually this will evaluate an expression passed to it and return it... WIP
int evaluateInt(string expression) {
	//For ++
	if (expression == operators[5]) {
		return 1;
	}
	//For --
	else if (expression == operators[6]) {
		return -1;
	}
	//for +=
	else if ((expression[0] == '+' || expression[0] == '-') && expression[1] == '=') {

		string newVal = "";
		for (int i = 2; i < expression.size(); i++) {
			newVal += expression[i];
		}
		//Check to see if the incrememnt is another variable
		if (doesVarExist(newVal)) {
			if (var_types[newVal] == datatypes[0]) {
				return int_vars[newVal];
			}
			else if (var_types[newVal] == datatypes[1]) {
				return (int)dec_vars[newVal];
			}
		}

		if (expression[0] == '-') {
			return -stoi(newVal);
		}
		return stoi(newVal);
	}

	//For all other expressions
	vector<string> expressionOperator;
	string opTemp = "";
	string numTemp = "";
	vector<string> numbers;
	bool foundOP = false;
	

	for (int i = 0; i < expression.size(); i++) {
		//runs through operators
		for (int j = 0; j < numOfBasicOperators; j++) {
			if (expression[i] == operators[j][0]) {
				opTemp += expression[i];
				expressionOperator.push_back(opTemp);
				numbers.push_back(numTemp);
				numTemp = "";
				opTemp = "";
				foundOP = true;
				continue;
			}
		}

		//sets the number
		if (!foundOP) {
			numTemp += expression[i];
		}
		else {
			foundOP = false;
		}
	}

	numbers.push_back(numTemp);

	int numCounter = 1;
	int result = 0;
	if (doesVarExist(numbers[0])) {
		string type = fetchVarType(numbers[0]);
		//int
		if (type == datatypes[0]) {
			result = int_vars[numbers[0]];
		}
		//dec
		if (type == datatypes[1]) {
			result = static_cast<int>(dec_vars[numbers[0]]);
		}
	}
	else {
		result = stoi(numbers[0]);
	}
	for (int i = 0; i < expressionOperator.size(); i++) {

		if (numbers.size() == numCounter) {
			warnings.push_back(warnStr + " Operator with no number after it.");
			return 0;
		}

		//Addition
		if (expressionOperator[i] == operators[0]) {
			if (doesVarExist(numbers[numCounter])) {

				string type = fetchVarType(numbers[numCounter]);
				//int
				if (type == datatypes[0]) {
					result += int_vars[numbers[numCounter]];
				}
				//dec
				if (type == datatypes[1]) {
					result += static_cast<int>(dec_vars[numbers[numCounter]]);
				}
			}
			else {
				try {
					result += stoi(numbers[numCounter]);
				}
				catch (invalid_argument) {
					warnings.push_back(warnStr + " Some invalid value was used.");
				}
				catch (out_of_range) {
					warnings.push_back(warnStr + " Some value is out of integer range.");
				}
			}
			numCounter++;
		}
		//Subtraction
		else if (expressionOperator[i] == operators[1]) {
			if (doesVarExist(numbers[numCounter])) {

				string type = fetchVarType(numbers[numCounter]);
				//int
				if (type == datatypes[0]) {
					result -= int_vars[numbers[numCounter]];
				}
				//dec
				if (type == datatypes[1]) {
					result -= static_cast<int>(dec_vars[numbers[numCounter]]);
				}
			}
			else {
				try {
					result -= stoi(numbers[numCounter]);
				}
				catch (invalid_argument) {
					warnings.push_back(warnStr + " Some invalid value was used.");
				}
				catch (out_of_range) {
					warnings.push_back(warnStr + " Some value is out of integer range.");
				}
			}
			numCounter++;
		}
		//Multiplication
		else if (expressionOperator[i] == operators[2]) {
			if (doesVarExist(numbers[numCounter])) {

				string type = fetchVarType(numbers[numCounter]);
				//int
				if (type == datatypes[0]) {
					result *= int_vars[numbers[numCounter]];
				}
				//dec
				if (type == datatypes[1]) {
					result *= static_cast<int>(dec_vars[numbers[numCounter]]);
				}
			}
			else {
				try {
					result *= stoi(numbers[numCounter]);
				}
				catch (invalid_argument) {
					warnings.push_back(warnStr + " Some invalid value was used.");
				}
				catch (out_of_range) {
					warnings.push_back(warnStr + " Some value is out of integer range.");
				}
			}
			numCounter++;
		}
		//Division
		else if (expressionOperator[i] == operators[3]) {
			if (doesVarExist(numbers[numCounter])) {

				string type = fetchVarType(numbers[numCounter]);
				//int
				if (type == datatypes[0]) {
					result /= int_vars[numbers[numCounter]];
				}
				//dec
				if (type == datatypes[1]) {
					result /= static_cast<int>(dec_vars[numbers[numCounter]]);
				}
			}
			else {
				try {
					result /= stoi(numbers[numCounter]);
				}
				catch (invalid_argument) {
					warnings.push_back(warnStr + " Some invalid value was used.");
				}
				catch (out_of_range) {
					warnings.push_back(warnStr + " Some value is out of integer range.");
				}
			}
			numCounter++;
		}
		//Modulus
		else if (expressionOperator[i] == operators[4]) {
		if (doesVarExist(numbers[numCounter])) {

			string type = fetchVarType(numbers[numCounter]);
			//int
			if (type == datatypes[0]) {
				result %= int_vars[numbers[numCounter]];
			}
			//dec
			if (type == datatypes[1]) {
				result %= static_cast<int>(dec_vars[numbers[numCounter]]);
			}
		}
		else {
			try {
				result %= stoi(numbers[numCounter]);
			}
			catch (invalid_argument) {
				warnings.push_back(warnStr + " Some invalid value was used.");
			}
			catch (out_of_range) {
				warnings.push_back(warnStr + " Some value is out of integer range.");
			}
		}
		numCounter++;
		}
	}

	return result;
}

float evaluateDec(string expression) {
	//For ++
	if (expression == operators[5]) {
		return 1.0;
	}
	//For --
	else if (expression == operators[6]) {
		return -1.0;
	}
	//for +=
	else if ((expression[0] == '+' || expression[0] == '-') && expression[1] == '=') {

		string newVal = "";
		for (int i = 2; i < expression.size(); i++) {
			newVal += expression[i];
		}

		//Check to see if the incrememnt is another variable
		if (doesVarExist(newVal)) {
			if (var_types[newVal] == datatypes[0]) {
				return (float)int_vars[newVal];
			}
			else if (var_types[newVal] == datatypes[1]) {
				return dec_vars[newVal];
			}
		}

		if (expression[0] == '-') {
			return -stof(newVal);
		}
		return stof(newVal);
	}

	//For all other expressions
	vector<string> expressionOperator;
	string opTemp = "";
	string numTemp = "";
	vector<string> numbers;
	bool foundOP = false;


	for (int i = 0; i < expression.size(); i++) {
		//runs through operators
		for (int j = 0; j < numOfBasicOperators; j++) {
			if (expression[i] == operators[j][0]) {
				opTemp += expression[i];
				expressionOperator.push_back(opTemp);
				numbers.push_back(numTemp);
				numTemp = "";
				opTemp = "";
				foundOP = true;
				continue;
			}
		}

		//sets the number
		if (!foundOP) {
			numTemp += expression[i];
		}
		else {
			foundOP = false;
		}
	}

	numbers.push_back(numTemp);

	int numCounter = 1;
	float result = 0;
	if (doesVarExist(numbers[0])) {
		string type = fetchVarType(numbers[0]);
		//int
		if (type == datatypes[0]) {
			result = static_cast<float>(int_vars[numbers[0]]);
		}
		//dec
		if (type == datatypes[1]) {
			result = dec_vars[numbers[0]];
		}
	}
	else {
		result = stof(numbers[0]);
	}
	for (int i = 0; i < expressionOperator.size(); i++) {

		if (numbers.size() == numCounter) {
			warnings.push_back(warnStr + " Operator with no number after it.");
			return 0;
		}

		//Addition
		if (expressionOperator[i] == operators[0]) {
			if (doesVarExist(numbers[numCounter])) {

				string type = fetchVarType(numbers[numCounter]);
				//int
				if (type == datatypes[0]) {
					result += static_cast<float>(int_vars[numbers[numCounter]]);
				}
				//dec
				if (type == datatypes[1]) {
					result += dec_vars[numbers[numCounter]];
				}
			}
			else {
				try {
					result += stof(numbers[numCounter]);
				}
				catch (invalid_argument) {
					warnings.push_back(warnStr + " Some invalid value was used.");
				}
				catch (out_of_range) {
					warnings.push_back(warnStr + " Some value is out of integer range.");
				}
			}
			numCounter++;
		}
		//Subtraction
		else if (expressionOperator[i] == operators[1]) {
			if (doesVarExist(numbers[numCounter])) {

				string type = fetchVarType(numbers[numCounter]);
				//int
				if (type == datatypes[0]) {
					result -= static_cast<float>(int_vars[numbers[numCounter]]);
				}
				//dec
				if (type == datatypes[1]) {
					result -= dec_vars[numbers[numCounter]];
				}
			}
			else {
				try {
					result -= stof(numbers[numCounter]);
				}
				catch (invalid_argument) {
					warnings.push_back(warnStr + " Some invalid value was used.");
				}
				catch (out_of_range) {
					warnings.push_back(warnStr + " Some value is out of integer range.");
				}
			}
			numCounter++;
		}
		//Multiplication
		else if (expressionOperator[i] == operators[2]) {
			if (doesVarExist(numbers[numCounter])) {

				string type = fetchVarType(numbers[numCounter]);
				//int
				if (type == datatypes[0]) {
					result *= static_cast<float>(int_vars[numbers[numCounter]]);
				}
				//dec
				if (type == datatypes[1]) {
					result *= dec_vars[numbers[numCounter]];
				}
			}
			else {
				try {
					result *= stof(numbers[numCounter]);
				}
				catch (invalid_argument) {
					warnings.push_back(warnStr + " Some invalid value was used.");
				}
				catch (out_of_range) {
					warnings.push_back(warnStr + " Some value is out of integer range.");
				}
			}
			numCounter++;
		}
		//Division
		else if (expressionOperator[i] == operators[3]) {
			if (doesVarExist(numbers[numCounter])) {

				string type = fetchVarType(numbers[numCounter]);
				//int
				if (type == datatypes[0]) {
					result /= static_cast<float>(int_vars[numbers[numCounter]]);
				}
				//dec
				if (type == datatypes[1]) {
					result /= dec_vars[numbers[numCounter]];
				}
			}
			else {
				try {
					result /= stof(numbers[numCounter]);
				}
				catch (invalid_argument) {
					warnings.push_back(warnStr + " Some invalid value was used.");
				}
				catch (out_of_range) {
					warnings.push_back(warnStr + " Some value is out of integer range.");
				}
			}
			numCounter++;
		}
		//Modulus
		else if (expressionOperator[i] == operators[4]) {
			warnings.push_back(warnStr + " Cannot perform modulus on a dec datatype.");
			return 0;
		}
	}

	return result;

}

//This will display all characters to the right of "display:" to the terminal
void displayFunction(line thisLine, int wordLocation) {
	
	if (thisLine.strLiteral == "") {
		warnings.push_back(warnStr + " Empty disaply function");
		return;
	}

	cout << thisLine.strLiteral;
	
}

//Returns true if you are in a condition, false if not. Line reader should check for tab spacing and execute if this was true
bool ifFunction(line thisLine) {
	int start = 2;
	string line = thisLine.lineStr;
	vector<string> valueVector;
	string tempValue = "";
	vector<string> valueVectorType;
	vector<bool> isVariable(20, false);
	vector<int> equalityLocation;

	for (int i = start; i < line.size(); i++) {
		if (line[i] == '=') {
			valueVector.push_back(tempValue);
			tempValue = "";
		}
		else {
			tempValue += line[i];
		}

	}
	//When the loop ends
	valueVector.push_back(tempValue);

	for (int i = 0; i < valueVector.size(); i++) {
		if (doesVarExist(valueVector.at(i))) {
			valueVectorType.push_back(fetchVarType(valueVector.at(i)));
			isVariable.at(i) = true;
		}
		else {
			if (valueVector.at(i) == "true") {
				valueVectorType.push_back("bol");
			}
			else if (valueVector.at(i) == "false") {
				valueVectorType.push_back("bol");
			}
			else {
				for (int j = 0; j < valueVector.at(i).size(); j++) {
					if (valueVector.at(i)[j] == '\"') {
						valueVectorType.push_back("str");
						//remove quotes
						string tempStr = valueVector[i];
						for (int k = 0; k < tempStr.size(); k++) {
							if (tempStr[k] == '\"') {
								tempStr.erase(tempStr.begin() + k);
							}
						}
						valueVector[i] = tempStr;
					}
					else if (valueVector.at(i)[j] == '.') {
						valueVectorType.push_back("dec");
					}
					else if (valueVector.at(i)[j] == '\'') {
						valueVectorType.push_back("chr");
						//remove single quotes
						string tempStr = valueVector[i];
						for (int k = 0; k < tempStr.size(); k++) {
							if (tempStr[k] == '\'') {
								tempStr.erase(tempStr.begin() + k);
							}
						}
						valueVector[i] = tempStr;
					}
				}
				valueVectorType.push_back("int");
			}
		}
	}

	string value1, value2;

	if (isVariable[0]) {
		//ints
		if (valueVectorType[0] == datatypes[0]) {
			value1 = to_string(int_vars[valueVector[0]]);
		}
		//dec
		else if (valueVectorType[0] == datatypes[1]) {
			value1 = to_string(dec_vars[valueVector[0]]);
		}
		//chr
		else if (valueVectorType[0] == datatypes[2]) {
			value1 = to_string(chr_vars[valueVector[0]]);
		}
		//str
		else if (valueVectorType[0] == datatypes[3]) {
			value1 = str_vars[valueVector[0]];
		}
		//bol
		else if (valueVectorType[0] == datatypes[4]) {
			if (bol_vars[valueVector[0]]) {
				value1 = "true";
			}
			else {
				value1 = "false";
			}
		}
	}

	for (int i = 0; i < valueVector.size(); i++) {
		cout << "value: " << valueVector[i] << "\t";
	}
	cout << endl;

	if (value1 == valueVector.at(1)) {
		return true;
	}
	else {
		return false;
	}

}

//This function will check if a keyword was used and handle that keywords function; returns int value of which keyword was found
int checkForKeywords(line thisLine) {
	for (auto word : keywords) {
		size_t found = thisLine.lineStr.find(word);
		if (found != string::npos) {
			//found display 
			if (word == keywords.at(0)) {
				displayFunction(thisLine, found);
				return 0;
			}
			//found if statement
			else if (word == keywords.at(1)) {
				if (ifFunction(thisLine)) {
					return 1;
				}
				else {
					//This returns the code to skip all prior tabbed lines			MUST CODE THIS IN READLINE
					return -2;
				}
			}
		}
	}

	return -1;
}



//This will EVENTUALLY update a variables value... WIP
void updateVar(line thisLine) {

	string varName = "";
	string expression = "";
	string updateBy = "";
	bool doUpdate = false;

	int newIntVal;
	float newDecVal;
	char newChrVal;
	string newStrVal;
	bool newBolVal;

	//Iterate through line to find stuff
	for (int i = 0; i < thisLine.lineStr.size(); i++) {

		varName += thisLine.lineStr[i];
		if (thisLine.lineStr[i] == '=' || thisLine.lineStr[i] == '+' || thisLine.lineStr[i] == '-') {

			varName.erase(i);
			//for ++ operator
			if (thisLine.lineStr[i] == '+' && thisLine.lineStr[i + 1] == '+') {
				expression = operators[5];
				updateBy = operators[5];
				for (int j = i + 1; j < thisLine.lineStr.size(); j++) {
					thisLine.lineStr.erase(i);
				}
			}
			//for -- operator
			else if (thisLine.lineStr[i] == '-' && thisLine.lineStr[i + 1] == '-') {
				expression = operators[6];
				updateBy = operators[6];
				for (int j = i + 1; j < thisLine.lineStr.size(); j++) {
					thisLine.lineStr.erase(i);
				}
			}
			//for += operator
			else if (thisLine.lineStr[i] == '+' && thisLine.lineStr[i + 1] == '=') {
				expression = operators[7];
				updateBy = operators[7];
				i++;
			}
			//for -= operator
			else if (thisLine.lineStr[i] == '-' && thisLine.lineStr[i + 1] == '=') {
				expression = operators[8];
				updateBy = operators[8];
				i++;
			}
			else if (thisLine.lineStr[i] == '=') {
				updateBy = "=";
			}
			//sets the expression after the update operator
			for (int j = i + 1; j < thisLine.lineStr.size(); j++) {

				expression += thisLine.lineStr[j];
			}
			doUpdate = true;
			break;
		}
	}
	if (doUpdate) {

		string varType = fetchVarType(varName);
		bool isExpression = false;
		
		//Determines if an expression is complex
		for (int i = 0; i < expression.size(); i++) {
			for (int j = 0; j < 5; j++) {
				if (expression[i] == operators[j][0]) {
					isExpression = true;
					break;
				}
			}
		}

		//new int value
		if (varType == datatypes[0]) {
			if (isExpression) {
				newIntVal = evaluateInt(expression);
				if (updateBy == operators[5] || updateBy == operators[6] || updateBy == operators[7] || updateBy == operators[8]) {
					int_vars[varName] += newIntVal;
				}
				else {
					int_vars[varName] = newIntVal;
				}
			}
			else {
				try {
					if (expression == "") {
						//If var is not declared set it to 0
						int_vars[varName] = 0;
					}
					else {
						int_vars[varName] = stoi(expression);
					}
				}
				catch (invalid_argument) {
					warnings.push_back(warnStr + " Incorrect value \'" + expression + "\' entered");
				}
				catch (out_of_range) {
					warnings.push_back(warnStr + " Value out of range");
				}
			}
		}
		//new dec value
		else if (varType == datatypes[1]) {
			if (isExpression) {
				newDecVal = evaluateDec(expression);
				if (updateBy == operators[5] || updateBy == operators[6] || updateBy == operators[7] || updateBy == operators[8]) {
					dec_vars[varName] += newDecVal;
				}
				else {
					dec_vars[varName] = newDecVal;
				}
			}
			else {
				try {
					if (expression == "") {
						//If var is not declared set it to 0
						dec_vars[varName] = 0;
					}
					else {
						dec_vars[varName] = stof(expression);
					}
				}
				catch (invalid_argument) {
					warnings.push_back(warnStr + " Incorrect value \'" + expression + "\' entered");
				}
				catch (out_of_range) {
					warnings.push_back(warnStr + " Value out of range");
				}
			}
		}
		//new chr value
		if (varType == datatypes[2]) {

			if (expression.size() > 1) {
				warnings.push_back(warnStr + " Incorrect value \'" + expression + "\' entered");
			}
			else {
				chr_vars[varName] = expression[0];
			}
		}
		//new str value
		else if (varType == datatypes[3]) {

			if (thisLine.strLiteral == "" && expression != "") {
				warnings.push_back(warnStr + " str value MUST be within quotes or be a var surrounded by ':'");
			}

			str_vars[varName] = thisLine.strLiteral;
		}
		//new bol value
		else if (varType == datatypes[4]) {

			if (expression == "") {
				bol_vars[varName] = true;
			}
			else if (expression == "true") {
				bol_vars[varName] = true;
			}
			else if (expression == "false") {
				bol_vars[varName] = false;
			}
			else {
				warnings.push_back(warnStr + " Incorrect value for " + varName + " EXPECTED: true or false or no value after '='");
			}
		}
		else {
			//do nothing
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

	//This is some mumbo jumbo to search through the line find literals
	for (int i = 0; i < thisLine.lineStr.size(); i++) {

		if (thisLine.lineStr[i] == '"' && thisLine.lineStr[i - 1] != '\\') {
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
		//Support for \n character
		if (inLiteral && thisLine.lineStr[i] == 'n' && thisLine.lineStr[i - 1] == '\\') {
			int size = thisLine.strLiteral.size();
			thisLine.strLiteral.erase(size - 1);
			thisLine.strLiteral.erase(size - 2	);
			thisLine.strLiteral += "\n";
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

				//this returns the vars type
				string varType = fetchVarType(varName);	
				varName = "";

				//This is for int
				if (varType == datatypes[0]) {	
					thisLine.strLiteral += to_string(int_vars[vars.at(varCounter - 1)]);
				}
				//This is for dec
				else if (varType == datatypes[1]) {
					thisLine.strLiteral += to_string(dec_vars[vars.at(varCounter - 1)]);
				}

				//This is for chr
				else if (varType == datatypes[2]) {
					thisLine.strLiteral += chr_vars[vars.at(varCounter - 1)];
				}

				//This is for str
				else if (varType == datatypes[3]) {
					thisLine.strLiteral += str_vars[vars.at(varCounter - 1)];
				}

				//This is for bol
				else if (varType == datatypes[4]) {
					if (bol_vars[vars.at(varCounter - 1)] == 1) {
						thisLine.strLiteral += "true";
					}
					else if (bol_vars[vars.at(varCounter - 1)] == 0) {
						thisLine.strLiteral += "false";
					}
				}
			}
		}

		//If a var is found, withing ':', this will store each char in varName.
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
		if (found != string::npos && found == 0) {
			newType.type = word;
			newType.location = found + (word.size() - 1);
			return newType;
		}
	}

	newType.location = -1;
	return newType;
}


//This function gets a new variable name and stores it in the proper unordered map of that variables declared type
int createNewVar(line thisLine, typelocation varType) {

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

	//The following loops will disallow invalid var names and tell the following if-else's to create a warning
	for (auto bad : operators) {
		if (varName == bad) {
			varType.type = "bad";
		}
	}

	for (auto bad : datatypes) {
		if (varName == bad) {
			varType.type = "bad";
		}
	}

	for (auto bad : keywords) {
		if (varName == bad) {
			varType.type = "bad";
		}
	}

	for (auto bad : var_types) {
		if (varName == bad.first) {
			varType.type = "bad";
		}
	}


	//Integer variable creation
	if (varType.type == datatypes[0]) {
		//Stores the name and data type ion the var_types unordered map
		var_types[varName] = varType.type;
		try {
			if (varValueStr == "") {
				//If var is not declared set it to 0
				int_vars[varName] = 0;	
			}
			else {
				int_vars[varName] = stoi(varValueStr);
			}
		}
		catch (invalid_argument) {
			warnings.push_back(warnStr + " Incorrect value \'" + varValueStr + "\' entered");
		}
		catch (out_of_range) {
			warnings.push_back(warnStr + " Value out of range");
		}
		return 1;
	}

	//dec variable creation 
	else if (varType.type == datatypes[1]) {
		//Stores the name and data type ion the var_types unordered map
		var_types[varName] = varType.type;
		try {
			if (varValueStr == "") {
				dec_vars[varName] = 0;
			}
			else {
				dec_vars[varName] = stof(varValueStr);
			}
		}
		catch (invalid_argument) {
			warnings.push_back(warnStr + " Incorrect value \'" + varValueStr + "\' entered");
		}
		catch (out_of_range) {
			warnings.push_back(warnStr + " Value out of range" );
		}
		return 1;
	}

	//chr variable creation
	else if (varType.type == datatypes[2]) {
		if (varValueStr.size() > 1) {
			warnings.push_back(warnStr + " Incorrect value \'" + varValueStr + "\' entered");
			return 0;
		}
		else {
			chr_vars[varName] = varValueStr[0];
			var_types[varName] = varType.type;
			return 1;
		}
	}

	//str variable creation
	else if (varType.type == datatypes[3]) {
		var_types[varName] = varType.type;
		if (thisLine.strLiteral == "" && varValueStr != "") {
			warnings.push_back(warnStr + " str value MUST be within quotes or be a var surrounded by ':'");
		}
		
		str_vars[varName] = thisLine.strLiteral;

		return 1;

	}

	//bol variable creation
	else if (varType.type == datatypes[4]) {
		var_types[varName] = varType.type;
		if (varValueStr == "") {
			bol_vars[varName] = 1;
		}
		else if (varValueStr == "true") {
			bol_vars[varName] = 1;
		}
		else if (varValueStr == "false") {
			bol_vars[varName] = 1;
		}
		else {
			warnings.push_back(warnStr + " Incorrect value for " + varName + " EXPECTED: true or false or no value after '='");
			return 0;
		}

		return 1;
	}

	else if (varType.type == "bad") {
		warnings.push_back(warnStr + " " +varName+ " is an invalid variable name");
		return 2;
	}

	return 2;
}

//this function will interpret each line
void readLine(line thisLine) {

	typelocation newType;
	warnStr = "WARNING[line " + to_string(thisLine.lineNum) + "]:";
	int varCreated = 0;
	int keyWordCode = -1;
	string conditional;

	//This will also handle the spacing within string literals
	thisLine = removeWhitespace(thisLine);

	//Find new datatype and then create a new variable
	newType = searchForType(thisLine);
	if (newType.location != -1) {
		varCreated = createNewVar(thisLine, newType);
	}
	
	//This will be called to update variables and check if a keyword was used 
	if (varCreated == 0 && thisLine.lineStr != "") {
		keyWordCode = checkForKeywords(thisLine);
		if (keyWordCode == -1) {
			updateVar(thisLine);
		}
		else if (keyWordCode == 1) {
			conditional = "The conditional on line " + to_string(thisLine.lineNum) + " will run";
			cout << conditional << endl;
		}
		else if (keyWordCode == -2) {
			conditional = "The conditional on line " + to_string(thisLine.lineNum) + " will NOT run";
			cout << conditional << endl;
		}
		
	}
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
	cout << "\n\n";
	openFile(fileName);

	/*	THIS DISPLAYS ALL THE VARS STORED IN THE PROGRAM
	cout << "\n\n\n\n" << "All vars listed below" << endl;
	for (auto var : int_vars) {
		cout << "int name: " << var.first << " = " << var.second << endl;
	}
	for (auto var : dec_vars) {
		cout << "dec name: " << var.first << " = " << var.second << endl;
	}
	for (auto var : chr_vars) {
		cout << "chr name: " << var.first << " = " << var.second << endl;
	}
	for (auto var : str_vars) {
		cout << "str name: " << var.first << " = " << var.second << endl;
	}
	for (auto var : bol_vars) {
		cout << "bol name: " << var.first << " = " << var.second << endl;
	}
	*/

	//This will display any warnings in case there are any
	if (warnings.size() >= 1) {
		cout << "\n\n\n\n";
		for (auto warn : warnings) {
			cout << warn << endl;
		}
	}

	return 0;
}
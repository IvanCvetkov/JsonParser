#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include "KeyValuePair.h"
#include "JsonObject.h"
#include "JsonInfo.h"
#include "Array.h"


using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::stringstream;



void serializeDefaultKeyValuePair(std::ofstream& fileStream, const KeyValuePair& kvp) {
	fileStream << "\"" << kvp.GetKey() << "\": \"" << kvp.GetValue() << ",";
}

void serializeKeyValuePairInJsonObject(std::ofstream& fileStream, const KeyValuePair& kvp, bool isLast) {
	if (!isLast)
		fileStream << "\"" << kvp.GetKey() << "\": \"" << kvp.GetValue() << "\",";
	else
		fileStream << "\"" << kvp.GetKey() << "\": \"" << kvp.GetValue() << "\"";
}


void serializeArray(std::ofstream& fileStream, Array& arr, bool isLast) {
	bool isKey = true;
	bool isCurrentIndexValid = false;
	vector<string> notEmptyIndexes;


	for (size_t i = 0; i < arr.GetElements().size(); i++)
		if (!arr.GetElements()[i].empty())
			notEmptyIndexes.push_back(arr.GetElements()[i]);


	fileStream << "\"" << arr.GetName() << "\": [{";
	for (size_t i = 0; i < notEmptyIndexes.size(); ++i) {

		bool tempContainsElement = false;
		vector<string> temp;


		if (i < (notEmptyIndexes.size() - 1)) {
			if (!isKey) {

				for (size_t j = 0; j < temp.size(); j++)
				{
					if (temp[j] == notEmptyIndexes[i]) {
						tempContainsElement = true;
					}
				}

				if (!tempContainsElement) {

					fileStream << "\"" << notEmptyIndexes[i] << "\",";
					temp.push_back(notEmptyIndexes[i]);
				}
				if (tempContainsElement) {

					fileStream << "\"" << notEmptyIndexes[i] << "\"},{";

					tempContainsElement = false;
					temp.clear();
				}

				isKey = true;
			}
			else {
				fileStream << "\"" << notEmptyIndexes[i] << "\":";
				isKey = false;
			}
		}
		else if (i == (notEmptyIndexes.size()) - 1)
			fileStream << "\"" << notEmptyIndexes[i] << "\"";
	}

	if (isLast)
		fileStream << "}]";
	else
		fileStream << "}],";
}

void serializeJsonObject(std::ofstream& fileStream, JsonObject& obj) {
	bool isLast = false;
	fileStream << "\"" << obj.GetName() << "\": {";
	for (size_t i = 0; i < obj.GetObjValues().size(); ++i) {
		if (i < obj.GetObjValues().size() - 1) {
			serializeKeyValuePairInJsonObject(fileStream, obj.GetObjValues()[i], isLast);
		}
		else {
			isLast = true;
			serializeKeyValuePairInJsonObject(fileStream, obj.GetObjValues()[i], isLast);
		}
	}

	fileStream << "},";
}

void serializeJsonInfo(JsonInfo& jsonInfo, string path) {
	bool isLast = false;
	std::ofstream outputFile(path);
	if (!outputFile.is_open()) {
		std::cerr << "Failed to create JSON file." << std::endl;
		return;
	}

	outputFile << "{";

	// Serializing key-value pairs
	for (size_t i = 0; i < jsonInfo.GetKVP().size(); ++i) {
		serializeDefaultKeyValuePair(outputFile, jsonInfo.GetKVP()[i]);
	}

	// Serializing objects
	for (size_t i = 0; i < jsonInfo.GetObjValues().size(); ++i) {
		serializeJsonObject(outputFile, jsonInfo.GetObjValues()[i]);
	}

	// Serializing arrays
	for (size_t i = 0; i < jsonInfo.GetArrays().size(); ++i) {
		if (i < jsonInfo.GetArrays().size() - 1) {
			serializeArray(outputFile, jsonInfo.GetArrays()[i], isLast);
		}
		else {
			isLast = true;
			serializeArray(outputFile, jsonInfo.GetArrays()[i], isLast);
		}
	}

	outputFile << "}";

	outputFile.close();
	std::cout << "JSON file created successfully." << std::endl;
}


static bool isCharacter(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static string ReverseStr(string str) {
	string temp;

	for (int i = (int)str.length() - 1; i >= 0; --i)
		temp.push_back(str[i]);

	return temp;
}



static JsonInfo parseJsonFile(const string& filePath) {
	JsonInfo jsonInfo;
	jsonInfo.GetFileName() = filePath;

	ifstream file(filePath);
	if (!file.is_open()) {
		cout << "Failed to open file: " << filePath << endl;
		return jsonInfo;
	}

	stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	string jsonString = buffer.str();

	size_t pos = 0;
	while (pos < jsonString.length()) {
		if (jsonString[pos] == '{') {
			++pos;
			continue;
		}

		if (jsonString[pos] == '}') {
			break;
		}

		string key = "";
		string value = "";
		if (jsonString[pos] == '"') {
			++pos;
			while (jsonString[pos] != '"') {
				key += jsonString[pos];
				++pos;
			}

			++pos;
			while (jsonString[pos] == ' ' || jsonString[pos] == ':') {
				++pos;
			}

			if (jsonString[pos] == '[') {
				++pos;
				Array array;
				array.GetName() = key;
				while (true) {
					while (jsonString[pos] == ' ' || jsonString[pos] == ',') {
						++pos;
					}

					if (jsonString[pos] == ']') {
						break;
					}

					string element = "";
					if (jsonString[pos] == '"') {
						++pos;
						while (jsonString[pos] != '"') {
							element += jsonString[pos];
							++pos;
						}
					}

					array.GetElements().push_back(element);
					++pos;
				}

				jsonInfo.GetArrays().push_back(array);
			}
			else if (jsonString[pos] == '{') {
				int backwardPos = 0;
				int quoteCounter = 0;

				string jsonObjectName = "";

				while (quoteCounter < 2) {
					--pos;

					if (jsonString[pos] == '"') quoteCounter++;
					else if (jsonString[pos] == ' ' || jsonString[pos] == ':') {}

					else jsonObjectName.push_back(jsonString[pos]);

					++backwardPos;
				}

				jsonObjectName = ReverseStr(jsonObjectName);
				pos += backwardPos;

				++pos;
				JsonObject jsonObject;
				jsonObject.GetName() = jsonObjectName;

				// Extract the name of the JsonObject
				while (jsonString[pos] == ' ' || jsonString[pos] == '"' || jsonString[pos] == ',') {
					++pos;
				}

				while (true) {
					while (jsonString[pos] == ' ' || jsonString[pos] == ',') {
						++pos;
					}

					if (jsonString[pos] == '}') {
						break;
					}

					string objectKey = "";
					string objectValue = "";
					if (jsonString[pos] == '"') {
						++pos;
						while (jsonString[pos] != '"') {
							objectKey += jsonString[pos];
							++pos;
						}

						++pos;
						while (jsonString[pos] == ' ' || jsonString[pos] == ':') {
							++pos;
						}

						if (jsonString[pos] == '"') {
							++pos;
							while (jsonString[pos] != '"') {
								objectValue += jsonString[pos];
								++pos;
							}
						}

						KeyValuePair kvp;
						kvp.GetKey() = objectKey;
						kvp.GetValue() = objectValue;
						jsonObject.GetObjValues().push_back(kvp);
					}

					++pos;
				}


				jsonInfo.GetObjValues().push_back(jsonObject);
			}
			else {
				while (jsonString[pos] == ' ' || jsonString[pos] == '"') {
					++pos;
				}

				while (jsonString[pos] != ',' && jsonString[pos] != '}') {
					value += jsonString[pos];
					++pos;
				}

				KeyValuePair kvp;
				kvp.GetKey() = key;
				kvp.GetValue() = value;
				jsonInfo.GetKVP().push_back(kvp);
			}
		}
		++pos;
	}

	return jsonInfo;
}

vector<string> splitString(const string& str, char delimiter) {
	vector<string> tokens;
	std::istringstream iss(str);
	string token;

	while (getline(iss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}



/*vector<vector<int>> takeIndexesToDelete(const JsonInfo& object, string str) {
 string dash = "/";
 vector<vector<int>> indexesToDelete; // all arrays indexes, and last index of vector for key-value pairs

 // if string contains / then search by rest of information
 if (str.find(dash) != std::string::npos) {
 for (int i = 0; i < object.arrays.size(); i++) {

 vector<int> indexes;
 for (int i = 0; i < object.arrays[i].elements.size(); i++) {

 // split string by delimeter
 vector<string> strings;
 std::istringstream f(str);

 string s;
 while (getline(f, s, '/')) {
 strings.push_back(s);
 }

 int res = object.arrays[i].elements[i].compare(strings[1]);
 if (res == 0) {
 indexes.push_back(i);
 }
 }

 indexesToDelete.push_back(indexes);
 }
 }
 else {
 vector<int> indexes;
 for (int i = 0; i < object.GetKVP().size(); i++) {
 int res = object.GetKVP()[i].key.compare(str);
 if (res == 0) {
 indexes.push_back(i);
 }
 }

 indexesToDelete.push_back(indexes);
 }

 return indexesToDelete;
 }*/

void deleteEntryByKey(JsonInfo& jsonInfo, string key) {
	// delete only first occurence as there can be multiple key-value pairs with the same key
	// Delete from key-values

	char delimeter = '/';
	vector<string> data = splitString(key, delimeter);


	// if no '/' present
	if (key.find(delimeter) != string::npos) {
		for (auto it = jsonInfo.GetKVP().begin(); it != jsonInfo.GetKVP().end();) {
			if (it->GetKey() == key) {
				it = jsonInfo.GetKVP().erase(it);
				cout << "Entry (KVP) deleted successfully!" << endl;
				return;
			}
			else
				++it;
		}

		for (size_t i = 0; i < jsonInfo.GetObjValues().size(); i++) {

			if (jsonInfo.GetObjValues()[i].GetName() == data[0]) {
				cout << "Entry (JsonObject) deleted successfully!" << endl;
				jsonInfo.GetObjValues().erase(jsonInfo.GetObjValues().begin() + i);
				return;
			}
		}
	}
	else {

		for (auto& arr : jsonInfo.GetArrays()) {
			if (arr.GetName() == data[0]) { // Check if the array name matches the first key
				vector<string> elements = arr.GetElements();

				// Iterate through the elements of the array
				for (auto it = elements.begin(); it != elements.end(); ++it) {
					if (*it == data[1]) { // Check if the element matches the value to be deleted
						elements.erase(it);

						auto nextIt = std::next(it);
						if (nextIt != elements.end())
							elements.erase(nextIt);

						cout << "Entry (Array) deleted successfully!" << endl;
						return;
					}
				}
			}
		}

		for (auto it = jsonInfo.GetObjValues().begin(); it != jsonInfo.GetObjValues().end(); ++it) {
			if (it->GetName() == data[0]) {
				for (auto it2 = it->GetObjValues().begin(); it2 != it->GetObjValues().end(); ++it2) {
					if (it2->GetKey() == data[1]) {
						cout << "Entry (JsonObject) deleted successfully!" << endl;
						it->GetObjValues().erase(it2);
						return;
					}
				}
			}
		}

		cout << "No such path!" << endl;
	}
}

string searchElement( JsonInfo& object, string str) {
	string result;
	string temp;
	int counter = 0;

	// check singular key-value pairs
	for ( auto& kvp : object.GetKVP()) {
		int res = kvp.GetKey().compare(str);

		if (res == 0) {
			temp.append("   \"");
			temp.append(kvp.GetValue());
			temp.append(",\n");

			result.append(temp);
			temp = "";

			counter++;
		}
	}

	// check arrays of key-value pairs
	for ( auto& array : object.GetArrays()) {
		for (int i = 0; i < array.GetElements().size(); i++) {
			int res = array.GetElements()[i].compare(str);

			if (res == 0) {
				temp.append("   \"");
				temp.append(array.GetElements()[i + 2]);
				temp.append("\",");
				temp.append("\n");

				result.append(temp);
				temp = "";

				counter++;
			}
		}
	}

	for ( auto& objectValue : object.GetObjValues()) {
		if (objectValue.GetName().compare(str) == 0) {

			temp.append("\"");
			temp.append(objectValue.GetName());
			temp.append("\"\n");
			counter++;

			for ( auto& kvp : objectValue.GetObjValues()) {
				temp.append("\"");
				temp.append(kvp.GetKey());
				temp.append("\"");
				temp.append(": ");
				temp.append("\"");
				temp.append(kvp.GetValue());
				temp.append("\"\n");


			}
		}

		result.append(temp);
		temp = "";
	}

	cout << "The key " << str << " occured in " << counter << " places!" << "\n[" << endl;
	return result;
}

void print(JsonInfo& jsonInfo) {
	cout << "File Name: " << jsonInfo.GetFileName() << endl;

	cout << "Key-Value Pairs:" << endl;
	for (const auto& kvp : jsonInfo.GetKVP())
		cout << kvp.GetKey() << ": " << kvp.GetValue() << endl;

	cout << "Arrays:" << endl;
	for (const auto& array : jsonInfo.GetArrays()) {
		cout << array.GetName() << ":" << endl;


		if (array.GetElements().empty())
			cout << "Empty Array!" << endl;
		else
			for (const auto& element : array.GetElements())
				cout << "- " << element << endl;
	}

	for ( auto& object : jsonInfo.GetObjValues()) {
		cout << "Object Key: " << object.GetName() << endl;
		cout << "Object Values:" << endl;
		for ( auto& kvp : object.GetObjValues()) {
			cout << "- Key: " << kvp.GetKey() << ", Value: " << kvp.GetValue() << endl;
		}
		cout << endl;
	}
}

bool validateBraces(string str) {
	int bracesCounter = 0;

	for (int i = 0; i < str.length(); i++)
		if (str[i] == '{' || str[i] == '}')
			bracesCounter++;

	return (bracesCounter % 2 == 0);
}

bool validateQuotes(string str) {
	int quotesCounter = 0;

	for (int i = 0; i < str.length(); i++)
		if (str[i] == '\"')
			quotesCounter++;

	return (quotesCounter % 2 == 0);
}

bool validate(const string& filePath) {

	// possible fix -> don't read from file / build string from JsonInfo class
	ifstream file(filePath);
	if (!file.is_open()) {
		cout << "Failed to open file: " << filePath << endl;
		return false;
	}

	stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	string str = buffer.str();


	return (validateBraces(str) && validateQuotes(str));
}

void find( JsonInfo& jsonInfo, string str) {
	int res;
	for (const auto& kvp : jsonInfo.GetKVP()) {
		res = kvp.GetKey().compare(str);
		if (res == 0) {
			cout << kvp.GetKey() << ": " << kvp.GetValue() << endl;
			return;
		}
	}



	for (const auto& array : jsonInfo.GetArrays()) {
		res = array.GetName().compare(str);
		if (array.GetElements().empty())
			cout << "Empty Array!" << endl;

		else if (res == 0) {
			cout << array.GetName() << ":" << endl;
			for (const auto& element : array.GetElements()) {
				cout << "- " << element << endl;
			}

			return;
		}
	}



	for ( auto& objectValue : jsonInfo.GetObjValues()) {
		res = objectValue.GetName().compare(str);
		if (res == 0) {
			cout << "Object Key: " << objectValue.GetName() << endl;
			cout << "Object Values: " << endl;
			for ( auto& kvp : objectValue.GetObjValues()) {
				cout << "- Key: " << kvp.GetKey() << ", Value: " << kvp.GetValue() << endl;
			}

			return;
		}
	}

	cout << "Path doesn't exist!" << endl;
}

void set(JsonInfo& object, string path, string value) {
	// set only the first occurence of the key
	// as it's thought to be unique given it's path.
	int res;
	char delimeter = '/';
	vector<string> data = splitString(path, delimeter);


	// if no '/' present
	if (path.find(delimeter) == string::npos) {
		for (auto& kvp : object.GetKVP()) {
			if (kvp.GetKey() == data[0]) {
				kvp.GetValue() = "\"" + value + "\"";
				cout << "Value set successfully!" << endl;
				return;
			}
		}
	}
	else {
		for (auto& objectValue : object.GetObjValues()) {
			res = objectValue.GetName().compare(data[0]);
			if (res == 0) {
				for (auto& kvp : objectValue.GetObjValues()) {
					res = kvp.GetKey().compare(data[1]);
					if (res == 0) {
						kvp.GetValue() = value;
						cout << "Value set successfully!" << endl;
						return;
					}
				}
			}
		}

		for (auto& array : object.GetArrays()) {
			res = array.GetName().compare(data[0]);

			if (res == 0) {
				for (int i = 0; i < array.GetElements().size(); i++) {
					res = array.GetElements()[i].compare(data[1]);
					if (res == 0) {
						array.GetElements()[i + 2] = value;
						cout << "Value set successfully!" << endl;
						return;
					}
				}
			}
		}
	}


	cout << "Can't set new value, path doesn't exist!" << endl;
}

bool validateValue(const string& str) {

	for (int i = 0; i < str.length(); i++)
		if (!isCharacter(str[i]))
			return false;

	return true;
}

void create(JsonInfo& object, string path, string key, string value) {

	char delimeter = '/';
	bool isValid = validateValue(key);
	vector<string> data = splitString(path, delimeter);

	if (isValid) {

		// default path is a key-value pair in the JSON structure
		// the key is validated beforehand
		if (path == "default") {
			KeyValuePair pair;
			pair.GetKey() = key;
			pair.GetValue() = value;

			object.GetKVP().push_back(pair);
			return;
		}


		// JsonObject
		else if (path.find(delimeter) != string::npos) {
			for (auto& objectValue : object.GetObjValues()) {

				if ((objectValue.GetName().compare(data[0])) == 0) {

					for (auto& kvp : objectValue.GetObjValues()) {
						if ((kvp.GetKey().compare(data[1])) == 0) {
							cout << "Key-Value pair already exists!" << endl;
							return;
						}
					}

					// check if kvp in arr exists, if so, print error
					// if not, add it to the arr's kvp's

					KeyValuePair pair;
					pair.GetKey() = key;
					pair.GetValue() = value;

					objectValue.GetObjValues().push_back(pair);

					return;
				}
			}


			// Arrays
			for (auto& array : object.GetArrays()) {

				if ((array.GetName().compare(data[0])) == 0) {

					for (auto& element : array.GetElements()) {
						if ((element.compare(data[1])) == 0) {
							cout << "Key-Value pair already exists!" << endl;
							return;
						}
					}


					array.GetElements().push_back(key);
					array.GetElements().push_back(" ");
					array.GetElements().push_back(value);

					return;
				}
			}
		}
	}
}

bool isArrayPath(JsonInfo& object, string path) {
	char delimeter = '/';
	vector<string> data = splitString(path, delimeter);

	for (int i = 0; i < object.GetArrays().size(); i++)
		if (object.GetArrays()[i].GetName() == data[0])
			return true;
	return false;
}

bool isJsonObjectPath(JsonInfo& object, string path) {
	char delimeter = '/';
	vector<string> data = splitString(path, delimeter);

	for (int i = 0; i < object.GetObjValues().size(); i++)
		if (object.GetObjValues()[i].GetName() == data[0])
			return true;
	return false;
}


void move(JsonInfo& object, string path, string newPath) {

	char delimeter = '/';
	vector<string> data = splitString(path, delimeter);

	// Array, JsonObject - KVP
	if (path == "default") {

		bool isArray = isJsonObjectPath(object, newPath);
		bool isJsonObject = isJsonObjectPath(object, newPath);



		if (isArray) {
			vector<KeyValuePair> pairsToAdd;

			// if '/' present
			if (path.find(delimeter) == string::npos) {
				// single KVP from array
				for (auto& array : object.GetArrays()) {
					if ((array.GetName().compare(data[0])) == 0) {
						for (int i = 0; i < array.GetElements().size(); i++) {
							if ((array.GetElements()[i].compare(data[1])) == 0) {
								KeyValuePair pair;
								pair.GetKey() = array.GetElements()[i];
								pair.GetValue() = array.GetElements()[i + 2];

								pairsToAdd.push_back(pair);
							}
						}
					}
				}
			}
			else {
				for (auto& array : object.GetArrays()) {
					if ((array.GetName().compare(data[0])) == 0) {
						for (int i = 0; i < array.GetElements().size(); i++) {
							KeyValuePair pair;
							pair.GetKey() = array.GetElements()[i];
							pair.GetValue() = array.GetElements()[i + 2];

							pairsToAdd.push_back(pair);
						}
					}
				}
			}

			for (int i = 0; i < pairsToAdd.size(); i++)
				object.GetKVP().push_back(pairsToAdd[i]);
		}

		if (isJsonObject) {
			vector<KeyValuePair> pairsToAdd;

			// if '/' present
			if (path.find(delimeter) == string::npos) {
				for (auto& objectValue : object.GetObjValues()) {
					if ((objectValue.GetName().compare(data[0])) == 0) {
						for (auto& kvp : objectValue.GetObjValues()) {
							if ((kvp.GetKey().compare(data[1])) == 0) {

								KeyValuePair pair;
								pair.GetKey() = kvp.GetKey();
								pair.GetValue() = kvp.GetValue();

								pairsToAdd.push_back(pair);
							}
						}
					}
				}
			}
			else {
				for (auto& objectValue : object.GetObjValues()) {
					if ((objectValue.GetName().compare(data[0])) == 0) {
						for (auto& kvp : objectValue.GetObjValues()) {

							KeyValuePair pair;
							pair.GetKey() = kvp.GetKey();
							pair.GetValue() = kvp.GetValue();

							pairsToAdd.push_back(pair);
						}
					}
				}
			}

			for (int i = 0; i < pairsToAdd.size(); i++)
				object.GetKVP().push_back(pairsToAdd[i]);
		}
	}


}


void save(JsonInfo& object, string filePath, string defaultPath) {
	if (filePath == "")
		serializeJsonInfo(object, defaultPath);
	else
		serializeJsonInfo(object, filePath);
}

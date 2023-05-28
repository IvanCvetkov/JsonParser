#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>

#define MAX 1000

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::stringstream;
//using std::cerr;

class KeyValuePair {
public:
	string key;
	string value;
};

class Array {
public:
	string name;
	vector<string> elements;
};

class JsonObject {
public:
	string name;
	vector<KeyValuePair> objectValues;
};

class JsonInfo : public JsonObject, KeyValuePair, Array {
public:
	string fileName;
	vector<KeyValuePair> keyValues;
	vector<JsonObject> objectValues;
	vector<Array> arrays;



	/*
	 {
	   "name":"ACME Software Co.",
	   "type":"Software Development Company",
	   "offices": [
				   {
					   "name":"Headquarters",
					   "address":"Sofia"
				   },
				   {
					   "name":"Front Office",
					   "address":"New York City"
				   }
				  ],
	   "members":[
				   {
					   "id" : "0",
					   "name" : "John Smith",
					   "birthdate" : "1980-01-01"
				   },
				   {
					   "id" : "1",
					   "name" : "Jane Smith",
					   "birthdate" : "1981-02-02"
				   },
				   {
					   "id" : "2",
					   "name" : "John Doe",
					   "birthdate" : "1982-03-03"
				   }
			   ],
	   "management":{
					   "directorId":"0",
					   "presidentId":"1"
					}
	 }
	 */



	void serializeDefaultKeyValuePair(std::ofstream& fileStream, const KeyValuePair& kvp) {
		fileStream << "\"" << kvp.key << "\": \"" << kvp.value << ",";
	}

	void serializeKeyValuePairInJsonObject(std::ofstream& fileStream, const KeyValuePair& kvp, bool isLast) {
		if (!isLast)
			fileStream << "\"" << kvp.key << "\": \"" << kvp.value << "\",";
		else
			fileStream << "\"" << kvp.key << "\": \"" << kvp.value << "\"";
	}


	void serializeArray(std::ofstream& fileStream, Array& arr, bool isLast) {
		bool isKey = true;
		bool isCurrentIndexValid = false;
		vector<string> notEmptyIndexes;
		

		
		for (size_t i = 0; i < arr.elements.size(); i++)
		{
			if (!arr.elements[i].empty()) {
				notEmptyIndexes.push_back(arr.elements[i]);
			}
		}


		fileStream << "\"" << arr.name << "\": [{";
		for (size_t i = 0; i < notEmptyIndexes.size(); ++i) {

			if (i < (notEmptyIndexes.size() - 1)) {
				if (!isKey) {
					fileStream << "\"" << notEmptyIndexes[i] << "\",";
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

	void serializeJsonObject(std::ofstream& fileStream, const JsonObject& obj) {
		bool isLast = false;
		fileStream << "\"" << obj.name << "\": {";
		for (size_t i = 0; i < obj.objectValues.size(); ++i) {
			if (i < obj.objectValues.size() - 1) {
				serializeKeyValuePairInJsonObject(fileStream, obj.objectValues[i], isLast);
			}
			else {
				isLast = true;
				serializeKeyValuePairInJsonObject(fileStream, obj.objectValues[i], isLast);
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
		for (size_t i = 0; i < jsonInfo.keyValues.size(); ++i) {
			serializeDefaultKeyValuePair(outputFile, jsonInfo.keyValues[i]);
		}

		// Serializing objects
		for (size_t i = 0; i < jsonInfo.objectValues.size(); ++i) {
			serializeJsonObject(outputFile, jsonInfo.objectValues[i]);
		}

		// Serializing arrays
		for (size_t i = 0; i < jsonInfo.arrays.size(); ++i) {
			if (i < jsonInfo.arrays.size() - 1) {
				serializeArray(outputFile, jsonInfo.arrays[i], isLast);
			}
			else {
				isLast = true;
				serializeArray(outputFile, jsonInfo.arrays[i], isLast);
			}
		}

		outputFile << "}";

		outputFile.close();
		std::cout << "JSON file created successfully." << std::endl;
	}












	//helper functions
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
		jsonInfo.fileName = filePath;

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
					array.name = key;
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

						array.elements.push_back(element);
						++pos;
					}

					jsonInfo.arrays.push_back(array);
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
					jsonObject.name = jsonObjectName;

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
							kvp.key = objectKey;
							kvp.value = objectValue;
							jsonObject.objectValues.push_back(kvp);
						}

						++pos;
					}


					jsonInfo.objectValues.push_back(jsonObject);
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
					kvp.key = key;
					kvp.value = value;
					jsonInfo.keyValues.push_back(kvp);
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
	 for (int i = 0; i < object.keyValues.size(); i++) {
	 int res = object.keyValues[i].key.compare(str);
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
			for (auto it = jsonInfo.keyValues.begin(); it != jsonInfo.keyValues.end();) {
				if (it->key == key) {
					it = jsonInfo.keyValues.erase(it);
					cout << "Entry (KVP) deleted successfully!" << endl;
					return;
				}
				else
					++it;
			}

			for (size_t i = 0; i < jsonInfo.objectValues.size(); i++) {

				if (jsonInfo.objectValues[i].name == data[0]) {
					cout << "Entry (JsonObject) deleted successfully!" << endl;
					jsonInfo.objectValues.erase(jsonInfo.objectValues.begin() + i);
					return;
				}
			}
		}
		else {

			for (auto& array : jsonInfo.arrays) {
				if (array.name == data[0]) { // Check if the array name matches the first key
					vector<string>& elements = array.elements;

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

			for (auto it = jsonInfo.objectValues.begin(); it != jsonInfo.objectValues.end(); ++it) {
				if (it->name == data[0]) {
					for (auto it2 = it->objectValues.begin(); it2 != it->objectValues.end(); ++it2) {
						if (it2->key == data[1]) {
							cout << "Entry (JsonObject) deleted successfully!" << endl;
							it->objectValues.erase(it2);
							return;
						}
					}
				}
			}

			cout << "No such path!" << endl;
		}
	}

	string searchElement(const JsonInfo& object, string str) {
		string result;
		string temp;
		int counter = 0;

		// check singular key-value pairs
		for (const auto& kvp : object.keyValues) {
			int res = kvp.key.compare(str);

			if (res == 0) {
				temp.append("   \"");
				temp.append(kvp.value);
				temp.append(",\n");

				result.append(temp);
				temp = "";

				counter++;
			}
		}

		// check arrays of key-value pairs
		for (const auto& array : object.arrays) {
			for (int i = 0; i < array.elements.size(); i++) {
				int res = array.elements[i].compare(str);

				if (res == 0) {
					temp.append("   \"");
					temp.append(array.elements[i + 2]);
					temp.append("\",");
					temp.append("\n");

					result.append(temp);
					temp = "";

					counter++;
				}
			}
		}

		for (const auto& objectValue : object.objectValues) {
			if (objectValue.name.compare(str) == 0) {

				temp.append("\"");
				temp.append(objectValue.name);
				temp.append("\"\n");
				counter++;

				for (const auto& kvp : objectValue.objectValues) {
					temp.append("\"");
					temp.append(kvp.key);
					temp.append("\"");
					temp.append(": ");
					temp.append("\"");
					temp.append(kvp.value);
					temp.append("\"\n");


				}
			}

			result.append(temp);
			temp = "";
		}

		cout << "The key " << str << " occured in " << counter << " places!" << "\n[" << endl;
		return result;
	}

	void print(const JsonInfo& jsonInfo) {
		cout << "File Name: " << jsonInfo.fileName << endl;

		cout << "Key-Value Pairs:" << endl;
		for (const auto& kvp : jsonInfo.keyValues)
			cout << kvp.key << ": " << kvp.value << endl;

		cout << "Arrays:" << endl;
		for (const auto& array : jsonInfo.arrays) {
			cout << array.name << ":" << endl;


			if (array.elements.empty())
				cout << "Empty Array!" << endl;
			else
				for (const auto& element : array.elements)
					cout << "- " << element << endl;
		}

		for (const auto& object : jsonInfo.objectValues) {
			cout << "Object Key: " << object.name << endl;
			cout << "Object Values:" << endl;
			for (const auto& kvp : object.objectValues) {
				cout << "- Key: " << kvp.key << ", Value: " << kvp.value << endl;
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

	void find(const JsonInfo& jsonInfo, string str) {
		int res;
		for (const auto& kvp : jsonInfo.keyValues) {
			res = kvp.key.compare(str);
			if (res == 0) {
				cout << kvp.key << ": " << kvp.value << endl;
				return;
			}
		}



		for (const auto& array : jsonInfo.arrays) {
			res = array.name.compare(str);
			if (array.elements.empty())
				cout << "Empty Array!" << endl;

			else if (res == 0) {
				cout << array.name << ":" << endl;
				for (const auto& element : array.elements) {
					cout << "- " << element << endl;
				}

				return;
			}
		}



		for (const auto& objectValue : jsonInfo.objectValues) {
			res = objectValue.name.compare(str);
			if (res == 0) {
				cout << "Object Key: " << objectValue.name << endl;
				cout << "Object Values: " << endl;
				for (const auto& kvp : objectValue.objectValues) {
					cout << "- Key: " << kvp.key << ", Value: " << kvp.value << endl;
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
			for (auto& kvp : object.keyValues) {
				if (kvp.key == key) {
					kvp.value = "\"" + value + "\"";
					cout << "Value set successfully!" << endl;
					return;
				}
			}
		}
		else {
			for (auto& objectValue : object.objectValues) {
				res = objectValue.name.compare(data[0]);
				if (res == 0) {
					for (auto& kvp : objectValue.objectValues) {
						res = kvp.key.compare(data[1]);
						if (res == 0) {
							kvp.value = value;
							cout << "Value set successfully!" << endl;
							return;
						}
					}
				}
			}

			for (auto& array : object.arrays) {
				res = array.name.compare(data[0]);

				if (res == 0) {
					for (int i = 0; i < array.elements.size(); i++) {
						res = array.elements[i].compare(data[1]);
						if (res == 0) {
							array.elements[i + 2] = value;
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
				pair.key = key;
				pair.value = value;

				object.keyValues.push_back(pair);
				return;
			}


			// JsonObject
			else if (path.find(delimeter) != string::npos) {
				for (auto& objectValue : object.objectValues) {

					if ((objectValue.name.compare(data[0])) == 0) {

						for (auto& kvp : objectValue.objectValues) {
							if ((kvp.key.compare(data[1])) == 0) {
								cout << "Key-Value pair already exists!" << endl;
								return;
							}
						}

						// check if kvp in arr exists, if so, print error
						// if not, add it to the arr's kvp's

						KeyValuePair pair;
						pair.key = key;
						pair.value = value;

						objectValue.objectValues.push_back(pair);

						return;
					}
				}


				// Arrays
				for (auto& array : object.arrays) {

					if ((array.name.compare(data[0])) == 0) {

						for (auto& element : array.elements) {
							if ((element.compare(data[1])) == 0) {
								cout << "Key-Value pair already exists!" << endl;
								return;
							}
						}


						array.elements.push_back(key);
						array.elements.push_back(" ");
						array.elements.push_back(value);

						return;
					}
				}
			}
		}
	}

	bool isArrayPath(JsonInfo& object, string path) {
		char delimeter = '/';
		vector<string> data = splitString(path, delimeter);

		for (int i = 0; i < object.arrays.size(); i++)
			if (object.arrays[i].name == data[0])
				return true;
		return false;
	}

	bool isJsonObjectPath(JsonInfo& object, string path) {
		char delimeter = '/';
		vector<string> data = splitString(path, delimeter);

		for (int i = 0; i < object.objectValues.size(); i++)
			if (object.objectValues[i].name == data[0])
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
					for (auto& array : object.arrays) {
						if ((array.name.compare(data[0])) == 0) {
							for (int i = 0; i < array.elements.size(); i++) {
								if ((array.elements[i].compare(data[1])) == 0) {
									KeyValuePair pair;
									pair.key = array.elements[i];
									pair.value = array.elements[i + 2];

									pairsToAdd.push_back(pair);
								}
							}
						}
					}
				}
				else {
					for (auto& array : object.arrays) {
						if ((array.name.compare(data[0])) == 0) {
							for (int i = 0; i < array.elements.size(); i++) {
								KeyValuePair pair;
								pair.key = array.elements[i];
								pair.value = array.elements[i + 2];

								pairsToAdd.push_back(pair);
							}
						}
					}
				}

				for (int i = 0; i < pairsToAdd.size(); i++)
					object.keyValues.push_back(pairsToAdd[i]);
			}

			if (isJsonObject) {
				vector<KeyValuePair> pairsToAdd;

				// if '/' present
				if (path.find(delimeter) == string::npos) {
					for (auto& objectValue : object.objectValues) {
						if ((objectValue.name.compare(data[0])) == 0) {
							for (auto& kvp : objectValue.objectValues) {
								if ((kvp.key.compare(data[1])) == 0) {

									KeyValuePair pair;
									pair.key = kvp.key;
									pair.value = kvp.value;

									pairsToAdd.push_back(pair);
								}
							}
						}
					}
				}
				else {
					for (auto& objectValue : object.objectValues) {
						if ((objectValue.name.compare(data[0])) == 0) {
							for (auto& kvp : objectValue.objectValues) {

								KeyValuePair pair;
								pair.key = kvp.key;
								pair.value = kvp.value;

								pairsToAdd.push_back(pair);
							}
						}
					}
				}

				for (int i = 0; i < pairsToAdd.size(); i++)
					object.keyValues.push_back(pairsToAdd[i]);
			}
		}


	}


	void save(JsonInfo& object, string filePath, string defaultPath) {
		if (filePath == "") 
			serializeJsonInfo(object, defaultPath);
		else 
			serializeJsonInfo(object, filePath);
	}
};


int main() {
	string filePath = "C:\\Users\\ivan-\\source\\repos\\JsonParserOopFMI\\JsonParserOopFMI\\output.json";
	JsonInfo jsonInfo = JsonInfo::parseJsonFile(filePath);

	//jsonInfo.deleteEntryByKey(jsonInfo, "management/directorId");

	//jsonInfo.create(jsonInfo, "management/DeveloperName", "DeveloperName", "Vanko");

	//jsonInfo.create(jsonInfo, "", "name", "VANKO");
	//cout << jsonInfo.searchElement(jsonInfo, "name") << endl;

	//jsonInfo.print(jsonInfo);

	//cout << jsonInfo.searchElement(jsonInfo, "management") << "]" << endl;

	//jsonInfo.create(jsonInfo, "", "randomId", "1");


	jsonInfo.save(jsonInfo, "", filePath);



	//DONE FUNCTIONALITY
	//validate - working
	//delete - working
	//search - working
	//print - working
	//find - working
	//set - working
	//create - working
	//save - working

	//move - need fix



	// implement Singleton pattern in project
	// split classes in *.hpp and *.cpp files
	// DOXYGEN
	// TESTS


	return 0;
}



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
protected:
    string key;
    string value;
    
public:
    
    KeyValuePair() = default;
    KeyValuePair(const string key, const string value) {
        this->key = key;
        this->value = value;
    }
    
    void SetKey(const string key) {
        this->key = key;
    }
    
    void SetValue(const string value) {
        this->value = value;
    }
    
    string GetKey() const {
        return this->key;
    }
    
    string GetValue() const {
        return this->value;
    }
    
    KeyValuePair operator =(const KeyValuePair pair) {
        SetKey(pair.GetKey());
        SetValue(pair.GetValue());
        
        return *this;
    }
};

class Array {
    
protected:
    string name;
    vector<string> elements;
public:
    
    Array() = default;
    Array(const string name, const vector<string> elements) {
        this->name = name;
        this->elements = elements;
    }
    
    void SetName(const string name) {
        this->name = name;
    }
    
    void SetElements(const vector<string> elements) {
        this->elements = elements;
    }
    
    string getName() const {
        return this->name;
    }
    
    vector<string> getElements() const {
        return this->elements;
    }
    
    void AddElement(const string element) {
        this->elements.push_back(element);
    }
    
    void SetArrayKVPValue(int index, string value) {
        this->elements[index] = value;
    }
};

class JsonObject {
protected:
    string name;
    vector<KeyValuePair> KVPJsonPairs;
    
public:
    JsonObject() = default;
    JsonObject(const string name, const vector<KeyValuePair> KVPJsonPairs) {
        this->name = name;
        this->KVPJsonPairs = KVPJsonPairs;
    }
    
    void SetName(const string name) {
        this->name = name;
    }
    
    string getName() const {
        return this->name;
    }
    
    vector<KeyValuePair> getKVPJsonPairs() {
        return this->KVPJsonPairs;
    }
    
    void AddKeyValuePair(const KeyValuePair kvp) {
        this->KVPJsonPairs.push_back(kvp);
    }
    
    void SetVectKVPValue(int index, string value) {
        this->KVPJsonPairs[index].SetValue(value);
    }
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
        fileStream << "\"" << kvp.GetKey() << "\": \"" << kvp.GetValue() << ",";
    }
    
    void serializeKeyValuePairInJsonObject(std::ofstream& fileStream, const KeyValuePair& kvp, bool isLast) {
        if (!isLast)
            fileStream << "\"" << kvp.GetKey() << "\": \"" << kvp.GetValue() << "\",";
        else
            fileStream << "\"" << kvp.GetKey() << "\": \"" << kvp.GetValue() << "\"";

    }

    void serializeArray(std::ofstream& fileStream, const Array& arr, bool isLast) {
        bool isKey = true;
        
        fileStream << "\"" << arr.getName() << "\": [";
        for (size_t i = 0; i < arr.getElements().size(); ++i) {
            if (arr.getElements()[i] == "")
                continue;
            
            if (i < (arr.getElements().size() - 1)) {
                if(!isKey) {
                    fileStream << "\"" << arr.getElements()[i] << "\",";
                    isKey = true;
                }
                
                else {
                    fileStream << "\"" << arr.getElements()[i] << "\":";
                    isKey = false;
                }
            }
            else if (i == (arr.getElements().size()))
                fileStream << "\"" << arr.getElements()[i] << "\"";
        }
        
        if (isLast)
            fileStream << "]";
        else
            fileStream << "],";
    }

    void serializeJsonObject(std::ofstream& fileStream, JsonObject& obj) {
        bool isLast = false;
        fileStream << "\"" << obj.getName() << "\": {";
        for (size_t i = 0; i < obj.getKVPJsonPairs().size(); ++i) {
            if (i < obj.getKVPJsonPairs().size() - 1) {
                serializeKeyValuePairInJsonObject(fileStream, obj.getKVPJsonPairs()[i], isLast);
            }
            else {
                isLast = true;
                serializeKeyValuePairInJsonObject(fileStream, obj.getKVPJsonPairs()[i], isLast);
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
            } else {
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
                    array.SetName(key);
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
                        
                        array.AddElement(element);
                        ++pos;
                    }
                    
                    jsonInfo.arrays.push_back(array);
                } else if (jsonString[pos] == '{') {
                    int backwardPos = 0;
                    int quoteCounter = 0;
                    
                    string jsonObjectName = "";
                    
                    while(quoteCounter < 2) {
                        --pos;
                        
                        if (jsonString[pos] == '"') quoteCounter++;
                        else if (jsonString[pos] == ' ' || jsonString[pos] == ':') { }
                        
                        else jsonObjectName.push_back(jsonString[pos]);
                        
                        ++backwardPos;
                    }
                    
                    jsonObjectName = ReverseStr(jsonObjectName);
                    pos += backwardPos;
                    
                    ++pos;
                    JsonObject jsonObject;
                    jsonObject.SetName(jsonObjectName);
                    
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
                            kvp.SetKey(objectKey);
                            kvp.SetValue(objectValue);
                            jsonObject.AddKeyValuePair(kvp);
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
                    kvp.SetKey(key);
                    kvp.SetValue(value);
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
                if (it->GetKey().compare(key) == 0) {
                    it = jsonInfo.keyValues.erase(it);
                    cout << "Entry (KVP) deleted successfully!" << endl;
                    return;
                }
                else
                    ++it;
            }
            
            for (size_t i = 0; i < jsonInfo.objectValues.size(); i++) {
                
                if (jsonInfo.objectValues[i].getName().compare(data[0]) == 0) {
                    cout << "Entry (JsonObject) deleted successfully!" << endl;
                    jsonInfo.objectValues.erase(jsonInfo.objectValues.begin() + i);
                    return;
                }
            }
        }
        else {
            
            for (auto& array : jsonInfo.arrays) {
                if (array.getName().compare(data[0]) == 0) { // Check if the array name matches the first key
                    vector<string> elements = array.getElements();
                    
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
                if (it->getName().compare(data[0]) == 0) {
                    for (auto it2 = it->getKVPJsonPairs().begin(); it2 != it->getKVPJsonPairs().end(); ++it2) {
                        if (it2->GetKey().compare(data[1]) == 0) {
                            cout << "Entry (JsonObject) deleted successfully!" << endl;
                            it->getKVPJsonPairs().erase(it2);
                            return;
                        }
                    }
                }
            }
            
            cout << "No such path!" << endl;
        }
    }
    
    void searchElement(JsonInfo& object, string str) {
        string result;
        string temp;
        int counter = 0;
        
        // check singular key-value pairs
        for (const auto& kvp : object.keyValues) {
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
        for (const auto& array : object.arrays) {
            for (int i = 0; i < array.getElements().size(); i++) {
                int res = array.getElements()[i].compare(str);
                
                if (res == 0) {
                    temp.append("   \"");
                    temp.append(array.getElements()[i + 2]);
                    temp.append("\",");
                    temp.append("\n");
                    
                    result.append(temp);
                    temp = "";
                    
                    counter++;
                }
            }
        }
        
        for (auto& objectValue : object.objectValues) {
            if (objectValue.getName().compare(str) == 0) {
                
                temp.append("\"");
                temp.append(objectValue.getName());
                temp.append("\"\n");
                counter++;
                
                for (auto& kvp : objectValue.getKVPJsonPairs()) {
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
        cout << result << endl;
    }
    
    void print(JsonInfo& jsonInfo) {
        cout << "File Name: " << jsonInfo.fileName << endl;
        
        cout << "Key-Value Pairs:" << endl;
        for (const auto& kvp : jsonInfo.keyValues)
            cout << kvp.GetKey() << ": " << kvp.GetValue() << endl;
        
        cout << "Arrays:" << endl;
        for (const auto& array : jsonInfo.arrays) {
            cout << array.getName() << ":" << endl;
            
            
            if (array.getElements().empty())
                cout << "Empty Array!" << endl;
            else
                for (const auto& element : array.getElements())
                    cout << "- " << element << endl;
        }
        
        for ( auto& object : jsonInfo.objectValues) {
            cout << "Object Key: " << object.getName() << endl;
            cout << "Object Values:" << endl;
            for ( auto& kvp : object.getKVPJsonPairs()) {
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
    
    void find(JsonInfo& jsonInfo, string str) {
        int res;
        for (const auto& kvp : jsonInfo.keyValues) {
            res = kvp.GetKey().compare(str);
            if (res == 0) {
                cout << kvp.GetKey() << ": " << kvp.GetValue() << endl;
                return;
            }
        }
        
        
        
        for (const auto& array : jsonInfo.arrays) {
            res = array.getName().compare(str);
            if (array.getElements().empty())
                cout << "Empty Array!" << endl;
            
            else if (res == 0) {
                cout << array.getName() << ":" << endl;
                for (const auto& element : array.getElements()) {
                    cout << "- " << element << endl;
                }
                
                return;
            }
        }
        
        
        
        for ( auto& objectValue : jsonInfo.objectValues) {
            res = objectValue.getName().compare(str);
            if (res == 0) {
                cout << "Object Key: " << objectValue.getName() << endl;
                cout << "Object Values: " << endl;
                for ( auto& kvp : objectValue.getKVPJsonPairs()) {
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
            for (auto& kvp : object.keyValues) {
                if(kvp.GetKey().compare(data[0]) == 0) {
                    kvp.SetValue("\"" + value + "\"");
                    cout << "Value set successfully!" << endl;
                    return;
                }
            }
        }
        else {
            for (auto& objectValue : object.objectValues) {
                if ((objectValue.getName().compare(data[0])) == 0) {
                    
                    
                    for (int i = 0; i < objectValue.getKVPJsonPairs().size(); i++) {
                        if ((objectValue.getKVPJsonPairs()[i].GetKey().compare(data[1])) == 0) {
                            objectValue.SetVectKVPValue(i, value);
                            
                            cout << "Value set successfully!" << endl;
                            return;
                        }
                    }
                }
            }
            
            for (auto& array : object.arrays) {
                res = array.getName().compare(data[0]);
                
                if (res == 0) {
                    for (int i = 0; i < array.getElements().size(); i++) {
                        res = array.getElements()[i].compare(data[1]);
                        if(res == 0) {
                            array.SetArrayKVPValue(i + 2, value);
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
                pair.SetKey(key);
                pair.SetValue(value);
                
                object.keyValues.push_back(pair);
                return;
            }
            
            
            // JsonObject
            else if (path.find(delimeter) != string::npos) {
                for (auto& objectValue : object.objectValues) {
                    
                    if ((objectValue.getName().compare(data[0])) == 0) {
                        
                        for (auto& kvp : objectValue.getKVPJsonPairs()) {
                            if ((kvp.GetKey().compare(data[1])) == 0) {
                                cout << "Key-Value pair already exists!" << endl;
                                return;
                            }
                        }
                        
                        // check if kvp in arr exists, if so, print error
                        // if not, add it to the arr's kvp's
                        
                        KeyValuePair pair;
                        pair.SetKey(key);
                        pair.SetValue(value);
                        
                        objectValue.AddKeyValuePair(pair);
                        
                        return;
                    }
                }
                
                
                // Arrays
                for (auto& array : object.arrays) {
                    
                    if ((array.getName().compare(data[0])) == 0) {
                        
                        for (auto& element : array.getElements()) {
                            if ((element.compare(data[1])) == 0) {
                                cout << "Key-Value pair already exists!" << endl;
                                return;
                            }
                        }
                        
                        
                        array.AddElement(key);
                        array.AddElement(" ");
                        array.AddElement(value);
                        
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
            if (object.arrays[i].getName().compare(data[0]) == 0)
                return true;
        return false;
    }
    
    bool isJsonObjectPath(JsonInfo& object, string path) {
        char delimeter = '/';
        vector<string> data = splitString(path, delimeter);
        
        for (int i = 0; i < object.objectValues.size(); i++)
            if (object.objectValues[i].getName().compare(data[0]) == 0)
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
                        if ((array.getName().compare(data[0])) == 0) {
                            for (int i = 0; i < array.getElements().size(); i++) {
                                if ((array.getElements()[i].compare(data[1])) == 0) {
                                    KeyValuePair pair;
                                    pair.SetKey(getElements()[i]);
                                    pair.SetKey(getElements()[i + 2]);
                                    
                                    pairsToAdd.push_back(pair);
                                }
                            }
                        }
                    }
                }
                else {
                    for (auto& array : object.arrays) {
                        if ((array.getName().compare(data[0])) == 0) {
                            for (int i = 0; i < array.getElements().size(); i++) {
                                KeyValuePair pair;
                                pair.SetKey(getElements()[i]);
                                pair.SetKey(getElements()[i + 2]);
                                
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
                        if ((objectValue.getName().compare(data[0])) == 0) {
                            for (auto& kvp : objectValue.getKVPJsonPairs()) {
                                if ((kvp.GetKey().compare(data[1])) == 0) {
                                    
                                    KeyValuePair pair;
                                    pair.SetKey(kvp.GetKey());
                                    pair.SetValue(kvp.GetValue());
                                    
                                    pairsToAdd.push_back(pair);
                                }
                            }
                        }
                    }
                } else {
                    for (auto& objectValue : object.objectValues) {
                        if ((objectValue.getName().compare(data[0])) == 0) {
                            for (auto& kvp : objectValue.getKVPJsonPairs()) {
                                
                                KeyValuePair pair;
                                pair.SetKey(kvp.GetKey());
                                pair.SetValue(kvp.GetValue());
                                
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
        
        //
        if (filePath == "") {
            serializeJsonInfo(object, defaultPath);
        }
        else {
            serializeJsonInfo(object, filePath);
        }
    }
};


int main() {
    string filePath = "/Users/ivancvetkov/Desktop/organisation1.json";
    JsonInfo jsonInfo = JsonInfo::parseJsonFile(filePath);

    //jsonInfo.deleteEntryByKey(jsonInfo, "management/directorId");
    
    //jsonInfo.create(jsonInfo, "management/DeveloperName", "DeveloperName", "Vanko");
    
    //jsonInfo.create(jsonInfo, "", "name", "VANKO");
    //jsonInfo.searchElement(jsonInfo, "name");

    //jsonInfo.print(jsonInfo);
    //jsonInfo.find(jsonInfo, "management");
    //cout << jsonInfo.searchElement(jsonInfo, "management") << "]" << endl;
    
    //jsonInfo.create(jsonInfo, "", "randomId", "1");
    jsonInfo.print(jsonInfo);
    
    
    
    //DONE FUNCTIONALITY
    //validate - working .
    //delete - working .
    //search - working .
    //print - working .
    //find - working .
    //set - working .
    //create - working .
    //save - working .
    
    //move - need fix

    
    
    // implement Singleton pattern in project
    // split classes in *.hpp and *.cpp files
    // DOXYGEN
    // TESTS
    
    
    return 0;
}



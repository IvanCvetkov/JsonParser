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



/*!
 * @brief
 * This class is used to represent key-value pairs in the Json's general structure
* */
class KeyValuePair {
protected:
    string key;
    string value;
    
public:
    
    KeyValuePair() = default;

    /*!
     * @brief
     * parameterized constructor for the KeyValuePair class
     * 
     *  
     * */
    KeyValuePair(const string key, const string value) {
        this->key = key;
        this->value = value;
    }
    

    /*!
    * \brief
     * method to set value of key field
     * */
    void SetKey(const string key) {
        this->key = key;
    }
    
    /*!
    *\brief
     * method to set value of value field
     * */
    void SetValue(const string value) {
        this->value = value;
    }



    /*!
    *\brief 
     * method to get value of key field
     * */
    string GetKey() const {
        return this->key;
    }
        /*!
    *\brief
     * method to get value of value field
     * */
    string GetValue() const {
        return this->value;
    }
    
    /*!
    *\brief
     * predefinition of operator= in order
     * to be able to initialize one KeyValuePair to another directly
     * by using "="
     * */
    KeyValuePair operator =(const KeyValuePair pair) {
        SetKey(pair.GetKey());
        SetValue(pair.GetValue());
        
        return *this;
    }
};


    /*!
    *\brief
    * This class is used to represent an Array in the JSON's structure.
    * */
class Array {
    
protected:
    string name;
    vector<string> elements;
public:
    

    Array() = default;

        /*!
        *\brief 
     * parameterized constructor for the Array class
     * */
    Array(const string name, const vector<string> elements) {
        this->name = name;
        this->elements = elements;
    }
    

        /*!
        *\brief 
     * method to set value of name field
     * */
    void SetName(const string name) {
        this->name = name;
    }
    
        /*!
        *\brief 
     * method to set the value of protected member array<string> elements (this->elements)
     * */
    void SetElements(const vector<string> elements) {
        this->elements = elements;
    }


        /*!
        *\brief 
     * method to return the name of the Array object
     * */
    string getName() const {
        return this->name;
    }
    
        /*!
        *\brief 
     * method to get elements of the protected member attribute vector<string> elements (this->elements)
     * */
    vector<string> getElements() const {
        return this->elements;
    }


        /*!
        *\brief 
     * method to add an element to the array of vector<string> 
     * */
    void AddElement(const string element) {
        this->elements.push_back(element);
    }
    

        /*!
        *\brief 
     * method that sets the value of the vector<string> member attribute of
     * the Array class at a specific index
     * by given value 
     * */
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

        /*!
        *\brief 
     * parameterized constructor
     * */
    JsonObject(const string name, const vector<KeyValuePair> KVPJsonPairs) {
        this->name = name;
        this->KVPJsonPairs = KVPJsonPairs;
    }
    

        /*!
        *\brief 
     * assigns a string to the name member attribute of the JsonObject class
     * */
    void SetName(const string name) {
        this->name = name;
    }
    
        /*!
        *\brief 
     * returns the name attribute 
     * */
    string getName() const {
        return this->name;
    }
    
        /*!
        *\brief 
     * returns all vector<KVP> of the JsonObject class
     * */
    vector<KeyValuePair> getKVPJsonPairs() {
        return this->KVPJsonPairs;
    }
    

        /*!
        *\brief 
     * method that adds an element to the vector<KVP> of the JsonObject class
     * */
    void AddKeyValuePair(const KeyValuePair kvp) {
        this->KVPJsonPairs.push_back(kvp);
    }
    
        /*!
        *\brief 
     * method to set the value of vector<KVP> of the JsonObject class at a specific index
     * */
    void SetVectKVPValue(int index, string value) {
        this->KVPJsonPairs[index].SetValue(value);
    }
};


    /*!
    *\brief 
     * The main class of the program that stores all the Json
     * data and manipulates it as given in the requirements 
     * provided. 
     * */
class JsonInfo : public JsonObject, KeyValuePair, Array {
private:

        /*!
        *\brief 
     * stores the name of the file
     * */
    string fileName;

        /*!
        *\brief 
     * stores all KVPs that represent Key-Value Pairs in the Json's structure
     * */
    vector<KeyValuePair> keyValues;

        /*!
        *\brief 
     * stores all JsonObjects that represent objects in the Json's structure
     * */
    vector<JsonObject> objectValues;
            /*!
            *\brief 
     * stores all Arrays that represent arrays in the Json's structure
     * */
    vector<Array> arrays;
    

            /*!
            *\brief 
     * helper function to split a string in a array<string> by a given delimeter
     * and then return the vector<string> containing all parts of the string 
     * split by the given delimeter
     * */
    vector<string> splitString(const string& str, char delimiter) {
        vector<string> tokens;
        std::istringstream iss(str);
        string token;
        
        while (getline(iss, token, delimiter)) {
            tokens.push_back(token);
        }
        
        return tokens;
    }

                /*!
                *\brief 
     * Checks if the given string is a valid path to a valid "Array" in the Json's structure
     * */
    bool isArrayPath(JsonInfo& object, string path) {
        char delimeter = '/';
        vector<string> data = splitString(path, delimeter);
        
        for (int i = 0; i < object.arrays.size(); i++)
            if (object.arrays[i].getName().compare(data[0]) == 0)
                return true;
        return false;
    }
    
                    /*!
                    *\brief 
     * Checks if the given string is a valid path to a valid "JsonObject" in the Json's structure
     * */
    bool isJsonObjectPath(JsonInfo& object, string path) {
        char delimeter = '/';
        vector<string> data = splitString(path, delimeter);
        
        for (int i = 0; i < object.objectValues.size(); i++)
            if (object.objectValues[i].getName().compare(data[0]) == 0)
                return true;
        return false;
    }
    
                    /*!
                    *\brief 
     * Inserts the given KVP into the filestream 
     * */
    void serializeDefaultKeyValuePair(std::ofstream& fileStream, const KeyValuePair& kvp) {
        fileStream << "\"" << kvp.GetKey() << "\": \"" << kvp.GetValue() << ",";
    }
    
                    /*!
                    *\brief 
     * Inserts the given KVP that represents a KVP in a JsonObject into the filestream
     * */
    void serializeKeyValuePairInJsonObject(std::ofstream& fileStream, const KeyValuePair& kvp, bool isLast) {
        if (!isLast)
            fileStream << "\"" << kvp.GetKey() << "\": \"" << kvp.GetValue() << "\",";
        else
            fileStream << "\"" << kvp.GetKey() << "\": \"" << kvp.GetValue() << "\"";

    }

                    /*!
                    *\brief 
     * Inserts the array into the filestream 
     * */
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

                        /*!
                        *\brief 
     * Inserts JsonObject into the filestream (by using the serializeKeyValuePairInJsonObject method)
     * */
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
    
                        /*!
                        *\brief 
     * Checks if given character is a valid character
     * */
    static bool isCharacter(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }
    
                        /*!
                        *\brief 
     * Reverses a string, obviously
     * */
    static string ReverseStr(string str) {
        string temp;
        
        for (int i = (int)str.length() - 1; i >= 0; --i)
            temp.push_back(str[i]);
        
        return temp;
    }
    
                        /*!
                        *\brief 
     * Checks if all braces are of even count
     * */
    bool validateBraces(string str) {
        int bracesCounter = 0;
        
        for (int i = 0; i < str.length(); i++)
            if (str[i] == '{' || str[i] == '}')
                bracesCounter++;
        
        return (bracesCounter % 2 == 0);
    }
    
                        /*!
                        *\brief 
     * Checks if all double quotes are of even count
     * */
    bool validateQuotes(string str) {
        int quotesCounter = 0;
        
        for (int i = 0; i < str.length(); i++)
            if (str[i] == '\"')
                quotesCounter++;
        
        return (quotesCounter % 2 == 0);
    }
    
                        /*!
                        *\brief 
     * Checks if a string is valid by using the isCharacter function (the string should)
     * consist only of letters.
     * */
    bool validateValue(const string& str) {
        
        for (int i = 0; i < str.length(); i++)
            if (!isCharacter(str[i]))
                return false;
        
        return true;
    }
    
public:

                        /*!
                        *\brief 
     * Uses methods that start with the "serialize" substring
     * in order to serialize the whole JsonInfo class 
     * (parse it in a json file)
     * */
    static void serializeJsonInfo(JsonInfo& jsonInfo, string path) {
        bool isLast = false;
        std::ofstream outputFile(path);
        if (!outputFile.is_open()) {
            std::cerr << "Failed to create JSON file." << std::endl;
            return;
        }

        outputFile << "{";
        
        // Serializing key-value pairs
        for (size_t i = 0; i < jsonInfo.keyValues.size(); ++i) {
            jsonInfo.serializeDefaultKeyValuePair(outputFile, jsonInfo.keyValues[i]);
        }

        // Serializing objects
        for (size_t i = 0; i < jsonInfo.objectValues.size(); ++i) {
            jsonInfo.serializeJsonObject(outputFile, jsonInfo.objectValues[i]);
        }

        // Serializing arrays
        for (size_t i = 0; i < jsonInfo.arrays.size(); ++i) {
            if (i < jsonInfo.arrays.size() - 1) {
                jsonInfo.serializeArray(outputFile, jsonInfo.arrays[i], isLast);
            } else {
                isLast = true;
                jsonInfo.serializeArray(outputFile, jsonInfo.arrays[i], isLast);
            }
        }

        outputFile << "}";

        outputFile.close();
        std::cout << "JSON file created successfully." << std::endl;
        return;
    }
    
                        /*!
                        *\brief 
     * Takes a given path and by following the json structure
     * extracts all the data from the provided path 
     * based on a predefined (by the classes) structure
     * */

                        /**@returns 
     * 
     * */
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
    /*
     *
     * deletes an entry in the private attributes of the JsonInfo class, based on a given key
     * ,only the first occurrence is deleted
     * it works by traversing all the JsonInfo object's private attributes and 
     * searching for the given key.
     */
    static bool deleteEntryByKey(JsonInfo& jsonInfo, string key) {
        // delete only first occurrence as there can be multiple key-value pairs with the same key
        // Delete from key-values
        
        char delimeter = '/';
        vector<string> data = jsonInfo.splitString(key, delimeter);
        
        
        // if no '/' present
        if (key.find(delimeter) != string::npos) {
            for (auto it = jsonInfo.keyValues.begin(); it != jsonInfo.keyValues.end();) {
                if (it->GetKey().compare(key) == 0) {
                    it = jsonInfo.keyValues.erase(it);
                    cout << "Entry (KVP) deleted successfully!" << endl;
                    return true;
                }
                else
                    ++it;
            }
            
            for (size_t i = 0; i < jsonInfo.objectValues.size(); i++) {
                
                if (jsonInfo.objectValues[i].getName().compare(data[0]) == 0) {
                    cout << "Entry (JsonObject) deleted successfully!" << endl;
                    jsonInfo.objectValues.erase(jsonInfo.objectValues.begin() + i);
                    return true;
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
                            return true;
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
                            return true;
                        }
                    }
                }
            }
            
            cout << "No such path!" << endl;
        }
        return false;
    }
    
                        /*!
                        *\brief 
     * This method traverses all private attributes of the JsonInfo object 
     * in order to find ALL occurrences of the given string
     * it then prints it to the console
     * */
   static void searchElement(JsonInfo& object, string str) {
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
    
                        /*!
                        *\brief 
     * The following function prints all the data inside the JsonInfo Object
     * */
    static void print(JsonInfo& jsonInfo) {
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
    

                        /*!
                        *\brief 
     * This function validates the Json structure of the JSON file that we receive as input
     * in our main function
     * */
    static bool validate(JsonInfo& jsonInfo, const string& filePath) {
        
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
        
        
        return (jsonInfo.validateBraces(str) && jsonInfo.validateQuotes(str));
    }
    
                        /*!
                        *\brief 
     * This method traverses all member attributes of the JsonInfo class 
     * in order to find if a given KVP, Array or JsonObject exists.
     * If the path to the attribute is invalid, it prints a proper message
     * to inform the user of it's non-existence.
     * */
    static bool find(JsonInfo& jsonInfo, string str) {
        int res;
        for (const auto& kvp : jsonInfo.keyValues) {
            res = kvp.GetKey().compare(str);
            if (res == 0) {
                cout << kvp.GetKey() << ": " << kvp.GetValue() << endl;
                return true;
            }
        }
        
        
        
        for (const auto& array : jsonInfo.arrays) {
            res = array.getName().compare(str);
            if (array.getElements().empty()){
                cout << "Empty Array!" << endl;
                return false;
                
            }
            
            else if (res == 0) {
                cout << array.getName() << ":" << endl;
                for (const auto& element : array.getElements()) {
                    cout << "- " << element << endl;
                    return true;
                }
                
                
            }
        }
        
        
        
        for ( auto& objectValue : jsonInfo.objectValues) {
            res = objectValue.getName().compare(str);
            if (res == 0) {
                cout << "Object Key: " << objectValue.getName() << endl;
                cout << "Object Values: " << endl;
                for ( auto& kvp : objectValue.getKVPJsonPairs()) {
                    cout << "- Key: " << kvp.GetKey() << ", Value: " << kvp.GetValue() << endl;
                    return true;
                }
            }
        }
        
        cout << "Path doesn't exist!" << endl;
        return false;
    }
    
                        /*!
                        *\brief 
     * This method sets the value of a given KVP, that it finds
     * by the the splitting the given path by a delimeter (if needed)
     * and "mapping" it through the JsonInfo object in order to find 
     * the corresponding KVP and set the new value
     * */
    static bool set(JsonInfo& object, string path, string value) {
        // set only the first occurence of the key
        // as it's thought to be unique given it's path.
        int res;
        char delimeter = '/';
        vector<string> data = object.splitString(path, delimeter);
        
        
        // if no '/' present
        if (path.find(delimeter) == string::npos) {
            for (auto& kvp : object.keyValues) {
                if(kvp.GetKey().compare(data[0]) == 0) {
                    kvp.SetValue("\"" + value + "\"");
                    cout << "Value set successfully!" << endl;
                    return true;
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
                            return true;
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
                            return true;
                        }
                    }
                }
            }
        }
        
        
        cout << "Can't set new value, path doesn't exist!" << endl;
        return false;
    }
    

                        /*!
                        *\brief 
     * Method used to create a new KVP and add it to the JsonInfo Object.
     * It works by first validating the key given (it should only contain letters)
     * and then searches through the JsonInfo object for the given key.
     * If the key already exists, it informs the user of it's existence.
     * If not, then a new KVP is added to the JsonInfo object.
     * */
    static bool create(JsonInfo& object, string path, string key, string value) {
        
        char delimeter = '/';
        bool isValid = object.validateValue(key);
        vector<string> data = object.splitString(path, delimeter);
        
        if (isValid) {
            
            // default path is a key-value pair in the JSON structure
            // the key is validated beforehand
            if (path == "default") {
                KeyValuePair pair;
                pair.SetKey(key);
                pair.SetValue(value);
                
                object.keyValues.push_back(pair);
                return true;
            }
            
            
            // JsonObject
            else if (path.find(delimeter) != string::npos) {
                for (auto& objectValue : object.objectValues) {
                    
                    if ((objectValue.getName().compare(data[0])) == 0) {
                        
                        for (auto& kvp : objectValue.getKVPJsonPairs()) {
                            if ((kvp.GetKey().compare(data[1])) == 0) {
                                cout << "Key-Value pair already exists!" << endl;
                                return false;
                            }
                        }
                        
                        // check if kvp in arr exists, if so, print error
                        // if not, add it to the arr's kvp's
                        
                        KeyValuePair pair;
                        pair.SetKey(key);
                        pair.SetValue(value);
                        
                        objectValue.AddKeyValuePair(pair);
                        
                        return true;
                    }
                }
                
                
                // Arrays
                for (auto& array : object.arrays) {
                    
                    if ((array.getName().compare(data[0])) == 0) {
                        
                        for (auto& element : array.getElements()) {
                            if ((element.compare(data[1])) == 0) {
                                cout << "Key-Value pair already exists!" << endl;
                                return false;
                            }
                        }
                        
                        
                        array.AddElement(key);
                        array.AddElement(" ");
                        array.AddElement(value);
                        
                        return true;
                    }
                }
            }
        }
        return false;
    }
    

    
                     /*!
                     *\brief 
     * Method that takes data from one path and pastes it on another place.
     * for example: management/directorId and offices/name, then the
     * directorId KVP will be taken from it's old path and set to the new one.
     * (by means of the JsonInfo's object structure, the directorId
     * KVP is deleted from its old path and set to the offices/... path)
     * */
    static void move(JsonInfo& object, string path, string newPath) {
        
        char delimeter = '/';
        vector<string> data = object.splitString(path, delimeter);
        
        // Array, JsonObject - KVP
        if (path == "default") {
            
            bool isArray = object.isJsonObjectPath(object, newPath);
            bool isJsonObject = object.isJsonObjectPath(object, newPath);
            
                    
            
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
                                    pair.SetKey(object.getElements()[i]);
                                    pair.SetKey(object.getElements()[i + 2]);
                                    
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
                                pair.SetKey(object.getElements()[i]);
                                pair.SetKey(object.getElements()[i + 2]);
                                
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
    
                        /*!
                        *\brief 
     * Saves the already manipulated JsonInfo object as a *.json file
     * if a filepath is not provided, it is then overwritten in the 
     * default path that is given when the function is called.
     * */
    static void save(JsonInfo& object, string filePath, string defaultPath) {
        
        //
        if (filePath == "") {
            object.serializeJsonInfo(object, defaultPath);
        }
        else {
            object.serializeJsonInfo(object, filePath);
        }
    }
};


int main() {
    string filePath = "/Users/ivancvetkov/Desktop/organisation1.json";
    JsonInfo jsonInfo = JsonInfo::parseJsonFile(filePath);

    
    
    // ПРИМЕРНИ КОМАНДИ ЗА ТЕСТВАНЕ НА ФУНКЦИОНАЛНОСТИТЕ
    //jsonInfo.deleteEntryByKey(jsonInfo, "management/directorId");
    //jsonInfo.create(jsonInfo, "management/DeveloperName", "DeveloperName", "Vanko");
    //jsonInfo.create(jsonInfo, "", "name", "VANKO");
    //jsonInfo.searchElement(jsonInfo, "name");

    //jsonInfo.find(jsonInfo, "management");
    //cout << jsonInfo.searchElement(jsonInfo, "management") << "]" << endl;
    //jsonInfo.create(jsonInfo, "default", "randomId", "1");
    //JsonInfo::print(jsonInfo);
    
    
    
    //DONE FUNCTIONALITY
    //validate - working . w tests
    //delete - working . w tests
    //search - working .
    //print - working .
    //find - working . w tests
    //set - working . w tests
    //create - working . w tests
    //save - working .
    //move - working .
    
    
    // DOXYGEN - done

    
    
    return 0;
}



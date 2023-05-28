#ifndef _JSON_INFO_H
#define _JSON_INFO_H
#include "KeyValuePair.h"
#include "Array.h"
#include "JsonObject.h"


class JsonInfo
{
private:
	string fileName;
	vector<KeyValuePair> keyValues;
	vector<JsonObject> objectValues;
	vector<Array> arrays;


public:
	JsonInfo() = default;
	JsonInfo(const string fileName, const vector<KeyValuePair> keyValues, const vector<JsonObject> objectValues, const vector<Array> arrays);

	string GetFileName();
	vector<KeyValuePair> GetKVP();
	vector<JsonObject> GetObjValues();
	vector<Array> GetArrays();



};

#endif // !_JSON_INFO_H




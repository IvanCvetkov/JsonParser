#include "JsonInfo.h"


string JsonInfo::GetFileName()
{
	return this->fileName;
}

vector<KeyValuePair> JsonInfo::GetKVP()
{
	return this->keyValues;
}

vector<JsonObject> JsonInfo::GetObjValues()
{
	return this->objectValues;
}

vector<Array> JsonInfo::GetArrays()
{
	return this->arrays;
}

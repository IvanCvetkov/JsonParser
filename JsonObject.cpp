#include "JsonObject.h"

JsonObject::JsonObject(const string name, const vector<KeyValuePair> objectValues)
{
	this->name;
	this->objectValues = objectValues;
}

string JsonObject::GetName() const
{
	return this->name;
}

vector<KeyValuePair> JsonObject::GetObjValues()
{
	return this->objectValues;
}

void JsonObject::SetName(const string name)
{
	this->name = name;
}

void JsonObject::SetObjValues(const vector<KeyValuePair> objectValues)
{
	this->objectValues = objectValues;
}



#include "KeyValuePair.h"

KeyValuePair::KeyValuePair(const string key, const string value)
{
	this->key = key;
	this->value = value;
}

string KeyValuePair::GetKey() const
{
	return this->key;
}

string KeyValuePair::GetValue() const
{
	return this->value;
}

void KeyValuePair::SetKey(const string key)
{
	this->key = key;
}

void KeyValuePair::SetValue(const string value)
{
	this->value = value;
}





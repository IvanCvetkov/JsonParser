#include "Array.h"

Array::Array(const string name, const vector<string> elements)
{
	this->name = name;
	this->elements = elements;
}

string Array::GetName() const
{
	return this->name;
}

vector<string> Array::GetElements() const
{
	return this->elements;
}

void Array::SetName(const string name)
{
	this->name = name;
}

void Array::SetElements(const vector<string> elements)
{
	this->elements = elements;
}

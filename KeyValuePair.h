#ifndef _KVP_H
#define _KVP_H
#include <string>
#include <iostream>


using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::stringstream;

class KeyValuePair
{
protected:
	string key;
	string value;
public:
	KeyValuePair() = default;
	KeyValuePair(const string key, const string value);

	string GetKey() const;
	string GetValue() const;

	void SetKey(const string key);
	void SetValue(const string value);

};

#endif // !_KVP_H



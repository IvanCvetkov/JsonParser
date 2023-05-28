#ifndef _ARR_H
#define _ARR_H
#include <string>
#include <vector>

using std::string;
using std::vector;

class Array
{
protected:
	string name;
	vector<string> elements;
public:
	Array() = default;
	Array(const string name, const vector<string> elements);

	string GetName() const;
	vector<string> GetElements() const;

	void SetName(const string name);
	void SetElements(const vector<string> elements);
};
#endif // !_ARR_H




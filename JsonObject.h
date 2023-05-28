#ifndef _JSON_OBJ_H
#define _JSON_OBJ_H
#include "KeyValuePair.h"
#include <vector>

using std::vector;

class JsonObject
{
protected:
	string name;
	vector<KeyValuePair> objectValues;
public:
	JsonObject() = default;
	JsonObject(const string name, const vector<KeyValuePair> objectValues);

	string GetName() const;
	vector<KeyValuePair> GetObjValues();

	void SetName(const string name);
	void SetObjValues(const vector<KeyValuePair> objectValues);


};

#endif // !_JSON_OBJ_H




// JsonParserProjectFMI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "JsonInfo.h"
#include <iostream>
#include "helper.cpp"

int main()
{
	string filePath = "C:\\Users\\ivan-\\source\\repos\\JsonParserOopFMI\\JsonParserOopFMI\\output.json";
	JsonInfo jsonInfo = parseJsonFile(filePath);


	save(jsonInfo, "", filePath);
}
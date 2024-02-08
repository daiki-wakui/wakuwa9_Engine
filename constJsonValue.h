#pragma once
#include "Vector3.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <string>

class constJsonValue
{
private:

	nlohmann::json m_json;

public:

	void LoadConstValue(std::string jsonName);
	Vector3 LoadVector3(std::string constName);
	float LoadFloat(std::string constName);
};


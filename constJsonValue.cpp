#include "constJsonValue.h"

void constJsonValue::LoadConstValue(std::string jsonName)
{
	std::string filenamea = jsonName;

	//ファイルを読み込んで内容を画面に表示する
	//読み込みに失敗した場合はエラーを表示する
	std::ifstream ifs(filenamea.c_str());
	if (ifs.good())
	{
		ifs >> m_json;
	}
	else
	{
		int a = 0;
		a++;
	}
}

Vector3 constJsonValue::LoadVector3(std::string constName)
{
	Vector3 ans;
	nlohmann::json& vec3 = m_json["Vector3"];

	ans.x = vec3[constName][0];
	ans.y = vec3[constName][1];
	ans.z = vec3[constName][2];

	return ans;
}

float constJsonValue::LoadFloat(std::string constName)
{
	float ans;
	ans = m_json[constName];
	return ans;
}

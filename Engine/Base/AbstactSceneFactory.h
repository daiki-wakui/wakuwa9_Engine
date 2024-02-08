#pragma once
#include "BaseScene.h"
#include <String>

class AbstractSceneFactory
{
public:

	//仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	//シーン作成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;

};


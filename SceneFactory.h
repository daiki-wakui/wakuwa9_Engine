#pragma once


#include "AbstactSceneFactory.h"

class SceneFactory : public AbstractSceneFactory
{
public:

	//シーン作成
	BaseScene* CreateScene(const std::string& sceneName) override;

};


#pragma once
#include "AbstactSceneFactory.h"
#include <memory>

class SceneFactory : public AbstractSceneFactory
{
public:

	//シーン作成
	std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) override;
};


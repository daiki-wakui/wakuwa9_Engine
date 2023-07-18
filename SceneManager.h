#pragma once
#include "BaseScene.h"

class SceneManager
{
private:

	BaseScene* scene_ = nullptr;

	BaseScene* nextScene_ = nullptr;

public:

	~SceneManager();

	void Update();

	void Draw();

	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }
};


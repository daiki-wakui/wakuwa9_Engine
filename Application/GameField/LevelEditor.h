#pragma once
#include "Model.h"
#include "EventBox.h"
#include "Object3D.h"
#include "LevelLoader.h"

class LevelEditor
{
private:

	LevelData leveldata_;

public:

	void Initialize();
	void Update();
	void Darw();
	void ReLoad();

};


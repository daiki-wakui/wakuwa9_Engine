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

	//初期化
	void Initialize();
	//更新処理
	void Update();
	//描画関数
	void Darw();
	//リロード関数
	void ReLoad();

};


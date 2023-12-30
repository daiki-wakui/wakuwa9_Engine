#pragma once
#include "Model.h"
#include "Object3D.h"

class DebugPoint
{
private:

	Model* model_;
	Object3D* object_;

public:

	bool valid_ = false;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="Model"></param> モデル見た目
	/// <param name="Object"></param> オブジェクト情報
	void Initialize(Model* Model, Object3D* Object);

	//更新処理
	void Update();

	//描画関数
	void Draw();

	//getter
	Vector3 GetWorldPos();
	bool GetIsArive() { return valid_; }
};


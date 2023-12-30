#pragma once
#include "Model.h"
#include "Object3D.h"

class EventBox
{
private:

	Model* model_;
	Object3D* object_;

	bool isLoadEditor_;
public:

	bool valid_ = false;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="Model"></param> モデルの見た目
	/// <param name="Object"></param> オブジェクト情報
	/// <param name="scene"></param> イベントムービーbox = false ステージチェンジbox = true
	void Initialize(Model* Model, Object3D* Object, bool scene = false);

	//更新処理
	void Update();

	//描画関数
	void Draw();

	//getter
	Vector3 GetWorldPos();
	bool GetIsArive() { return valid_; }
	bool GetLoadEditor() { return isLoadEditor_; }
};


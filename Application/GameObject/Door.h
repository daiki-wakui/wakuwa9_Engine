#pragma once
#include "Model.h"
#include "Object3D.h"

/**
 * @file Door
 * @brief フィールドにあるドアの更新、描画クラス
 */

class Door
{
private:

	const float OBJECT_ROT_VOLUE = 0.3f;
	const float OBJECT_ROT_MAX = -90.0f;
	const float OBJECT_ROT_MIN = 90.0f;

private:

	Model* model_;
	Object3D* object_;

	DirectX::XMFLOAT3 pos_;
	float objectRot_ = 0.0f;

	bool isChangeStage_ = false;

public:

	bool valid_ = false;
	bool filp_ = false;

	/// <summary>
	///	初期化
	/// </summary>
	/// <param name="Model"></param> モデルの見た目
	/// <param name="Object"></param> オブジェクト情報
	/// <param name="filp"></param> 反転させるか否か
	void Initialize(Model* Model, Object3D* Object, bool filp = false);

	//更新処理
	void Update();

	//描画関数
	void Draw();

	//getter
	DirectX::XMFLOAT3 GetWorldPos();

	//setter
	void SetTutorial(bool isChangeStage) { isChangeStage_ = isChangeStage; }
};


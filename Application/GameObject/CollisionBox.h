#pragma once
#include "Model.h"
#include "Object3D.h"

class CollisionBox
{
private:

	Model* model_;
	Object3D* object_;

	Vector3 scale_;

public:

	bool hit_ = false;

	bool arive = false;

	
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

	//当たったときの処理
	bool OnCollision();

	//getter
	Vector3 GetWorldPos();
	const Vector3& GetScale() const { return scale_; }
	bool GetCollision() const { return hit_; }

	//setter
	void SetScale(const Vector3& scale) { scale_ = scale; }

};


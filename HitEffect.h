#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Vector3.h"

class HitEffect
{
private:

	Model* thisModel_;
	std::unique_ptr<Object3D> thisObject_;
	float addPow_ = 0;
	Vector3 velocity_;
	Vector3 pos_;
	Vector3 rot_;
	float scale_ = 1.5f;
	bool isDown_ = false;

	Vector3 scaleEs_ = { 0,0,0 };

	bool isDead_ = false;

	float ariveFrame_ = 0;
	float timer_;
	float maxScale_ = 0.5f;

public:


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos"></param> 初期座標
	/// <param name="velocity"></param> オブジェクトに加わる速度
	/// <param name="model"></param> エフェクトに使うモデルの見た目
	void Initialize(Vector3 pos, Vector3 rot, float scale, Vector3& velocity, Model* model);

	//更新処理
	void Update();

	//描画関数
	void Draw();

	//スケール変更
	void SetScale(float scale);

	//getter
	bool IsDead() const { return isDead_; }
};

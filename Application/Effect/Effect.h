#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Vector3.h"

/**
 * @file Effect
 * @brief 飛び散るエフェクトを出すクラス
 */

class Effect
{
private:

	Model* bulletModel_;
	std::unique_ptr<Object3D> bulletObject_;
	Vector3 velocity_;

	float pow_ = 0;

	Vector3 pos_;
	Vector3 tPos_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;

	Vector3 rot_ = {};

	float scale_ = 1;

public:
	bool isDead_ = false;


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos"></param> 初期座標
	/// <param name="velocity"></param> オブジェクトに加わる速度
	/// <param name="model"></param> エフェクトに使うモデルの見た目
	void Initialize(Vector3 pos, Vector3& velocity, Model* model);

	//更新処理
	void Update();

	//描画関数
	void Draw();

	//スケール変更
	void SetScale(float scale);

	//getter
	bool IsDead() const { return isDead_; }
};


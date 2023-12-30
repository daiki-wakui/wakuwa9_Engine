#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Vector3.h"

/**
 * @file BossBullet
 * @brief ボスの弾の攻撃の処理、描画を行うクラス
 */

class BossBullet
{
private:

	const float OBJECT_SCALE = 3.0f;
	const float ADD_MIN_RAND = -0.1f;
	const float ADD_MAX_RAND = 3.0f;


private:

	Model* bulletModel_;
	std::unique_ptr<Object3D> bulletObject_;
	Vector3 velocity_;

	Vector3 tPos_;
	float addRand_ = 0;
	int32_t bulletState_ = 0;


	static const int32_t kLifeTime = 60 * 4;
	int32_t deathTimer_ = kLifeTime;

public:
	bool isDead_ = false;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos"></param> 初期座標
	/// <param name="velocity"></param> 弾の速度、方向
	/// <param name="model"></param> 弾のモデル
	/// <param name="randState"></param> ここのパラメータによって弾の軌道を変える
	void Initialize(Vector3 pos, Vector3& velocity, Model* model, int32_t randState);

	//更新処理
	void Update();

	//描画関数
	void Draw();

	//getter
	bool IsDead() const { return isDead_; }
	//座標のgetter
	Vector3 GetWorldPos();

};


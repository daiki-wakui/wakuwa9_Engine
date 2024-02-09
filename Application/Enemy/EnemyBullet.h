#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Vector3.h"
#include "BaseObject.h"

/**
 * @file EnemyBullet
 * @brief 雑魚敵の弾の攻撃の更新、描画を行うクラス
 */

class EnemyBullet : public BaseObject
{
private:	//定数

	const float VELOCITY_RATE = 5.0f;

private:

	Model* bulletModel_;
	std::unique_ptr<Object3D> bulletObject_;

	Vector3 velocity_;

	Vector3 tPos_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;

public:
	bool isDead_ = false;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos"></param> 初期座標
	/// <param name="velocity"></param> 弾の速度
	/// <param name="model"></param> モデルの見た目
	void Initialize(Vector3 pos, Vector3& velocity,Model* model);

	//更新処理
	void Update();

	//描画関数
	void Draw();

	//getter
	Vector3 GetWorldPos();
	bool IsDead() const { return isDead_; }

};



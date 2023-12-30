#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Vector3.h"

/**
 * @file PlayerBullet
 * @brief プレイヤーの弾の攻撃の処理、描画を行うクラス
 */

class PlayerBullet
{
private:

	Model* bulletModel_;
	std::unique_ptr<Object3D> bulletObject_;

	Vector3 velocity_;

	Vector3 pos_;
	Vector3 tPos_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;

	bool missile_ = false;

	Vector3 Start;
	Vector3 End;
	Vector3 p0;

	float timer = 0;
	float timerMax = 65;

	float prot_ = 0;
public:
	bool isDead_ = false;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos"></param> 初期座標
	/// <param name="velocity"></param>	弾の方向、速度
	/// <param name="model"></param> モデルの見た目
	void Initialize(Vector3 pos, Vector3& velocity,Model* model);

	//更新処理
	void Update();

	//描画関数
	void Draw();

public: //getter,setter

	bool IsDead() const { return isDead_; }
	bool GetMissile() const { return missile_; }
	Vector3 GetWorldPos();

	void SetMissile(bool missile) { missile_ = missile; }
};



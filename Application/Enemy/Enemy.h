#pragma once
#include "Model.h"
#include "Object3D.h"
#include "KeyBoard.h"
#include "Vector3.h"

#include "EnemyBullet.h"

#include <memory>
#include <list>

class Player;

/**
 * @file Enemy
 * @brief 雑魚敵の行動や攻撃を行うクラス
 */

class Enemy
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	const float ENEMY_SCALE_VALUE = 3.0f;
	const float SHADOW_SCALE_VALUE = 3.5f;
	const float ADD_POS_Y = 10.0f;
	const float END_Y_VOLUE = 45.0f;
	const float MOVE_SPEED_VOLUE = 50.0f;
	const int32_t COOLTIME_VOLUE = 50;

private:

	KeyBoard* input_;
	Player* player_ = nullptr;

	//モデル、オブジェクトを作成する変数
	Object3D* enemyObject_;
	Model* bulletModel_;
	Model* shadowModel_;
	std::unique_ptr<Object3D> shadowObject_;
	
	//playerの情報
	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3	differenceVec;

	//雑魚敵の情報
	Vector3 pos_;
	Vector3 vPos_;
	int32_t frame_ = 0;
	int32_t hp_ = 5;
	int32_t coolTime_ = 10;
	int shotOrNotShot_;	//弾撃つか撃たないか
	float timer_ = 0;
	float timerMax_ = 120;
	bool isDead_ = false;
	bool isMove_ = false;

	//線形補間の使う変数
	Vector3 start_;
	Vector3 end_;
	Vector3 p0_;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//弾の処理
	void Shot();

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="enemyObject"></param> オブジェクト情報
	/// <param name="pos"></param> 初期座標
	/// <param name="player"></param> player情報
	/// <param name="hp"></param> このオブジェクトのHP
	/// <param name="pattern"></param> 行動パターンパラメータ
	void Initialize(Object3D* enemyObject, Vector3 pos, Player* player, int hp = 3, int pattern = 1);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="shot"></param> 弾を撃つか撃たないか
	void Update(bool shot);

	//描画関数
	void Draw();

	//当たったときの処理
	void OnCollision();

	//getter
	bool IsDead() const { return isDead_; }
	bool GetIsMove() { return isMove_; }
	Vector3 GetWorldPos();
	Object3D* GetObj();
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//setter
	void SetBulletModel(Model* model);
	void SetIsMove(bool isMove) { isMove_ = isMove; }
	void SetShadow(Model* model);
};



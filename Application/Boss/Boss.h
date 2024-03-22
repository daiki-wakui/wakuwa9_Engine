#pragma once
#include "Model.h"
#include "Object3D.h"
#include "BossBullet.h"
#include "KeyBoard.h"
#include "Vector2.h"
#include "Effect.h"
#include "constJsonValue.h"

#include <memory>

//ボスがエリアのどこにいるか
enum pArea
{
	LTop,	//左上
	LBottom,	//左下
	RTop,	//右上
	RBottom	//右下
};

enum tailState
{
	Wait,
	Attack,
	Back
};

enum movementPattern
{
	NONE,
	StraightShot,	//直線
	SpinningShot,	//回転
	ScatterShot,	//全方向ばらまく
};

/**
 * @file Boss
 * @brief ボスの行動や攻撃を行うクラス
 */

class Boss
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	std::unique_ptr<constJsonValue> json_;

	KeyBoard* key_ = KeyBoard::GetInstance();

	//ボス戦の時間
	float frame_;

	//ボスエリアの中心
	Vector3 centerPos_;

	//ボスモデル,オブジェクト
	Model* model_;
	Model* tailModel_;
	Model* bulletModel_;
	Object3D* object_;
	std::unique_ptr<Object3D> tailObject_;

	//player情報取得
	Vector3 playerPos;
	int32_t toPlayerArea_ = 0;	//playerどのエリアにいるか

	//playerとボスのベクトル
	Vector3 myPos;
	Vector3	differenceVec;

	//ボスの情報
	Vector3 pos_;	//座標
	Vector3 visualRot_;	//見た目の回転
	XMFLOAT3 bulletDirRot_;	//弾を発射する回転
	XMFLOAT3 addRot_;	//ボス本体に加算する回転
	int32_t randMoveChangeTime_;	//瞬間移動するまでの時間
	int32_t nowState_ = 0;	//現在の攻撃フェーズ
	Vector2 bossLimit_ = { 140,120 };	//ボスの移動距離制限
	Vector3 vPos_;
	Vector3 leapScale_;
	int32_t movementPattern_[10];
	int32_t movementPatternCount_;

	bool isDisappear_;	//瞬間移動始まり
	bool isPop_;	//瞬間移動終わり

	int32_t moveTimer_;	//ボスの瞬間移動
	float moveDisappearTime_;

	//線形補間に使う
	Vector3 disappearStart_;
	Vector3 disappearEnd_;
	Vector3 popStart_;
	Vector3 popEnd_;

	///-------しっぽにつかう変数--------///
	Vector3 frontVec = { 0, 0, 0 };
	Vector3 tailPos_;
	Vector3 tailAngle_;
	Vector3 tailRot_;
	Vector3 teleLen_;

	//線形補間に使う
	Vector3 attackStart_;
	Vector3 attackEnd_;
	Vector3 rokStart_;	//しっぽの着弾地点に向かう
	Vector3 rokEnd_;	//しっぽの着弾地点
	Vector3 backTailStart_;	//しっぽがもどる時の始めの座標
	Vector3 tailRotLeapStart_;
	Vector3 tailRotLeapEnd_;

	float tailStateTime_[3];
	float movetime_;	//座標の線形補間に使用
	float attackingTime_;
	float radian_;
	float len_;

	bool iaAttacking_;
	bool isTailAttacking_;
	bool backTail_;
	bool endAttack_;
	bool grandTail_;
	bool startAttack_;
	///------------------------------------///

	///---------ボスの弾に使う変数---------------///
	std::list<std::unique_ptr<BossBullet>> bullets_;

	uint8_t coolTime_ = 10;
	Vector3 velocity_ = { 0,0,0 };
	///----------------------------------------///

	//エフェクトに使う
	std::list<std::unique_ptr<Effect>> effects_;
	bool isEffect_;
	//シェイク
	Vector2 randShack_;
	float shackTimer_;

	//移動処理
	void Move();
	//弾打つ関数
	void Shot();
	//しっぽの処理関数
	void Tail();

	//しっぽ攻撃時に発生するシェイク
	void ShackEffect();

public:

	bool arive_ = false;
	int32_t hp;

	//初期化
	//Model ボスのモデル
	//Vector3 初期座標
	//Object3D ボスのオブジェクト情報
	//Player プレイヤーの情報
	void Initialize(Model* model, Vector3 pos, Object3D* Object);

	//更新処理
	//bool ボスが移動可能な状態か
	void Update(bool move);

	//描画関数
	void Draw();

	//当たったときの処理
	void OnCollision();

	//弾のモデルの見た目登録
	void SetBulletModel(Model* model);

	//しっぽのモデルの見た目登録
	void SetBossModels(Model* tailmodel);

	//gettter
	int32_t GetHP() const { return hp; }
	bool GetArive() const { return arive_; }
	const std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bullets_; }
	//座標のgettter
	Vector3 GetWorldPos();
	Vector3 GetBossTailWorldPos();
};


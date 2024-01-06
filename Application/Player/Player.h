#pragma once
#include "Model.h"
#include "Object3D.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "PlayerBullet.h"

#include "Vector3.h"
#include "Vector2.h"
#include "ParticleManager.h"

#include <memory>
#include <list>

class Enemy;
class Boss;

/**
 * @file Player
 * @brief Playerの行動、攻撃を行うクラス
 */

class Player
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	
	
	
	const float EFFECT_MIN = -0.2f;
	const float EFFECT_MAX = 0.2f;
	const int32_t PARTICLE_TIME = 60;
	const Vector3 PARTICLE_ACCEL = { 0,0,0 };
	const float PARTICLE_START_SCALE = 2.0f;
	const float PARTICLE_END_SCALE = 0.0f;

	

	


	const float ADD_BIT_X_VOLUE = 5.0f;
	const int32_t BIT_FRAME_RATE = 40;
	const float BIT_POS_Y_RATE = 0.5f;
	const float ADD_BIT_POS_Y_VOLUE = 10.0f;

	const float BULLET_VECTOR_Y_VOLUE = 5.0f;
	const float BULLET_RT_VECTOR_RATE = -150.0f;

	const int32_t INVINCIBLE_FRAME_VLOUE = 200;
	const int32_t MISSLE_NUM = 4;

	const float ROK_CAMERA_POS_RATE = 30.0f;
	const float ROK_CAMERA_POS_Y_VOLUE = 10.0f;

	const float SHOT_SPEED_VOLUE = 15.0f;
	const int32_t SHOT_COOLTIME_VOLUE = 4;

	
	

private:

	Boss* targetBoss_ = nullptr;
	Enemy* targetEnemy_ = nullptr;
	KeyBoard* input_;
	GamePad* inputPad_;

	Model* playerModel_;
	Model* bulletModel_;


	Object3D* playerObject_;
	Object3D* podObject_;
	Object3D* bulletObject_;

	std::unique_ptr<Object3D> reticle3DObject_ = std::make_unique<Object3D>();

	Vector3 rot_;
	Vector3 pos_;
	Vector3 posPod_;
	Vector3 eye_;
	Vector3 target_;

	Vector3 podRot_;

	XMFLOAT3 tmpPos_;

	Vector3 sideMove_ = { 0,0,0 };
	Vector3 frontMove_ = { 0,0,0 };

	Vector3 frontVec = {0,0,0};
	Vector3 moveXVec = { 0,0,0 };
	Vector3 tmpVecY = { 0,1,0 };

	Vector3 bulletVec_ = { 0,0,0 };
	Vector3 bulletRTVec_ = { 0,0,0 };
	Vector3 bulletRTPos_ = { 0,0,0 };

	//float dashPower = 5.0f;
	bool isStep = false;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	int32_t coolTime = 7;

	bool isMove = false;

	const double PI = 3.141492;

	float radi_;
	float angle_;

	float cameraAngle_ = 180.0f;
	float cameraTargetAngle_ = 0.0f;
	float r_;
	float r2_;

	float toCameraXZLen;

	bool isJump = false;

	float jumpPower = 4.0f;

	int32_t frame = 0;

	bool dash = false;
	float dashPower_;
	float friction = 0.8f;

	float addAngle = 0.0f;

	bool isHit_ = false;
	bool isShot_ = false;

	float moveLimitX = 119.0f;
	float moveLimitZ = 119.0f;

	int32_t	invincibleFrame_ = 0;
	bool isInvincible_ = false;

	std::unique_ptr<ParticleManager> moveParticle_ = std::make_unique<ParticleManager>();

	float prot_ = 0;

	XMFLOAT3 randShake_;

	int32_t isBossRokon_ = false;


	float cAngle_;
	float cRadian_;

public:
	int32_t HP = 10;
	bool isDead = false;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="playerModel"></param> モデルの見た目
	/// <param name="playerObject"></param> オブジェクト情報
	/// <param name="input"></param> キーボード情報
	/// <param name="inputPad"></param> ゲームパッド情報
	/// <param name="podObject"></param> ビットオブジェクト情報
	void Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input, GamePad* inputPad,Object3D* podObject);
	
	//更新処理
	void Update();

	//描画関数
	void Draw();

	//パーティクル描画
	void ParticleDraw();
	
	//弾リストをクリア
	void clear();
	//当たったときの処理
	bool OnCollision();

	//弾撃つ処理
	void Shot();
	//ミサイル攻撃処理
	void Missle();
	//ロックオンカメラ
	void tergetCamera();
	//操作カメラ
	void PlayerCamera();
	//角度算出
	void RotateAngle();

public:	//getter,setter

	bool GetIsShot() { return isShot_; }
	bool GetIsHit() { return isHit_; }
	bool Getinvincible() const { return isInvincible_; }
	bool IsDead() const { return isDead; }
	bool GetIsStep() const { return isStep; }

	int GetCoolTime() const { return coolTime; };
	int GetHP() const { return HP; }

	Vector3 GetWorldPos();
	Vector3 GetScreenRTPos();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	void SetRok(bool isactive) { isBossRokon_ = isactive; }
	void SetIsShot(bool isShot) { isShot_ = isShot; }
	void SetIsHit(bool isHit) { isHit_ = isHit; }
	void SetBulletModel(Model* model, Object3D* obj);
	void SetEnemy(Boss* boss);
	void SetPos(Vector3 pos);
};


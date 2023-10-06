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

class Player
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	Enemy* targetEnemy_ = nullptr;

	Model* playerModel_;
	Model* bulletModel_;


	Object3D* playerObject_;
	Object3D* podObject_;
	Object3D* bulletObject_;
	KeyBoard* input_;
	GamePad* inputPad_;

	XMFLOAT3 rot_;
	XMFLOAT3 pos_;
	XMFLOAT3 posPod_;
	XMFLOAT3 eye_;
	XMFLOAT3 target_;

	XMFLOAT3 podRot_;

	XMFLOAT3 tmpPos_;

	Vector3 sideMove_ = { 0,0,0 };
	Vector3 frontMove_ = { 0,0,0 };

	Vector3 frontVec = {0,0,0};
	Vector3 moveXVec = { 0,0,0 };
	Vector3 tmpVecY = { 0,1,0 };

	Vector3 bulletVec_ = { 0,0,0 };

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

	bool wallHit_ = false;

	int32_t	invincibleFrame_ = 0;
	bool isInvincible_ = false;

public:
	bool GetIsShot() { return isShot_; }
	void SetIsShot(bool isShot) { isShot_ = isShot; }

	bool GetIsHit() { return isHit_; }
	void SetIsHit(bool isHit) { isHit_ = isHit; }

	bool Getinvincible() const { return isInvincible_; }

	int32_t HP = 5;
	bool isDead = false;

	void Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input, GamePad* inputPad,Object3D* podObject);
	void Update();
	void Draw();
	void SetBulletModel(Model* model,Object3D* obj);

	int GetCoolTime() const { return coolTime; };

	void clear();
	bool OnCollision();
	bool IsDead() const { return isDead; }
	bool GetIsStep() const { return isStep; }

	int GetHP() const { return HP; }

	void Shot();
	void Missle();

	void PlayerCamera();
	void RotateAngle();
	
	void wallHit();

	void SetEnemy(Enemy* enemy);

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	DirectX::XMFLOAT3 GetWorldPos();

};


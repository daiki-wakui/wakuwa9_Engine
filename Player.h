#pragma once
#include "Model.h"
#include "Object3D.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include "Vector3.h"
#include "Vector2.h"

class Player
{
private:

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

	XMFLOAT3 move_ = { 0,0,0 };

	Vector3 frontVec = {0,0,0};
	Vector3 moveXVec = { 0,0,0 };
	Vector3 tmpVecY = { 0,1,0 };

	float dashPower = 5.0f;
	bool isStep = false;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	int32_t coolTime = 7;

	bool isMove = false;

	const double PI = 3.141492;

	float radi_;

	float angle_;

	float cameraAngle_ = 180.0f;
	float r_;

	float toCameraXZLen;

public:
	int32_t HP = 5;
	bool isDead = false;

	void Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input, GamePad* inputPad,Object3D* podObject);
	void Update();
	void Draw();
	void SetBulletModel(Model* model,Object3D* obj);

	int GetCoolTime() const { return coolTime; };

	void clear();
	void OnCollision();
	bool IsDead() const { return isDead; }
	bool GetIsStep() const { return isStep; }

	int GetHP() const { return HP; }

	void Shot();
	void PlayerCamera();
	
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	DirectX::XMFLOAT3 GetWorldPos();

};


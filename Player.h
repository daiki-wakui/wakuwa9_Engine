#pragma once
#include "Model.h"
#include "Object3D.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include "Vector3.h"

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

	Model* hpModel_;
	Object3D* hpObj_[5];

	XMFLOAT3 rot_;
	XMFLOAT3 pos_;
	XMFLOAT3 posPod_;
	XMFLOAT3 eye_;

	XMFLOAT3 move_ = { 0,0,0 };

	float dashPower = 5.0f;
	bool isStep = false;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	int coolTime = 7;

	bool isMove = false;

public:
	int HP = 5;
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
	
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	DirectX::XMFLOAT3 GetWorldPos();

};


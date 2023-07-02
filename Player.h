#pragma once
#include "Model.h"
#include "Object3D.h"
#include "KeyBoard.h"
//#include "PlayerBullet.h"
#include <memory>
#include <list>
#include "Vector3.h"

class Player
{
private:

	Model* playerModel_;
	Object3D* playerObject_;
	Object3D* podObject_;
	KeyBoard* input_;

	Model* hpModel_;
	Object3D* hpObj_[5];

	XMFLOAT3 rot;
	XMFLOAT3 pos;
	XMFLOAT3 posPod;
	XMFLOAT3 eye;

	XMFLOAT3 move = { 0,0,0 };

	float dashPower = 5.0f;
	bool isStep = false;

	//std::list<std::unique_ptr<PlayerBullet>> bullets_;
	int coolTime = 7;

	bool isMove = false;

public:
	int HP = 5;
	bool isDead = false;

	void Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input, Object3D* podObject);
	void Update();
	void Draw();

	int GetCoolTime() const { return coolTime; };

	void clear();
	void OnCollision();
	bool IsDead() const { return isDead; }
	bool GetIsStep() const { return isStep; }

	
	//const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	DirectX::XMFLOAT3 GetWorldPos();

};


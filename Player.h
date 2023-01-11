#pragma once
#include "Model.h"
#include "Object3D.h"
#include "KeyBoard.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

class Player
{
private:

	Model* playerModel_;
	Object3D* playerObject_;
	KeyBoard* input_;


	XMFLOAT3 pos3d;
	XMFLOAT3 pos3d2;

	float dashPower = 5.0f;
	bool isStep = false;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	int coolTime = 7;

	int HP = 5;
	bool isDead = false;

public:

	void Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input);
	void Update();
	void Draw();

	int GetCoolTime() const { return coolTime; };

	void OnCollision();
	bool IsDead() const { return isDead; }

	//’eƒŠƒXƒg‚ðŽæ“¾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	DirectX::XMFLOAT3 GetWorldPos();
};


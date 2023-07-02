#pragma once
#include "Model.h"
#include "Object3D.h"
#include "KeyBoard.h"
#include <memory>
#include <list>
#include "EnemyBullet.h"
#include "Player.h"

class Enemy
{
private:
	Object3D* enemyObject_;
	KeyBoard* input_;

	XMFLOAT3 pos_;

	Player* player_ = nullptr;
	XMFLOAT3 playerPos;
	XMFLOAT3 enemyPos;
	Vector3	differenceVec;

	Model* bulletModel_;


	bool isDead = false;

	int HP = 5;

	int coolTime = 10;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	int pattern_;

public:

	void Initialize(Object3D* enemyObject, XMFLOAT3 pos, Player* player, int hp = 3, int pattern = 1);
	void Update();
	void Draw();

	bool IsDead() const { return isDead; }

	void OnCollision();

	XMFLOAT3 GetWorldPos();

	void SetBulletModel(Model* model);
	
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }
};



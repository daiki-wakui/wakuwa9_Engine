#pragma once
#include "Model.h"
#include "Object3D.h"
#include "KeyBoard.h"
#include "Vector3.h"

#include "EnemyBullet.h"

#include <memory>
#include <list>

class Player;

class Enemy
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	Object3D* enemyObject_;
	KeyBoard* input_;

	Vector3 pos_;
	Vector3 vPos_;

	Player* player_ = nullptr;
	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3	differenceVec;

	Model* bulletModel_;

	Model* shadowModel_;
	std::unique_ptr<Object3D> shadowObject_;


	bool isDead_ = false;

	int32_t hp_ = 5;

	int32_t coolTime_ = 10;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	int pattern_;

	float addMoveX_ = 0;

	bool isMove_ = false;

	Vector3 start_;
	Vector3 end_;
	Vector3 p0_;

	float timer_ = 0;
	float timerMax_ = 120;

public:

	void Initialize(Object3D* enemyObject, Vector3 pos, Player* player, int hp = 3, int pattern = 1);
	void Update(bool shot);
	void Draw();

	bool IsDead() const { return isDead_; }

	void OnCollision();

	Vector3 GetWorldPos();
	Object3D* GetObj();

	int32_t frame_ = 0;

	void SetBulletModel(Model* model);
	
	bool GetIsMove() { return isMove_; }
	void SetIsMove(bool isMove) { isMove_ = isMove; }
	void SetShadow(Model* model);

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }
};



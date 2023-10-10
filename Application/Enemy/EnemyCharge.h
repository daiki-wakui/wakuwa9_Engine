#pragma once
#include "Model.h"
#include "Object3D.h"
#include "KeyBoard.h"
#include "Vector3.h"

#include "EnemyBullet.h"
#include "Player.h"

#include <memory>
#include <list>

class EnemyCharge
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

	XMFLOAT3 pos_;
	Vector3 vPos_;

	Player* player_ = nullptr;
	XMFLOAT3 playerPos;
	XMFLOAT3 enemyPos;
	Vector3	differenceVec;

	Model* bulletModel_;


	bool isDead_ = false;

	int32_t hp_ = 5;

	int32_t coolTime_ = 10;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	int pattern_;

	float addMoveX_ = 0;

	bool isMove_ = false;
	float friction_ = 0.8f;


	Vector3 start_;
	Vector3 end_;
	Vector3 p0_;

	Vector3 moveVec_ = {};

	float timer_ = 0;
	float timerMax_ = 120;

public:

	void Initialize(Object3D* enemyObject, XMFLOAT3 pos, Player* player, int hp = 3, int pattern = 1);
	void Update();
	void Draw();

	bool IsDead() const { return isDead_; }

	void OnCollision();

	DirectX::XMFLOAT3 GetWorldPos();
	Object3D* GetObj();

	int32_t frame_ = 0;
	int32_t stopFrame_ = 0;


	bool GetIsMove() { return isMove_; }
	void SetIsMove(bool isMove) { isMove_ = isMove; }
};


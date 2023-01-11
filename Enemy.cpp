#include "Enemy.h"

void Enemy::Initialize(Object3D* enemyObject, XMFLOAT3 pos)
{
	enemyObject_ = enemyObject;
	pos_ = pos;
}

void Enemy::Update()
{
	enemyObject_->SetPosition(pos_);

	enemyObject_->Update();
}

void Enemy::Draw()
{
	enemyObject_->Draw();
}

void Enemy::OnCollision()
{
	HP--;

	if (HP <= 0) {
		isDead = true;
	}
}

XMFLOAT3 Enemy::GetWorldPos()
{
	XMFLOAT3 worldPos;

	worldPos.x = pos_.x;
	worldPos.y = pos_.y;
	worldPos.z = pos_.z;

	return worldPos;
}



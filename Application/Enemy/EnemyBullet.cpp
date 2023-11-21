#include "EnemyBullet.h"

void EnemyBullet::Initialize(Vector3 pos, Vector3& velocity, Model* model)
{
	tPos_ = pos;
	velocity_ = velocity;

	bulletModel_ = model;

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(bulletModel_);
	bulletObject_->Initialize();

	bulletObject_->SetPosition(tPos_);
}

void EnemyBullet::Update()
{
	tPos_.x -= velocity_.x / 5;
	tPos_.y -= velocity_.y / 5;
	tPos_.z -= velocity_.z / 5;

	bulletObject_->SetPosition(tPos_);

	bulletObject_->Update();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw()
{
	bulletObject_->Draw();
}

Vector3 EnemyBullet::GetWorldPos()
{
	Vector3 worldPos;

	worldPos.x = tPos_.x;
	worldPos.y = tPos_.y;
	worldPos.z = tPos_.z;

	return worldPos;
}

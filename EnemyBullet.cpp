#include "EnemyBullet.h"

void EnemyBullet::Initialize(DirectX::XMFLOAT3 pos, Vector3& velocity)
{
	tPos_ = pos;
	velocity_ = velocity;

	bulletModel = Model::LoadFromObj("enemyBullet");
	bulletObject_ = Object3D::Create(bulletModel, { (1.0f),(1.0f),(1.0f) });

	bulletObject_->SetPosition(tPos_);
}

void EnemyBullet::Update()
{
	tPos_.x -= velocity_.x/5;
	tPos_.y -= velocity_.y/5;
	tPos_.z -= velocity_.z/5;
	
	//tPos_.z--;
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

DirectX::XMFLOAT3 EnemyBullet::GetWorldPos()
{
	DirectX::XMFLOAT3 worldPos;

	worldPos.x = tPos_.x;
	worldPos.y = tPos_.y;
	worldPos.z = tPos_.z;

	return worldPos;
}

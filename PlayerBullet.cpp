#include "PlayerBullet.h"

void PlayerBullet::Initialize(DirectX::XMFLOAT3 pos, Vector3& velocity, Model* model,Object3D* obj)
{
	tPos_ = pos;
	velocity_ = velocity;

	bulletModel_ = model;

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(bulletModel_);
	bulletObject_->Initialize();

	bulletObject_->SetPosition(tPos_);
}

void PlayerBullet::Update()
{
	tPos_.x += velocity_.x;
	tPos_.y += velocity_.y;
	tPos_.z += velocity_.z;
	bulletObject_->SetPosition(tPos_);

	bulletObject_->Update();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw()
{
	bulletObject_->Draw();
}

DirectX::XMFLOAT3 PlayerBullet::GetWorldPos()
{
	DirectX::XMFLOAT3 worldPos;

	worldPos.x = tPos_.x;
	worldPos.y = tPos_.y;
	worldPos.z = tPos_.z;

	return worldPos;
}

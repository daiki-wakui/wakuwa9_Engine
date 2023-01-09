#include "PlayerBullet.h"

void PlayerBullet::Initialize(DirectX::XMFLOAT3 pos, Vector3& velocity)
{
	tPos_ = pos;
	velocity_ = velocity;

	/*pos_.x = tPos_.x;
	pos_.y = tPos_.y;
	pos_.z = tPos_.z;*/

	bulletModel = Model::LoadFromObj("Cube");
	bulletObject_ = Object3D::Create(bulletModel, { (1.0f),(1.0f),(1.0f) });

	bulletObject_->SetPosition(tPos_);
}

void PlayerBullet::Update()
{
	//tPos_ = bulletObject_->GetPosition();
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

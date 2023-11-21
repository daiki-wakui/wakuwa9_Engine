#include "BossBullet.h"
#include <random>
#include "MyRandom.h"

void BossBullet::Initialize(DirectX::XMFLOAT3 pos, Vector3& velocity, Model* model, int32_t randState)
{
	tPos_ = pos;
	velocity_ = velocity;

	bulletModel_ = model;

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(bulletModel_);
	bulletObject_->Initialize();
	bulletObject_->SetScale({ 3,3,3 });

	bulletObject_->SetPosition(tPos_);

	addRand_ = MyRandom::GetFloatRandom(-0.1f, 0.3f);
	bulletState_ = randState;
}

void BossBullet::Update()
{

	if (bulletState_ != 3) {
		if (bulletState_) {
			tPos_.x -= velocity_.x + addRand_;
		}
		else {
			tPos_.x -= velocity_.x - addRand_;
		}

		tPos_.y -= velocity_.y + addRand_;
		tPos_.z -= velocity_.z;
	}
	else {
		tPos_.x -= velocity_.x;
		tPos_.y -= velocity_.y;
		tPos_.z -= velocity_.z;
	}

	bulletObject_->SetPosition(tPos_);

	bulletObject_->Update();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void BossBullet::Draw()
{
	bulletObject_->Draw();
}

DirectX::XMFLOAT3 BossBullet::GetWorldPos()
{
	DirectX::XMFLOAT3 worldPos;

	worldPos.x = tPos_.x;
	worldPos.y = tPos_.y;
	worldPos.z = tPos_.z;

	return worldPos;
}

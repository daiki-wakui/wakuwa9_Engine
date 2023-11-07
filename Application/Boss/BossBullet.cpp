#include "BossBullet.h"
#include <random>

void BossBullet::Initialize(DirectX::XMFLOAT3 pos, Vector3& velocity, Model* model, bool randState)
{
	tPos_ = pos;
	velocity_ = velocity;

	bulletModel_ = model;

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(bulletModel_);
	bulletObject_->Initialize();

	bulletObject_->SetPosition(tPos_);

	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float> ve(-0.1f, 0.3f);

	addRand_ = ve(engine);
	bulletState_ = randState;
}

void BossBullet::Update()
{

	if (bulletState_) {
		tPos_.x -= velocity_.x + addRand_;
	}
	else {
		tPos_.x -= velocity_.x - addRand_;
	}

	tPos_.y -= velocity_.y + addRand_;
	tPos_.z -= velocity_.z;

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

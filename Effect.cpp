#include "Effect.h"
#include <random>

void Effect::Initialize(DirectX::XMFLOAT3 pos, Vector3& velocity, Model* model)
{
	tPos_ = pos;
	velocity_ = velocity;

	bulletModel_ = model;

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(bulletModel_);
	bulletObject_->Initialize();

	bulletObject_->SetPosition(tPos_);

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> posX(-1, 1);

	velocity_.x = posX(engine);
	velocity_.z = posX(engine);
}

void Effect::Update()
{
	pow_ += 0.005f;
	velocity_.y -= pow_;

	rot_.x = bulletObject_->GetRotation().x;
	rot_.y = bulletObject_->GetRotation().y;
	rot_.z = bulletObject_->GetRotation().z;

	rot_.x += 1000;
	rot_.z += 1000;

	bulletObject_->SetRotation({ rot_.x,rot_.y,rot_.z });

	bulletObject_->Update();

	tPos_.x += velocity_.x;
	tPos_.y += velocity_.y;
	tPos_.z += velocity_.z;

	bulletObject_->SetPosition(tPos_);

	if (pow_ > 0.5f) {
		isDead_ = true;
	}
}

void Effect::Draw()
{
	bulletObject_->Draw();
}

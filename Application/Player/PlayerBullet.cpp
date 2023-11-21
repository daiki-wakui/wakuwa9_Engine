#include "PlayerBullet.h"
#include <random>
#include "Easing.h"
#include "MyRandom.h"

void PlayerBullet::Initialize(Vector3 pos, Vector3& velocity, Model* model)
{
	tPos_ = pos;
	velocity_ = velocity;

	bulletModel_ = model;

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(bulletModel_);
	bulletObject_->Initialize();

	bulletObject_->SetPosition(tPos_);
	bulletObject_->SetScale({ 0.7f,0.7f,0.7f });

	Start.x = tPos_.x;
	Start.y = tPos_.y;
	Start.z = tPos_.z;

	p0.x = Start.x + MyRandom::GetFloatRandom(-50.0f, 50.0f);
	p0.y = Start.y + 50;
	p0.z = Start.z - 10;

	End.x = velocity_.x;
	End.y = velocity_.y;
	End.z = velocity_.z;
}

void PlayerBullet::Update()
{

	//effectParticle_->Update();

	prot_ += 0.2f;

	

	bulletObject_->Update();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	if (missile_ == true) {
		timer++;

		Vector3 a = a.lerp(Start, p0, Easing::EaseOutCubic(timer , timerMax));
		Vector3 b = b.lerp(p0, End, Easing::EaseOutCubic(timer , timerMax));

		pos_ = pos_.lerp(a, b, Easing::EaseOutCubic(timer , timerMax));

		tPos_.x = pos_.x;
		tPos_.y = pos_.y;
		tPos_.z = pos_.z;

		/*DirectX::XMFLOAT3 as;

		as.x = tPos_.x + (sinf(prot_) * 1);
		as.y = tPos_.y + (cosf(prot_) * 1);
		as.z = tPos_.z;*/

		//effectParticle_->Add(60, tPos_, { 0,0,0 }, { 0,0,0 }, 1.0f, 0.0f);
	}
	else {
		tPos_.x += velocity_.x;
		tPos_.y += velocity_.y;
		tPos_.z += velocity_.z;
		
		//effectParticle_->Add(60, tPos_, { 0,0,0 }, { 0,0,0 }, 1.0f, 0.0f);
	}

	bulletObject_->SetPosition(tPos_);
}

void PlayerBullet::Draw()
{
	bulletObject_->Draw();
}

void PlayerBullet::pDraw()
{
	effectParticle_->Draw();
}

Vector3 PlayerBullet::GetWorldPos()
{
	Vector3 worldPos;

	worldPos.x = tPos_.x;
	worldPos.y = tPos_.y;
	worldPos.z = tPos_.z;

	return worldPos;
}

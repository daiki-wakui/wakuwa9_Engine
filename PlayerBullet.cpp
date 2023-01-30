#include "PlayerBullet.h"
#include <random>


void PlayerBullet::Initialize(DirectX::XMFLOAT3 pos, Vector3& velocity,int missile)
{
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> posX(-100.0f, 100.0f);

	missile_ = missile;
	tPos_ = pos;
	velocity_ = velocity;

	Start.x = tPos_.x;
	Start.y = tPos_.y;
	Start.z = tPos_.z;

	p0.x = posX(engine);
	p0.y = Start.y + 80;
	p0.z = Start.z - 100;

	End.x = velocity_.x;
	End.y = velocity_.y;
	End.z = velocity_.z;

	//pos_.x = tPos_.x;
	//pos_.y = tPos_.y;
	//pos_.z = tPos_.z;

	bulletModel = Model::LoadFromObj("Cube");
	bulletObject_ = Object3D::Create(bulletModel, { (1.0f),(1.0f),(1.0f) });

	bulletObject_->SetPosition(tPos_);
}

void PlayerBullet::Update()
{
	if (missile_ == true) {
		timer++;
		if (timer >= 60) {
			timer = 60;
		}

		Vector3 a = a.lerp(Start, p0, EaseOut(timer / timerMax));
		Vector3 b = b.lerp(p0, End, EaseOut(timer / timerMax));

		pos = pos.lerp(a, b, EaseOut(timer / timerMax));

		tPos_.x = pos.x;
		tPos_.y = pos.y;
		tPos_.z = pos.z;
	}
	else {
		//tPos_ = bulletObject_->GetPosition();
		tPos_.x += velocity_.x;
		tPos_.y += velocity_.y;
		tPos_.z += velocity_.z;
	}

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

float PlayerBullet::EaseOut(float t) {
	return sin((t * 3.14f) / 2.0f);
}
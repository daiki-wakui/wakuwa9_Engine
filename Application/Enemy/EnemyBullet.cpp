#include "EnemyBullet.h"

//初期化
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

//更新処理
void EnemyBullet::Update()
{
	tPos_.x -= velocity_.x / VELOCITY_RATE;
	tPos_.y -= velocity_.y / VELOCITY_RATE;
	tPos_.z -= velocity_.z / VELOCITY_RATE;

	bulletObject_->SetPosition(tPos_);

	bulletObject_->Update();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

//描画関数
void EnemyBullet::Draw()
{
	bulletObject_->Draw();
}

//getter
Vector3 EnemyBullet::GetWorldPos()
{
	Vector3 worldPos;

	worldPos.x = tPos_.x;
	worldPos.y = tPos_.y;
	worldPos.z = tPos_.z;

	return worldPos;
}

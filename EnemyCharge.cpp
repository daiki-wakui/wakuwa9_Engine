#include "EnemyCharge.h"

void EnemyCharge::Initialize(Object3D* enemyObject, XMFLOAT3 pos, Player* player, int hp, int pattern)
{
	enemyObject_ = enemyObject;
	pos_ = pos;
	//pos_.y += 10;
	player_ = player;
	hp_ = hp;
	vPos_.x = pos_.x;
	vPos_.y = pos_.y;
	vPos_.z = pos_.z;
	enemyObject_->SetScale({ 1,1,1 });

	pattern_ = pattern;

	start_.x = pos.x;
	start_.y = pos.y;
	start_.z = pos.z;

	end_.x = start_.x;
	end_.y = start_.y - 45;
	end_.z = start_.z;
}

void EnemyCharge::Update()
{
	enemyObject_->Update();

	frame_++;


	if (frame_ < 100) {
		isMove_ = true;
	}

	
	if (frame_ > 100) {
		isMove_ = false;
		stopFrame_++;

		if (stopFrame_ > 60) {
			frame_ = 0;
			stopFrame_ = 0;

			moveVec_.x = player_->GetWorldPos().x - enemyObject_->GetPosition().x;
			moveVec_.z = player_->GetWorldPos().z - enemyObject_->GetPosition().z;

			moveVec_.normalize();
		}
	}

	if (isMove_) {
		

		vPos_ += moveVec_ / 2;

		enemyObject_->SetPosition({ vPos_.x,vPos_.y,vPos_.z });
	}
}

void EnemyCharge::Draw()
{
	enemyObject_->Draw();
}

void EnemyCharge::OnCollision()
{
	hp_--;

	if (hp_ <= 0) {
		isDead_ = true;
	}
}

DirectX::XMFLOAT3 EnemyCharge::GetWorldPos()
{
	XMFLOAT3 worldPos;

	worldPos.x = pos_.x;
	worldPos.y = pos_.y;
	worldPos.z = pos_.z;

	return worldPos;
}

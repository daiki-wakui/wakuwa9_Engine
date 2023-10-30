#include "Boss.h"

DirectX::XMFLOAT3 Boss::GetWorldPos()
{
	DirectX::XMFLOAT3 worldPos;

	worldPos.x = object_->GetPosition().x;
	worldPos.y = object_->GetPosition().y;
	worldPos.z = object_->GetPosition().z;

	return worldPos;
}

void Boss::Initialize(Model* Model, XMFLOAT3 pos, Object3D* Object, Player* player)
{
	model_ = Model;
	object_ = Object;
	player_ = player;
	pos_ = pos;

	hp = 50;
	arive_ = true;
}

void Boss::Update(bool move)
{
	if (!move) {
		coolTime_--;
		frame_++;
		pos_.x = 2.0f * cosf(3.14f * frame_ / 200) + pos_.x;

	}

	if (coolTime_ == 0) {
		playerPos = player_->GetWorldPos();
		enemyPos = GetWorldPos();

		differenceVec.x = enemyPos.x - playerPos.x;
		differenceVec.y = enemyPos.y - playerPos.y;
		differenceVec.z = enemyPos.z - playerPos.z;
		differenceVec.normalize();
		differenceVec /= 3;

		Vector3 velocity(differenceVec);

		velocity.multiplyMat4(object_->matWorld_);

		std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();
		newBullet->Initialize(pos_, velocity, bulletModel_);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

		coolCount_++;

		if (coolCount_ > 10) {
			coolTime_ = 20;

			if (coolCount_ >= 20) {
				coolCount_ = 0;
			}
		}
		else {
			coolTime_ = 10;
		}
		
	}

	//デスフラグが立った弾を削除
	bullets_.remove_if([](std::unique_ptr<BossBullet>& bullet) {
		return bullet->IsDead();
		});

	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Update();
	}

	object_->SetPosition(pos_);

	object_->Update();
}

void Boss::Draw()
{
	object_->Draw();

	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

void Boss::OnCollision()
{
	hp--;

	if (hp <= 0) {
		arive_ = false;
	}
}

void Boss::SetBulletModel(Model* model)
{
	bulletModel_ = model;
}

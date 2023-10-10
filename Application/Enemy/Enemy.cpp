#include "Enemy.h"
#include "Player.h"
#include "Easing.h"

void Enemy::Initialize(Object3D* enemyObject, XMFLOAT3 pos, Player* player, int hp, int pattern)
{
	enemyObject_ = enemyObject;
	pos_ = pos;
	pos_.y += 10;
	player_ = player;
	hp_ = hp;

	vPos_.x = pos_.x;
	vPos_.y = pos_.y;
	vPos_.z = pos_.z;

	enemyObject_->SetScale({ 3,3,3 });

	pattern_ = pattern;

	bullets_.clear();

	start_.x = pos.x;
	start_.y = pos.y;
	start_.z = pos.z;

	end_.x = start_.x;
	end_.y = start_.y - 45;
	end_.z = start_.z;

	shadowObject_ = std::make_unique<Object3D>();
	shadowObject_->SetModel(shadowModel_);
	shadowObject_->Initialize();
	shadowObject_->SetScale({ 3.5f, 3.5f, 3.5f });
}

void Enemy::Update(bool shot)
{

	if (isMove_) {

		timer_++;

		vPos_ = vPos_.lerp(start_, end_, Easing::EaseOutBack(timer_, timerMax_));

		

		if (timer_ > timerMax_) {
			pos_.y = vPos_.y;
			pos_.z = vPos_.z;

			pos_.y = sinf(3.14f * frame_ * 50) + pos_.y;

			frame_++;
		}
		else {
			pos_.x = vPos_.x;
			pos_.y = vPos_.y;
			pos_.z = vPos_.z;
		}
		

		if (shot) {
			coolTime_--;
		}

	}
	
	if (coolTime_ == 0) {
		playerPos = player_->GetWorldPos();
		enemyPos = GetWorldPos();

		differenceVec.x = enemyPos.x - playerPos.x;
		differenceVec.y = enemyPos.y - playerPos.y;
		differenceVec.z = enemyPos.z - playerPos.z;
		differenceVec.normalize();

		Vector3 velocity(differenceVec);

		velocity.multiplyMat4(enemyObject_->matWorld_);

		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(pos_, velocity, bulletModel_);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

		coolTime_ = 50;
	}

	//デスフラグが立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
	});

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	enemyObject_->SetPosition(pos_);

	enemyObject_->Update();

	shadowObject_->SetPosition({ pos_.x,0,pos_.z });
	shadowObject_->Update();
}

void Enemy::Draw()
{
	enemyObject_->Draw();
	shadowObject_->Draw();

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

void Enemy::OnCollision()
{
	hp_--;

	if (hp_ <= 0) {
		isDead_ = true;
	}
}

DirectX::XMFLOAT3 Enemy::GetWorldPos()
{
	XMFLOAT3 worldPos;

	worldPos.x = pos_.x;
	worldPos.y = pos_.y;
	worldPos.z = pos_.z;

	return worldPos;
}

Object3D* Enemy::GetObj()
{
	Object3D* obj;

	obj = enemyObject_;

	return obj;
}

void Enemy::SetBulletModel(Model* model)
{
	bulletModel_ = model;
}

void Enemy::SetShadow(Model* model)
{
	shadowModel_ = model;
}


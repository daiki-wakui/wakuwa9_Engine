#include "Enemy.h"
#include "Player.h"

void Enemy::Initialize(Object3D* enemyObject, XMFLOAT3 pos, Player* player, int hp, int pattern)
{
	enemyObject_ = enemyObject;
	pos_ = pos;
	pos_.y += 10;
	player_ = player;
	hp_ = hp;

	enemyObject_->SetScale({ 3,3,3 });

	pattern_ = pattern;

	bullets_.clear();
}

void Enemy::Update(bool shot)
{
	frame_++;

	addMoveX_++;

	if (addMoveX_ < 60) {
		pos_.x += 0.1f;

	}
	else if (addMoveX_ > 60) {
		pos_.x -= 0.1f;
	}

	if (addMoveX_ > 120) {
		addMoveX_ = 0;
	}
	
	pos_.y = sinf(3.14f * frame_ * 100) + 10;


	if (shot) {
		coolTime_--;
	}

	if (coolTime_ == 0) {
		playerPos = player_->GetWorldPos();
		enemyPos = GetWorldPos();

		differenceVec.x = enemyPos.x - playerPos.x;
		differenceVec.y = enemyPos.y - playerPos.y;
		differenceVec.z = enemyPos.z - playerPos.z;
		differenceVec.normalize();

		const float kBulletSpeed = -1.0f;
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
}

void Enemy::Draw()
{
	enemyObject_->Draw();

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


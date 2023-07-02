#include "Enemy.h"

void Enemy::Initialize(Object3D* enemyObject, XMFLOAT3 pos, Player* player, int hp, int pattern)
{
	enemyObject_ = enemyObject;
	pos_ = pos;
	pos_.y += 10;
	player_ = player;
	HP = hp;

	enemyObject_->SetScale({ 3,3,3 });

	pattern_ = pattern;

	bullets_.clear();
}

void Enemy::Update()
{

	coolTime--;

	if (coolTime == 0) {
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

		//’e‚ğ“o˜^‚·‚é
		bullets_.push_back(std::move(newBullet));

		coolTime = 50;
	}

	//ƒfƒXƒtƒ‰ƒO‚ª—§‚Á‚½’e‚ğíœ
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
	HP--;

	if (HP <= 0) {
		isDead = true;
	}
}

XMFLOAT3 Enemy::GetWorldPos()
{
	XMFLOAT3 worldPos;

	worldPos.x = pos_.x;
	worldPos.y = pos_.y;
	worldPos.z = pos_.z;

	return worldPos;
}

void Enemy::SetBulletModel(Model* model)
{
	bulletModel_ = model;
}


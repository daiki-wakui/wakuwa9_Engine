#include "Enemy.h"
#include "Easing.h"
#include "wa9Math.h"

//初期化処理
void Enemy::Initialize(Object3D* enemyObject, Vector3 pos,  int hp, int pattern)
{
	enemyObject_ = enemyObject;
	pos_ = pos;
	pos_.y += ADD_POS_Y;
	hp_ = hp;

	vPos_ = pos_;
	enemyObject_->SetScale({ ENEMY_SCALE_VALUE,ENEMY_SCALE_VALUE,ENEMY_SCALE_VALUE });
	shotOrNotShot_ = pattern;
	bullets_.clear();
	start_ = pos;

	end_.x = start_.x;
	end_.y = start_.y - END_Y_VOLUE;
	end_.z = start_.z;

	shadowObject_ = std::make_unique<Object3D>();
	shadowObject_->SetModel(shadowModel_);
	shadowObject_->Initialize();
	shadowObject_->SetScale({ SHADOW_SCALE_VALUE, SHADOW_SCALE_VALUE, SHADOW_SCALE_VALUE });
}

//更新処理
void Enemy::Update(bool shot)
{
	//動き開始
	if (isMove_) {
		timer_++;

		if (shotOrNotShot_) {
			vPos_ = vPos_.lerp(start_, end_, Easing::EaseOutBack(timer_, timerMax_));
		}

		if (timer_ > timerMax_) {
			pos_.y = vPos_.y;
			pos_.z = vPos_.z;

			pos_.y = sinf(wa9Math::PI() * frame_ * MOVE_SPEED_VOLUE) + pos_.y;

			frame_++;
		}
		else {
			pos_ = vPos_;
		}

		//弾を撃つ敵なら
		if (shot && shotOrNotShot_) {
			coolTime_--;
		}
	}
	
	Shot();

	enemyObject_->SetPosition(pos_);
	enemyObject_->Update();
	shadowObject_->SetPosition({ pos_.x,0,pos_.z });
	shadowObject_->Update();
}

//描画関数
void Enemy::Draw()
{
	enemyObject_->Draw();
	shadowObject_->Draw();

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

//当たったときの処理
void Enemy::OnCollision()
{
	hp_--;

	if (hp_ <= 0) {
		isDead_ = true;
	}
}

//座標のgetter
Vector3 Enemy::GetWorldPos()
{
	Vector3 worldPos;

	worldPos.x = pos_.x;
	worldPos.y = pos_.y;
	worldPos.z = pos_.z;

	return worldPos;
}

//オブジェクト情報のgetter
Object3D* Enemy::GetObj()
{
	Object3D* obj;

	obj = enemyObject_;

	return obj;
}

//setter
void Enemy::SetBulletModel(Model* model)
{
	bulletModel_ = model;
}

void Enemy::SetShadow(Model* model)
{
	shadowModel_ = model;
}

//弾の処理
void Enemy::Shot()
{
	//弾発射
	if (coolTime_ == 0) {
		//playerPos = player_->GetWorldPos();
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

		coolTime_ = COOLTIME_VOLUE;
	}

	//デスフラグが立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
}
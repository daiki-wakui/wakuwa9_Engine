#include "Boss.h"
#include <random>
#include "Easing.h"
#include <stdlib.h>
#include "MyRandom.h"
#include "wa9Math.h"
#include "Player.h"

//getter
Vector3 Boss::GetWorldPos()
{
	Vector3 worldPos;

	worldPos.x = object_->GetPosition().x;
	worldPos.y = object_->GetPosition().y;
	worldPos.z = object_->GetPosition().z;

	return worldPos;
}

//初期化
void Boss::Initialize(Model* model, Vector3 pos, Object3D* Object, Player* player)
{
	model_ = model;
	object_ = Object;
	player_ = player;
	pos_ = pos;
	centerPos_ = pos;

	hp = 50;
	arive_ = true;
	nowState_ = 0;
	frame_ = 0;
	moveTimer_ = 0;

	tailObject_.reset();
	tailObject_ = std::make_unique<Object3D>();
	tailObject_->SetModel(tailModel_);
	tailObject_->Initialize();
	tailObject_->SetScale({ 10,10,10 });
	tailObject_->SetPosition(object_->GetPosition());

	leapScale_ = { 15,15,15 };
	randMoveChangeTime_ = 60;
	bullets_.clear();

	movementPattern_[0] = 1;
	movementPattern_[1] = 1;
	movementPattern_[2] = 2;
	movementPattern_[3] = 3;
	movementPattern_[4] = 2;
	movementPattern_[5] = 3;
	movementPatternCount_ = 0;

	tailPos_.x = object_->GetPosition().x;
	tailPos_.y = object_->GetPosition().y;
	tailPos_.z = object_->GetPosition().z + 30;
}

//更新処理
void Boss::Update(bool move)
{
	if (movementPatternCount_ >= 6) {
		movementPatternCount_ = 0;
	}

	if (KeyBoard::GetInstance()->keyInstantPush(DIK_1)) {
		nowState_ = 1;
	}
	if (KeyBoard::GetInstance()->keyInstantPush(DIK_2)) {
		nowState_ = 2;
	}
	if (KeyBoard::GetInstance()->keyInstantPush(DIK_3)) {
		nowState_ = 3;
	}

	arive_ = true;

	if (!move) {
		Move();
	}

	frame_++;

	pos_.y = 0.3f * cosf(wa9Math::PI() * frame_ / 50) + pos_.y;

	switch (nowState_)
	{
	case 1:
		if (!isPop_ && !isDisappear_) {
			coolTime_--;
		}

		visualRot_.x++;
		visualRot_.z++;
		
		break;
	case 2:
		addRot_.y = 2;
		addRot_.x = 2;
		
		if (!isPop_ && !isDisappear_) {
			coolTime_--;
		}
		bulletDirRot_.y += 1.5f;
		visualRot_.y += 1.5f;
		
		break;
	case 3:
		addRot_.y += 0.1f;
		addRot_.x += 0.1f;

		if (!isPop_ && !isDisappear_) {
			coolTime_--;
		}

		visualRot_.z += 14.5f;
		visualRot_.x += 14.5f;

		bulletDirRot_.y += 0.5f;
		break;
	default:
		break;
	}

	Shot();

	//デスフラグが立った弾を削除
	bullets_.remove_if([](std::unique_ptr<BossBullet>& bullet) {
		return bullet->IsDead();
		});

	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//弾の更新処理
	for (std::unique_ptr<Effect>& effect : effects_) {
		effect->Update();
	}

	//デスフラグが立った弾を削除
	effects_.remove_if([](std::unique_ptr<Effect>& bullet) {
		return bullet->IsDead();
	});

	Tail();

	ShackEffect();

	playerPos = player_->GetWorldPos();

	object_->SetScale({ leapScale_.x,leapScale_.y ,leapScale_.z });
	object_->SetPosition(pos_);
	object_->SetRotation(visualRot_);
	tailObject_->SetRotation({ tailAngle_.x,-tailAngle_.y,0});
	tailObject_->SetPosition(tailPos_);

	object_->Update();
	tailObject_->Update();
}

//描画関数
void Boss::Draw()
{
	object_->Draw();
	tailObject_->Draw();

	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	for (std::unique_ptr<Effect>& effect : effects_) {
		effect->Draw();
	}
}

//当たったときの処理
void Boss::OnCollision()
{
	hp--;

	if (hp <= 0) {
		arive_ = false;
	}
}

//弾のモデル
void Boss::SetBulletModel(Model* model)
{
	bulletModel_ = model;
}

//ボスのしっぽモデル
void Boss::SetBossModels(Model* framemodel)
{
	tailModel_ = framemodel;
}

Vector3 Boss::GetBossTailWorldPos()
{
	Vector3 pos = tailPos_;

	return pos;
}

//移動処理
void Boss::Move()
{
	if (tailStateTime_[Wait] > 120 && !iaAttacking_) {
		startAttack_ = true;
	}
	else {
		tailStateTime_[Wait]++;
	}

	if (iaAttacking_) {
		tailStateTime_[Attack]++;

		if (tailStateTime_[Attack] == 60) {
			attackingTime_ = 0;
			isTailAttacking_ = false;
			backTail_ = false;
			rokStart_ = tailPos_;
			rokEnd_ = playerPos;
		}

		if (tailStateTime_[Attack] >= 80) {
			isTailAttacking_ = true;
			tailStateTime_[Wait] = 0;
		}
	}

	if (startAttack_) {
		iaAttacking_ = true;
		attackStart_ = tailPos_;
		attackEnd_ = tailPos_;
		attackEnd_.y += 50;
		startAttack_ = false;
	}

	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float> moveLimitX(-bossLimit_.x, bossLimit_.x);
	std::uniform_real_distribution<float> moveLimitY(-bossLimit_.y, bossLimit_.y);

	std::uniform_real_distribution<float> moveLimitLBX(0, bossLimit_.x);
	std::uniform_real_distribution<float> moveLimitLBY(0, bossLimit_.y);

	std::uniform_real_distribution<float> moveLimitLTX(0, bossLimit_.x);
	std::uniform_real_distribution<float> moveLimitLTY(-bossLimit_.y, 0);

	std::uniform_real_distribution<float> moveLimitRBX(-bossLimit_.x, 0);
	std::uniform_real_distribution<float> moveLimitRBY(0, bossLimit_.y);

	std::uniform_real_distribution<float> moveLimitRTX(-bossLimit_.x, 0);
	std::uniform_real_distribution<float> moveLimitRTY(-bossLimit_.y, 0);

	//playerが左下にいるとき
	if (player_->GetWorldPos().x < centerPos_.x && player_->GetWorldPos().z < centerPos_.z) {
		toPlayerArea_ = LBottom;
	}
	//左上にいるとき
	else if (player_->GetWorldPos().x < centerPos_.x && player_->GetWorldPos().z > centerPos_.z) {
		toPlayerArea_ = LTop;
	}
	//右下にいるとき
	else if (player_->GetWorldPos().x > centerPos_.x && player_->GetWorldPos().z < centerPos_.z) {
		toPlayerArea_ = RBottom;
	}
	//右上にいるとき
	else if (player_->GetWorldPos().x > centerPos_.x && player_->GetWorldPos().z > centerPos_.z) {
		toPlayerArea_ = RTop;
	}
	else {
		toPlayerArea_ = 0;
	}

	moveTimer_++;

	if (moveTimer_ >= randMoveChangeTime_) {
		if (!isDisappear_) {
			moveDisappearTime_ = 0;
			isDisappear_ = true;
			disappearStart_.x = object_->GetPosition().x;
			disappearStart_.y = object_->GetPosition().y;
			disappearStart_.z = object_->GetPosition().z;

			disappearEnd_ = disappearStart_;
		}

		moveTimer_ = 0;
	}



	//瞬間移動始め
	if (isDisappear_) {
		moveDisappearTime_++;
		moveDisappearTime_ = min(20, moveDisappearTime_);

		vPos_ = vPos_.lerp(disappearStart_, disappearEnd_, Easing::EaseOutQuint(moveDisappearTime_, 20));
		leapScale_ = leapScale_.lerp({ 15,15,15 }, { 0,0,15 }, Easing::EaseOutQuint(moveDisappearTime_, 20));

		pos_.x = vPos_.x;
		pos_.y = vPos_.y;
		pos_.z = vPos_.z;
		
		if (moveDisappearTime_ >= 20) {
			isPop_ = true;

			pos_.x = centerPos_.x;
			pos_.z = centerPos_.z;

			if (toPlayerArea_ == LBottom) {
				pos_.x += moveLimitLBX(engine);
				pos_.z += moveLimitLBY(engine);
			}
			else if (toPlayerArea_ == LTop) {
				pos_.x += moveLimitLTX(engine);
				pos_.z += moveLimitLTY(engine);
			}
			else if (toPlayerArea_ == RBottom) {
				pos_.x += moveLimitRBX(engine);
				pos_.z += moveLimitRBY(engine);
			}
			else if (toPlayerArea_ == RTop) {
				pos_.x += moveLimitRTX(engine);
				pos_.z += moveLimitRTY(engine);
			}
			else {
				pos_.x += moveLimitX(engine);
				pos_.z += moveLimitY(engine);
			}

			isDisappear_ = false;
			isPop_ = true;

			moveDisappearTime_ = 0;

			object_->SetPosition(pos_);
			object_->Update();

			popStart_.x = object_->GetPosition().x;
			popStart_.y = object_->GetPosition().y;
			popStart_.z = object_->GetPosition().z;

			popEnd_ = popStart_;
			popEnd_.y = centerPos_.y;

			vPos_ = { 0,0,0 };
			addRot_ = { 0,0,0 };
			frame_ = 0;
		}
	}

	//瞬間移動後
	if (isPop_) {
		moveDisappearTime_++;
		moveDisappearTime_ = min(20, moveDisappearTime_);

		vPos_ = vPos_.lerp(popStart_, popEnd_, Easing::EaseOutQuint(moveDisappearTime_, 20));
		leapScale_ = leapScale_.lerp({ 0,0,15 }, { 15,15,15 }, Easing::EaseOutQuint(moveDisappearTime_, 20));

		pos_.x = vPos_.x;
		pos_.y = vPos_.y;
		pos_.z = vPos_.z;
		
		if (moveDisappearTime_ >= 20) {
			isPop_ = false;
			nowState_ = movementPattern_[movementPatternCount_];
			randMoveChangeTime_ = rand() % 300 + 60;
			movementPatternCount_++;
		}
	}

}

//弾の処理
void Boss::Shot()
{
	if (coolTime_ <= 0) {

		if (nowState_ == 1) {
			playerPos = player_->GetWorldPos();
			myPos = object_->GetPosition();

			differenceVec.x = myPos.x - playerPos.x;
			differenceVec.y = myPos.y - playerPos.y;
			differenceVec.z = myPos.z - playerPos.z;
			differenceVec /= 3;
			differenceVec.normalize();

			object_->SetRotation({ 0,0,0 });
			object_->Update();

			velocity_ = differenceVec;
			velocity_.multiplyMat4(object_->matWorld_);
			velocity_ /= 10;
		}
		else if (nowState_ == 2) {
			Vector3 start = { GetWorldPos().x,GetWorldPos().y,GetWorldPos().z };
			Vector3 end({ 0,0,0 });
			Vector3 length = { 0, 0, 10 };
			frontVec = { 0, 0, 0 };

			//終点座標を設定
			end.x = start.x + length.x;
			end.y = start.y + length.y;
			end.z = start.z + length.z;

			//回転を考慮した座標を設定
			end.x = start.x + sinf(bulletDirRot_.y / 40);
			end.z = start.z + cosf(bulletDirRot_.y / 40);

			//始点と終点から正面ベクトルを求める
			frontVec.x = end.x - start.x;
			frontVec.y = end.y - start.y;
			frontVec.z = end.z - start.z;


			frontVec.normalize();


			velocity_ = frontVec;
		}
		else if (nowState_ == 3) {
			Vector3 start = { GetWorldPos().x,GetWorldPos().y,GetWorldPos().z };
			Vector3 end({ 0,0,0 });
			Vector3 length = { 0, 0, 10 };
			frontVec = { 0, 0, 0 };

			//終点座標を設定
			end.x = start.x + length.x;
			end.y = start.y + length.y;
			end.z = start.z + length.z;

			//回転を考慮した座標を設定
			end.x = start.x + sinf(bulletDirRot_.y);
			end.z = start.z + cosf(bulletDirRot_.y);

			//始点と終点から正面ベクトルを求める
			frontVec.x = end.x - start.x;
			frontVec.y = end.y - start.y;
			frontVec.z = end.z - start.z;

			frontVec.normalize();

			velocity_ = frontVec;
		}

		if (nowState_ == 1) {
			std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();

			newBullet->Initialize(pos_, velocity_, bulletModel_, 3);

			//弾を登録する
			bullets_.push_back(std::move(newBullet));

			coolTime_ = 7;
		}
		else if (nowState_ == 2) {

			for (int i = 0; i < 2; i++) {
				std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();

				if (i == 0) {
					newBullet->Initialize(pos_, velocity_, bulletModel_, 0);
				}
				else {
					newBullet->Initialize(pos_, velocity_, bulletModel_, 1);
				}

				//弾を登録する
				bullets_.push_back(std::move(newBullet));

			}

			coolTime_ = 5;
		}
		else if (nowState_ == 3) {
			for (int i = 0; i < 2; i++) {
				std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();

				if (i == 0) {
					newBullet->Initialize(pos_, velocity_, bulletModel_, 0);
				}
				else {
					newBullet->Initialize(pos_, velocity_, bulletModel_, 1);
				}

				//弾を登録する
				bullets_.push_back(std::move(newBullet));

			}

			coolTime_ = 2;
		}


	}
}

//しっぽ処理
void Boss::Tail()
{
	//しっぽの座標
	teleLen_ = pos_ - tailPos_;
	len_ = teleLen_.length();
	teleLen_.normalize();

	if (iaAttacking_ && !isTailAttacking_) {
		movetime_++;
		movetime_ = min(20, movetime_);
		tailPos_ = tailPos_.lerp(attackStart_, attackEnd_, Easing::EaseOutQuint(movetime_, 20));
		tailAngle_.x = 120;

		radian_ = std::atan2(tailPos_.z - playerPos.z, tailPos_.x - playerPos.x);
		tailAngle_.y = radian_ * (wa9Math::Degree180() / wa9Math::PI()) + wa9Math::Degree90();
	}
	else if (isTailAttacking_) {
		tailAngle_.y = radian_ * (wa9Math::Degree180() / wa9Math::PI()) + wa9Math::Degree90();
	}
	else {
		tailPos_.y = 0.2f * cosf(wa9Math::PI() * frame_ / 80) + tailPos_.y;
		tailAngle_.y = 0;
		movetime_ = 0;
	}


	//しっぽ攻撃中
	if (isTailAttacking_) {

		tailPos_ = tailPos_.lerp(rokStart_, rokEnd_, Easing::EaseInCubic(attackingTime_, 30));

		if (attackingTime_ == 30) {
			grandTail_ = true;
		}

		//戻る
		if (attackingTime_ >= 60) {
			backTail_ = true;
		}

		if (backTail_) {
			attackingTime_--;
			attackingTime_ = max(0, attackingTime_);

			//終了
			if (attackingTime_ <= 0) {
				tailStateTime_[Wait] = 0;
				tailStateTime_[Attack] = 0;
				startAttack_ = false;

				iaAttacking_ = false;
				isTailAttacking_ = false;
				endAttack_ = true;
				backTailStart_ = object_->GetPosition();
				backTailStart_.z += 30;
				tailRotLeapStart_.x = tailAngle_.x;
				tailRotLeapEnd_.x = 0;
				isEffect_ = false;
			}
		}
		else {
			attackingTime_++;
		}

	}

	if (endAttack_) {
		attackingTime_++;
		tailPos_ = tailPos_.lerp(rokStart_, backTailStart_, Easing::EaseInCubic(attackingTime_, 30));
		tailRot_ = tailRot_.lerp(tailRotLeapStart_, tailRotLeapEnd_, Easing::EaseOutBack(attackingTime_, 20));
		tailAngle_.x = tailRot_.x;

		if (attackingTime_ >= 30) {
			endAttack_ = false;
		}
	}
}

void Boss::ShackEffect()
{
	if (grandTail_) {
		Vector3 toEye = Object3D::GetEye();
		Vector3 toTerget = Object3D::GetTarget();
		randShack_.x = MyRandom::GetFloatRandom(-2.0f, 2.0f);
		randShack_.y = MyRandom::GetFloatRandom(-2.0f, 2.0f);

		toEye.x += randShack_.x;
		toEye.y += randShack_.y;
		toTerget.x += randShack_.x;
		toTerget.y += randShack_.y;

		Object3D::SetEye(toEye);
		Object3D::SetTarget(toTerget);

		shackTimer_++;

		if (shackTimer_ > 15) {
			grandTail_ = false;
			shackTimer_ = 0;
		}

		if (!isEffect_) {
			Vector3 v = { 0,2.0f,0 };
			for (int i = 0; i < 15; i++) {

				std::unique_ptr<Effect> newObj = std::make_unique<Effect>();
				newObj->Initialize(tailPos_, v, model_);
				newObj->SetScale(2.0f);
				effects_.push_back(std::move(newObj));
			}
			isEffect_ = true;
		}

	}
}
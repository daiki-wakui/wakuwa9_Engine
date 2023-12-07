#include "Boss.h"
#include <random>
#include "Easing.h"
#include <stdlib.h>
#include "MyRandom.h"

Vector3 Boss::GetWorldPos()
{
	Vector3 worldPos;

	worldPos.x = object_->GetPosition().x;
	worldPos.y = object_->GetPosition().y;
	worldPos.z = object_->GetPosition().z;

	return worldPos;
}

void Boss::Initialize(Model* model, Vector3 pos, Object3D* Object, Player* player)
{
	model_ = model;
	object_ = Object;
	player_ = player;
	pos_ = pos;
	centerPos_ = pos;

	hp = 50;
	arive_ = true;
	state_ = 0;
	frame_ = 0;
	moveTimer_ = 0;

	randState_ = 0;
	

	frameObject_.reset();
	frameObject_ = std::make_unique<Object3D>();
	frameObject_->SetModel(frameModel_);
	frameObject_->Initialize();
	frameObject_->SetScale({ 10,10,10 });
	frameObject_->SetPosition(object_->GetPosition());

	vScale_ = { 15,15,15 };
	
	

	bulletCononObject_.reset();
	bulletCononObject_ = std::make_unique<Object3D>();
	bulletCononObject_->SetModel(bulletCononModel_);
	bulletCononObject_->Initialize();
	bulletCononObject_->SetScale({ 15,15,15 });
	bulletCononObject_->SetPosition(object_->GetPosition());
	cononPos_ = object_->GetPosition();

	randMoveChange_ = 60;

	bullets_.clear();


	movementPattern_[0] = 1;
	movementPattern_[1] = 1;
	movementPattern_[2] = 2;
	movementPattern_[3] = 3;
	movementPattern_[4] = 2;
	movementPattern_[5] = 3;


	movementPatternCount_ = 0;

	telePos_.x = object_->GetPosition().x;
	telePos_.y = object_->GetPosition().y;
	telePos_.z = object_->GetPosition().z + 30;
}

void Boss::Update(bool move)
{
	if (movementPatternCount_ >= 6) {
		movementPatternCount_ = 0;
	}


	if (KeyBoard::GetInstance()->keyInstantPush(DIK_1)) {
		state_ = 1;
	}
	if (KeyBoard::GetInstance()->keyInstantPush(DIK_2)) {
		state_ = 2;
	}
	if (KeyBoard::GetInstance()->keyInstantPush(DIK_3)) {
		state_ = 3;
	}

	arive_ = true;

	if (!move) {
		Move();
	}

	state_ = randState_;
	frame_++;

	//frameRot_.y++;
	//frameRot_.x++;

	pos_.y = 0.3f * cosf(3.14f * frame_ / 50) + pos_.y;
	cononPos_.y = pos_.y;

	switch (state_)
	{
	case 1:

		//frameRot_.y += 3;
		//frameRot_.x -= 10;

		if (!isPop_ && !isDisappear_) {
			coolTime_--;
		}

		visualRot_.x++;
		visualRot_.z++;

		cononPos_.y = pos_.y - 25;

		break;
	case 2:


		addRot_.y = 2;
		addRot_.x = 2;

		//frameRot_.y += addRot_.y;
		//frameRot_.x += addRot_.x;

		if (!isPop_ && !isDisappear_) {
			coolTime_--;
		}
		bulletDirRot_.y += 1.5f;
		visualRot_.y += 1.5f;
		cononPos_.y = pos_.y - 25;

		break;
	case 3:


		addRot_.y += 0.1f;
		addRot_.x += 0.1f;

		//frameRot_.y += addRot_.y;
		//frameRot_.x += addRot_.x;

		if (!isPop_ && !isDisappear_) {
			coolTime_--;
		}

		visualRot_.z += 14.5f;
		visualRot_.x += 14.5f;

		bulletDirRot_.y += 0.5f;
		cononPos_.y = pos_.y - 25;

		break;
	default:
		break;
	}

	if (coolTime_ <= 0) {

		if (state_ == 1) {
			playerPos = player_->GetWorldPos();
			enemyPos = object_->GetPosition();

			differenceVec.x = enemyPos.x - playerPos.x;
			differenceVec.y = enemyPos.y - playerPos.y;
			differenceVec.z = enemyPos.z - playerPos.z;
			differenceVec /= 3;
			differenceVec.normalize();

			object_->SetRotation({ 0,0,0 });
			object_->Update();

			velocity_ = differenceVec;
			velocity_.multiplyMat4(object_->matWorld_);
			velocity_ /= 10;
		}
		else if (state_ == 2) {
			Vector3 start = { GetWorldPos().x,GetWorldPos().y,GetWorldPos().z };
			Vector3 end({ 0,0,0 });
			Vector3 length = { 0, 0, 10 };
			frontVec = { 0, 0, 0 };

			//終点座標を設定
			end.x = start.x + length.x;
			end.y = start.y + length.y;
			end.z = start.z + length.z;

			//回転を考慮した座標を設定
			end.x = start.x + sinf(bulletDirRot_.y/40);
			end.z = start.z + cosf(bulletDirRot_.y/40);

			//始点と終点から正面ベクトルを求める
			frontVec.x = end.x - start.x;
			frontVec.y = end.y - start.y;
			frontVec.z = end.z - start.z;


			frontVec.normalize();


			velocity_ = frontVec;
		}
		else if (state_ == 3) {
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

		if (state_ == 1) {
			std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();

			newBullet->Initialize(pos_, velocity_, bulletModel_, 3);

			//弾を登録する
			bullets_.push_back(std::move(newBullet));

			coolTime_ = 7;
		}
		else if (state_ == 2) {
			
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
		else if (state_ == 3) {
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

	//しっぽの座標
	
	//telePos_.y = 0.5f * cosf(3.14f * frame_ / 100) + telePos_.y;

	teleLen_ = pos_ - telePos_;
	len_ = teleLen_.length();
	teleLen_.normalize();

	if (iaAttack_ && !isAk_) {
		movetime_++;
		movetime_ = min(20, movetime_);
		telePos_ = telePos_.lerp(attackStart_, attackEnd_, Easing::EaseOutQuint(movetime_, 20));
		angle2_ = 120;

		radi_ = std::atan2(telePos_.z - playerPos.z, telePos_.x - playerPos.x);
		angle_ = radi_ * (180 / 3.14f) + 90;
	}
	else if (isAk_) {
		angle_ = radi_ * (180 / 3.14f) + 90;
	}
	else {
		telePos_.y = 0.2f * cosf(3.14f * frame_ / 80) + telePos_.y;
		angle_ = 0;
		//angle2_ = 0;
		movetime_ = 0;
	}

	
	//しっぽ攻撃中
	if (isAk_) {
		
		telePos_ = telePos_.lerp(rokStart_, rokEnd_, Easing::EaseInCubic(atting_, 30));

		if (atting_ == 30) {
			grandtele_ = true;
		}

		//戻る
		if (atting_ >= 60) {
			backAk_ = true;
		}

		if (backAk_) {
			atting_--;
			atting_ = max(0, atting_);

			//終了
			if (atting_ <= 0) {
				atkTime_[0] = 0;
				atkTime_[1] = 0;
				startAtk_ = false;
				isRokAtk_ = false;

				iaAttack_ = false;
				isAk_ = false;
				endAk_ = true;
				endStart_ = object_->GetPosition();
				endStart_.z += 30;
				rotLeapS_.x = angle2_;
				rotLeapE_.x = 0;
				isEffect_ = false;
			}
		}
		else {
			atting_++;
			//atting_ = min(30, atting_);
		}
		
	}

	if (grandtele_) {
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
			grandtele_ = false;
			shackTimer_ = 0;
		}

		if (!isEffect_) {
			Vector3 v = { 0,2.0f,0 };
			for (int i = 0; i < 15; i++) {

				std::unique_ptr<Effect> newObj = std::make_unique<Effect>();
				newObj->Initialize(telePos_, v, model_);
				newObj->SetScale(2.0f);
				effects_.push_back(std::move(newObj));
			}
			isEffect_ = true;
		}
		
	}

	if (endAk_) {
		atting_++;
		telePos_ = telePos_.lerp(rokStart_, endStart_, Easing::EaseInCubic(atting_, 30));
		tealRot_ = tealRot_.lerp(rotLeapS_, rotLeapE_, Easing::EaseOutBack(atting_, 20));
		angle2_ = tealRot_.x;

		if (atting_ >= 30) {
			endAk_ = false;
		}
	}

	playerPos = player_->GetWorldPos();

	object_->SetScale({ vScale_.x,vScale_.y ,vScale_.z });
//	frameObject_->SetScale({ vScale_.x,vScale_.y ,vScale_.z });
	bulletCononObject_->SetScale({ vScale_.x,vScale_.y ,vScale_.z });

	object_->SetPosition(pos_);
	object_->SetRotation(visualRot_);
	frameObject_->SetRotation({ angle2_,-angle_,0});
	frameObject_->SetPosition(telePos_);

	bulletCononObject_->SetPosition(cononPos_);
	bulletCononObject_->Update();

	object_->Update();
	frameObject_->Update();
}

void Boss::Draw()
{
	object_->Draw();
	frameObject_->Draw();

	//bulletCononObject_->Draw();

	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	for (std::unique_ptr<Effect>& effect : effects_) {
		effect->Draw();
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

void Boss::SetBossModels(Model* framemodel, Model* cannonModel)
{
	bulletCononModel_ = cannonModel;
	frameModel_ = framemodel;
}

Vector3 Boss::GetBossteleWorldPos()
{
	Vector3 pos = telePos_;

	return pos;
}


void Boss::Move()
{
	if (atkTime_[0] > 120 && !iaAttack_) {
		startAtk_ = true;
	}
	else {
		atkTime_[0]++;
	}

	if (iaAttack_) {
		atkTime_[1]++;

		if (atkTime_[1] == 60) {
			atting_ = 0;
			isAk_ = false;
			backAk_ = false;
			rokStart_ = telePos_;
			rokEnd_ = playerPos;
		}

		if (atkTime_[1] >= 80) {
			isAk_ = true;
			atkTime_[0] = 0;
		}
	}

	if (startAtk_) {
		iaAttack_ = true;
		attackStart_ = telePos_;
		attackEnd_ = telePos_;
		attackEnd_.y += 50;
		startAtk_ = false;
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

	if (moveTimer_ >= randMoveChange_) {
		if (!isDisappear_) {
			moveT_ = 0;
			isDisappear_ = true;
			disappearStart_.x = object_->GetPosition().x;
			disappearStart_.y = object_->GetPosition().y;
			disappearStart_.z = object_->GetPosition().z;

			disappearEnd_ = disappearStart_;
			//disappearEnd_.y;
		}

		moveTimer_ = 0;
	}



	//瞬間移動始め
	if (isDisappear_) {
		moveT_++;
		moveT_ = min(20, moveT_);

		vPos_ = vPos_.lerp(disappearStart_, disappearEnd_, Easing::EaseOutQuint(moveT_, 20));
		vScale_ = vScale_.lerp({ 15,15,15 }, { 0,0,15 }, Easing::EaseOutQuint(moveT_, 20));

		pos_.x = vPos_.x;
		pos_.y = vPos_.y;
		pos_.z = vPos_.z;
		cononPos_ = pos_;

		if (moveT_ >= 20) {
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

			moveT_ = 0;

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
		moveT_++;
		moveT_ = min(20, moveT_);

		vPos_ = vPos_.lerp(popStart_, popEnd_, Easing::EaseOutQuint(moveT_, 20));
		vScale_ = vScale_.lerp({ 0,0,15 }, { 15,15,15 }, Easing::EaseOutQuint(moveT_, 20));

		pos_.x = vPos_.x;
		pos_.y = vPos_.y;
		pos_.z = vPos_.z;
		cononPos_ = pos_;

		if (moveT_ >= 20) {
			isPop_ = false;
			randState_ = movementPattern_[movementPatternCount_];
			randMoveChange_ = rand() % 300 + 60;
			movementPatternCount_++;
		}
	}

}
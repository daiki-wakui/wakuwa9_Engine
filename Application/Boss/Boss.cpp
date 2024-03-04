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
	INITpos_ = pos;

	model_ = model;
	object_ = Object;
	player_ = player;
	pos_ = pos;
	centerPos_ = pos;

	hp = BOSS_HP;
	arive_ = true;
	nowState_ = 0;
	frame_ = 0;
	moveTimer_ = 0;

	
	leapScale_ = { OBJECT_SCALE,OBJECT_SCALE,OBJECT_SCALE };
	randMoveChangeTime_ = MOVE_CHANGE_TIME;
	bullets_.clear();

	//行動パターン
	movementPattern_[0] = 2;
	movementPattern_[1] = 2;
	movementPattern_[2] = 2;
	movementPattern_[3] = 1;
	movementPattern_[4] = 3;
	movementPattern_[5] = 2;
	movementPattern_[6] = 1;
	movementPattern_[7] = 1;
	movementPatternCount_ = 0;

	//しっぽ情報
	tailObject_.reset();
	tailObject_ = std::make_unique<Object3D>();
	tailObject_->SetModel(tailModel_);
	tailObject_->Initialize();
	tailObject_->SetScale({ TAIL_SCALE,TAIL_SCALE,TAIL_SCALE });
	tailObject_->SetPosition(object_->GetPosition());
	
	tailPos_.x = object_->GetPosition().x;
	tailPos_.y = object_->GetPosition().y;
	tailPos_.z = object_->GetPosition().z + TAIL_POSZ_VOLUE;

	for (int i = 0; i < 10; i++) {
		tailBallObject_[i] = std::make_unique<Object3D>();
		tailBallObject_[i]->SetModel(bulletModel_);
		tailBallObject_[i]->Initialize();
		tailBallObject_[i]->SetScale({ 2,2,2 });
	}

	SadowObject_ = std::make_unique<Object3D>();
	SadowObject2_ = std::make_unique<Object3D>();
	SadowObject_->SetModel(sadowModel_);
	SadowObject2_->SetModel(sadowModel_);

	SadowObject_->SetScale({ 10,10,10 });
	SadowObject2_->SetScale({ 10,10,10 });
	SadowObject2_->Initialize();
	SadowObject_->Initialize();


	handObjectL_ = std::make_unique<Object3D>();
	handObjectR_ = std::make_unique<Object3D>();

	handObjectL_->SetModel(handModel_);
	handObjectR_->SetModel(handModel_);

	handObjectL_->Initialize();
	handObjectR_->Initialize();

	handObjectL_->SetScale({ 5,5,5 });
	handObjectR_->SetScale({ 5,5,5 });
}

//更新処理
void Boss::Update(bool move, bool debug)
{
	//ムービー時の移動
	if (halfMovie_) {
		MoiveFrame_++;
		handT_++;
		handT_ = min(handT_, 60);

		EndhandLPos_ = INITpos_;
		EndhandRPos_ = INITpos_;
		EndhandLPos_.x = INITpos_.x - 30;
		EndhandRPos_.x = INITpos_.x + 30;


		if (MoiveFrame_ < 60) {
			handLPos_ = handLPos_.lerp(INITpos_, EndhandLPos_, Easing::EaseOutCubic(handT_, 60));
			handRPos_ = handLPos_.lerp(INITpos_, EndhandRPos_, Easing::EaseOutCubic(handT_, 60));


		}
		else {

			handT2_++;

			if (handT2_ > 40) {
				if (L_ == 0) {
					movepowL_.x = -10;
					movepowL_.y = 5;
					movepowR_.x = 15;
					movepowR_.y = 4;
				}
				else if(L_ == 1){
					movepowL_.x = 20;
					movepowL_.y = 5;
					movepowR_.x = -7;
					movepowR_.y = 3;
				}
				else {
					movepowL_.x = -12;
					movepowL_.y = -7;
					movepowR_.x = -6;
					movepowR_.y = -3;
				}
				

				handT2_ = 0;

				if (L_ == 0) {
					L_ = 1;
				}
				else if (L_ == 1) {
					L_ = 2;
				}
				else {
					L_ = 0;
				}
			}

			handLPos_.y += movepowL_.y;
			handRPos_.y += movepowR_.y;

			handLPos_.x += movepowL_.x;
			handRPos_.x += movepowR_.x;

			movepowL_ *= 0.8f;
			movepowR_ *= 0.8f;

		}
	}
	else {
		handLPos_ = pos_;
		handLPos_.x = pos_.x - 30;
		handRPos_ = pos_;
		handRPos_.x = pos_.x + 30;
	}

	tailVec_ = pos_ - tailPos_;
	taillen_ = tailVec_.length();
	tailVec_.normalize();

	for (int i = 0; i < 10; i++) {
		tailballpos_ = pos_;
		tailballpos_ += -tailVec_ * (taillen_ / 10 * (i + 1));
		tailBallObject_[i]->SetPosition(tailballpos_);
		tailBallObject_[i]->Update();
	}
	arive_ = true;


	//デバック確認
	if (KeyBoard::GetInstance()->keyInstantPush(DIK_1)) {
		nowState_ = 1;
	}
	if (KeyBoard::GetInstance()->keyInstantPush(DIK_2)) {
		nowState_ = 2;
	}
	if (KeyBoard::GetInstance()->keyInstantPush(DIK_3)) {
		nowState_ = 3;
	}
	if (KeyBoard::GetInstance()->keyInstantPush(DIK_4)) {
		nowState_ = 4;
	}

	playerPos = player_->GetWorldPos();

	//行動パターン繰り返し
	if (movementPatternCount_ >= MAX_MOVEMENT) {
		movementPatternCount_ = 0;
	}

	if (nowState_ == 0) {
		nowState_ = movementPattern_[movementPatternCount_];
		movementPatternCount_++;
	}

	if (nowState_ == 1) {
		//しっぽ
		//映像外なら
		if (!halfMovie_ && akTail_) {
			Tail();
		}
	}
	if (nowState_ == 2) {
		//移動処理
		if (!move || debug) {
			Move();
		}
	}
	if (nowState_ == 3) {
		aidle_++;
		if (aidle_ >= 120) {
			nowState_ = 0;
			aidle_ = 0;
		}
	}

	if (nowState_ == 4) {

	}
	
	frame_++;
	pos_.y = MOVE_Y_VOLUE * cosf(wa9Math::PI() * frame_ / MOVE_SPEED_VOLUE) + pos_.y;

	Stateframe_++;
	


	//しっぽのエフェクトの更新処理
	for (std::unique_ptr<Effect>& effect : effects_) {
		effect->Update();
	}

	//デスフラグが立った弾を削除
	effects_.remove_if([](std::unique_ptr<Effect>& bullet) {
		return bullet->IsDead();
	});

	
	

	//しっぽが地面に刺さった時のエフェクト
	ShackEffect();


	
	if (isShake_) {
		sahkeTimer_++;
		randShake_.x = MyRandom::GetFloatRandom(-3, 3);
		randShake_.y = MyRandom::GetFloatRandom(-0.1f, 0.1f);
		randShake_.z = MyRandom::GetFloatRandom(-3, 3);

		pos_ += randShake_;

		if (sahkeTimer_ > 20) {
			isShake_ = false;
		}
	}
	else {
		randShake_ = { 0,0,0 };
	}

	if (halfMovie_) {
		pos_ = INITpos_;
		visualRot_ = { 0,0,0 };


	}

	//オブジェクト更新処理
	object_->SetScale({ leapScale_.x,leapScale_.y ,leapScale_.z });
	object_->SetPosition(pos_);
	object_->SetRotation(visualRot_);
	tailObject_->SetRotation({ tailAngle_.x,-tailAngle_.y,0});
	tailObject_->SetPosition(tailPos_);
	object_->Update();
	tailObject_->Update();

	handObjectL_->SetPosition(handLPos_);
	handObjectR_->SetPosition(handRPos_);

	handObjectL_->Update();
	handObjectR_->Update();


	SadowObject_->SetPosition({ pos_.x,0,pos_.z });
	SadowObject2_->SetPosition({ tailPos_.x,0,tailPos_.z });
	SadowObject_->Update();
	SadowObject2_->Update();
}

//描画関数
void Boss::Draw()
{
	object_->Draw();

	SadowObject_->Draw();
	SadowObject2_->Draw();

	if (!halfMovie_) {
		tailObject_->Draw();
		for (int i = 0; i < 10; i++) {
			tailBallObject_[i]->Draw();
		}
	}

	/*if (hp < 25) {
		handObjectL_->Draw();
		handObjectR_->Draw();
	}*/
	
	
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

	//倒した時
	if (hp <= 0) {
		arive_ = false;
	}

	isShake_ = true;
}

//弾のモデル
void Boss::SetBulletModel(Model* model)
{
	bulletModel_ = model;
}

//ボスのしっぽモデル
void Boss::SetBossModels(Model* framemodel,Model* handModel, Model* sadowModel)
{
	tailModel_ = framemodel;
	handModel_ = handModel;
	sadowModel_ = sadowModel;
}

void Boss::boolInfo(bool mive)
{
	halfMovie_ = mive;
}

Vector3 Boss::GetBossTailWorldPos()
{
	Vector3 pos = tailPos_;

	return pos;
}

void Boss::SetStateMent(int32_t num, int32_t i)
{
	movementPattern_[i] = num;
}

//移動処理
void Boss::Move()
{
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

	if (!halfMovie_) {
		moveTimer_++;

		if (moveTimer_ > 30) {

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
	}

	//瞬間移動始め
	if (isDisappear_) {
		moveDisappearTime_++;
		moveDisappearTime_ = min(MAX_DISAPPEAR_TIME, moveDisappearTime_);

		vPos_ = vPos_.lerp(disappearStart_, disappearEnd_, Easing::EaseOutQuint(moveDisappearTime_, MAX_DISAPPEAR_TIME));
		leapScale_ = leapScale_.lerp({ OBJECT_SCALE,OBJECT_SCALE,OBJECT_SCALE }, { 0,0,OBJECT_SCALE }, Easing::EaseOutQuint(moveDisappearTime_, MAX_DISAPPEAR_TIME));

		pos_ = vPos_;

		//オブジェクトが消えるタイミング
		if (moveDisappearTime_ >= MAX_DISAPPEAR_TIME) {
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
		moveDisappearTime_ = min(MAX_DISAPPEAR_TIME, moveDisappearTime_);

		vPos_ = vPos_.lerp(popStart_, popEnd_, Easing::EaseOutQuint(moveDisappearTime_, MAX_DISAPPEAR_TIME));
		leapScale_ = leapScale_.lerp({ 0,0,OBJECT_SCALE }, { OBJECT_SCALE,OBJECT_SCALE,OBJECT_SCALE }, Easing::EaseOutQuint(moveDisappearTime_, MAX_DISAPPEAR_TIME));

		pos_.x = vPos_.x;
		pos_.y = vPos_.y;
		pos_.z = vPos_.z;
		
		//瞬間移動終わり
		if (moveDisappearTime_ >= MAX_DISAPPEAR_TIME) {
			isPop_ = false;
			nowState_ = 0;
		}
	}

}

//弾の処理
void Boss::Shot()
{
	
}

//しっぽ処理
void Boss::Tail()
{
	//攻撃開始
	if (tailStateTime_[Wait] > MAX_TAIL_WAIT_TIME && !iaAttacking_) {
		startAttack_ = true;
	}
	else {	//待機
		tailStateTime_[Wait]++;
	}

	//攻撃中
	if (iaAttacking_) {
		tailStateTime_[Attack]++;

		//着弾地点をロック
		if (tailStateTime_[Attack] == MAX_TAIL_ROK_TIME) {
			attackingTime_ = 0;
			isTailAttacking_ = false;
			backTail_ = false;
			rokStart_ = tailPos_;
			rokEnd_ = playerPos;
		}

		//着弾地点めがけ始め
		if (tailStateTime_[Attack] >= MAX_TAIL_ATTACK_TIME) {
			isTailAttacking_ = true;
			tailStateTime_[Wait] = 0;
		}
	}

	//攻撃開始時のフレーム
	if (startAttack_) {
		iaAttacking_ = true;
		attackStart_ = tailPos_;
		attackEnd_ = tailPos_;
		attackEnd_.y += ATTACK_END_Y_VOLUE;
		startAttack_ = false;
	}

	//しっぽの座標
	teleLen_ = pos_ - tailPos_;
	len_ = teleLen_.length();
	teleLen_.normalize();

	if (iaAttacking_ && !isTailAttacking_) {
		movetime_++;
		movetime_ = min(MAX_DISAPPEAR_TIME, movetime_);
		tailPos_ = tailPos_.lerp(attackStart_, attackEnd_, Easing::EaseOutQuint(movetime_, MAX_DISAPPEAR_TIME));
		tailAngle_.x = TAIL_ANGLE_VOLUE;

		radian_ = std::atan2(tailPos_.z - playerPos.z, tailPos_.x - playerPos.x);
		tailAngle_.y = radian_ * (wa9Math::Degree180() / wa9Math::PI()) + wa9Math::Degree90();
	}
	else if (isTailAttacking_) {
		tailAngle_.y = radian_ * (wa9Math::Degree180() / wa9Math::PI()) + wa9Math::Degree90();
	}
	else {
		tailPos_.y = TAIL_POS_VOLUE * cosf(wa9Math::PI() * frame_ / TAIL_POS_FRAME_RATE) + tailPos_.y;
		tailAngle_.y = 0;
		movetime_ = 0;
	}


	//しっぽ攻撃中
	if (isTailAttacking_) {

		tailPos_ = tailPos_.lerp(rokStart_, rokEnd_, Easing::EaseInCubic(attackingTime_, TAIL_ATTACK_MAX_TIME));

		if (attackingTime_ == TAIL_ATTACK_MAX_TIME) {
			grandTail_ = true;
		}

		//戻る
		if (attackingTime_ >= TAIL_ATTACK_END_TIME) {
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
				backTailStart_.z += TAIL_BACK_START_Z;
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
		tailPos_ = tailPos_.lerp(rokStart_, backTailStart_, Easing::EaseInCubic(attackingTime_, TAIL_ATTACK_MAX_TIME));
		tailRot_ = tailRot_.lerp(tailRotLeapStart_, tailRotLeapEnd_, Easing::EaseOutBack(attackingTime_, MAX_DISAPPEAR_TIME));
		tailAngle_.x = tailRot_.x;

		if (attackingTime_ >= TAIL_ATTACK_MAX_TIME) {
			endAttack_ = false;
			nowState_ = 0;
		}
	}
}

void Boss::ShackEffect()
{
	if (grandTail_) {
		Vector3 toEye = Object3D::GetEye();
		Vector3 toTerget = Object3D::GetTarget();
		randShack_.x = MyRandom::GetFloatRandom(-SHACK_VOLUE, SHACK_VOLUE);
		randShack_.y = MyRandom::GetFloatRandom(-SHACK_VOLUE, SHACK_VOLUE);

		toEye.x += randShack_.x;
		toEye.y += randShack_.y;
		toTerget.x += randShack_.x;
		toTerget.y += randShack_.y;

		Object3D::SetEye(toEye);
		Object3D::SetTarget(toTerget);

		shackTimer_++;

		if (shackTimer_ > SHACK_TIMER) {
			grandTail_ = false;
			shackTimer_ = 0;
		}

		if (!isEffect_) {
			Vector3 v = { 0,EFFECT_Y,0 };
			for (int i = 0; i < EFFECT_NUM; i++) {

				std::unique_ptr<Effect> newObj = std::make_unique<Effect>();
				newObj->Initialize(tailPos_, v, model_);
				newObj->SetScale(EFFECT_SCALE);
				effects_.push_back(std::move(newObj));
			}
			isEffect_ = true;
		}

	}
}
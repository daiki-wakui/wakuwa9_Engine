#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

#include <cmath>
#include <algorithm>
#include <random>

#include "MyRandom.h"
#include "wa9Math.h"
#include "Easing.h"

//初期化
void Player::Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input, GamePad* inputPad,Object3D* podObject)
{
	playerModel_ = playerModel;
	playerObject_ = playerObject;
	podObject_ = podObject;
	input_ = input;
	inputPad_ = inputPad;

	bullets_.clear();

	moveParticle_->Initialize(L"Resources/01.png");
	moveParticle_->Update();

	reticle3DObject_->Initialize();
	reticle3DObject_->SetModel(bulletModel_);
}

//弾リストをクリア
void Player::clear() {
	bullets_.clear();
}

//更新処理
void Player::Update()
{
	moveParticle_->Update();

	rot_ = playerObject_->GetRotation();
	pos_ = playerObject_->GetPosition();
	eye_ = playerObject_->GetEye();
	target_ = playerObject_->GetTarget();


	frontVec.x = eye_.x - target_.x;
	frontVec.z = eye_.z - target_.z;

	frontVec.normalize();
	frontVec /= FRONT_VECTOR_RATE;

	if (isDash_) {
		dashPower_ += DASH_POWTER_VOLUE;

		dashPower_ = min(dashPower_, DASH_POWTER_MAX_VOLUE);

		frontVec.x *= dashPower_;
		frontVec.z *= dashPower_;
	}

	/*if (inputPad_->PushInstantA()) {
		isStep_ = true;
		stepPower_ = 8.0f;
	}*/

	

	if (isStep_) {
		pos_.y++;
		frontVec.x *= stepPower_;
		frontVec.z *= stepPower_;
		stepPower_--;

		if (stepPower_ <= 0) {
			isStep_ = false;
			isJustStep_ = false;
		}
	}

	if (inputPad_->InputLStickUp()||input_->keyPush(DIK_UP)) {
		Vector3 tmppos = pos_;

		Vector3 vel{};
		vel.x = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);
		vel.y = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);

		moveParticle_->Add(PARTICLE_TIME, tmppos, vel, PARTICLE_ACCEL, PARTICLE_START_SCALE, PARTICLE_END_SCALE);

		frontMove_.x = -frontVec.x;
		frontMove_.z = -frontVec.z;
		rot_.x++;
		rot_.x = min(rot_.x, ROT_MAX);
	}
	else {
		if (!inputPad_->InputLStick() && rot_.x > 0) {
			rot_.x--;
			rot_.x = max(rot_.x, 0);
		}
		
	}

	if (inputPad_->InputLStickDown()||input_->keyPush(DIK_DOWN)) {
		Vector3 tmppos = pos_;

		Vector3 vel{};
		vel.x = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);
		vel.y = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);

		moveParticle_->Add(PARTICLE_TIME, tmppos, vel, PARTICLE_ACCEL, PARTICLE_START_SCALE, PARTICLE_END_SCALE);

		frontMove_.x = frontVec.x;
		frontMove_.z = frontVec.z;
		rot_.x--;
		rot_.x = max(rot_.x, -ROT_MAX);
	}
	else {
		if (!inputPad_->InputLStick() && rot_.x < 0) {
			rot_.x++;
			rot_.x = min(rot_.x, 0);
		}
		
		frontMove_ *= friction;
	}

	moveXVec = tmpVecY.cross(frontVec);

	if (inputPad_->InputLStickRight()||input_->keyPush(DIK_RIGHT)) {

		Vector3 tmppos = pos_;

		Vector3 vel{};
		vel.x = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);
		vel.y = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);

		moveParticle_->Add(PARTICLE_TIME, tmppos, vel, PARTICLE_ACCEL, PARTICLE_START_SCALE, PARTICLE_END_SCALE);

		sideMove_.x = -moveXVec.x;
		sideMove_.z = -moveXVec.z;
		rot_.z--;
		rot_.z = max(rot_.z, -ROT_MAX);
	}
	else {
		sideMove_ *= friction;
		if (!inputPad_->InputLStick() && rot_.z < 0) {
			rot_.z++;
			rot_.z = min(rot_.z, 0);
		}
	}

	if (inputPad_->InputLStickLeft()||input_->keyPush(DIK_LEFT)) {

		Vector3 tmppos = pos_;

		Vector3 vel{};
		vel.x = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);
		vel.y = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);

		moveParticle_->Add(PARTICLE_TIME, tmppos, vel, PARTICLE_ACCEL, PARTICLE_START_SCALE, PARTICLE_END_SCALE);

		sideMove_.x = moveXVec.x;
		sideMove_.z = moveXVec.z;
		rot_.z++;
		rot_.z = min(rot_.z, ROT_MAX);
	}
	else {
		if (!inputPad_->InputLStick() && rot_.z > 0) {
			rot_.z--;
			rot_.z = max(rot_.z, 0);
		}
		sideMove_ *= friction;
	}
	
	if (!inputPad_->InputLStick()) {
		isDash_ = false;
		dashPower_ = 1.0f;
	}


	if (inputPad_->RTrigger()) {
		isDash_ = true;
	}

	pos_.x += frontMove_.x;
	pos_.y += frontMove_.y;
	pos_.z += frontMove_.z;

	pos_.x += sideMove_.x;
	pos_.y += sideMove_.y;
	pos_.z += sideMove_.z;
	
	pos_.y = sinf(wa9Math::PI() * (frame + ADD_FRAME) * FRAME_RATE) * POS_Y_RATE + ADD_POS_Y_VOLUE;

	frame++;

	if (!stepPower_) {
		if (inputPad_->InputRStick() == false && inputPad_->InputLStick() == false) {
			RotateAngle();
		}
		else if (inputPad_->InputLStick() && inputPad_->InputRStick()) {
			RotateAngle();

			rot_.y = -angle_;
		}
		else if (inputPad_->InputLStick() && inputPad_->InputRStick() == false) {
			RotateAngle();

			rot_.y = -angle_;
		}
	}

	

	
	if (inputPad_->PushInstantLB()) {
		isBossRokon_++;
		isBossRokon_ = isBossRokon_ % 2;
	}

	if (isBossRokon_) {
		tergetCamera();
	}
	else {
		PlayerCamera();

	}

	posPod_ = pos_;

	posPod_.x += sideMove_.x + ADD_BIT_X_VOLUE;

	posPod_.y = sinf(wa9Math::PI() * frame * BIT_FRAME_RATE) * BIT_POS_Y_RATE + ADD_BIT_POS_Y_VOLUE;
	posPod_.y += 7;

	bulletVec_.x = eye_.x - pos_.x;
	bulletVec_.z = eye_.z - posPod_.z;
	bulletVec_.y = (eye_.y - BULLET_VECTOR_Y_VOLUE) - posPod_.y;

	bulletVec_.normalize();

	podRot_.y = cameraAngle_;


	if (nokBack_) {
		nokPower_ *= 0.9f;

		pos_ += nokPower_;
		noktimer_++;

		if (noktimer_ > 60) {
			nokBack_ = false;
		}

		rot_.x -= nokPower_.x;
	}
	else {
		noktimer_ = 0;
	}

	playerObject_->SetRotation(rot_);
	playerObject_->SetPosition(pos_);
	podObject_->SetRotation(podRot_);
	podObject_->SetPosition(posPod_);

	playerObject_->Update();
	podObject_->Update();

	bulletRTPos_ = posPod_;
	bulletRTVec_ = bulletVec_;

	bulletRTVec_ *= BULLET_RT_VECTOR_RATE;

	bulletRTPos_ += bulletRTVec_;


	reticle3DObject_->SetPosition(bulletRTPos_);
	reticle3DObject_->Update();


	if (inputPad_->PushButtonRB()) {
		coolTime--;
	}

	Shot();

	invincibleFrame_--;
	invincibleFrame_ = max(invincibleFrame_, 0);

	if (invincibleFrame_ == 0) {
		isInvincible_ = false;
	}

	
}

//描画関数
void Player::Draw()
{
	playerObject_->Draw();
	podObject_->Draw();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}

}

//パーティクル描画
void Player::ParticleDraw()
{
	moveParticle_->Draw();
}

//当たったときの処理
bool Player::OnCollision()
{
	nokBack_ = true;
	nokPower_ = frontVec * 5;

	if (!isInvincible_) {
		HP--;

		isHit_ = true;
		isInvincible_ = true;
		invincibleFrame_ = INVINCIBLE_FRAME_VLOUE;
	}

	if (HP <= 0) {
		isDead = true;
	}

	return true;
}

//ミサイル攻撃
void Player::Missle()
{
	Vector3 target;
	target.x = targetEnemy_->GetWorldPos().x;
	target.y = targetEnemy_->GetWorldPos().y;
	target.z = targetEnemy_->GetWorldPos().z;

	//弾の生成と初期化
	for (int i = 0; i < MISSLE_NUM; i++) {
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(posPod_, target, bulletModel_);
		newBullet->SetMissile(true);
		bullets_.push_back(std::move(newBullet));
	}
}

//ロックオンカメラ
void Player::tergetCamera()
{
	Vector3 terget;
	Vector3 eye;
	Vector3 camerapos;
	terget = targetBoss_->GetWorldPos();
	camerapos = pos_ - targetBoss_->GetWorldPos();
	camerapos.y = 0.0f;

	camerapos.normalize();
	camerapos *= ROK_CAMERA_POS_RATE;
	camerapos.y = ROK_CAMERA_POS_Y_VOLUE;

	eye = pos_ + camerapos;

	ParticleManager::SetEye(eye);
	ParticleManager::SetTarget(terget);
	playerObject_->SetEye(eye);
	playerObject_->SetTarget(terget);

	cRadian_ = std::atan2(pos_.z - targetBoss_->GetWorldPos().z, pos_.x - targetBoss_->GetWorldPos().x);
	cAngle_ = cRadian_ * (wa9Math::Degree180() / (float)PI) - wa9Math::Degree90();
	cameraAngle_ = -cAngle_;
}

//弾攻撃
void Player::Shot(){

	if (coolTime < 0) {
		isShot_ = true;

		//弾の速度
		const float kBulletSpeed = SHOT_SPEED_VOLUE;

		bulletVec_ *= -kBulletSpeed;

		//弾の生成と初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(posPod_, bulletVec_, bulletModel_);
		bullets_.push_back(std::move(newBullet));

		coolTime = SHOT_COOLTIME_VOLUE;
	}

	//弾の更新処理
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//デスフラグが立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
	});
}

//操作カメラ
void Player::PlayerCamera(){

	Vector3 toCameraPosXZ;
	toCameraPosXZ.x = eye_.x - target_.x;
	toCameraPosXZ.y = eye_.y - target_.y;
	toCameraPosXZ.z = eye_.z - target_.z;


	float height = toCameraPosXZ.y;
	toCameraPosXZ.y = 0.0f;
	float CameraXZLen = MOVE_CAMERA_LENGTH_VOLUE;
	toCameraPosXZ.normalize();

	Vector3 terget = pos_;
	terget.y += MOVE_CAMERA_TARGET_Y_VOLUE;

	XMFLOAT3 toNewCameraPos;
	toNewCameraPos.x = eye_.x - terget.x;
	toNewCameraPos.y = eye_.y - terget.y;
	toNewCameraPos.z = eye_.z - terget.z;

	toNewCameraPos.y = 0.0f;

	Vector3 toNewCameraPosv;

	toNewCameraPosv.x = toNewCameraPos.x;
	toNewCameraPosv.y = toNewCameraPos.y;
	toNewCameraPosv.z = toNewCameraPos.z;

	toNewCameraPosv.normalize();

	if (inputPad_->InputLStick()) {
		weghtV_ += 0.01f;
		weghtV_ = min(weghtV_, 0.7f);
	}
	else {
		weghtV_ = 0.0f;
	}
	
	if (inputPad_->PushButtonRB()) {
		weghtV_ = 0;
	}

	float weight = weghtV_;

	toNewCameraPosv = toNewCameraPosv * weight + toCameraPosXZ * (MOVE_CAMERA_ROT_MAX_WEIGHT_VOLUE - weight);
	toNewCameraPosv.normalize();
	toNewCameraPosv *= CameraXZLen;
	toNewCameraPosv.y = height;

	Vector3 newEye;
	newEye.x = terget.x + toNewCameraPosv.x;
	newEye.y = terget.y + toNewCameraPosv.y;
	newEye.z = terget.z + toNewCameraPosv.z;

	eye_ = newEye;
	target_ = terget;
	Leye_ = eye_;

	cameraTargetAngle_ = playerObject_->GetEye().y;

	if (inputPad_->InputRStickRight()) {
		isStcikRight_ = true;

		if (cameraAnglePower_ < 0) {
			cameraAnglePower_ = 0;
		}

		if (inputPad_->PushButtonRB()) {
			cameraAnglePower_ = 0.95f;
		}
		else {
			cameraAnglePower_ += 0.15f;
			cameraAnglePower_ = min(cameraAnglePower_, 1.75f);
		}


		cameraAngle_ += cameraAnglePower_;
	}
	else if (inputPad_->InputRStickLeft()) {
		isStcikRight_ = false;

		if (cameraAnglePower_ > 0) {
			cameraAnglePower_ = 0;
		}

		if (inputPad_->PushButtonRB()) {
			cameraAnglePower_ = -0.95f;
		}
		else {
			cameraAnglePower_ -= 0.15f;
			cameraAnglePower_ = max(cameraAnglePower_, -1.75f);

		}


		cameraAngle_ += cameraAnglePower_;
	}
	else {
		cameraAnglePower_ = 0;
		/*	if (isStcikRight_) {
				cameraAnglePower_ -= 0.09f;
				cameraAnglePower_ = max(cameraAnglePower_, 0);
				cameraAngle_ += cameraAnglePower_;
			}
			else {
				cameraAnglePower_ += 0.09f;
				cameraAnglePower_ = min(cameraAnglePower_, 0);
				cameraAngle_ += cameraAnglePower_;
			}*/

	}

	if (inputPad_->InputRStickUp()) {
		cameraTargetAngle_ -= MOVE_CAMERA_TARGET_ANGLE_VOLUE;
	}
	else if (inputPad_->InputRStickDown()) {
		cameraTargetAngle_ += MOVE_CAMERA_TARGET_ANGLE_VOLUE;
	}

	if (inputPad_->InputRStick()) {

		SticTimer_++;
		SticTimer_ = min(SticTimer_, 20);
		cameraTargetAngle_ = max(cameraTargetAngle_, MOVE_CAMERA_TARGET_MIN_ANGLE);
		cameraTargetAngle_ = min(cameraTargetAngle_, MOVE_CAMERA_TARGET_MAX_ANGLE);


		r_ = cameraAngle_ * wa9Math::PI() / wa9Math::Degree180();
		eye_.x = pos_.x + (sinf(r_) * CameraXZLen);
		eye_.z = pos_.z + (cosf(r_) * CameraXZLen);

		Reye_ = eye_;

		eye_ = eye_.lerp(Leye_, Reye_, Easing::EaseIn(SticTimer_, 20));
	}
	else {
		SticTimer_ = 0;
	}

	eye_.y = cameraTargetAngle_;

	playerObject_->SetEye(eye_);
	playerObject_->SetTarget(target_);
	ParticleManager::SetEye(eye_);
	ParticleManager::SetTarget(target_);

}

//角度算出
void Player::RotateAngle()
{
	radi_ = std::atan2(pos_.z - eye_.z, pos_.x - eye_.x);
	angle_ = radi_ * (wa9Math::Degree180() / wa9Math::PI()) - wa9Math::Degree90();
}

void Player::SetBulletModel(Model* model, Object3D* obj)
{
	bulletModel_ = model;
	bulletObject_ = obj;
}

void Player::SetEnemy(Boss* boss)
{
	targetBoss_ = boss;
}

Vector3 Player::GetWorldPos()
{
	Vector3 worldPos;

	worldPos.x = pos_.x;
	worldPos.y = pos_.y;
	worldPos.z = pos_.z;

	return worldPos;
}

Vector3 Player::GetScreenRTPos()
{
	Vector3 screenPos;

	screenPos = reticle3DObject_->Screen();

	return screenPos;
}

Vector3 Player::GetRot()
{
	return playerObject_->GetRotation();
}

Vector3 Player::GetFrontVec()
{
	return frontVec;
}

void Player::SetPos(Vector3 pos)
{
	playerObject_->SetPosition(pos);
}
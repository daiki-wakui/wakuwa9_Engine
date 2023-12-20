#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

#include <cmath>
#include <algorithm>
#include <random>

#include "MyRandom.h"

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

void Player::clear() {
	bullets_.clear();
}

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
	frontVec /= 1.5f;

	if (dash) {
		dashPower_ += 0.2f;

		dashPower_ = min(dashPower_, 1.5f);

		frontVec.x *= dashPower_;
		frontVec.z *= dashPower_;
	}

	if (inputPad_->InputLStickUp()||input_->keyPush(DIK_UP)) {
		Vector3 tmppos = pos_;

		Vector3 vel{};
		vel.x = MyRandom::GetFloatRandom(-0.2f, 0.2f);
		vel.y = MyRandom::GetFloatRandom(-0.2f, 0.2f);

		moveParticle_->Add(60, tmppos, vel, { 0,0,0 }, 2.0f, 0.0f);

		frontMove_.x = -frontVec.x;
		frontMove_.z = -frontVec.z;
		rot_.x++;
		rot_.x = min(rot_.x, 5);
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
		vel.x = MyRandom::GetFloatRandom(-0.2f, 0.2f);
		vel.y = MyRandom::GetFloatRandom(-0.2f, 0.2f);

		moveParticle_->Add(60, tmppos, vel, { 0,0,0 }, 2.0f, 0.0f);

		frontMove_.x = frontVec.x;
		frontMove_.z = frontVec.z;
		rot_.x--;
		rot_.x = max(rot_.x, -5);
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
		vel.x = MyRandom::GetFloatRandom(-0.2f, 0.2f);
		vel.y = MyRandom::GetFloatRandom(-0.2f, 0.2f);

		moveParticle_->Add(60, tmppos, vel, { 0,0,0 }, 2.0f, 0.0f);

		sideMove_.x = -moveXVec.x;
		sideMove_.z = -moveXVec.z;
		rot_.z--;
		rot_.z = max(rot_.z, -5);
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
		vel.x = MyRandom::GetFloatRandom(-0.2f, 0.2f);
		vel.y = MyRandom::GetFloatRandom(-0.2f, 0.2f);

		moveParticle_->Add(60, tmppos, vel, { 0,0,0 }, 2.0f, 0.0f);

		sideMove_.x = moveXVec.x;
		sideMove_.z = moveXVec.z;
		rot_.z++;
		rot_.z = min(rot_.z, 5);
	}
	else {
		if (!inputPad_->InputLStick() && rot_.z > 0) {
			rot_.z--;
			rot_.z = max(rot_.z, 0);
		}
		sideMove_ *= friction;
	}
	
	if (!inputPad_->InputLStick()) {
		dash = false;
		dashPower_ = 1.0f;
	}


	if (inputPad_->RTrigger()) {
		dash = true;
	}

	if (wallHit_) {
		frontMove_ = -frontMove_;
		sideMove_ = -sideMove_;
	}

	pos_.x += frontMove_.x;
	pos_.y += frontMove_.y;
	pos_.z += frontMove_.z;

	pos_.x += sideMove_.x;
	pos_.y += sideMove_.y;
	pos_.z += sideMove_.z;
	
	pos_.y = sinf(3.14f * (frame + 20) * 40) * 0.5f + 10;

	frame++;

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

	posPod_.x += sideMove_.x + 5;

	posPod_.y = sinf(3.14f * frame * 40) * 0.5f + 10;
	posPod_.y += 7;

	bulletVec_.x = eye_.x - pos_.x;
	bulletVec_.z = eye_.z - posPod_.z;
	bulletVec_.y = (eye_.y - 5) - posPod_.y;

	bulletVec_.normalize();

	podRot_.y = cameraAngle_;


	playerObject_->SetRotation(rot_);
	playerObject_->SetPosition(pos_);
	podObject_->SetRotation(podRot_);
	podObject_->SetPosition(posPod_);

	playerObject_->Update();
	podObject_->Update();

	bulletRTPos_ = posPod_;
	bulletRTVec_ = bulletVec_;

	bulletRTVec_ *= -150;

	bulletRTPos_ += bulletRTVec_;


	reticle3DObject_->SetPosition(bulletRTPos_);
	reticle3DObject_->Update();


	if (inputPad_->PushButtonRB()) {
		coolTime--;
	}

	Shot();

	wallHit_ = false;

	invincibleFrame_--;
	invincibleFrame_ = max(invincibleFrame_, 0);

	if (invincibleFrame_ == 0) {
		isInvincible_ = false;
	}

	
}

void Player::Draw()
{
	playerObject_->Draw();
	podObject_->Draw();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}

}

void Player::pDraw()
{
	moveParticle_->Draw();
}

void Player::SetBulletModel(Model* model,Object3D* obj)
{
	bulletModel_ = model;
	bulletObject_ = obj;
}

bool Player::OnCollision()
{
	if (!isInvincible_) {
		HP--;

		isHit_ = true;
		isInvincible_ = true;
		invincibleFrame_ = 200;
	}

	if (HP <= 0) {
		isDead = true;
	}

	return true;
}

void Player::wallHit()
{
	wallHit_ = true;
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

void Player::SetPos(Vector3 pos)
{
	playerObject_->SetPosition(pos);
}

void Player::Missle()
{
	Vector3 target;
	target.x = targetEnemy_->GetWorldPos().x;
	target.y = targetEnemy_->GetWorldPos().y;
	target.z = targetEnemy_->GetWorldPos().z;

	//弾の生成と初期化
	for (int i = 0; i < 4; i++) {
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(posPod_, target, bulletModel_);
		newBullet->SetMissile(true);
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::tergetCamera()
{
	Vector3 terget;
	Vector3 eye;
	Vector3 camerapos;
	terget = targetBoss_->GetWorldPos();
	camerapos = pos_ - targetBoss_->GetWorldPos();
	camerapos.y = 0.0f;

	camerapos.normalize();
	camerapos *= 30;
	camerapos.y = 10.0f;

	eye = pos_ + camerapos;

	ParticleManager::SetEye(eye);
	ParticleManager::SetTarget(terget);
	playerObject_->SetEye(eye);
	playerObject_->SetTarget(terget);

	cRadian_ = std::atan2(pos_.z - targetBoss_->GetWorldPos().z, pos_.x - targetBoss_->GetWorldPos().x);
	cAngle_ = cRadian_ * (180 / (float)PI) - 90;
	cameraAngle_ = -cAngle_;
}

void Player::Shot(){

	if (coolTime < 0) {
		isShot_ = true;

		//弾の速度
		const float kBulletSpeed = 15.0f;

		bulletVec_ *= -kBulletSpeed;


		//弾の生成と初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(posPod_, bulletVec_, bulletModel_);
		bullets_.push_back(std::move(newBullet));

		coolTime = 4;
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

void Player::PlayerCamera(){

	Vector3 toCameraPosXZ;
	toCameraPosXZ.x = eye_.x - target_.x;
	toCameraPosXZ.y = eye_.y - target_.y;
	toCameraPosXZ.z = eye_.z - target_.z;


	float height = toCameraPosXZ.y;
	toCameraPosXZ.y = 0.0f;
	float CameraXZLen = 30;
	toCameraPosXZ.normalize();

	Vector3 terget = pos_;
	terget.y += 10.0f;

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

	float weight = 0.3f;

	toNewCameraPosv = toNewCameraPosv * weight + toCameraPosXZ * (1.0f - weight);
	toNewCameraPosv.normalize();
	toNewCameraPosv *= CameraXZLen;
	toNewCameraPosv.y = height;

	Vector3 newEye;
	newEye.x = terget.x + toNewCameraPosv.x;
	newEye.y = terget.y + toNewCameraPosv.y;
	newEye.z = terget.z + toNewCameraPosv.z;

	eye_ = newEye;
	target_ = terget;

	cameraTargetAngle_ = playerObject_->GetEye().y;

	if (inputPad_->InputRStickRight()) {
		cameraAngle_ += 1.5f;
	}
	else if (inputPad_->InputRStickLeft()) {
		cameraAngle_ -= 1.5f;
	}

	if (inputPad_->InputRStickUp()) {
		cameraTargetAngle_ -= 0.25f;
	}
	else if (inputPad_->InputRStickDown()) {
		cameraTargetAngle_ += 0.25f;
	}

	if (inputPad_->InputRStick()) {
		r_ = cameraAngle_ * 3.14f / 180.0f;
		cameraTargetAngle_ = max(cameraTargetAngle_, 0.5f);
		cameraTargetAngle_ = min(cameraTargetAngle_, 30.0f);

		eye_.x = pos_.x + (sinf(r_) * CameraXZLen);
		eye_.z = pos_.z + (cosf(r_) * CameraXZLen);
	}

	eye_.y = cameraTargetAngle_;

	playerObject_->SetEye(eye_);
	playerObject_->SetTarget(target_);
	ParticleManager::SetEye(eye_);
	ParticleManager::SetTarget(target_);

}

void Player::RotateAngle()
{
	radi_ = std::atan2(pos_.z - eye_.z, pos_.x - eye_.x);
	angle_ = radi_ * (180 / (float)PI) - 90;
}

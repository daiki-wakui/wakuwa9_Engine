#include "Player.h"

#include <cmath>
#include <algorithm>

void Player::Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input, GamePad* inputPad,Object3D* podObject)
{
	playerModel_ = playerModel;
	playerObject_ = playerObject;
	podObject_ = podObject;
	input_ = input;
	inputPad_ = inputPad;

	bullets_.clear();
}

void Player::clear() {
	bullets_.clear();
}

void Player::Update()
{

	rot_ = playerObject_->GetRotation();
	pos_ = playerObject_->GetPosition();
	eye_ = playerObject_->GetEye();
	target_ = playerObject_->GetTarget();


	frontVec.x = eye_.x - target_.x;
	frontVec.z = eye_.z - target_.z;

	frontVec.normalize();
	frontVec /= 1.5f;

	bulletVec_ = frontVec;

	if (dash) {
		dashPower_ += 0.2f;

		dashPower_ = min(dashPower_, 1.5f);

		frontVec.x *= dashPower_;
		frontVec.z *= dashPower_;
	}

	if (inputPad_->InputLStickUp()||input_->keyPush(DIK_UP)) {
		frontMove_.x = -frontVec.x;
		frontMove_.z = -frontVec.z;
	}
	else if (inputPad_->InputLStickDown()||input_->keyPush(DIK_DOWN)) {
		frontMove_.x = frontVec.x;
		frontMove_.z = frontVec.z;
	}
	else {
		frontMove_ *= friction;
	}

	moveXVec = tmpVecY.cross(frontVec);

	if (inputPad_->InputLStickRight()||input_->keyPush(DIK_RIGHT)) {
		sideMove_.x = -moveXVec.x;
		sideMove_.z = -moveXVec.z;
	}
	else if (inputPad_->InputLStickLeft()||input_->keyPush(DIK_LEFT)) {
		sideMove_.x = moveXVec.x;
		sideMove_.z = moveXVec.z;
	}
	else {
		sideMove_ *= friction;
	}
	
	if (!inputPad_->InputLStick()) {
		dash = false;
		dashPower_ = 1.0f;
	}


	if (inputPad_->RTrigger()) {
		dash = true;
	}


	pos_.x += frontMove_.x;
	pos_.y += frontMove_.y;
	pos_.z += frontMove_.z;

	pos_.x += sideMove_.x;
	pos_.y += sideMove_.y;
	pos_.z += sideMove_.z;

	frame++;
	/*

	if (input_->keyInstantPush(DIK_SPACE)) {
		isJump = true;
		jumpPower = 5.0f;
	}

	if (isJump == true) {
		
		jumpPower = 7.0f - 9.8f * frame / 50;
		pos_.y += jumpPower;
	}*/

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


	/*if (inputPad_->InputLStickLeft() && inputPad_->InputRStick() == false) {
		RotateAngle();

		addAngle += 0.1f;
		angle_ += addAngle;

		rot_.y = -angle_;
	}
	if (inputPad_->InputLStickRight() && inputPad_->InputRStick() == false) {
		RotateAngle();

		addAngle -= 0.1f;
		angle_ += addAngle;

		rot_.y = -angle_;
	}*/

	PlayerCamera();

	posPod_ = pos_;

	posPod_.x += 5;

	posPod_.y = sinf(3.14f * frame * 40)*0.5f + 18;


	playerObject_->SetRotation(rot_);
	playerObject_->SetPosition(pos_);
	podObject_->SetPosition(posPod_);

	playerObject_->Update();
	podObject_->Update();


	if (inputPad_->PushButtonRB()) {
		coolTime--;
	}
	Shot();
}

void Player::Draw()
{
	playerObject_->Draw();
	podObject_->Draw();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

void Player::SetBulletModel(Model* model,Object3D* obj)
{
	bulletModel_ = model;
	bulletObject_ = obj;
}

bool Player::OnCollision()
{
	isHit_ = true;

	HP--;

	if (HP <= 0) {
		isDead = true;
	}

	return true;
}

DirectX::XMFLOAT3 Player::GetWorldPos()
{
	DirectX::XMFLOAT3 worldPos;

	worldPos.x = pos_.x;
	worldPos.y = pos_.y;
	worldPos.z = pos_.z;

	return worldPos;
}

void Player::Shot(){

	if (coolTime < 0) {
		isShot_ = true;

		//弾の速度
		const float kBulletSpeed = 15.0f;

		bulletVec_ *= -kBulletSpeed;


		//弾の生成と初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(posPod_, bulletVec_, bulletModel_, bulletObject_);
		bullets_.push_back(std::move(newBullet));

		coolTime = 5;
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
	float toCameraXZLen = toCameraPosXZ.length();
	toCameraPosXZ.normalize();

	XMFLOAT3 terget = pos_;
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
	toNewCameraPosv *= toCameraXZLen;
	toNewCameraPosv.y = height;

	XMFLOAT3 newEye;
	newEye.x = terget.x + toNewCameraPosv.x;
	newEye.y = terget.y + toNewCameraPosv.y;
	newEye.z = terget.z + toNewCameraPosv.z;

	eye_ = newEye;
	target_ = terget;

	if (inputPad_->InputRStickRight()) {
		cameraAngle_++;
	}
	else if (inputPad_->InputRStickLeft()) {
		cameraAngle_--;
	}

	if (inputPad_->InputRStick()) {
		r_ = cameraAngle_ * 3.14f / 180.0f;

		eye_.x = pos_.x + (sinf(r_) * toCameraXZLen);
		eye_.z = pos_.z + (cosf(r_) * toCameraXZLen);
	}

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

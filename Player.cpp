#include "Player.h"

#include <cmath>


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
	const float MoveLimitX = 1000.0f;
	const float MoveLimitZ = 1000.0f;

	rot_ = playerObject_->GetRotation();
	pos_ = playerObject_->GetPosition();
	eye_ = playerObject_->GetEye();
	target_ = playerObject_->GetTarget();


	frontVec.x = eye_.x - target_.x;
	frontVec.z = eye_.z - target_.z;

	frontVec.normalize();
	frontVec /= 1.5f;

	if (inputPad_->InputLStickUp()) {
		pos_.x -= frontVec.x;
		pos_.z -= frontVec.z;
	}
	else if (inputPad_->InputLStickDown()) {
		pos_.x += frontVec.x;
		pos_.z += frontVec.z;
	}

	moveXVec = tmpVecY.cross(frontVec);

	if (inputPad_->InputLStickRight()) {
		pos_.x -= moveXVec.x;
		pos_.z -= moveXVec.z;
	}
	else if (inputPad_->InputLStickLeft()) {
		pos_.x += moveXVec.x;
		pos_.z += moveXVec.z;
	}

	radi_ = std::atan2(pos_.z - eye_.z, pos_.x - eye_.x);
	angle_ = radi_ * (180 / (float)PI)-90;

	rot_.y = -angle_;

	PlayerCamera();

	posPod_ = pos_;

	posPod_.x += 5;
	posPod_.y += 7;

	playerObject_->SetRotation(rot_);
	playerObject_->SetPosition(pos_);
	podObject_->SetPosition(posPod_);

	playerObject_->Update();
	podObject_->Update();

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

void Player::OnCollision()
{
	HP--;

	if (HP <= 0) {
		isDead = true;
	}
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

	coolTime--;

	if (coolTime < 0) {
		//弾の速度
		const float kBulletSpeed = 2.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity.multiplyMat4(playerObject_->matWorld_);
		velocity.normalize();
		velocity *= kBulletSpeed;

		//弾の生成と初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(posPod_, velocity, bulletModel_, bulletObject_);
		bullets_.push_back(std::move(newBullet));

		coolTime = 8;
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

	bool Rstick = false;

	if (inputPad_->InputRStickRight()) {
		cameraAngle_ += 2;
		Rstick = true;
	}
	else if (inputPad_->InputRStickLeft()) {
		cameraAngle_ -= 2;
		Rstick = true;
	}

	if (Rstick == true) {
		r_ = cameraAngle_ * 3.14f / 180.0f;

		eye_.x = pos_.x + (sinf(r_) * toCameraXZLen);
		eye_.z = pos_.z + (cosf(r_) * toCameraXZLen);

		Rstick = false;
	}

	playerObject_->SetEye(eye_);
	playerObject_->SetTarget(target_);
}

#include "Player.h"

#include <cmath>


void Player::Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input, GamePad* inputPad,Object3D* podObject)
{
	playerModel_ = playerModel;
	playerObject_ = playerObject;
	podObject_ = podObject;
	input_ = input;
	inputPad_ = inputPad;

	//hpModel_ = Model::LoadFromObj("Cube");

	//for (int i = 0; i < 5; i++) {
	//	hpObj_[i] = Object3D::Create(hpModel_, { (1.0f),(1.0f),(1.0f) });
	//}

	bullets_.clear();
}

void Player::clear() {
	bullets_.clear();
}

void Player::Update()
{
	const float MoveLimitX = 1000.0f;
	const float MoveLimitZ = 1000.0f;

	Vector3 CameraVec = { 0,0,0 };
	Vector3 CameraRot = { 0,0,0 };
	Vector3 tmpVecY = { 0,1,0 };

	rot_ = playerObject_->GetRotation();
	pos_ = playerObject_->GetPosition();
	eye_ = playerObject_->GetEye();
	target_ = playerObject_->GetTarget();

	CameraVec.x = eye_.x - pos_.x;
	CameraVec.z = eye_.z - pos_.z;
	CameraVec.normalize();
	CameraRot = tmpVecY.cross(CameraVec);
	

	angle_ = inputPad_->GetLStickAngle(angle_);

	rot_.y = angle_;

	if (inputPad_->InputLStick()) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity.multiplyMat4(playerObject_->matWorld_);
		velocity.normalize();
		velocity *= kBulletSpeed;

		pos_.x += velocity.x;
		pos_.y += velocity.y;
		pos_.z += velocity.z;
	}
	


	//if (input_->keyPush(DIK_RIGHT)||inputPad_->InputLStickRight()) {
	//	rot_.y = 90.0f;

	//	if (pos_.x >= MoveLimitX) {
	//		pos_.x = MoveLimitX;
	//	}
	//	else {
	//		pos_.x ++;
	//		move_ = { 1,0,0 };
	//		//Object3D::CameraMoveVector(move_);
	//	}
	//}
	//if (input_->keyPush(DIK_LEFT)||inputPad_->InputLStickLeft()) {
	//	rot_.y = -90.0f;

	//	if (pos_.x <= -MoveLimitX) {
	//		pos_.x = -MoveLimitX;
	//	}
	//	else {
	//		pos_.x--;
	//		move_ = { -1,0,0 };
	//	}

	//}
	//if (input_->keyPush(DIK_UP)||inputPad_->InputLStickUp()) {
	//	rot_.y = 0.0f;

	//	if (pos_.z >= MoveLimitZ) {
	//		pos_.z = MoveLimitZ;
	//	}
	//	else {
	//		pos_.z++;
	//		move_ = { 0,0,1 };
	//	}

	//}
	//if (input_->keyPush(DIK_DOWN)||inputPad_->InputLStickDown()) {
	//	rot_.y = 180.0f;

	//	if (pos_.z <= -MoveLimitZ - 20) {
	//		pos_.z = -MoveLimitZ - 20;
	//	}
	//	else {

	//		move_ = { 0,0,-1 };
	//		pos_.z--;
	//	}
	//}

	//if (isMove == 0) {
	//	if (input_->keyPush(DIK_D)) {
	//		//rot_.y+=2;

	//		rot_.y = 90.0f;
	//	}
	//	if (input_->keyPush(DIK_A)) {
	//		//rot_.y-=2;

	//		rot_.y = -90.0f;
	//	}
	//}

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

	float weight = 0.7f;

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

	playerObject_->SetEye(eye_);
	playerObject_->SetTarget(target_);

	posPod_ = pos_;

	posPod_.x += 5;
	posPod_.y += 7;

	playerObject_->SetRotation(rot_);
	playerObject_->SetPosition(pos_);
	podObject_->SetPosition(posPod_);

	//XMFLOAT3 lifePos[5];

	//for (int i = 0; i < 5; i++) {
	//	lifePos[i] = pos3d2;
	//}

	//lifePos[0].x -= 10;
	//lifePos[0].y += 15;

	//lifePos[1].x -= 5;
	//lifePos[1].y += 15;

	//lifePos[2].y += 15;

	//lifePos[3].x += 5;
	//lifePos[3].y += 15;

	//lifePos[4].x += 10;
	//lifePos[4].y += 15;

	//for (int i = 0; i < 5; i++) {
	//	hpObj_[i]->SetPosition(lifePos[i]);
	//	hpObj_[i]->Update();
	//}

	playerObject_->Update();
	podObject_->Update();

	//coolTime--;

	//if (coolTime < 0) {
	//	//弾の速度
	//	const float kBulletSpeed = 2.0f;
	//	Vector3 velocity(0, 0, kBulletSpeed);

	//	velocity.multiplyMat4(playerObject_->matWorld_);
	//	velocity.normalize();
	//	velocity *= kBulletSpeed;

	//	//弾の生成と初期化
	//	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//	newBullet->Initialize(posPod_, velocity, bulletModel_, bulletObject_);
	//	bullets_.push_back(std::move(newBullet));

	//	coolTime = 8;
	//}

	//弾の更新処理
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//デスフラグが立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
	});
}

void Player::Draw()
{
	playerObject_->Draw();
	podObject_->Draw();

	/*for (int i = 0; i < HP; i++) {
		hpObj_[i]->Draw();
	}*/

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

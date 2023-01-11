#include "Player.h"

void Player::Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input)
{
	playerModel_ = playerModel;
	playerObject_ = playerObject;
	input_ = input;

	bullets_.clear();
}

void Player::Update()
{
	Vector3 CameraVec = { 0,0,0 };
	Vector3 CameraRot = { 0,0,0 };
	Vector3 tmpVecY = { 0,1,0 };

	if (input_->keyInstantPush(DIK_W) && isStep == false) {
		isStep = true;
		dashPower = 10.0f;
	}
	if (dashPower < 0) {
		isStep = false;
	}

	pos3d = playerObject_->GetRotation();
	pos3d2 = playerObject_->GetPosition();

	CameraVec.x = pos3d.x - pos3d2.x;
	CameraVec.z = pos3d.z - pos3d2.z;
	CameraVec.normalize();
	CameraRot = tmpVecY.cross(CameraVec);

	

	if (input_->keyPush(DIK_RIGHT)) {
		pos3d2.x++;
		Object3D::CameraMoveVector({ +1.0f,0.0f,0.0f });

		if (isStep == true) {
			pos3d2.x += dashPower;
			Object3D::CameraMoveVector({ dashPower,0.0f,0.0f });
			dashPower--;
		}
	}
	if (input_->keyPush(DIK_LEFT)) {
		pos3d2.x--;
		Object3D::CameraMoveVector({ -1.0f,0.0f,0.0f });

		if (isStep == true) {
			pos3d2.x -= dashPower;
			Object3D::CameraMoveVector({ -dashPower,0.0f,0.0f });
			dashPower--;
		}
	}
	if (input_->keyPush(DIK_UP)) {
		pos3d2.z++;
		Object3D::CameraMoveVector({ 0.0f,0.0f,1.0f });

		if (isStep == true) {
			pos3d2.z += dashPower;
			Object3D::CameraMoveVector({ 0.0f,0.0f,dashPower });
			dashPower--;
		}
	}
	if (input_->keyPush(DIK_DOWN)) {
		Object3D::CameraMoveVector({ 0.0f,0.0f,-1.0f });
		pos3d2.z--;

		if (isStep == true) {
			pos3d2.z -= dashPower;
			Object3D::CameraMoveVector({ 0.0f,0.0f,-dashPower });
			dashPower--;
		}
	}

	if (input_->keyPush(DIK_D)) {


		if (pos3d.y >= 18.0f) {
			pos3d.y = 18.0f;
		}
		else {
			pos3d.y++;
			Object3D::CameraEyeMoveVector(-CameraRot);
		}
	}
	if (input_->keyPush(DIK_A)) {


		if (pos3d.y <= -18.0f) {
			pos3d.y = -18.0f;
		}
		else {
			pos3d.y--;
			Object3D::CameraEyeMoveVector(CameraRot);
		}
	}

	playerObject_->SetRotation(pos3d);
	playerObject_->SetPosition(pos3d2);

	playerObject_->Update();

	coolTime--;

	if (coolTime < 0) {
		//弾の速度
		const float kBulletSpeed = 2.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity.multiplyMat4(playerObject_->matWorld);
		velocity.normalize();
		velocity *= kBulletSpeed;

		//弾の生成と初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(pos3d2, velocity);
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

void Player::Draw()
{
	playerObject_->Draw();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

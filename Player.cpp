#include "Player.h"

void Player::Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input, Object3D* podObject)
{
	playerModel_ = playerModel;
	playerObject_ = playerObject;
	podObject_ = podObject;
	input_ = input;

	hpModel_ = Model::LoadFromObj("Cube");

	for (int i = 0; i < 5; i++) {
		hpObj_[i] = Object3D::Create(hpModel_, {(1.0f),(1.0f),(1.0f)});
	}

	bullets_.clear();
}

void Player::clear() {
	bullets_.clear();
}

void Player::Update()
{
	const float MoveLimitX = 148.0f;
	const float MoveLimitZ = 134.0f;

	Vector3 CameraVec = { 0,0,0 };
	Vector3 CameraRot = { 0,0,0 };
	Vector3 tmpVecY = { 0,1,0 };

	if (input_->keyInstantPush(DIK_W) && isStep == false) {
		isStep = true;
		dashPower = 7.0f;
	}
	if (dashPower < 0) {
		isStep = false;
	}

	pos3d = playerObject_->GetRotation();
	pos3d2 = playerObject_->GetPosition();
	eye = playerObject_->GetEye();

	CameraVec.x = eye.x - pos3d2.x;
	CameraVec.z = eye.z - pos3d2.z;
	CameraVec.normalize();
	CameraRot = tmpVecY.cross(CameraVec);

	isMove = 0;

	if (input_->keyPush(DIK_RIGHT)) {
		
		if (pos3d2.x >= MoveLimitX-20) {
			isStep = false;
		}

		if (pos3d2.x >= MoveLimitX) {
			pos3d2.x = MoveLimitX;
		}
		else {
			pos3d2.x++;
			Object3D::CameraMoveVector({ +1.0f,0.0f,0.0f });

			if (isStep == true) {
				pos3d2.x += dashPower;
				Object3D::CameraMoveVector({ dashPower,0.0f,0.0f });
				dashPower--;
			}
		}
	}
	if (input_->keyPush(DIK_LEFT)) {
		
		if (pos3d2.x <= -MoveLimitX + 20) {
			isStep = false;
		}

	    if (pos3d2.x <= -MoveLimitX) {
			pos3d2.x = -MoveLimitX;
		}
		else {
			pos3d2.x--;
			Object3D::CameraMoveVector({ -1.0f,0.0f,0.0f });

			if (isStep == true) {
				pos3d2.x -= dashPower;
				Object3D::CameraMoveVector({ -dashPower,0.0f,0.0f });
				dashPower--;
			}
		}
		
	}
	if (input_->keyPush(DIK_UP)) {
		
		if (pos3d2.z >= MoveLimitZ - 20) {
			isStep = false;
		}

		if (pos3d2.z >= MoveLimitZ) {
			pos3d2.z = MoveLimitZ;
		}
		else {
			pos3d2.z++;
			Object3D::CameraMoveVector({ 0.0f,0.0f,1.0f });

			if (isStep == true) {
				pos3d2.z += dashPower;
				Object3D::CameraMoveVector({ 0.0f,0.0f,dashPower });
				dashPower--;
			}
		}
		
	}
	if (input_->keyPush(DIK_DOWN)) {
		
		if (pos3d2.z <= (-MoveLimitZ - 20) + 20) {
			isStep = false;
		}

		if (pos3d2.z <= -MoveLimitZ - 20) {
			pos3d2.z = -MoveLimitZ - 20;
		}
		else {
			Object3D::CameraMoveVector({ 0.0f,0.0f,-1.0f });
			pos3d2.z--;

			if (isStep == true) {
				pos3d2.z -= dashPower;
				Object3D::CameraMoveVector({ 0.0f,0.0f,-dashPower });
				dashPower--;
			}
		}
	}

	if (isMove == 0) {
		if (input_->keyPush(DIK_D)) {


			if (pos3d.y >= 18.0f) {
				pos3d.y = 18.0f;
			}
			else {
				pos3d.y++;
				Object3D::CameraEyeMoveVector(CameraRot);
			}
		}
		if (input_->keyPush(DIK_A)) {


			if (pos3d.y <= -18.0f) {
				pos3d.y = -18.0f;
			}
			else {
				pos3d.y--;
				Object3D::CameraEyeMoveVector(-CameraRot);
			}
		}
	}

	posPod = pos3d2;

	posPod.x += 10;
	posPod.y += 10;

	playerObject_->SetRotation(pos3d);
	playerObject_->SetPosition(pos3d2);
	podObject_->SetPosition(posPod);

	XMFLOAT3 lifePos[5];

	for (int i = 0; i < 5; i++) {
		lifePos[i] = pos3d2;
	}

	lifePos[0].x -= 10;
	lifePos[0].y += 15;

	lifePos[1].x -= 5;
	lifePos[1].y += 15;

	lifePos[2].y += 15;

	lifePos[3].x += 5;
	lifePos[3].y += 15;

	lifePos[4].x += 10;
	lifePos[4].y += 15;

	for (int i = 0; i < 5; i++) {
		hpObj_[i]->SetPosition(lifePos[i]);
		hpObj_[i]->Update();
	}

	playerObject_->Update();
	podObject_->Update();

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
		newBullet->Initialize(posPod, velocity);
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

void Player::Draw()
{
	playerObject_->Draw();
	podObject_->Draw();

	for (int i = 0; i < HP; i++) {
		hpObj_[i]->Draw();
	}

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}
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

	worldPos.x = pos3d2.x;
	worldPos.y = pos3d2.y;
	worldPos.z = pos3d2.z;

	return worldPos;
}
#include "Player.h"

void Player::Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input, Object3D* podObject)
{
	playerModel_ = playerModel;
	playerObject_ = playerObject;
	podObject_ = podObject;
	input_ = input;

	//hpModel_ = Model::LoadFromObj("Cube");

	//for (int i = 0; i < 5; i++) {
	//	hpObj_[i] = Object3D::Create(hpModel_, { (1.0f),(1.0f),(1.0f) });
	//}

	//bullets_.clear();
}

void Player::clear() {
	//bullets_.clear();
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

	rot = playerObject_->GetRotation();
	pos = playerObject_->GetPosition();
	eye = playerObject_->GetEye();

	CameraVec.x = eye.x - pos.x;
	CameraVec.z = eye.z - pos.z;
	CameraVec.normalize();
	CameraRot = tmpVecY.cross(CameraVec);

	//isMove = 0;

	

	if (input_->keyPush(DIK_RIGHT)) {

		if (pos.x >= MoveLimitX - 20) {
			isStep = false;
		}

		if (pos.x >= MoveLimitX) {
			pos.x = MoveLimitX;
		}
		else {
			pos.x ++;
			move = { 1,0,0 };
			Object3D::CameraMoveVector(move);

			if (isStep == true) {
				pos.x += dashPower;
				move = { dashPower,0,0 };
				Object3D::CameraMoveVector(move);
				dashPower--;
			}
		}
	}
	if (input_->keyPush(DIK_LEFT)) {

		if (pos.x <= -MoveLimitX + 20) {
			isStep = false;
		}

		if (pos.x <= -MoveLimitX) {
			pos.x = -MoveLimitX;
		}
		else {
			pos.x--;
			move = { -1,0,0 };
			Object3D::CameraMoveVector(move);

			//Object3D::CameraMoveVector({ -1.0f,0.0f,0.0f });

			if (isStep == true) {
				pos.x -= dashPower;
				//Object3D::CameraMoveVector({ -dashPower,0.0f,0.0f });

				move = { -dashPower,0,0 };
				Object3D::CameraMoveVector(move);
				dashPower--;
			}
		}

	}
	if (input_->keyPush(DIK_UP)) {

		if (pos.z >= MoveLimitZ - 20) {
			isStep = false;
		}

		if (pos.z >= MoveLimitZ) {
			pos.z = MoveLimitZ;
		}
		else {
			pos.z++;

			move = { 0,0,1 };
			Object3D::CameraMoveVector(move);
			//Object3D::CameraMoveVector({ 0.0f,0.0f,1.0f });

			if (isStep == true) {
				pos.z += dashPower;

				move = { 0,0,dashPower };
				Object3D::CameraMoveVector(move);
				//Object3D::CameraMoveVector({ 0.0f,0.0f,dashPower });
				dashPower--;
			}
		}

	}
	if (input_->keyPush(DIK_DOWN)) {

		if (pos.z <= (-MoveLimitZ - 20) + 20) {
			isStep = false;
		}

		if (pos.z <= -MoveLimitZ - 20) {
			pos.z = -MoveLimitZ - 20;
		}
		else {

			move = { 0,0,-1 };
			Object3D::CameraMoveVector(move);
			//Object3D::CameraMoveVector({ 0.0f,0.0f,-1.0f });
			pos.z--;

			if (isStep == true) {
				pos.z -= dashPower;

				move = { 0,0,-dashPower };
				Object3D::CameraMoveVector(move);
				//Object3D::CameraMoveVector({ 0.0f,0.0f,-dashPower });
				dashPower--;
			}
		}
	}

	if (isMove == 0) {
		if (input_->keyPush(DIK_D)) {


			if (rot.y >= 18.0f) {
				rot.y = 18.0f;
			}
			else {
				rot.y++;
				Vector3 rot = CameraRot / 2;
				Object3D::CameraEyeMoveVector(rot);
			}
		}
		if (input_->keyPush(DIK_A)) {


			if (rot.y <= -18.0f) {
				rot.y = -18.0f;
			}
			else {
				rot.y--;
				Vector3 rot = -CameraRot/2;
				Object3D::CameraEyeMoveVector(rot);
			}
		}
	}

	posPod = pos;

	posPod.x += 5;
	posPod.y += 7;

	playerObject_->SetRotation(rot);
	playerObject_->SetPosition(pos);
	podObject_->SetPosition(posPod);

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

	coolTime--;

	if (coolTime < 0) {
		//弾の速度
		const float kBulletSpeed = 2.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

	//	velocity.multiplyMat4(playerObject_->matWorld_);
		velocity.normalize();
		velocity *= kBulletSpeed;

		//弾の生成と初期化
		/*std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(posPod, velocity);
		bullets_.push_back(std::move(newBullet));*/

		coolTime = 8;
	}

	//弾の更新処理
	//for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
	//	bullet->Update();
	//}

	////デスフラグが立った弾を削除
	//bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
	//	return bullet->IsDead();
	//	});
}

void Player::Draw()
{
	playerObject_->Draw();
	podObject_->Draw();

	/*for (int i = 0; i < HP; i++) {
		hpObj_[i]->Draw();
	}*/

	//for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
	//	bullet->Draw();
	//}
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

	worldPos.x = pos.x;
	worldPos.y = pos.y;
	worldPos.z = pos.z;

	return worldPos;
}

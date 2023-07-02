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

	if (input_->keyInstantPush(DIK_W) && isStep == false) {
		isStep = true;
		dashPower = 7.0f;
	}
	if (dashPower < 0) {
		isStep = false;
	}

	rot_ = playerObject_->GetRotation();
	pos_ = playerObject_->GetPosition();
	eye_ = playerObject_->GetEye();

	CameraVec.x = eye_.x - pos_.x;
	CameraVec.z = eye_.z - pos_.z;
	CameraVec.normalize();
	CameraRot = tmpVecY.cross(CameraVec);

	//isMove = 0;

	

	if (input_->keyPush(DIK_RIGHT)) {

		if (pos_.x >= MoveLimitX - 20) {
			isStep = false;
		}

		if (pos_.x >= MoveLimitX) {
			pos_.x = MoveLimitX;
		}
		else {
			pos_.x ++;
			move_ = { 1,0,0 };
			Object3D::CameraMoveVector(move_);

			if (isStep == true) {
				pos_.x += dashPower;
				move_ = { dashPower,0,0 };
				Object3D::CameraMoveVector(move_);
				dashPower--;
			}
		}
	}
	if (input_->keyPush(DIK_LEFT)) {

		if (pos_.x <= -MoveLimitX + 20) {
			isStep = false;
		}

		if (pos_.x <= -MoveLimitX) {
			pos_.x = -MoveLimitX;
		}
		else {
			pos_.x--;
			move_ = { -1,0,0 };
			Object3D::CameraMoveVector(move_);

			//Object3D::CameraMoveVector({ -1.0f,0.0f,0.0f });

			if (isStep == true) {
				pos_.x -= dashPower;
				//Object3D::CameraMoveVector({ -dashPower,0.0f,0.0f });

				move_ = { -dashPower,0,0 };
				Object3D::CameraMoveVector(move_);
				dashPower--;
			}
		}

	}
	if (input_->keyPush(DIK_UP)) {

		if (pos_.z >= MoveLimitZ - 20) {
			isStep = false;
		}

		if (pos_.z >= MoveLimitZ) {
			pos_.z = MoveLimitZ;
		}
		else {
			pos_.z++;

			move_ = { 0,0,1 };
			Object3D::CameraMoveVector(move_);
			//Object3D::CameraMoveVector({ 0.0f,0.0f,1.0f });

			if (isStep == true) {
				pos_.z += dashPower;

				move_ = { 0,0,dashPower };
				Object3D::CameraMoveVector(move_);
				//Object3D::CameraMoveVector({ 0.0f,0.0f,dashPower });
				dashPower--;
			}
		}

	}
	if (input_->keyPush(DIK_DOWN)) {

		if (pos_.z <= (-MoveLimitZ - 20) + 20) {
			isStep = false;
		}

		if (pos_.z <= -MoveLimitZ - 20) {
			pos_.z = -MoveLimitZ - 20;
		}
		else {

			move_ = { 0,0,-1 };
			Object3D::CameraMoveVector(move_);
			//Object3D::CameraMoveVector({ 0.0f,0.0f,-1.0f });
			pos_.z--;

			if (isStep == true) {
				pos_.z -= dashPower;

				move_ = { 0,0,-dashPower };
				Object3D::CameraMoveVector(move_);
				//Object3D::CameraMoveVector({ 0.0f,0.0f,-dashPower });
				dashPower--;
			}
		}
	}

	if (isMove == 0) {
		if (input_->keyPush(DIK_D)) {


			if (rot_.y >= 18.0f) {
				rot_.y = 18.0f;
			}
			else {
				rot_.y++;
				Vector3 rot = CameraRot / 2;
				Object3D::CameraEyeMoveVector(rot);
			}
		}
		if (input_->keyPush(DIK_A)) {


			if (rot_.y <= -18.0f) {
				rot_.y = -18.0f;
			}
			else {
				rot_.y--;
				Vector3 rot = -CameraRot/2;
				Object3D::CameraEyeMoveVector(rot);
			}
		}
	}

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

	coolTime--;

	if (coolTime < 0) {
		//�e�̑��x
		const float kBulletSpeed = 2.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity.multiplyMat4(playerObject_->matWorld_);
		velocity.normalize();
		velocity *= kBulletSpeed;

		//�e�̐����Ə�����
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(posPod_, velocity, bulletModel_, bulletObject_);
		bullets_.push_back(std::move(newBullet));

		coolTime = 8;
	}

	//�e�̍X�V����
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�f�X�t���O���������e���폜
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
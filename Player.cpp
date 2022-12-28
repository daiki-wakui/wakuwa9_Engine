#include "Player.h"

void Player::Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input)
{
	playerModel_ = playerModel;
	playerObject_ = playerObject;
	input_ = input;
}

void Player::Update()
{
	if (input_->keyInstantPush(DIK_SPACE) && isStep == false) {
		isStep = true;
		dashPower = 10.0f;
	}
	if (dashPower < 0) {
		isStep = false;
	}

	pos3d = playerObject_->GetRotation();
	pos3d2 = playerObject_->GetPosition();

	if (input_->keyPush(DIK_A)) {
		pos3d.y++;
	}
	if (input_->keyPush(DIK_D)) {
		pos3d.y--;
	}

	

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

	playerObject_->SetRotation(pos3d);
	playerObject_->SetPosition(pos3d2);

	playerObject_->Update();
}

void Player::Draw()
{
	playerObject_->Draw();
}

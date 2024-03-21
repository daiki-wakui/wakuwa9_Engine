#include "PlayerCamera.h"
#include "wa9Math.h"

void PlayerCamera::Initialize(Object3D* object)
{
	obj_ = object;

	//jsonファイルから定数を読み込み
	json_ = std::make_unique<constJsonValue>();
	json_->LoadConstValue("Resources/json/playerCameraConst.json");
}

void PlayerCamera::Update()
{
	eye_ = obj_->GetEye();
	target_ = obj_->GetTarget();
	pos_ = obj_->GetPosition();

	Vector3 toCameraPosXZ;
	toCameraPosXZ = eye_ - target_;

	float height = toCameraPosXZ.y;
	toCameraPosXZ.y = 0.0f;
	float CameraXZLen = json_->LoadFloat("MOVE_CAMERA_LENGTH_VOLUE");
	toCameraPosXZ.normalize();

	

	Vector3 terget = pos_;
	terget.y += json_->LoadFloat("MOVE_CAMERA_TARGET_Y_VOLUE");

	DirectX::XMFLOAT3 toNewCameraPos;
	toNewCameraPos.x = eye_.x - terget.x;
	toNewCameraPos.y = eye_.y - terget.y;
	toNewCameraPos.z = eye_.z - terget.z;

	toNewCameraPos.y = 0.0f;

	Vector3 toNewCameraPosv;

	toNewCameraPosv.x = toNewCameraPos.x;
	toNewCameraPosv.y = toNewCameraPos.y;
	toNewCameraPosv.z = toNewCameraPos.z;

	toNewCameraPosv.normalize();

	
	float weight = json_->LoadFloat("MOVE_CAMERA_ROT_WEIGHT_VOLUE");
	
	
	toNewCameraPosv = toNewCameraPosv * weight + toCameraPosXZ * (json_->LoadFloat("MOVE_CAMERA_ROT_MAX_WEIGHT_VOLUE") - weight);
	toNewCameraPosv.normalize();
	toNewCameraPosv *= CameraXZLen;
	toNewCameraPosv.y = height;

	Vector3 newEye;
	newEye.x = terget.x + toNewCameraPosv.x;
	newEye.y = terget.y + toNewCameraPosv.y;
	newEye.z = terget.z + toNewCameraPosv.z;

	eye_ = newEye;
	target_ = terget;

	cameraTargetAngle_ = obj_->GetEye().y;

	if (inputPad_->InputRStickRight()) {
		cameraAngle_ += json_->LoadFloat("MOVE_CAMERA_ANGLE_VOLUE");
	}
	else if (inputPad_->InputRStickLeft()) {
		cameraAngle_ -= json_->LoadFloat("MOVE_CAMERA_ANGLE_VOLUE");
	}
	

	if (inputPad_->InputRStickUp()) {
		cameraTargetAngle_ -= json_->LoadFloat("MOVE_CAMERA_TARGET_ANGLE_VOLUE");
	}
	else if (inputPad_->InputRStickDown()) {
		cameraTargetAngle_ += json_->LoadFloat("MOVE_CAMERA_TARGET_ANGLE_VOLUE");
	}

	if (inputPad_->InputRStick()) {
		r_ = cameraAngle_ * wa9Math::PI() / wa9Math::Degree180();
		cameraTargetAngle_ = max(cameraTargetAngle_, json_->LoadFloat("MOVE_CAMERA_TARGET_MIN_ANGLE"));
		cameraTargetAngle_ = min(cameraTargetAngle_, json_->LoadFloat("MOVE_CAMERA_TARGET_MAX_ANGLE"));

		eye_.x = pos_.x + (sinf(r_) * CameraXZLen);
		eye_.z = pos_.z + (cosf(r_) * CameraXZLen);
	}

	eye_.y = cameraTargetAngle_;

	obj_->SetEye(eye_);
	obj_->SetTarget(target_);
	Object3D::SetEye(eye_);
	Object3D::SetTarget(target_);
}

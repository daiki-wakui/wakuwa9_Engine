#pragma once
#include "PlayerBasis.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "Vector3.h"

class PlayerCamera
{
public:	//メンバ関数
	//初期化
	void Initialize(Object3D* object);
	//毎フレーム
	void Update();


private:

	const float MOVE_CAMERA_LENGTH_VOLUE = 30.0f;
	const float MOVE_CAMERA_TARGET_Y_VOLUE = 10.0f;
	const float MOVE_CAMERA_ROT_MAX_WEIGHT_VOLUE = 1.0f;
	const float MOVE_CAMERA_ROT_WEIGHT_VOLUE = 0.3f;
	const float MOVE_CAMERA_ANGLE_VOLUE = 1.5f;
	const float MOVE_CAMERA_TARGET_ANGLE_VOLUE = 0.25f;
	const float MOVE_CAMERA_TARGET_MIN_ANGLE = 0.5f;
	const float MOVE_CAMERA_TARGET_MAX_ANGLE = 30.0f;

private:

	KeyBoard* input_ = KeyBoard::GetInstance();
	GamePad* inputPad_ = GamePad::GetInstance();

	Object3D* obj_;

	Vector3 pos_;
	Vector3 eye_;
	Vector3 target_;

	float cameraAngle_ = 180.0f;
	float cameraTargetAngle_ = 0.0f;
	float r_;

	
};


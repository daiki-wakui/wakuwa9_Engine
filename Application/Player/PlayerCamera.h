#pragma once
#include "PlayerBasis.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "Vector3.h"
#include "constJsonValue.h"

class PlayerCamera
{
public:	//メンバ関数
	//初期化
	void Initialize(Object3D* object);
	//毎フレーム
	void Update();

private:

	KeyBoard* input_ = KeyBoard::GetInstance();
	GamePad* inputPad_ = GamePad::GetInstance();

	std::unique_ptr<constJsonValue> json_;

	Object3D* obj_;

	Vector3 pos_;
	Vector3 eye_;
	Vector3 target_;

	float cameraAngle_ = 180.0f;
	float cameraTargetAngle_ = 0.0f;
	float r_;

	
};


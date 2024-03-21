#pragma once
#include "Object3D.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "constJsonValue.h"


class BitMove
{
public:

	//初期化
	void Initialize(Object3D* object);
	//毎フレーム
	void Update();
	void Move();

	Vector3 GetEye() { return eye_; }
	Vector3 GetPos() { return pos_; }

private:

	std::unique_ptr<constJsonValue> json_;

	Object3D* obj_;

	KeyBoard* input_ = KeyBoard::GetInstance();
	GamePad* inputPad_ = GamePad::GetInstance();

	Vector3 rot_;
	Vector3 pos_;
	Vector3 posPod_;
	Vector3 eye_;
	Vector3 target_;

	Vector3 frontVec = { 0,0,0 };
	Vector3 moveXVec = { 0,0,0 };
	Vector3 tmpVecY = { 0,1,0 };

	Vector3 sideMove_ = { 0,0,0 };
	Vector3 frontMove_ = { 0,0,0 };

	bool dash = false;

	float dashPower_;

	float friction = 0.8f;

	int32_t frame = 0;
};


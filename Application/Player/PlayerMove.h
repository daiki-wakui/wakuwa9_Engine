#pragma once
#include "PlayerBasis.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "Vector3.h"
#include "constJsonValue.h"

class PlayerMove
{
public:	//メンバ関数
	//初期化
	void Initialize(Object3D* object);
	//毎フレーム
	void Update();

private:
	//角度算出
	void RotateAngle();
	void Move();

private:

	std::unique_ptr<constJsonValue> json_;

protected:

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

	float radi_;
	float angle_;
};


#pragma once
#include "PlayerBasis.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "Vector3.h"

class PlayerMove
{
public:	//メンバ関数
	//初期化
	void Initialize(Object3D* object);
	//毎フレーム
	void Update();

private:

	void Move();

private:

	const float FRONT_VECTOR_RATE = 1.5f;
	const float DASH_POWTER_VOLUE = 0.2f;
	const float ROT_MAX = 5;
	const int32_t ADD_FRAME = 20;
	const float DASH_POWTER_MAX_VOLUE = 1.5f;


	const int32_t FRAME_RATE = 40;
	const float POS_Y_RATE = 0.5f;
	const float ADD_POS_Y_VOLUE = 10.0f;
private:
	KeyBoard* input_ = KeyBoard::GetInstance();
	GamePad* inputPad_ = GamePad::GetInstance();
	
	Object3D* obj_;

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


#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Vector3.h"
#include <cmath>

class PlayerBullet
{
private:

	Model* bulletModel = nullptr;
	Object3D* bulletObject_;
	Vector3 velocity_;

	Vector3 pos_;
	DirectX::XMFLOAT3 tPos_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	
	float timer = 0;
	float timerMax = 60;

	Vector3 Start;
	Vector3 End;
	Vector3 p0;

	Vector3 pos;

	int missile_;

public:
	bool isDead_ = false;

	bool IsDead() const { return isDead_; }

	void Initialize(DirectX::XMFLOAT3 pos, Vector3& velocity, int missile = false);
	void Update();
	void Draw();

	float EaseOut(float t);

	DirectX::XMFLOAT3 GetWorldPos();
};


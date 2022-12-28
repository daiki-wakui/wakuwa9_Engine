#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Vector3.h"

class PlayerBullet
{
private:

	Model* model = nullptr;
	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
public:


};


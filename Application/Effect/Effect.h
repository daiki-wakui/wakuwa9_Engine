#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Vector3.h"

class Effect
{
private:

	Model* bulletModel_;
	std::unique_ptr<Object3D> bulletObject_;
	Vector3 velocity_;

	float pow_ = 0;

	Vector3 pos_;
	Vector3 tPos_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;

	Vector3 rot_ = {};

	float scale_ = 1;

public:
	bool isDead_ = false;

	bool IsDead() const { return isDead_; }

	void Initialize(Vector3 pos, Vector3& velocity, Model* model);
	void Update();
	void Draw();

	DirectX::XMFLOAT3 GetWorldPos();
};


#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Vector3.h"

class BossBullet
{
private:

	Model* bulletModel_;
	std::unique_ptr<Object3D> bulletObject_;

	//Model* bulletModel = nullptr;
	//Object3D* bulletObject_;
	Vector3 velocity_;

	DirectX::XMFLOAT3 tPos_;
	float addRand_ = 0;
	int32_t bulletState_ = 0;


	static const int32_t kLifeTime = 60 * 2;
	int32_t deathTimer_ = kLifeTime;

public:
	bool isDead_ = false;

	bool IsDead() const { return isDead_; }

	void Initialize(DirectX::XMFLOAT3 pos, Vector3& velocity, Model* model, int32_t randState);
	void Update();
	void Draw();

	DirectX::XMFLOAT3 GetWorldPos();

};


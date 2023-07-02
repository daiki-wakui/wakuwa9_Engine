#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Vector3.h"

class PlayerBullet
{
private:

	Model* bulletModel_;
	std::unique_ptr<Object3D> bulletObject_;

	//Model* bulletModel = nullptr;
	//Object3D* bulletObject_;
	Vector3 velocity_;

	Vector3 pos_;
	DirectX::XMFLOAT3 tPos_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;

public:
	bool isDead_ = false;

	bool IsDead() const { return isDead_; }

	void Initialize(DirectX::XMFLOAT3 pos, Vector3& velocity,Model* model,Object3D* obj);
	void Update();
	void Draw();

	DirectX::XMFLOAT3 GetWorldPos();
};



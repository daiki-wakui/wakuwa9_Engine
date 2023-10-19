#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Vector3.h"
#include "ParticleManager.h"

class PlayerBullet
{
private:

	std::unique_ptr<ParticleManager> effectParticle_ = std::make_unique<ParticleManager>();


	Model* bulletModel_;
	std::unique_ptr<Object3D> bulletObject_;

	//Model* bulletModel = nullptr;
	//Object3D* bulletObject_;
	Vector3 velocity_;

	Vector3 pos_;
	DirectX::XMFLOAT3 tPos_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;

	bool missile_ = false;

	Vector3 Start;
	Vector3 End;
	Vector3 p0;

	float timer = 0;
	float timerMax = 65;

	float prot_ = 0;
public:
	bool isDead_ = false;

	bool IsDead() const { return isDead_; }

	void Initialize(DirectX::XMFLOAT3 pos, Vector3& velocity,Model* model);
	void Update();
	void Draw();
	void pDraw();

	DirectX::XMFLOAT3 GetWorldPos();

	bool GetMissile() const { return missile_; }
	void SetMissile(bool missile) { missile_ = missile; }
};



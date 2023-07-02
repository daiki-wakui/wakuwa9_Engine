#pragma once
#include "Model.h"
#include "Object3D.h"

class Boss
{
private:

	Model* model_;
	Object3D* object_;

	int32_t hp = 50;

public:

	bool arive_ = false;

	DirectX::XMFLOAT3 GetWorldPos();

	void Initialize(Model* Model, Object3D* Object);
	void Update();
	void Draw();
	void OnCollision();

	int32_t GetHP() const { return hp; }
	bool GetArive() const { return arive_; }
};


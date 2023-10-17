#pragma once
#include "Model.h"
#include "Object3D.h"

class EventBox
{
private:

	Model* model_;
	Object3D* object_;

public:

	bool valid_ = false;

	DirectX::XMFLOAT3 GetWorldPos();

	void Initialize(Model* Model, Object3D* Object);
	void Update();
	void Draw();

	bool GetIsArive() { return valid_; }
};


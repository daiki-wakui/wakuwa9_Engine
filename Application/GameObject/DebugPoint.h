#pragma once
#include "Model.h"
#include "Object3D.h"

class DebugPoint
{
private:

	Model* model_;
	Object3D* object_;

public:

	bool valid_ = false;

	Vector3 GetWorldPos();

	void Initialize(Model* Model, Object3D* Object);
	void Update();
	void Draw();

	bool GetIsArive() { return valid_; }
};


#pragma once
#include "Model.h"
#include "Object3D.h"

class EventBox
{
private:

	Model* model_;
	Object3D* object_;

public:

	bool arive = false;

	void Initialize(Model* Model, Object3D* Object);
	void Update();
	void Draw();
};


#pragma once
#include "Model.h"
#include "Object3D.h"

class IventBox
{
private:

	Model* playerModel_;
	Object3D* playerObject_;

public:

	bool arive = false;
	
	void Initialize(Model* playerModel, Object3D* playerObject);
	void Update();
	void Draw();
};


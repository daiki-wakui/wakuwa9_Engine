#pragma once
#include "Model.h"
#include "Object3D.h"
#include "KeyBoard.h"

class Player
{
private:

	Model* playerModel_;
	Object3D* playerObject_;
	KeyBoard* input_;

public:

	void Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input);
	void Update();
	void Draw();
};


#pragma once
#include "Model.h"
#include "Object3D.h"

class EventBox
{
private:

	Model* model_;
	Object3D* object_;

	bool isLoadEditor_;
public:

	bool valid_ = false;

	Vector3 GetWorldPos();

	void Initialize(Model* Model, Object3D* Object, bool scene = false);
	void Update();
	void Draw();

	bool GetIsArive() { return valid_; }
	bool GetLoadEditor() { return isLoadEditor_; }
};


#pragma once
#include "Model.h"
#include "Object3D.h"

class Door
{
private:

	Model* model_;
	Object3D* object_;

	DirectX::XMFLOAT3 pos_;
	float objectRot_ = 0.0f;

	bool isChangeStage_ = false;

public:

	bool valid_ = false;
	bool filp_ = false;

	DirectX::XMFLOAT3 GetWorldPos();

	void Initialize(Model* Model, Object3D* Object, bool filp = false);
	void Update();
	void Draw();

	void SetTutorial(bool isChangeStage) { isChangeStage_ = isChangeStage; }
	//bool GetIsArive() { return valid_; }
};


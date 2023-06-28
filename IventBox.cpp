#include "IventBox.h"

void IventBox::Initialize(Model* playerModel, Object3D* playerObject)
{
	playerModel_ = playerModel;
	playerObject_ = playerObject;

	arive = true;
}


void IventBox::Update()
{
	playerObject_->Update();
}

void IventBox::Draw()
{
	playerObject_->Draw();
}

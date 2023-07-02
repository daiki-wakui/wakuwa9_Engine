#include "EventBox.h"

void EventBox::Initialize(Model* Model, Object3D* Object)
{
	model_ = Model;
	object_ = Object;

	arive = true;
}


void EventBox::Update()
{
	object_->Update();
}

void EventBox::Draw()
{
	object_->Draw();
}

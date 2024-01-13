#include "ChangeStageBox.h"

ChangeStageBox::ChangeStageBox()
{
}

ChangeStageBox::~ChangeStageBox()
{
}

void ChangeStageBox::Initialize(Model* model, Object3D* object)
{
	thismodel_ = model;
	thisObject_ = object;
}

void ChangeStageBox::Update()
{
	thisObject_->Update();
}

void ChangeStageBox::Draw()
{
	thisObject_->Draw();
}

Vector3 ChangeStageBox::GetWorldPos()
{
	return thisObject_->GetPosition();
}

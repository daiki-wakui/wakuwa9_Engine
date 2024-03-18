#include "ChangeStageBox.h"

ChangeStageBox::ChangeStageBox()
{
}

ChangeStageBox::~ChangeStageBox()
{
}

void ChangeStageBox::Initialize(Model* model, Object3D* object)
{
	name_ = "ChangeStageBox";
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

Vector3 ChangeStageBox::GetScale()
{
	return thisObject_->GetScale();
}

void ChangeStageBox::OnCollison()
{

}

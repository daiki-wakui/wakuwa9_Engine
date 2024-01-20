#include "StageObject.h"

StageObject::StageObject()
{
}

StageObject::~StageObject()
{
}

void StageObject::Initialize(Model* model, Object3D* object)
{
	thismodel_ = model;
	thisObject_ = object;
}

void StageObject::Update()
{
	thisObject_->Update();
}

void StageObject::Draw()
{
	thisObject_->Draw();
}

Vector3 StageObject::GetWorldPos()
{
	return Vector3();
}

Vector3 StageObject::GetScale()
{
	return Vector3();
}

void StageObject::OnCollison()
{
}

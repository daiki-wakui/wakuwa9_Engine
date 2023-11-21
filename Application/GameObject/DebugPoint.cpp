#include "DebugPoint.h"

Vector3 DebugPoint::GetWorldPos()
{
	Vector3 worldPos;

	worldPos.x = object_->GetPosition().x;
	worldPos.y = object_->GetPosition().y;
	worldPos.z = object_->GetPosition().z;

	return worldPos;
}

void DebugPoint::Initialize(Model* Model, Object3D* Object)
{
	model_ = Model;
	object_ = Object;

	valid_ = true;
}

void DebugPoint::Update()
{
	object_->Update();
}

void DebugPoint::Draw()
{
	object_->Draw();
}

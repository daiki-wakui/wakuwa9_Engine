#include "EventBox.h"

DirectX::XMFLOAT3 EventBox::GetWorldPos()
{
	DirectX::XMFLOAT3 worldPos;

	worldPos.x = object_->GetPosition().x;
	worldPos.y = object_->GetPosition().y;
	worldPos.z = object_->GetPosition().z;

	return worldPos;
}

void EventBox::Initialize(Model* Model, Object3D* Object, bool scene)
{
	model_ = Model;
	object_ = Object;

	valid_ = true;
	isLoadEditor_ = scene;
}


void EventBox::Update()
{
	object_->Update();
}

void EventBox::Draw()
{
	object_->Draw();
}

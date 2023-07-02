#include "Boss.h"

DirectX::XMFLOAT3 Boss::GetWorldPos()
{
	DirectX::XMFLOAT3 worldPos;

	worldPos.x = object_->GetPosition().x;
	worldPos.y = object_->GetPosition().y;
	worldPos.z = object_->GetPosition().z;

	return worldPos;
}

void Boss::Initialize(Model* Model, Object3D* Object)
{
	model_ = Model;
	object_ = Object;

	arive_ = true;
}

void Boss::Update()
{
	object_->Update();
}

void Boss::Draw()
{
	object_->Draw();
}

#include "CollisionBox.h"

Vector3 CollisionBox::GetWorldPos()
{
	Vector3 worldPos;

	worldPos.x = object_->GetPosition().x;
	worldPos.y = object_->GetPosition().y;
	worldPos.z = object_->GetPosition().z;

	return worldPos;
}

void CollisionBox::Initialize(Model* Model, Object3D* Object)
{

	model_ = Model;
	object_ = Object;

	arive = true;
}

void CollisionBox::Update()
{
	object_->Update();
}

void CollisionBox::Draw()
{

	object_->Draw();
}

bool CollisionBox::OnCollision()
{
	hit_ = true;

	return true;
}

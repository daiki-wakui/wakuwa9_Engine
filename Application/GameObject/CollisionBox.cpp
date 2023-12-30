#include "CollisionBox.h"

//getter
Vector3 CollisionBox::GetWorldPos()
{
	Vector3 worldPos;

	worldPos.x = object_->GetPosition().x;
	worldPos.y = object_->GetPosition().y;
	worldPos.z = object_->GetPosition().z;

	return worldPos;
}

//初期化
void CollisionBox::Initialize(Model* Model, Object3D* Object)
{

	model_ = Model;
	object_ = Object;

	arive = true;
}

//更新処理
void CollisionBox::Update()
{
	object_->Update();
}

//描画関数
void CollisionBox::Draw()
{

	object_->Draw();
}

//当たったときの処理
bool CollisionBox::OnCollision()
{
	hit_ = true;

	return true;
}

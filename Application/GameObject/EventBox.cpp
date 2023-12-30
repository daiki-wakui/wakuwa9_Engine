#include "EventBox.h"

//getter
Vector3 EventBox::GetWorldPos()
{
	Vector3 worldPos;

	worldPos.x = object_->GetPosition().x;
	worldPos.y = object_->GetPosition().y;
	worldPos.z = object_->GetPosition().z;

	return worldPos;
}

//初期化
void EventBox::Initialize(Model* Model, Object3D* Object, bool scene)
{
	model_ = Model;
	object_ = Object;

	valid_ = true;
	isLoadEditor_ = scene;
}

//更新処理
void EventBox::Update()
{
	object_->Update();
}

//描画関数
void EventBox::Draw()
{
	object_->Draw();
}

#include "DebugPoint.h"

//getter
Vector3 DebugPoint::GetWorldPos()
{
	Vector3 worldPos;

	worldPos.x = object_->GetPosition().x;
	worldPos.y = object_->GetPosition().y;
	worldPos.z = object_->GetPosition().z;

	return worldPos;
}

//初期化
void DebugPoint::Initialize(Model* Model, Object3D* Object)
{
	model_ = Model;
	object_ = Object;

	valid_ = true;
}

//更新処理
void DebugPoint::Update()
{
	object_->Update();
}

//描画関数
void DebugPoint::Draw()
{
	object_->Draw();
}

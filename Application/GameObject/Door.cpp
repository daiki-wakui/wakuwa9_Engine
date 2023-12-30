#include "door.h"

DirectX::XMFLOAT3 Door::GetWorldPos()
{
	DirectX::XMFLOAT3 worldPos;

	worldPos.x = object_->GetPosition().x;
	worldPos.y = object_->GetPosition().y;
	worldPos.z = object_->GetPosition().z;

	return worldPos;
}

void Door::Initialize(Model* Model, Object3D* Object,bool filp)
{
	model_ = Model;
	object_ = Object;

	valid_ = true;

	filp_ = filp;
}


void Door::Update()
{
	if (!isChangeStage_) {
		if (filp_) {
			objectRot_ -= OBJECT_ROT_VOLUE;
			objectRot_ = max(OBJECT_ROT_MAX, objectRot_);
		}
		else {
			objectRot_ += OBJECT_ROT_VOLUE;
			objectRot_ = min(OBJECT_ROT_MIN, objectRot_);
		}
	}
	object_->SetRotation({0,objectRot_,0 });
	object_->Update();
}

void Door::Draw()
{
	object_->Draw();
}


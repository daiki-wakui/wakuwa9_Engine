#include "FieldEffect.h"

void FieldEffect::Initialize(Vector3 pos, Vector3 rot,Vector3& velocity, Model* model)
{
	pos_ = pos;
	rot_ = rot;
	velocity_ = velocity;
	thisModel_ = model;

	thisObject_ = std::make_unique<Object3D>();
	thisObject_->SetModel(thisModel_);
	thisObject_->Initialize();
	thisObject_->SetPosition(pos_);
	thisObject_->SetRotation(rot_);
}

void FieldEffect::Update()
{
	scale_ -= 0.005f;
	thisObject_->SetScale({ scale_ ,scale_ ,scale_ });

	thisObject_->Update();

	if (scale_ < 0) {
		isDead_ = true;
	}
}

void FieldEffect::Draw()
{
	thisObject_->Draw();
}

void FieldEffect::SetScale(float scale)
{
	scale_ = scale;
	thisObject_->SetScale({scale_,scale_,scale_});
}

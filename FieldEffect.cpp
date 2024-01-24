#include "FieldEffect.h"
#include "Easing.h"

void FieldEffect::Initialize(Vector3 pos, Vector3 rot,float scale,Vector3& velocity, Model* model)
{
	maxScale_ = scale;
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
	ariveFrame_++;

	if (isDown_) {
		timer_--;
	}
	else {
		timer_++;
	}

	if (timer_ == 90) {
		isDown_ = true;
	}
	else if (timer_ == 0) {
		isDown_ = false;
	}

	pos_ += velocity_;

	scaleEs_ = scaleEs_.lerp({ 0,0,0 }, { maxScale_,maxScale_,maxScale_ }, Easing::EaseOutBack(timer_, 90));
	thisObject_->SetScale(scaleEs_);
	thisObject_->SetPosition(pos_);
	thisObject_->Update();

	if (ariveFrame_ > 180) {
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

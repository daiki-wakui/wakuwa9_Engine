#include "HitEffect.h"
#include "Easing.h"
#include "MyRandom.h"

void HitEffect::Initialize(Vector3 pos, Vector3 rot, float scale, Vector3& velocity, Model* model)
{
	maxScale_ = scale;
	pos_ = pos;
	rot_ = rot;
	velocity_ = velocity;
	thisModel_ = model;

	rot_.z += MyRandom::GetFloatRandom(-100, 100);

	thisObject_ = std::make_unique<Object3D>();
	thisObject_->SetModel(thisModel_);
	thisObject_->Initialize();
	thisObject_->SetPosition(pos_);
	thisObject_->SetRotation(rot_);
	thisObject_->SetScale({ maxScale_ + MyRandom::GetFloatRandom(0,3),maxScale_,maxScale_ });
}

void HitEffect::Update()
{
	ariveFrame_++;

	pos_ += velocity_;

	rot_.z += 10;

	//scaleEs_ = scaleEs_.lerp({ 0,0,0 }, { maxScale_,maxScale_,maxScale_ }, Easing::EaseOutBack(timer_, 90));
	//thisObject_->SetScale(scaleEs_);
	thisObject_->SetPosition(pos_);
	thisObject_->SetRotation(rot_);
	thisObject_->Update();

	if (ariveFrame_ > 60) {
		isDead_ = true;
	}
}

void HitEffect::Draw()
{
	thisObject_->Draw();
}

void HitEffect::SetScale(float scale)
{
	scale_ = scale;
	thisObject_->SetScale({ scale_,scale_,scale_ });
}

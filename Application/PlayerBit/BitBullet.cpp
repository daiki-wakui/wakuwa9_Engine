#include "BitBullet.h"

BitBullet::BitBullet()
{
}

BitBullet::~BitBullet()
{
}

void BitBullet::Initialize(Model* model, Object3D* object)
{
	thismodel_ = model;
	obj_ = object;
	thisObject_ = std::make_unique<Object3D>();
	thisObject_->SetModel(thismodel_);
	thisObject_->Initialize();

	thisObject_->SetPosition(pos_);
}

void BitBullet::Update()
{
	thisObject_->Update();

	pos_ += velocity_;
	thisObject_->SetPosition(pos_);

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void BitBullet::Draw()
{
	thisObject_->Draw();
}

Vector3 BitBullet::GetWorldPos()
{
	return Vector3();
}

Vector3 BitBullet::GetScale()
{
	return Vector3();
}

void BitBullet::OnCollison()
{
}

void BitBullet::SetInfo(Vector3 pos, Vector3 velocity)
{
	pos_ = pos;
	velocity_ = velocity;
}

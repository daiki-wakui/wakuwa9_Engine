#include "BossMovieBox.h"

BossMovieBox::BossMovieBox()
{
}

BossMovieBox::~BossMovieBox()
{
}

void BossMovieBox::Initialize(Model* model, Object3D* object)
{
	thismodel_ = model;
	thisObject_ = object;
}

void BossMovieBox::Update()
{
	thisObject_->Update();
}

void BossMovieBox::Draw()
{
	thisObject_->Draw();
}

Vector3 BossMovieBox::GetWorldPos()
{
	return thisObject_->GetPosition();
}

Vector3 BossMovieBox::GetScale()
{
	return Vector3();
}

void BossMovieBox::OnCollison()
{
}

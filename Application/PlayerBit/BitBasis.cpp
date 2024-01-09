#include "BitBasis.h"

BitBasis::BitBasis()
{
}

BitBasis::~BitBasis()
{
}

void BitBasis::Initialize(Model* model, Object3D* object)
{
	thismodel_ = model;
	thisObject_ = object;

	move_->Initialize(thisObject_);
}

void BitBasis::Update()
{
	thisObject_->Update();
	move_->Update();
}

void BitBasis::Draw()
{
	thisObject_->Draw();
}

Vector3 BitBasis::GetWorldPos()
{
	return thisObject_->GetPosition();
}

#include "BossBasis.h"

BossBasis::BossBasis()
{
}

BossBasis::~BossBasis()
{
}

void BossBasis::Initialize(Model* model, Object3D* object)
{
	thismodel_ = model;
	thisObject_ = object;
}

void BossBasis::Update()
{
	thisObject_->Update();
}

void BossBasis::Draw()
{
	thisObject_->Draw();
}

Vector3 BossBasis::GetWorldPos()
{
	return thisObject_->GetPosition();
}
#include "EnemyBasis.h"

EnemyBasis::EnemyBasis()
{
}

EnemyBasis::~EnemyBasis()
{
}

void EnemyBasis::Initialize(Model* model, Object3D* object)
{
	thismodel_ = model;
	thisObject_ = object;
}

void EnemyBasis::Update()
{
	thisObject_->Update();
}

void EnemyBasis::Draw()
{
	thisObject_->Draw();
}

Vector3 EnemyBasis::GetWorldPos()
{
	return thisObject_->GetPosition();
}

Vector3 EnemyBasis::GetScale()
{
	return Vector3();
}

void EnemyBasis::OnCollison()
{
}

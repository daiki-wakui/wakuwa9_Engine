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
	thisObject_->SetScale({ 3,3,3 });

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

#include "PlayerBasis.h"
#include "PlayerMove.h"
#include "PlayerCamera.h"

PlayerBasis::PlayerBasis()
{
}

PlayerBasis::~PlayerBasis()
{
}

void PlayerBasis::Initialize(Model* model, Object3D* object)
{
	name_ = "player";

	thismodel_ = model;
	thisObject_ = object;

	move_->Initialize(thisObject_);
	camera_->Initialize(thisObject_);
}

void PlayerBasis::Update()
{
	thisObject_->Update();
	move_->Update();
	camera_->Update();
}

void PlayerBasis::Draw()
{
	thisObject_->Draw();
}

Vector3 PlayerBasis::GetWorldPos()
{
	return thisObject_->GetPosition();
}

Vector3 PlayerBasis::GetScale()
{
	return thisObject_->GetScale();
}

void PlayerBasis::OnCollison()
{
}

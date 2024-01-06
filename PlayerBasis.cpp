#include "PlayerBasis.h"
#include "PlayerMove.h"

PlayerBasis::PlayerBasis()
{
}

PlayerBasis::~PlayerBasis()
{
}

void PlayerBasis::Initialize(Model* model, Object3D* object)
{
	thismodel_ = model;
	thisObject_ = object;

	move_->Initialize(thisObject_);
}

void PlayerBasis::Update()
{
	thisObject_->Update();
	move_->Update();
}

void PlayerBasis::Draw()
{
	thisObject_->Draw();
}

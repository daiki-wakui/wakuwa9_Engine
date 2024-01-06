#include "PlayerBasis.h"

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
}

void PlayerBasis::Update()
{
	thisObject_->Update();
}

void PlayerBasis::Draw()
{
	thisObject_->Draw();
}

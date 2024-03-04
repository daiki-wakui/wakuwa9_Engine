#include "BossCanon.h"

void BossCanon::Initialize(Model* model, Vector3 pos, Player* player)
{
	pos_ = pos;
	Object_ = std::make_unique<Object3D>();
	Object_->SetModel(model);
	Object_->SetPosition(pos_);
	Object_->Initialize();
	info_ = player;
}

void BossCanon::Update()
{
	vro_.y--;
	pos_ = vro_;

	Object_->SetPosition(pos_);
	Object_->Update();
}

void BossCanon::Draw()
{
	Object_->Draw();
}

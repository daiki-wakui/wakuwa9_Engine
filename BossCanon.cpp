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
	if (pos_.y > 300) {
		isFall_ = true;
		pos_.x = info_->GetWorldPos().x;
		pos_.z = info_->GetWorldPos().z;

	}

	if (!isFall_) {
		pos_.y += 3;

	}
	else {
		pos_.y -= 3;

	}

	detchTimer_++;

	if (detchTimer_ > 300) {
		isDead_ = true;
	}
	
	Object_->SetPosition(pos_);
	Object_->Update();
}

void BossCanon::Draw()
{
	Object_->Draw();
}

void BossCanon::SetPos(Vector3 pos)
{
	Object_->SetPosition(pos);
	Object_->Update();
}

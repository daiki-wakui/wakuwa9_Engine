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

	playerBulletCubeModel_ = std::make_unique<Model>();
	playerBulletCubeModel_->LoadFromObj("cube3");

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(playerBulletCubeModel_.get());
	bulletObject_->Initialize();
}

void BitBasis::Update()
{
	thisObject_->Update();
	move_->Update();

	if (input_->keyInstantPush(DIK_G)) {
		std::unique_ptr<BaseObject> newBullet = std::make_unique<BitBullet>();
		newBullet->SetInfo(thisObject_->GetPosition(), { 1,0,0 });
		newBullet->Initialize(playerBulletCubeModel_.get(), bulletObject_.get());
		bullets_.push_back(std::move(newBullet));
	}

	//弾の更新処理
	for (std::unique_ptr<BaseObject>& bullet : bullets_) {
		bullet->Update();
	}
}

void BitBasis::Draw()
{
	thisObject_->Draw();

	//弾の更新処理
	for (std::unique_ptr<BaseObject>& bullet : bullets_) {
		bullet->Draw();
	}
}

Vector3 BitBasis::GetWorldPos()
{
	return thisObject_->GetPosition();
}

#include "BitBasis.h"

BitBasis::BitBasis()
{
}

BitBasis::~BitBasis()
{
}

void BitBasis::Initialize(Model* model, Object3D* object)
{
	cooltime_ = 7;
	thismodel_ = model;
	thisObject_ = object;

	move_->Initialize(thisObject_);

	playerBulletCubeModel_ = std::make_unique<Model>();
	playerBulletCubeModel_->LoadFromObj("cube3");

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(playerBulletCubeModel_.get());
	bulletObject_->Initialize();

	reticle3DObject_->SetModel(playerBulletCubeModel_.get());
	reticle3DObject_->Initialize();
}

void BitBasis::Update()
{
	thisObject_->Update();
	move_->Update();

	eye_ = thisObject_->GetEye();

	bulletRTPos_ = thisObject_->GetPosition();
	bulletRTPos_.x -= 5;
	bulletRTPos_.z += 50;

	reticle3DObject_->SetPosition(bulletRTPos_);
	reticle3DObject_->Update();


	bulletVec_.x = eye_.x - (thisObject_->GetPosition().x - 5);
	bulletVec_.y = (eye_.y - 5) - thisObject_->GetPosition().y;
	bulletVec_.z = eye_.z - thisObject_->GetPosition().z;


	bulletVec_.normalize();
	bulletVec_ *= -15;

	if (inputPad_->RTrigger()) {
		cooltime_--;
	}

	if (cooltime_ == 0) {
		std::unique_ptr<BaseObject> newBullet = std::make_unique<BitBullet>();
		newBullet->SetInfo(thisObject_->GetPosition(), bulletVec_);
		newBullet->Initialize(playerBulletCubeModel_.get(), bulletObject_.get());
		bullets_.push_back(std::move(newBullet));
		cooltime_ = 7;
	}

	//弾の更新処理
	for (std::unique_ptr<BaseObject>& bullet : bullets_) {
		bullet->Update();
	}

	//デスフラグが立った弾を削除
	bullets_.remove_if([](std::unique_ptr<BaseObject>& bullet) {
		return bullet->IsDead();
	});
}

void BitBasis::Draw()
{
	thisObject_->Draw();

	reticle3DObject_->Draw();
	//弾の更新処理
	for (std::unique_ptr<BaseObject>& bullet : bullets_) {
		bullet->Draw();
	}
}

Vector3 BitBasis::GetWorldPos()
{
	return thisObject_->GetPosition();
}

Vector3 BitBasis::GetScale()
{
	return Vector3();
}

void BitBasis::OnCollison()
{
}

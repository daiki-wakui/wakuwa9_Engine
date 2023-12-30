#include "Effect.h"
#include <random>
#include "MyRandom.h"

//初期化
void Effect::Initialize(Vector3 pos, Vector3& velocity, Model* model)
{
	tPos_ = pos;
	velocity_ = velocity;

	bulletModel_ = model;

	bulletObject_ = std::make_unique<Object3D>();
	bulletObject_->SetModel(bulletModel_);
	bulletObject_->Initialize();

	bulletObject_->SetPosition(tPos_);

	velocity_.x = MyRandom::GetFloatRandom(-2, 3);
	velocity_.z = MyRandom::GetFloatRandom(-2, 4);

}

//更新処理
void Effect::Update()
{
	pow_ += 0.01f;
	velocity_.y -= pow_;

	scale_ -= 0.02f;

	rot_.x = bulletObject_->GetRotation().x;
	rot_.y = bulletObject_->GetRotation().y;
	rot_.z = bulletObject_->GetRotation().z;

	
	rot_.x += 1000;
	rot_.z += 1000;

	bulletObject_->SetRotation({ rot_.x,rot_.y,rot_.z });
	bulletObject_->SetScale({ scale_,scale_,scale_ });
	bulletObject_->Update();

	//飛び散る
	tPos_.x += velocity_.x;
	tPos_.y += velocity_.y;
	tPos_.z += velocity_.z;

	bulletObject_->SetPosition(tPos_);

	if (pow_ > 0.5f) {
		isDead_ = true;
	}
}

//描画関数
void Effect::Draw()
{
	bulletObject_->Draw();
}

//スケール変更
void Effect::SetScale(float scale)
{
	scale_ = scale;
}
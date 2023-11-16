#include "Boss.h"


DirectX::XMFLOAT3 Boss::GetWorldPos()
{
	DirectX::XMFLOAT3 worldPos;

	worldPos.x = object_->GetPosition().x;
	worldPos.y = object_->GetPosition().y;
	worldPos.z = object_->GetPosition().z;

	return worldPos;
}

void Boss::Initialize(Model* model, XMFLOAT3 pos, Object3D* Object, Player* player)
{
	model_ = model;
	object_ = Object;
	player_ = player;
	pos_ = pos;

	hp = 50;
	arive_ = true;


	//frameModel_.reset();
	frameModel_ = std::make_unique<Model>();
	frameModel_->LoadFromObj("bossframe");

	//frameObject_.reset();
	frameObject_ = std::make_unique<Object3D>();
	frameObject_->SetModel(frameModel_.get());
	frameObject_->Initialize();
	frameObject_->SetScale({ 15,15,15 });
	frameObject_->SetPosition(object_->GetPosition());

	
	bulletCononModel_ = std::make_unique<Model>();
	bulletCononModel_->LoadFromObj("bossconon");

	//frameObject_.reset();
	bulletCononObject_ = std::make_unique<Object3D>();
	bulletCononObject_->SetModel(bulletCononModel_.get());
	bulletCononObject_->Initialize();
	bulletCononObject_->SetScale({ 15,15,15 });
	bulletCononObject_->SetPosition(object_->GetPosition());
	cononPos_ = object_->GetPosition();
}

void Boss::Update(bool move)
{
	arive_ = true;

	if (key_->keyInstantPush(DIK_1)) {
		state_ = 1;
	}
	else if (key_->keyInstantPush(DIK_2)) {
		state_ = 2;
	}
	else if (key_->keyInstantPush(DIK_3)) {
		state_ = 3;
	}


	switch (state_)
	{
	case 1:
		
		coolTime_--;
		
		/*visualRot_.x++;
		visualRot_.z++;*/

		cononPos_.y = object_->GetPosition().y - 25;

		break;
	case 2:

		coolTime_--;
		bulletDirRot_.y += 0.5f;
		//visualRot_.y += 0.5f;

		break;
	case 3:

		coolTime_--;
		bulletDirRot_.y += 0.5f;
		break;
	default:
		break;
	}
	

	if (!move) {
		
		
	}

	frame_++;
	addRot_.y = 2;
	addRot_.x = 2;

	frameRot_.y += addRot_.y;
	frameRot_.x += addRot_.x;

	if (coolTime_ == 0) {

		if (state_ == 1) {
			playerPos = player_->GetWorldPos();
			enemyPos = bulletCononObject_->GetPosition();

			differenceVec.x = enemyPos.x - playerPos.x;
			differenceVec.y = enemyPos.y - playerPos.y;
			differenceVec.z = enemyPos.z - playerPos.z;
			differenceVec /= 3;
			differenceVec.normalize();

			object_->SetRotation({ 0,0,0 });
			object_->Update();

			velocity_ = differenceVec;
			velocity_.multiplyMat4(object_->matWorld_);
			velocity_ /= 10;
		}
		else if (state_ == 2) {
			Vector3 start = { GetWorldPos().x,GetWorldPos().y,GetWorldPos().z };
			Vector3 end({ 0,0,0 });
			Vector3 length = { 0, 0, 10 };
			Vector3 frontVec = { 0, 0, 0 };

			//終点座標を設定
			end.x = start.x + length.x;
			end.y = start.y + length.y;
			end.z = start.z + length.z;

			//回転を考慮した座標を設定
			end.x = start.x + sinf(bulletDirRot_.y / 40);
			end.z = start.z + cosf(bulletDirRot_.y / 40);

			//始点と終点から正面ベクトルを求める
			frontVec.x = end.x - start.x;
			frontVec.y = end.y - start.y;
			frontVec.z = end.z - start.z;


			frontVec.normalize();


			velocity_ = frontVec;
		}
		else if (state_ == 3) {
			Vector3 start = { GetWorldPos().x,GetWorldPos().y,GetWorldPos().z };
			Vector3 end({ 0,0,0 });
			Vector3 length = { 0, 0, 10 };
			Vector3 frontVec = { 0, 0, 0 };

			//終点座標を設定
			end.x = start.x + length.x;
			end.y = start.y + length.y;
			end.z = start.z + length.z;

			//回転を考慮した座標を設定
			end.x = start.x + sinf(bulletDirRot_.y);
			end.z = start.z + cosf(bulletDirRot_.y);

			//始点と終点から正面ベクトルを求める
			frontVec.x = end.x - start.x;
			frontVec.y = end.y - start.y;
			frontVec.z = end.z - start.z;

			frontVec.normalize();

			velocity_ = frontVec;
		}
		

		
		if (state_ == 1) {
			std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();

			newBullet->Initialize(cononPos_, velocity_, bulletModel_, 3);

			//弾を登録する
			bullets_.push_back(std::move(newBullet));
		}
		else if (state_ == 2) {
			for (int i = 0; i < 2; i++) {
				std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();

				if (i == 0) {
					newBullet->Initialize(cononPos_, velocity_, bulletModel_, 0);
				}
				else {
					newBullet->Initialize(cononPos_, velocity_, bulletModel_, 1);
				}

				//弾を登録する
				bullets_.push_back(std::move(newBullet));

			}
		}
		else if (state_ == 3) {
			for (int i = 0; i < 2; i++) {
				std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();

				if (i == 0) {
					newBullet->Initialize(cononPos_, velocity_, bulletModel_, 0);
				}
				else {
					newBullet->Initialize(cononPos_, velocity_, bulletModel_, 1);
				}

				//弾を登録する
				bullets_.push_back(std::move(newBullet));

			}
		}
		
		coolTime_ = 5;
	}

	//デスフラグが立った弾を削除
	bullets_.remove_if([](std::unique_ptr<BossBullet>& bullet) {
		return bullet->IsDead();
		});

	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Update();
	}

	if (key_->keyInstantPush(DIK_L)) {
		bossLimit_.x *= -1;
	}
	if (key_->keyInstantPush(DIK_K)) {
		bossLimit_.y *= -1;
	}

	object_->SetPosition(pos_);
	object_->SetRotation(visualRot_);
	frameObject_->SetRotation(frameRot_);
	frameObject_->SetPosition(object_->GetPosition());

	bulletCononObject_->SetPosition(cononPos_);
	bulletCononObject_->Update();

	object_->Update();
	frameObject_->Update();
}

void Boss::Draw()
{
	object_->Draw();
	frameObject_->Draw();
	bulletCononObject_->Draw();

	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Draw();
	}
}

void Boss::OnCollision()
{
	hp--;

	if (hp <= 0) {
		arive_ = false;
	}
}

void Boss::SetBulletModel(Model* model)
{
	bulletModel_ = model;
}

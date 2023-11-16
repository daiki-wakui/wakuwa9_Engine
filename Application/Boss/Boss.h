#pragma once
#include "Model.h"
#include "Object3D.h"
#include "BossBullet.h"
#include "Player.h"
#include "KeyBoard.h"
#include "Vector2.h"

#include <memory>

class Boss
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	KeyBoard* key_ = KeyBoard::GetInstance();

	float frame_;

	Model* model_;
	Object3D* object_;

	std::unique_ptr<Model> frameModel_;
	std::unique_ptr<Object3D> frameObject_;

	Player* player_ = nullptr;
	XMFLOAT3 playerPos;
	XMFLOAT3 enemyPos;
	Vector3	differenceVec;

	XMFLOAT3 pos_;
	XMFLOAT3 visualRot_;
	XMFLOAT3 bulletDirRot_;
	XMFLOAT3 frameRot_;
	XMFLOAT3 addRot_;


	Model* bulletModel_;

	std::list<std::unique_ptr<BossBullet>> bullets_;

	int32_t coolTime_ = 10;
	int32_t coolCount_ = 0;

	int32_t state_ = 0;

	Vector3 velocity_ = { 0,0,0 };
	Vector2 bossLimit_ = { 100,100 };

public:

	bool arive_ = false;
	int32_t hp = 50;

	DirectX::XMFLOAT3 GetWorldPos();

	void Initialize(Model* model, XMFLOAT3 pos, Object3D* Object,Player* player);
	void Update(bool move);
	void Draw();
	void OnCollision();

	void SetBulletModel(Model* model);

	int32_t GetHP() const { return hp; }
	bool GetArive() const { return arive_; }

	const std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bullets_; }
};


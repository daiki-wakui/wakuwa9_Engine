#pragma once
#include "Model.h"
#include "Object3D.h"
#include "BossBullet.h"
#include "Player.h"
#include "KeyBoard.h"
#include "Vector2.h"
#include "Effect.h"

#include <memory>

enum pArea
{
	LTop,
	LBottom,
	RTop,
	RBottom
};

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

	Model* frameModel_;
	std::unique_ptr<Object3D> frameObject_;

	Model* bulletCononModel_;
	std::unique_ptr<Object3D> bulletCononObject_;

	Player* player_ = nullptr;
	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3	differenceVec;

	Vector3 pos_;
	Vector3 visualRot_;
	XMFLOAT3 bulletDirRot_;
	Vector3 frameRot_;
	XMFLOAT3 addRot_;

	//しっぽにつかう
	Vector3 frontVec = { 0, 0, 0 };
	Vector3 telePos_;
	bool iaAttack_;
	float radi_;
	float angle_;
	float angle2_;
	float movetime_;
	Vector3 attackStart_;
	Vector3 attackEnd_;

	Vector3 rokStart_;
	Vector3 rokEnd_;
	bool isAk_;
	float atting_;
	float atting2_;
	bool backAk_;

	Vector3 endStart_;
	bool endAk_;
	Vector3 rotLeapS_;
	Vector3 rotLeapE_;
	Vector3 tealRot_;

	Vector3 teleLen_;
	float len_;

	bool grandtele_;
	Vector2 randShack_;
	float shackTimer_;
	bool startAtk_;
	bool isRokAtk_;
	float atkTime_[3];

	std::list<std::unique_ptr<Effect>> effects_;
	bool isEffect_;
	//

	Vector3 cononPos_;
	Vector3 centerPos_;

	int32_t moveTimer_;
	int32_t randMoveChange_;

	Model* bulletModel_;

	std::list<std::unique_ptr<BossBullet>> bullets_;

	int32_t coolTime_ = 10;
	int32_t coolCount_ = 0;

	int32_t state_ = 0;

	Vector3 velocity_ = { 0,0,0 };
	Vector2 bossLimit_ = { 140,120 };

	int32_t toPlayerArea_ = 0;

	bool isDisappear_;
	bool isPop_;

	float moveT_;
	Vector3 disappearStart_;
	Vector3 disappearEnd_;

	Vector3 popStart_;
	Vector3 popEnd_;

	Vector3 vPos_;
	Vector3 vScale_;

	void Move();

	int32_t randState_;


	int32_t movementPattern_[10];
	int32_t movementPatternCount_;

public:

	bool arive_ = false;
	int32_t hp = 50;

	Vector3 GetWorldPos();

	void Initialize(Model* model, Vector3 pos, Object3D* Object,Player* player);
	void Update(bool move);
	void Draw();
	void OnCollision();

	void SetBulletModel(Model* model);
	void SetBossModels(Model* framemodel, Model* cannonModel);

	int32_t GetHP() const { return hp; }
	bool GetArive() const { return arive_; }

	const std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bullets_; }

	Vector3 GetBossteleWorldPos();
};


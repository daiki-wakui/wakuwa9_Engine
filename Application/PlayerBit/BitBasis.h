#pragma once
#include "BaseObject.h"
#include "BitMove.h"
#include "BitBullet.h"

class BitBasis : public BaseObject
{
public:
	//コンストラクタ、デストラクタ
	BitBasis();
	~BitBasis();
	//初期化
	void Initialize(Model* model, Object3D* object) override;
	//毎フレーム
	void Update() override;
	//描画
	void Draw() override;

	Vector3 GetWorldPos() override;
	Vector3 GetScale() override;
	void OnCollison() override;
public:

	const float BULLET_VECTOR_Y_VOLUE = 5.0f;
	const float BULLET_RT_VECTOR_RATE = -150.0f;


private:
	KeyBoard* input_ = KeyBoard::GetInstance();
	GamePad* inputPad_ = GamePad::GetInstance();
	Model* thismodel_;

	std::unique_ptr<BitMove> move_ = std::make_unique<BitMove>();

	std::unique_ptr<Model> playerBulletCubeModel_;
	std::unique_ptr<Object3D> bulletObject_;
	std::unique_ptr<Object3D> reticle3DObject_ = std::make_unique<Object3D>();


	std::list<std::unique_ptr<BaseObject>> bullets_;

	Vector3 bulletRTVec_ = { 0,0,0 };
	Vector3 bulletRTPos_ = { 0,0,0 };
	Vector3 bulletVec_ = { 0,0,0 };

	uint8_t cooltime_;

	Vector3 eye_;

protected:


	Object3D* thisObject_;
};


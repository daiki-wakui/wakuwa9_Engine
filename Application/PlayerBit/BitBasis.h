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
private:
	KeyBoard* input_ = KeyBoard::GetInstance();
	GamePad* inputPad_ = GamePad::GetInstance();
	Model* thismodel_;

	std::unique_ptr<BitMove> move_ = std::make_unique<BitMove>();

	std::unique_ptr<Model> playerBulletCubeModel_;
	std::unique_ptr<Object3D> bulletObject_;

	std::list<std::unique_ptr<BaseObject>> bullets_;
protected:


	Object3D* thisObject_;
};


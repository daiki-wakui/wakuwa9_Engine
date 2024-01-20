#pragma once
#include "BaseObject.h"

class PlayerMove;
class PlayerCamera;

class PlayerBasis : public BaseObject
{
public:
	//コンストラクタ、デストラクタ
	PlayerBasis();
	~PlayerBasis();
	//初期化
	void Initialize(Model* model, Object3D* object) override;
	//毎フレーム
	void Update() override;
	//描画
	void Draw() override;

	Vector3 GetWorldPos() override;
	Vector3 GetScale() override;
	void OnCollison() override;
private:	//メンバ変数

	Model* thismodel_;

	std::unique_ptr<PlayerMove> move_ = std::make_unique<PlayerMove>();
	std::unique_ptr<PlayerCamera> camera_ = std::make_unique<PlayerCamera>();
protected:

	Object3D* thisObject_;
};


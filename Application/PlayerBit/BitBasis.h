#pragma once
#include "BaseObject.h"
#include "BitMove.h"

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

	Model* thismodel_;

	std::unique_ptr<BitMove> move_ = std::make_unique<BitMove>();
protected:

	Object3D* thisObject_;
};


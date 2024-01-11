#pragma once
#include "BaseObject.h"

class BitBullet : public BaseObject 
{
public:
	//コンストラクタ、デストラクタ
	BitBullet();
	~BitBullet();
	//初期化
	void Initialize(Model* model, Object3D* object) override;
	//毎フレーム
	void Update() override;
	//描画
	void Draw() override;

	Vector3 GetWorldPos() override;


private:	//メンバ変数

	Model* thismodel_;
	Object3D* obj_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;

protected:

	std::unique_ptr<Object3D> thisObject_;
};


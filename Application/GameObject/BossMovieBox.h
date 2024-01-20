#pragma once
#include "BaseObject.h"

class BossMovieBox : public BaseObject
{
public:

	BossMovieBox();
	~BossMovieBox();

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

protected:

	Object3D* thisObject_;
};


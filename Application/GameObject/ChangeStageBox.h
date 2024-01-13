#pragma once
#include "BaseObject.h"

class ChangeStageBox : public BaseObject
{
public:

	ChangeStageBox();
	~ChangeStageBox();

	//初期化
	void Initialize(Model* model, Object3D* object) override;
	//毎フレーム
	void Update() override;
	//描画
	void Draw() override;

	Vector3 GetWorldPos() override;
private:	//メンバ変数

	Model* thismodel_;

protected:

	Object3D* thisObject_;
};


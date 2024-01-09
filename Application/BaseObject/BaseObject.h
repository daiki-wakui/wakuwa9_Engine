#pragma once
#include "Model.h"
#include "Object3D.h"

class BaseObject
{
public:
	virtual ~BaseObject() = default;
	//初期化
	virtual void Initialize(Model* model,Object3D* object) = 0;
	//毎フレーム
	virtual void Update() = 0;
	//描画
	virtual void Draw() = 0;

	virtual Vector3 GetWorldPos() = 0;

	virtual void SetInfo(Vector3 pos, Vector3 velocity);
protected:

	Vector3 pos_;
	Vector3 velocity_;
};


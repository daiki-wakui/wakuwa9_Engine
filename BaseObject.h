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

};


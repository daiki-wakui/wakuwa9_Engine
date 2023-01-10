#pragma once
#include "Model.h"
#include "Object3D.h"
#include "KeyBoard.h"
#include <memory>
#include <list>

class Enemy
{
private:
	Object3D* enemyObject_;
	KeyBoard* input_;

	XMFLOAT3 pos_;

public:

	void Initialize(Object3D* enemyObject,XMFLOAT3 pos);
	void Update();
	void Draw();

	XMFLOAT3 GetWorldPos();
};


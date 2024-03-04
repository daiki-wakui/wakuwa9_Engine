#pragma once
#include "Model.h"
#include "Object3D.h"
#include "Player.h"

#include <memory>

class BossCanon
{
private:
	Player* info_;
	std::unique_ptr<Object3D> Object_;

	Vector3 pos_;
	Vector3 vro_;

public:

	void Initialize(Model* model, Vector3 pos, Player* player);

	void Update();

	void Draw();

};


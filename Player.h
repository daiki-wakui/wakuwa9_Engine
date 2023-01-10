#pragma once
#include "Model.h"
#include "Object3D.h"
#include "KeyBoard.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

class Player
{
private:

	Model* playerModel_;
	Object3D* playerObject_;
	KeyBoard* input_;


	XMFLOAT3 pos3d;
	XMFLOAT3 pos3d2;

	float dashPower = 10.0f;
	bool isStep = false;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	int coolTime = 7;


	

public:

	void Initialize(Model* playerModel, Object3D* playerObject, KeyBoard* input);
	void Update();
	void Draw();

	//’eƒŠƒXƒg‚ðŽæ“¾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//XMFLOAT3 GetWorldPos();
};


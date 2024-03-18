#pragma once
#include "BaseObject.h"
#include "GameObjectList.h"
#include "LevelEditor.h"

#include <memory>
#include <list>

class GameCollisons
{
public:

	GameCollisons();
	~GameCollisons();

	void Update();

	void SetObjectList(LevelEditor* lv);

	bool GetIsStageChange() { return isChange_; }
	void SetIsStageChanhe(bool isChange) { isChange_ = isChange; }

private:

	bool isChange_;

	LevelEditor* lv_;

	Vector3 posA_;
	Vector3 posB_;
	float scaleA_;
	float scaleB_;

	//当たり判定関数
	bool Collison(Vector3 posa, Vector3 posb, float aScale, float bScale);
};


#include "GameCollisons.h"

GameCollisons::GameCollisons()
{
}

GameCollisons::~GameCollisons()
{
}

void GameCollisons::Update()
{
	for (const std::unique_ptr<BaseObject>& objectB : lv_->GetObjects()) {

		if (objectB->GetName() == "stageObject") {
			continue;
		}

		if (objectB->GetName() == "player") {
			continue;
		}

		posA_ = lv_->GetPlayer()->GetWorldPos();
		posB_ = objectB->GetWorldPos();
		scaleA_ = lv_->GetPlayer()->GetScale().x;
		scaleB_ = objectB->GetScale().x;

		if (Collison(posA_, posB_, scaleA_, scaleB_)) {
			lv_->GetPlayer()->OnCollison();
			objectB->OnCollison();
		}
	}
}

void GameCollisons::SetObjectList(LevelEditor* lv)
{
	lv_ = lv;
}

bool GameCollisons::Collison(Vector3 posa, Vector3 posb, float aScale, float bScale)
{
	float r = aScale + bScale;

	Vector3 dis;
	dis.x = posb.x - posa.x;
	dis.y = posb.y - posa.y;
	dis.z = posb.z - posa.z;

	if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {
		return true;
	}

	return false;
}

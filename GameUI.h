#pragma once
#include "TextureManager.h"
#include "Sprite.h"

class GameUI : public TextureManager
{
private:

	//スプライト
	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> titleUISprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> fillSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> sceneSprite_ = std::make_unique<Sprite>();

public:

	GameUI();
	~GameUI();

	Sprite* GetsceneSprite() { return sceneSprite_.get(); }

	void Initialize();
	void Update();
	void Draw();
	void OffDarw();

	void ChangeEffect();
};


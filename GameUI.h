#pragma once
#include "TextureManager.h"
#include "Sprite.h"

class GameUI : public TextureManager
{

private:

	std::unique_ptr<Sprite> ddsSprite_ = std::make_unique<Sprite>();

	//スプライト
	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> titleUISprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> fillSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> sceneSprite_ = std::make_unique<Sprite>();

 	float ChangeAlpha_ = 0;
public:

	float GetSceneChangeAlpha() { return ChangeAlpha_; }

	GameUI();
	~GameUI();

	void Initialize();
	void TitleUpdate(bool sceneChange);
	void Draw();
	void OffDraw();
};


#pragma once
#include "BaseScene.h"
#include "Boss.h"
#include "Player.h"

#include <memory>
#include <list>

class CreateBossScene : public BaseScene
{
private:

	KeyBoard* keyboard_ = KeyBoard::GetInstance();
	GamePad* gamePad_ = GamePad::GetInstance();


private:

	//boss
	std::unique_ptr<Boss> boss_ = std::make_unique<Boss>();
	std::unique_ptr<Object3D> bossObject_ = {};
	//player
	std::unique_ptr<Player> player_ = std::make_unique<Player>();


public:

	//初期化
	void Initialize()override;

	//終了
	void Finalize()override;

	//更新処理
	void Update()override;

	//描画
	void Draw()override;
};


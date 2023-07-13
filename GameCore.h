#pragma once
#include "Framework.h"

#include "Sprite.h"
#include "Sound.h"
#include "Model.h"

#include "PostEffect.h"

#include "LevelLoader.h"

#include "EventBox.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

#include <fstream>
#include <cassert>
#include <map>

#include <memory>
#include <string>
#include <DirectXTex.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <cstdint>

class GameCore : public Framework 
{
private:

	DirectXBasis* directX_ = Framework::GetDirectX();
	SpriteBasis* spBasis_ = Framework::GetSpBasis();
	WindowsApp* windows_ = Framework::GetWindows();
	KeyBoard* keyboard_ = Framework::GetInput();
	GamePad* gamePad_ = Framework::GetInputPad();
	ImGuiManager* imGuiM_ = Framework::GetImGui();
	Sound* sound_ = Framework::GetSound();

	//OBJからモデルを読み込む
	std::unique_ptr<Model> floorModel_;
	std::unique_ptr<Model> skydomModel_;

	std::unique_ptr<Object3D> skyObject_;
	std::unique_ptr<Object3D> objectFloor_;

	std::unique_ptr<Model> playerModel_;
	std::unique_ptr<Object3D> playerObject_;

	std::unique_ptr<Model> podModel_;
	std::unique_ptr<Object3D> podObject_;

	std::unique_ptr<Object3D> bulletObject_;


	std::unique_ptr<Model> enemyModel_;
	std::unique_ptr<Object3D> bossObject_;


	std::unique_ptr<Model> bossModel_;
	std::unique_ptr<Model> cubeModel_;

	std::unique_ptr<Model> filedModel_;

	

	std::unique_ptr<EventBox> eventBox_;

	//FBX
	std::unique_ptr<FbxModel> testModel_;
	std::unique_ptr<FbxObject3d> testObj_;

	
	//スプライト
	std::unique_ptr<Sprite> sprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> backSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> playerHPSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> bossHPSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> gameoverSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> gameclearSprite_ = std::make_unique<Sprite>();

	//画像
	int32_t tex1_ = 0;
	int32_t tex2_ = 0;
	int32_t tex3_ = 0;

	int32_t backTex_ = 0;
	int32_t postTex = 100;

	int32_t playerHP_ = 0;
	int32_t bossHP_ = 0;

	int32_t title_ = 0;
	int32_t gameover_ = 0;
	int32_t gameclear_ = 0;

	bool HitBox = false;

	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_ = std::make_unique<PostEffect>();
	std::unique_ptr<PostEffect> gaussianEffect_ = std::make_unique<PostEffect>();


	

	//レベルエディタ
	LevelData* levelData_ = nullptr;

	std::map<std::string, Model*> models;
	std::vector<Object3D*> objects;

	std::unique_ptr<Object3D> newObject[100] = {};

	std::unique_ptr<Enemy> newEnemy[100] = {};

	int32_t objSize = 0;
	int32_t enemySize = 0;


	std::unique_ptr<Player> player_ = std::make_unique<Player>();
	//Player* player = new Player;
	//player->Initialize(playerModel_, playerObject, input_, podObject);

	std::unique_ptr<Boss> boss_ = std::make_unique<Boss>();

	std::list<std::unique_ptr<Enemy>> enemys_;

	//std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	std::unique_ptr<Enemy> newEnemy2 = std::make_unique<Enemy>();
	std::unique_ptr<Enemy> newEnemy3 = std::make_unique<Enemy>();

	int32_t scene = 1;

public:

	//初期化
	void Initialize() override;

	void Finalize() override;
	
	//毎フレーム
	void Update() override;

	void Draw() override;

	void Inport(Model* model, int32_t size);
};


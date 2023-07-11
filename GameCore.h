#pragma once
#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "ImGuiManager.h"
#include "Sprite.h"
#include "SpriteBasis.h"
#include "Sound.h"
#include "DirectionalLight.h"
#include "LightGroup.h"
#include "Object3D.h"
#include "Model.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "DirectionalLight.h"

#include <memory>

#include <string>
#include <DirectXTex.h>

#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <cstdint>

#include "PostEffect.h"

#include "LevelLoader.h"
#include <fstream>
#include <cassert>
#include <map>
#include "EventBox.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "GamePad.h"

class GameCore
{
private:

	//windowsAPIの生成クラス
	std::unique_ptr<WindowsApp> windows_ = std::make_unique<WindowsApp>();

	//keyboradクラスの生成
	std::unique_ptr<KeyBoard> keyboard_ = std::make_unique<KeyBoard>();

	std::unique_ptr<GamePad> gamePad_ = std::make_unique<GamePad>();

	//DirectXの基盤生成クラス
	std::unique_ptr<DirectXBasis> directX_ = std::make_unique<DirectXBasis>();

	//imgui
	std::unique_ptr<ImGuiManager> imGuiM_ = std::make_unique<ImGuiManager>();

	//ライトグループ
	std::unique_ptr<LightGroup> lightGroup = std::make_unique<LightGroup>();

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
	std::unique_ptr<Object3D> enemyObject_;
	std::unique_ptr<Object3D> enemyObject2_;
	std::unique_ptr<Object3D> enemyObject3_;
	std::unique_ptr<Object3D> bossObject_;


	std::unique_ptr<Model> bossModel_;
	std::unique_ptr<Model> cubeModel_;

	std::unique_ptr<Model> filedModel_;

	

	std::unique_ptr<EventBox> eventBox_;

	//FBX
	std::unique_ptr<FbxModel> testModel_;
	std::unique_ptr<FbxObject3d> testObj_;

	//スプライト基盤
	std::unique_ptr<SpriteBasis> spBasis_ = std::make_unique<SpriteBasis>();

	//スプライト
	std::unique_ptr<Sprite> sprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> backSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> playerHPSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> bossHPSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> gameoverSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> gameclearSprite_ = std::make_unique<Sprite>();
	

	int32_t state = 1;

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


	//サウンド
	std::unique_ptr<Sound> sound_ = std::make_unique<Sound>();

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
	void Initialize();

	void Finalize();

	void Inport(Model* model,int32_t size);

	//毎フレーム
	void Update();

	void Draw();

	WindowsApp* GetWindows() { return windows_.get(); }
	DirectXBasis* GetDirectX() { return directX_.get(); }
	ImGuiManager* GetImGui() { return imGuiM_.get(); }
	KeyBoard* GetInput() { return keyboard_.get(); }
};


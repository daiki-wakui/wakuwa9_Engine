#pragma once
#include "Framework.h"
#include "BaseScene.h"

#include "Model.h"
#include "Sprite.h"
#include "EventBox.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyCharge.h"
#include "Boss.h"
#include "CollisionBox.h"

#include "LevelLoader.h"
#include "Easing.h"
#include "Effect.h"

#include <memory>
#include <list>

class GameScene : public BaseScene
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	DirectXBasis* directX_ = nullptr;
	SpriteBasis* spBasis_ = nullptr;
	WindowsApp* windows_ = nullptr;
	ImGuiManager* imGuiM_ = nullptr;
	Sound* sound_ = nullptr;

	KeyBoard* keyboard_ = KeyBoard::GetInstance();
	GamePad* gamePad_ = GamePad::GetInstance();
	
private:	//メンバ変数

	//OBJからモデルを読み込む
	//天球
	std::unique_ptr<Model> skydomModel_;
	std::unique_ptr<Object3D> skyObject_;

	//player
	std::unique_ptr<Model> playerModel_;
	std::unique_ptr<Object3D> playerObject_;

	//ポッド
	std::unique_ptr<Model> podModel_;
	std::unique_ptr<Object3D> podObject_;
	std::unique_ptr<Object3D> bulletObject_;

	//敵
	std::unique_ptr<Model> enemyModel_;
	std::unique_ptr<Model> enemyModel2_;

	//ボス
	std::unique_ptr<Model> bossModel_;
	std::unique_ptr<Object3D> bossObject_;

	//フィールドに使うモデル
	std::unique_ptr<Model> cubeModel_;
	std::unique_ptr<Model> playerBulletCubeModel_;
	std::unique_ptr<Model> filedCubeModel_;
	std::unique_ptr<Model> filedModel_;
	std::unique_ptr<Model> filedTentoModel_;
	std::unique_ptr<Model> filedTouModel_;


	std::unique_ptr<Model> poriModel_;
	std::unique_ptr<Object3D> poriObject_;


	std::unique_ptr<Model> shadowModel_;
	std::unique_ptr<Object3D> shadowObject_;

	//ボス戦に入るボックス
	std::unique_ptr<EventBox> eventBox_;

	//FBX
	std::unique_ptr<FbxModel> testModel_;
	std::unique_ptr<FbxObject3d> testObj_;
	XMFLOAT3 podRot = {};

	std::unique_ptr<Model> konnpeModel_;

	//スプライト
	std::unique_ptr<Sprite> playerHPSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> bossHPSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> gameoverSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> gameclearSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> fillSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> sceneSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> reticleSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> sSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> iventSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> RBSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> dFilterSprite_ = std::make_unique<Sprite>();

	bool isLifeOne_ = false;
	float fillAlpha_ = 0;
	int32_t fillTimer_ = 0;
	int32_t sockSound_ = 0;

	int32_t pow_ = 0;
	int32_t count_ = 0;
	float wTimer_ = 0;
	float wMax_ = 10;
	float popFrame_ = 0;

	Vector3 wSize_ = {};
	bool movieEnd_ = false;

	std::unique_ptr<Sprite> waringSprite_ = std::make_unique<Sprite>();

	//画像

	//HP
	int32_t playerHP_ = 0;
	int32_t bossHP_ = 0;

	//タイトル
	int32_t title_ = 0;
	int32_t gameover_ = 0;
	int32_t gameclear_ = 0;
	int32_t scene_ = 0;
	int32_t fillter_ = 0;

	int32_t reticleImage_ = 0;

	int32_t tesImage_ = 0;
	int32_t targetImage_ = 0;

	int32_t iventImage_ = 0;
	int32_t warningImage_ = 0;
	int32_t manualImageRB_ = 0;

	int32_t exImage_ = 0;

	int32_t damageFilter_ = 0;


	//player
	std::unique_ptr<Player> player_ = std::make_unique<Player>();

	//boss
	std::unique_ptr<Boss> boss_ = std::make_unique<Boss>();

	//レベルエディタ関連
	LevelData* levelData_ = nullptr;

	std::map<std::string, Model*> models;
	std::vector<Object3D*> objects;
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::list<std::unique_ptr<EnemyCharge>> enemycharges_;
	std::list<std::unique_ptr<CollisionBox>> collisions_;


	std::unique_ptr<Object3D> newObject[1000] = {};
	std::unique_ptr<Enemy> newEnemy[1000] = {};
	std::unique_ptr<EnemyCharge> newEnemy2[1000] = {};
	std::unique_ptr<CollisionBox> collBox[1000] = {};

	int32_t objSize_ = 0;
	int32_t enemySize_ = 0;
	int32_t enemySize2_ = 0;
	int32_t collSize_ = 0;

	//パーティクル
	std::unique_ptr<ParticleManager> particleMan_ = std::make_unique<ParticleManager>();
	bool BulletEffect = false;

	bool start_ = false;
	bool change_ = false;
	XMFLOAT2 pos = { 0,0 };
	float power = -1;

	bool playBGM_ = false;
	bool bossBGM_ = false;

	
	XMFLOAT2 reticleSize_ = {0,720};

	XMFLOAT3 screenPos_ = {};
	XMFLOAT2 spPos_ = {};

	float alpha_ = 0;
	bool isIvent_ = false;

	Vector3 iventEye_ = { 360,100,700 };
	Vector3 iventTarget_ = { 360, 100,1000 };

	Vector3 endEye_ = { 360,10,600 };
	float timer_ = 0;
	float maxTime_ = 300;


	XMFLOAT3 screenPosPlayer_ = {};
	bool manualOK_ = false;
	float alphaRB_ = 0;
	float addYRB_ = 0;
	XMFLOAT2 posRB_ = {};

	std::list<std::unique_ptr<Effect>> effects_;
	bool isEffect_ = 0;
	XMFLOAT3 startEffect_ = {};

	int32_t coolTime_ = 7;
	bool isShotEffect_ = false;

	float ChangeAlpha_ = 1;
	bool resetOn_ = false;

	bool hitBox_ = false;
	bool isTutorial_ = false;
	bool isChangeStage_ = false;

public:

	bool Collison(XMFLOAT3 posa, XMFLOAT3 posb, float aScale, float bScale);
	void AllCollison();

	//初期化
	void Initialize()override;

	//終了
	void Finalize()override;

	//毎フレーム
	void Update()override;

	void SpriteUpdate();
	void SpriteDraw();
	void ObjectUpdate();

	//描画
	void Draw()override;

	void pDraw();

	void EditorLoad();

	void ReLoad(const std::string filename);

	void Inport(Model* model, int32_t size);

	void Reset();

public:

	Player* GetPlayer() { return player_.get(); }
	bool GetChange() { return change_; }
	void SetChange(bool change) { change_ = change; }
	bool GetStart() { return start_; }
	void SetStart(bool start) { start_ = start; }
	void SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound_);
};


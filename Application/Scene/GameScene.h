#pragma once
#include "Framework.h"
#include "BaseScene.h"
#include "GameUI.h"

#include "EventBox.h"
#include "Enemy.h"
#include "CollisionBox.h"
#include "Door.h"

#include "LevelLoader.h"

#include "Effect.h"
#include "DebugPoint.h"

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

	DirectXBasis* directX_ = DirectXBasis::GetInstance();
	SpriteBasis* spBasis_ = SpriteBasis::GetInstance();
	WindowsApp* windows_ = WindowsApp::GetInstance();
	ImGuiManager* imGuiM_ = ImGuiManager::GetInstance();
	Sound* sound_ = Sound::GetInstance();

	KeyBoard* keyboard_ = KeyBoard::GetInstance();
	GamePad* gamePad_ = GamePad::GetInstance();
	
private:	//メンバ変数

	//OBJからモデルを読み込む
	//天球
	std::unique_ptr<Object3D> skyObject_;
	std::unique_ptr<Object3D> poriObject_;
	std::unique_ptr<Object3D> shadowObject_;

	//player,肩のポッド
	std::unique_ptr<Object3D> playerObject_;
	std::unique_ptr<Object3D> podObject_;
	std::unique_ptr<Object3D> bulletObject_;

	//ボス
	std::unique_ptr<Object3D> bossObject_;	

	//ボス戦に入るボックス
	std::unique_ptr<EventBox> eventBox_;
	std::unique_ptr<EventBox> ChangeBox_;

	Vector3 podRot = {};
	int32_t sockSound_ = 0;

	std::unique_ptr<GameUI> gameUI_ = std::make_unique<GameUI>();

	//player
	std::unique_ptr<Player> player_ = std::make_unique<Player>();

	//boss
	std::unique_ptr<Boss> boss_ = std::make_unique<Boss>();

	std::unique_ptr<DebugPoint> dPoint_ = std::make_unique<DebugPoint>();


	//レベルエディタ関連
	LevelData* levelData_ = nullptr;

	std::map<std::string, Model*> models;
	std::vector<Object3D*> objects;
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::list<std::unique_ptr<CollisionBox>> collisions_;
	std::list<std::unique_ptr<Door>> doors_;

	
	std::unique_ptr<Object3D> newObject[1000] = {};
	std::unique_ptr<Enemy> newEnemy[1000] = {};
	std::unique_ptr<CollisionBox> collBox[1000] = {};
	std::unique_ptr<Door> newDoor[100] = {};

	int32_t objSize_ = 0;
	int32_t enemySize_ = 0;
	int32_t enemySize2_ = 0;
	int32_t collSize_ = 0;
	int32_t doorCount_ = 0;

	//パーティクル
	std::unique_ptr<ParticleManager> particleMan_ = std::make_unique<ParticleManager>();
	bool BulletEffect = false;

	bool start_ = false;
	bool change_ = false;
	XMFLOAT2 pos = { 0,0 };
	float power = -1;

	bool playBGM_ = false;
	bool bossBGM_ = false;
	
	Vector3 screenPos_ = {};
	XMFLOAT2 spPos_ = {};

	float alpha_ = 0;
	bool isIvent_ = false;

	Vector3 iventEye_ = { 450,100,750 };
	Vector3 iventTarget_ = { 450, 100,1000 };

	Vector3 endEye_ = { 450,10,600 };
	float timer_ = 0;
	float maxTime_ = 300;

	float addYRB_ = 0;
	XMFLOAT2 posRB_ = {};

	std::list<std::unique_ptr<Effect>> effects_;
	bool isEffect_ = 0;
	Vector3 startEffect_ = {};

	int32_t coolTime_ = 7;
	bool isShotEffect_ = false;
	
	bool resetOn_ = false;

	bool hitBox_ = true;
	bool isTutorial_ = false;
	bool isChangeStage_ = false;
	
	bool isDebugBoss_ = false;
	bool isShake_;
	int32_t shakeTimer_;

public:
	
	bool Collison(Vector3 posa, Vector3 posb, float aScale, float bScale);
	void AllCollison();
	void Object3DGenerate();

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

	void EditorLoad(const std::string filename);

	void ReLoad(const std::string filename);

	void Inport(Model* model, int32_t size);

	void Reset();

public:	//getter,setter

	DebugPoint* GetDebugPoint() { return dPoint_.get(); }
	Player* GetPlayer() { return player_.get(); }
	Boss* GetBoss() { return boss_.get(); }

	bool GetChange() { return change_; }
	void SetChange(bool change) { change_ = change; }
	bool GetStart() { return start_; }
	void SetStart(bool start) { start_ = start; }

	bool GetHitBox() { return hitBox_; }
	bool GetIsIvent() { return isIvent_; }
};


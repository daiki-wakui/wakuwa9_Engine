#pragma once
#include "Framework.h"
#include "BaseScene.h"
#include "GameUI.h"

#include "EventBox.h"
#include "Enemy.h"
#include "Door.h"

#include "LevelLoader.h"

#include "Effect.h"
#include "DebugPoint.h"

#include <memory>
#include <list>

#include "BaseObject.h"
#include "GameObjectList.h"

#include "LevelEditor.h"
#include "GameCollisons.h"

#include "constJsonValue.h"

/**
 * @file GameScene
 * @brief ゲームシーンを動かしているクラス
 */

class GameScene : public BaseScene
{
private:

	KeyBoard* keyboard_ = KeyBoard::GetInstance();
	GamePad* gamePad_ = GamePad::GetInstance();

private:	//メンバ変数
	std::unique_ptr<constJsonValue> json_;
	BaseObject* playerObj_;

	std::unique_ptr<GameCollisons> col_ = std::make_unique<GameCollisons>();
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

	Vector3 podRot = {};
	int32_t sockSound_ = 0;

	std::unique_ptr<GameUI> gameUI_ = std::make_unique<GameUI>();

	//boss
	std::unique_ptr<Boss> boss_ = std::make_unique<Boss>();

	
	//パーティクル
	std::unique_ptr<ParticleManager> particleMan_ = std::make_unique<ParticleManager>();
	bool BulletEffect = false;

	bool start_ = false;
	bool change_ = false;
	
	bool bossBGM_ = false;
	
	Vector3 screenPos_ = {};

	float alpha_ = 0;
	bool isIvent_ = false;

	Vector3 iventEye_ = { 450,100,750 };
	Vector3 iventTarget_ = { 450, 100,1000 };

	Vector3 endEye_ = { 450,10,600 };
	float timer_ = 0;
	float maxTime_ = 300;

	float addYRB_ = 0;

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

	Model3DManager* modelData_ = Model3DManager::GetInstance();

	std::unique_ptr<LevelEditor> lv = std::make_unique<LevelEditor>();

public:
	//3dオブジェクト生成
	void Object3DGenerate();

	//初期化
	void Initialize()override;

	//終了
	void Finalize()override;

	//更新処理
	void Update()override;
	//スプライト更新処理
	void SpriteUpdate();
	//スプライト描画
	void SpriteDraw();

	//3dオブジェクト更新処理
	void ObjectUpdate();

	//描画
	void Draw()override;

	//パーティクル描画
	void ParticleDraw();

	//シーンリセット関数
	void Reset();

public:	//getter,setter
	Boss* GetBoss() { return boss_.get(); }

	bool GetChange() { return change_; }
	void SetChange(bool change) { change_ = change; }
	bool GetStart() { return start_; }
	void SetStart(bool start) { start_ = start; }

	bool GetHitBox() { return hitBox_; }
	bool GetIsIvent() { return isIvent_; }
};


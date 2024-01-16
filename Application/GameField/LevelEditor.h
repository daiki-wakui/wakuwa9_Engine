#pragma once
#include "Model.h"
#include "EventBox.h"
#include "Object3D.h"
#include "LevelLoader.h"

#include "Enemy.h"
#include "Door.h"
#include "BaseObject.h"
#include "GameObjectList.h"
#include "Model3DManager.h"
#include "DebugPoint.h"

#include <memory>
#include <list>
#include <map>

/**
 * @file LevelEditor
 * @brief ゲームシーンクラスからの分離先として作ってるクラス
 */

class LevelEditor
{
private:

	const Vector3 PLAYER_SCALE = { 1.0f,1.0f,1.0f };
	const Vector3 LNIT_EYE = { 0.0f,20.0f,-30.0f };
	const Vector3 LNIT_TERGET = { 0.0f,10.0f,0.0f };
	const float DOOR_POS_VOLUE_X = 15.0f;
	const Vector3 BOSS_SCALE = { 15.0f,15.0f,15.0f };


private:

	Model3DManager* m = Model3DManager::GetInstance();

	std::list<std::unique_ptr<BaseObject>> gameObjects_;
	std::unique_ptr<BaseObject> baseObject_[1000] = {};
	int32_t objNum_ = 0;

	//レベルエディタ関連
	LevelData* levelData_ = nullptr;

	std::map<std::string, Model*> models;
	std::vector<Object3D*> objects;
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::list<std::unique_ptr<Door>> doors_;


	std::unique_ptr<Object3D> newObject[1000] = {};
	std::unique_ptr<Enemy> newEnemy[1000] = {};
	std::unique_ptr<Door> newDoor[100] = {};

	int32_t objSize_ = 0;
	int32_t enemySize_ = 0;
	int32_t enemySize2_ = 0;
	int32_t collSize_ = 0;
	int32_t doorCount_ = 0;



	std::unique_ptr<Object3D> playerObject_;
	std::unique_ptr<Object3D> podObject_;
	std::unique_ptr<DebugPoint> dPoint_ = std::make_unique<DebugPoint>();
	//boss
	//std::unique_ptr<Boss> boss_ = std::make_unique<Boss>();

public:

	//初期化
	void Initialize();

	//終了
	void Finalize();

	//更新処理
	void Update();
	//描画関数
	void Darw();

	//フィールド情報読み込み
	void EditorLoad(const std::string filename);
	//フィールド情報再読み込み
	void ReLoad(const std::string filename);
	//オブジェクト配置時の情報
	void Inport(Model* model, int32_t size);

};



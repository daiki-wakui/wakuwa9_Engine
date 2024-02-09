#pragma once
#include "Model.h"
#include <map>

/**
 * @file Model3DManager
 * @brief ゲームで使う3Dモデルの読み込みクラス
 */

class Model3DManager
{
protected:

	//タイトルで使うモデルデータ
	std::unique_ptr<Model> skydomTitleModel_ = std::make_unique<Model>();

	//ゲームシーンで使うモデルデータ
	std::unique_ptr<Model> skydomGameModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> playerModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> podModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> enemyModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> enemyModel2_ = std::make_unique<Model>();
	std::unique_ptr<Model> bossModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> bossBulletModel_ = std::make_unique<Model>();

	//フィールドに使うモデル
	std::unique_ptr<Model> cubeModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> playerBulletCubeModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> filedCubeModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> filedModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> filedTentoModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> filedTouModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> bossFiledModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> bossFiledGateModel_ = std::make_unique<Model>();

	std::unique_ptr<Model> drModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> LeftDoorModel_ = std::make_unique<Model>();

	std::unique_ptr<Model> poriModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> shadowModel_ = std::make_unique<Model>();

	std::unique_ptr<Model> konnpeModel_ = std::make_unique<Model>();

	std::unique_ptr<Model> bulletCononModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> frameModel_ = std::make_unique<Model>();

private:
	// charをキー、intを値として扱う連想配列
	std::map<std::string, Model*> models_;

	Model3DManager() = default;
	~Model3DManager() = default;
	//コピーコンストラクタ無効
	Model3DManager(const Model3DManager& obj) = delete;
	//代入演算子を無効
	Model3DManager& operator=(const Model3DManager& obj) = delete;
public:

	static Model3DManager* GetInstance();

	void insertModel();

	//ゲームシーンで使うモデル読み込み
	void LoadGame3DModel();

	Model* Get3DModel(std::string name);
};


#pragma once
#include "Model.h"

/**
 * @file Model3DManager
 * @brief ゲームで使う3Dモデルの読み込みクラス
 */

class Model3DManager
{
protected:

	//タイトルで使うモデルデータ
	std::unique_ptr<Model> skydomTitleModel_;

	//ゲームシーンで使うモデルデータ
	std::unique_ptr<Model> skydomGameModel_;
	std::unique_ptr<Model> playerModel_;
	std::unique_ptr<Model> podModel_;
	std::unique_ptr<Model> enemyModel_;
	std::unique_ptr<Model> enemyModel2_;
	std::unique_ptr<Model> bossModel_;
	std::unique_ptr<Model> bossBulletModel_;

	//フィールドに使うモデル
	std::unique_ptr<Model> cubeModel_;
	std::unique_ptr<Model> playerBulletCubeModel_;
	std::unique_ptr<Model> filedCubeModel_;
	std::unique_ptr<Model> filedModel_;
	std::unique_ptr<Model> filedTentoModel_;
	std::unique_ptr<Model> filedTouModel_;
	std::unique_ptr<Model> bossFiledModel_;
	std::unique_ptr<Model> bossFiledGateModel_;

	std::unique_ptr<Model> drModel_;
	std::unique_ptr<Model> LeftDoorModel_;

	std::unique_ptr<Model> poriModel_;
	std::unique_ptr<Model> poriFModel_;
	std::unique_ptr<Model> shadowModel_;

	std::unique_ptr<Model> konnpeModel_;

	std::unique_ptr<Model> bulletCononModel_ = std::make_unique<Model>();
	std::unique_ptr<Model> frameModel_ = std::make_unique<Model>();

public:

	//コンストラクタ、デストラクタ
	Model3DManager();
	~Model3DManager();

	//タイトルシーンで使うモデル読み込み
	void LoadTitle3DModel();

	//ゲームシーンで使うモデル読み込み
	void LoadGame3DModel();

};


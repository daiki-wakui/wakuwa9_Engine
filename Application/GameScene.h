#pragma once
#include "Framework.h"
#include "BaseScene.h"

#include "Model.h"
#include "Sprite.h"
#include "EventBox.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

#include "LevelLoader.h"

class GameScene : public BaseScene
{
private:

	DirectXBasis* directX_ = nullptr;
	SpriteBasis* spBasis_ = nullptr;
	WindowsApp* windows_ = nullptr;
	ImGuiManager* imGuiM_ = nullptr;
	Sound* sound_ = nullptr;

	KeyBoard* keyboard_ = nullptr;
	GamePad* gamePad_ = nullptr;
	
private:

	//OBJ���烂�f����ǂݍ���
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


	//�X�v���C�g
	std::unique_ptr<Sprite> sprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> backSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> playerHPSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> bossHPSprite_ = std::make_unique<Sprite>();

	std::unique_ptr<Sprite> titleSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> gameoverSprite_ = std::make_unique<Sprite>();
	std::unique_ptr<Sprite> gameclearSprite_ = std::make_unique<Sprite>();

	//�摜
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

	std::unique_ptr<Player> player_ = std::make_unique<Player>();

	std::unique_ptr<Boss> boss_ = std::make_unique<Boss>();

	std::list<std::unique_ptr<Enemy>> enemys_;

	//���x���G�f�B�^
	LevelData* levelData_ = nullptr;

	std::map<std::string, Model*> models;
	std::vector<Object3D*> objects;

	std::unique_ptr<Object3D> newObject[100] = {};

	std::unique_ptr<Enemy> newEnemy[100] = {};

	int32_t objSize = 0;
	int32_t enemySize = 0;

public:

	Player* GetPlayer() { return player_.get(); }
	

	void SetBasis(WindowsApp* windows, DirectXBasis* directX, ImGuiManager* imGuiM, SpriteBasis* spBasis, Sound* sound_);
	void SetInputInfo(KeyBoard* keyboard, GamePad* gamePad);

	//������
	void Initialize()override;

	//�I��
	void Finalize()override;

	//���t���[��
	void Update()override;

	//�`��
	void Draw()override;

	void EditorLoad();

	void ReLoad();

	void Inport(Model* model, int32_t size);
};


#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "Object3D.h"
#include "Model.h"
#include "Player.h"
#include "Enemy.h"

#include <memory>
#include <string>
#include <DirectXTex.h>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <xaudio2.h>
#pragma comment (lib,"xaudio2.lib")
#include <fstream>
#include <wrl.h>

#include <memory>
#include <list>

//チャンクヘッダ
struct ChunkHeader
{
	char id[4];	//チャンクごとのid
	int32_t size;	//チャンクサイズ
};

//RIFFヘッダチャンク
struct RiffHeader
{
	ChunkHeader chunk;	//RIFF
	char type[4];	//WAVE
};

//FMTチャンク
struct FormatChunk
{
	ChunkHeader chunk;	//fmt
	WAVEFORMATEX fmt;	//波形フォーマット
};

//音声データ
struct SoundData
{
	//波形フォーマット
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファのサイズ
	unsigned int bufferSize;

	IXAudio2SourceVoice* pSoundVoice_;
};

SoundData SoundLoadWave(const char* filename) {
	HRESULT result;

	//ファイルオープン
	//ファイル入力ストリームのインスタンス
	std::ifstream file;

	//.wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	//ファイルオープン失敗をチェック
	assert(file.is_open());

	//wav読み込み
	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	/*if (strncmp(format.chunk.id, "fmt", 4) != 0) {
		assert(0);
	}*/
	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		//読み込み位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//ファイルクローズ
	file.close();

	//読み込んだ音声データをretrun
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

//音声データ解放
void SoundUnload(SoundData* soundData) {
	//バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

//音声再生
void SoundPlayWave(IXAudio2* xAudio2, SoundData& soundData) {
	HRESULT result;

	//波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSoundVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSoundVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	soundData.pSoundVoice_ = pSoundVoice;

	//波形データの再生
	result = pSoundVoice->SubmitSourceBuffer(&buf);
	result = pSoundVoice->Start();

	//
}

void SoundStopWAVE(IXAudio2* xAudio2, const SoundData& soundData) {
	soundData.pSoundVoice_->Stop();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//windowsAPIの生成クラス
	std::unique_ptr<WindowsApp> windows;
	WindowsApp* winApp = new WindowsApp();

	//keyboradクラスの生成
	std::unique_ptr<KeyBoard> keyboard;
	KeyBoard* input_ = new KeyBoard();

	//DirectXの基盤生成クラス
	std::unique_ptr<DirectXBasis> DirectX;
	DirectXBasis* dxBasis = new DirectXBasis();

	//windowsAPI初期化
	winApp->Initalize();
	windows.reset(winApp);
	MSG msg{};

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	//DirectX初期化
	dxBasis->Initialize(winApp);
	DirectX.reset(dxBasis);

	HRESULT result = S_FALSE;
	//オーディオ初期化
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	Object3D::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//keyborad初期化
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

	//音
	SoundData soundData1 = SoundLoadWave("Resources/Sound/PerituneMaterial.wav");
	SoundData soundData2 = SoundLoadWave("Resources/Sound/Hit.wav");
	SoundData soundData3 = SoundLoadWave("Resources/Sound/Electric Wild.wav");
	SoundData soundData4 = SoundLoadWave("Resources/Sound/Shot.wav");
	SoundData soundData5 = SoundLoadWave("Resources/Sound/damage.wav");
	SoundData soundData6 = SoundLoadWave("Resources/Sound/dash.wav");
	SoundData soundData7 = SoundLoadWave("Resources/Sound/Victory.wav");
	SoundData soundData8 = SoundLoadWave("Resources/Sound/shot2.wav");

	int PlayBGM = 0;
	int ChangeBGM = 0;

	//OBJからモデルを読み込む
	Model* model = Model::LoadFromObj("boss");
	Model* model2 = Model::LoadFromObj("world");

	Model* playerModel = Model::LoadFromObj("Cube");
	Model* floorModel = Model::LoadFromObj("Floor");

	Model* fieldBlock = Model::LoadFromObj("bobj");
	Model* fieldBlock2 = Model::LoadFromObj("aobj");

	Model* playerModel_ = Model::LoadFromObj("player");
	Model* podModel_ = Model::LoadFromObj("pad");
	Model* enemyModel_ = Model::LoadFromObj("enemy");

	Model* UIModel[13];
	UIModel[0] = Model::LoadFromObj("gameover");
	UIModel[1] = Model::LoadFromObj("kaihi");
	UIModel[2] = Model::LoadFromObj("move");

	UIModel[3] = Model::LoadFromObj("rot");
	UIModel[4] = Model::LoadFromObj("spaceStart");
	UIModel[5] = Model::LoadFromObj("spaceTitle");
	UIModel[6] = Model::LoadFromObj("wave13");
	UIModel[7] = Model::LoadFromObj("wave23");
	UIModel[8] = Model::LoadFromObj("wave33");
	UIModel[9] = Model::LoadFromObj("retry");
	UIModel[10] = Model::LoadFromObj("auto");
	UIModel[11] = Model::LoadFromObj("buster");
	UIModel[12] = Model::LoadFromObj("gameclear");

#pragma region  オブジェクト生成

	//UI
	Object3D* UIobj[13];

	UIobj[10] = Object3D::Create(UIModel[10], { 3,3,3 });
	UIobj[11] = Object3D::Create(UIModel[11], { 3,3,3 });
	UIobj[4] = Object3D::Create(UIModel[4]);

	UIobj[1] = Object3D::Create(UIModel[1],{ 5,5,5 });
	UIobj[2] = Object3D::Create(UIModel[2], { 5,5,5 });
	UIobj[3] = Object3D::Create(UIModel[3], { 5,5,5 });

	UIobj[6] = Object3D::Create(UIModel[6], { 10,10,10 });
	UIobj[7] = Object3D::Create(UIModel[7], { 10,10,10 });
	UIobj[8] = Object3D::Create(UIModel[8], { 10,10,10 });

	UIobj[12] = Object3D::Create(UIModel[12], { 10,10,10 });
	UIobj[5] = Object3D::Create(UIModel[5], { 5,5,5 });

	UIobj[0] = Object3D::Create(UIModel[0], { 8,8,8 });
	UIobj[9] = Object3D::Create(UIModel[9], { 5,5,5 });

	//UI
	UIobj[10]->SetPosition({ -40,20,50 });
	UIobj[11]->SetPosition({ 30,10,25 });
	UIobj[4]->SetPosition({ -5,-10,0 });

	UIobj[1]->SetPosition({ -150,50,300 });
	UIobj[2]->SetPosition({ 0,50,300 });
	UIobj[3]->SetPosition({ 150,50,300 });

	UIobj[6]->SetPosition({ 0,50,300 });
	UIobj[7]->SetPosition({ 0,50,300 });
	UIobj[8]->SetPosition({ 0,50,300 });

	UIobj[12]->SetPosition({ -100,70,300 });
	UIobj[5]->SetPosition({ -50,0,300 });

	UIobj[0]->SetPosition({ -50,120,300 });
	UIobj[9]->SetPosition({ -100,70,300 });

	//3Dオブジェクト生成
	Object3D* object3d3 = Object3D::Create(model2, { (500.0f),(500.0f),(500.0f) });

	Object3D* enemyObject = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject2 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject3 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });

	Object3D* enemyObject4 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject5 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject6 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });
	Object3D* enemyObject7 = Object3D::Create(enemyModel_, { (7.0f),(7.0f),(7.0f) });


	Object3D* playerObject = Object3D::Create(playerModel_, { (2.0f),(2.0f),(2.0f) });
	Object3D* podObject = Object3D::Create(podModel_, { (2),(2),(2) });

	Object3D* floorObject = Object3D::Create(floorModel, { (25.0f),(25.0f),(25.0f) });
	Object3D* fieldblock = Object3D::Create(fieldBlock, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock2 = Object3D::Create(fieldBlock, { (15.0f),(10.0f),(10.0f) });
	Object3D* fieldblock3 = Object3D::Create(fieldBlock, { (10.0f),(20.0f),(10.0f) });
	Object3D* fieldblock4 = Object3D::Create(fieldBlock2, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock5 = Object3D::Create(fieldBlock2, { (15.0f),(15.0f),(15.0f) });

	Object3D* fieldblock6 = Object3D::Create(fieldBlock, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock7 = Object3D::Create(fieldBlock, { (15.0f),(10.0f),(10.0f) });
	Object3D* fieldblock8 = Object3D::Create(fieldBlock, { (10.0f),(20.0f),(10.0f) });
	Object3D* fieldblock9 = Object3D::Create(fieldBlock2, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock10 = Object3D::Create(fieldBlock2, { (15.0f),(15.0f),(15.0f) });

	Object3D* fieldblock11 = Object3D::Create(fieldBlock, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock12 = Object3D::Create(fieldBlock, { (15.0f),(10.0f),(10.0f) });
	Object3D* fieldblock13 = Object3D::Create(fieldBlock, { (10.0f),(20.0f),(10.0f) });
	Object3D* fieldblock14 = Object3D::Create(fieldBlock2, { (10.0f),(10.0f),(10.0f) });
	Object3D* fieldblock15 = Object3D::Create(fieldBlock2, { (15.0f),(15.0f),(15.0f) });

	playerObject->SetPosition({ 0,0,-20 });
	floorObject->SetPosition({ 0,-10,0 });

	fieldblock->SetPosition({ -40,40,300 });
	fieldblock2->SetPosition({ 150,20,300 });
	fieldblock3->SetPosition({ -150,80,300 });
	fieldblock4->SetPosition({ 30,100,300 });
	fieldblock5->SetPosition({ -100,200,300 });

	fieldblock6->SetPosition({ 300,40,120 });
	fieldblock7->SetPosition({ 300,20,0 });
	fieldblock8->SetPosition({ 300,80,-50 });
	fieldblock9->SetPosition({ 270,100,280 });
	fieldblock10->SetPosition({ 300,150,200 });

	fieldblock11->SetPosition({ -300,40,200 });
	fieldblock12->SetPosition({ -300,120,280 });
	fieldblock13->SetPosition({ -300,80,-20 });
	fieldblock14->SetPosition({ -250,20,-100 });
	fieldblock15->SetPosition({ -350,150,150 });


	fieldblock4->SetRotation({ 0,90,0 });
	fieldblock5->SetRotation({ 0,-90,0 });
	fieldblock9->SetRotation({ 0,90,0 });
	fieldblock14->SetRotation({ 0,-90,0 });
#pragma endregion

	Object3D::CameraMoveVector({ 0.0f,20.0f,-30.0f });

	float angle = 0.0f;

	Player* player = new Player;
	player->Initialize(playerModel_, playerObject, input_, podObject);

	int isPlayerDamege = 0;
	int invincibleTime = 0;
	int invincible = 0;

	int wave = 0;
	std::list<std::unique_ptr<Enemy>> enemys_;

	Enemy* enemys[3];

	Vector3 enemyPos;

	int isPop = 0;
	int scene = 0;
	int knockDownNum = 0;
	int popCoolTime = 0;
	XMFLOAT3 domeRot={};

	int PlaySE = 0;

	//ゲームループ
	while (true) {
		//×ボタンで終了メッセージがきたら
		if (winApp->gameloopExit(msg) == true) {
			break;	//ゲームループ終了
		}

		//-----ここから更新処理-----//
		//シーン切り替え
		//タイトル
		if (scene == 0) {
			if (input_->keyInstantPush(DIK_SPACE)) {
				scene = 1;
				player->HP = 5;
				player->isDead = false;
				playerObject->SetPosition({ 0,0,-20 });
				playerObject->SetRotation({ 0,0,0 });
				Object3D::SetCamera();
				enemys_.clear();
				player->clear();
				wave = 0;
				isPop = 0;
				knockDownNum = 0;
				popCoolTime = 0;
				isPlayerDamege = 0;
				invincibleTime = 0;
				invincible = 0;
				PlaySE = 0;
				if (ChangeBGM == 0) {
					SoundStopWAVE(xAudio2.Get(), soundData1);
				}
				PlayBGM = 0;
				ChangeBGM = 1;
			}
		}
		//ゲームオーバー
		else if (scene == 2) {
			if (input_->keyInstantPush(DIK_SPACE)) {

				if (ChangeBGM == 1) {
					SoundStopWAVE(xAudio2.Get(), soundData3);
				}
				PlayBGM = 0;
				ChangeBGM = 0;

				scene = 0;
			}

			if (input_->keyInstantPush(DIK_R)) {
				if (ChangeBGM == 1) {
					SoundStopWAVE(xAudio2.Get(), soundData3);
				}
				PlayBGM = 0;
				ChangeBGM = 1;
				PlaySE = 0;

				scene = 1;
				player->HP = 5;
				player->isDead = false;
				playerObject->SetPosition({ 0,0,-20 });
				playerObject->SetRotation({ 0,0,0 });
				Object3D::SetCamera();
				enemys_.clear();
				player->clear();
				wave = 0;
				isPop = 0;
				knockDownNum = 0;
				popCoolTime = 0;
				isPlayerDamege = 0;
				invincibleTime = 0;
				invincible = 0;
			}
		}
		//ゲームクリア
		else if (scene == 3) {
			if (ChangeBGM == 1) {
				SoundStopWAVE(xAudio2.Get(), soundData3);
			}

			if (PlaySE == 0) {
				SoundPlayWave(xAudio2.Get(), soundData7);
				PlaySE = 1;
			}

			if (input_->keyInstantPush(DIK_SPACE)) {
				scene = 0;
				PlayBGM = 0;
				ChangeBGM = 0;
				PlaySE = 0;

				player->HP = 5;
				player->isDead = false;
				playerObject->SetPosition({ 0,0,-20 });
				playerObject->SetRotation({ 0,0,0 });
				Object3D::SetCamera();
				enemys_.clear();
				player->clear();
				wave = 0;
				isPop = 0;
				knockDownNum = 0;
				popCoolTime = 0;
				isPlayerDamege = 0;
				invincibleTime = 0;
				invincible = 0;
			}
		}

		if (PlayBGM == 0 && ChangeBGM == 0) {
			SoundPlayWave(xAudio2.Get(), soundData1);
			PlayBGM = 1;
		}
		if (PlayBGM == 0 && ChangeBGM == 1) {
			SoundPlayWave(xAudio2.Get(), soundData3);
			PlayBGM = 1;
		}

		//keyborad更新処理
		input_->Update();

#pragma region  オブジェクト更新処理

		//タイトル
		if (scene == 0) {
			UIobj[10]->Update();
			UIobj[11]->Update();
			UIobj[4]->Update();
		}

		object3d3->Update();

		if (scene == 1) {

			if (input_->keyInstantPush(DIK_K)) {
				player->OnCollision();
			}

			if (player->IsDead() == true) {
				scene = 2;
			}

#pragma region  ウェーブ管理

			//チュートリアル
			if (wave == 0) {

				UIobj[1]->Update();
				UIobj[2]->Update();
				UIobj[3]->Update();

				if (isPop == 0) {

					for (int i = 0; i < 3; i++) {
						enemys[i] = new Enemy;
					}

					enemys[0]->Initialize(enemyObject, { -70,0,100 }, player, 0);
					enemys[1]->Initialize(enemyObject2, { 0,0,100 }, player, 0);
					enemys[2]->Initialize(enemyObject3, { 70,0,100 }, player, 0);

					//std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
					//std::unique_ptr<Enemy> newEnemy2 = std::make_unique<Enemy>();
					//std::unique_ptr<Enemy> newEnemy3 = std::make_unique<Enemy>();

					//newEnemy->Initialize(enemyObject, { -70,0,100 }, player,0);
					//newEnemy2->Initialize(enemyObject2, { 0,0,100 }, player,0);
					//newEnemy3->Initialize(enemyObject3, { 70,0,100 }, player,0);

					////敵を登録する
					//enemys_.push_back(std::move(newEnemy));
					//enemys_.push_back(std::move(newEnemy2));
					//enemys_.push_back(std::move(newEnemy3));
					isPop++;
				}

				if (input_->keyInstantPush(DIK_A)) {
					enemyPos.x = enemys[0]->GetWorldPos().x;
					enemyPos.y = enemys[0]->GetWorldPos().y;
					enemyPos.z = enemys[0]->GetWorldPos().z;
				}
				if (input_->keyInstantPush(DIK_S)) {
					enemyPos.x = enemys[1]->GetWorldPos().x;
					enemyPos.y = enemys[1]->GetWorldPos().y;
					enemyPos.z = enemys[1]->GetWorldPos().z;
				}
				if (input_->keyInstantPush(DIK_D)) {
					enemyPos.x = enemys[2]->GetWorldPos().x;
					enemyPos.y = enemys[2]->GetWorldPos().y;
					enemyPos.z = enemys[2]->GetWorldPos().z;
				}

				
				if (input_->keyInstantPush(DIK_SPACE)) {
					SoundPlayWave(xAudio2.Get(), soundData8);
				}
				

				if (knockDownNum == 3) {
					wave++;
					knockDownNum = 0;
					isPop = 0;
				}
			}

			

#pragma endregion

			floorObject->Update();

			fieldblock->Update();
			fieldblock2->Update();
			fieldblock3->Update();
			fieldblock4->Update();
			fieldblock5->Update();
			fieldblock6->Update();
			fieldblock7->Update();
			fieldblock8->Update();
			fieldblock9->Update();
			fieldblock10->Update();
			fieldblock11->Update();
			fieldblock12->Update();
			fieldblock13->Update();
			fieldblock14->Update();
			fieldblock15->Update();

			player->Update(enemyPos);

			//発射SE
			if (player->GetCoolTime() == 0) {
				SoundPlayWave(xAudio2.Get(), soundData4);
			}

			if (player->GetIsStep() == true && input_->keyInstantPush(DIK_W)) {
				SoundPlayWave(xAudio2.Get(), soundData6);
			}

			//
			//無敵時間
			if (player->GetIsStep() == true) {
				invincible = true;
			}
			else {
				invincible = false;
			}

			if (isPlayerDamege == 1) {
				invincibleTime++;

				if (invincibleTime >= 100) {
					isPlayerDamege = 0;
					invincibleTime = 0;
				}
			}

			//enemyの死亡フラグ
			enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
				return enemy->IsDead();
			});

			//敵の動き
			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->Update(wave);
			}

			for (int i = 0; i < 3; i++) {
				enemys[i]->Update(wave);
			}
		}
		
		if (scene == 2) {
			floorObject->Update();
			fieldblock->Update();
			fieldblock2->Update();
			fieldblock3->Update();
			fieldblock4->Update();
			fieldblock5->Update();
			fieldblock6->Update();
			fieldblock7->Update();
			fieldblock8->Update();
			fieldblock9->Update();
			fieldblock10->Update();
			fieldblock11->Update();
			fieldblock12->Update();
			fieldblock13->Update();
			fieldblock14->Update();
			fieldblock15->Update();

			//敵の動き
			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->Update(wave);
			}
			UIobj[5]->SetPosition({ 50,70,300 });

			UIobj[9]->Update();
			UIobj[0]->Update();
			UIobj[5]->Update();
		}

		if (scene == 3) {
			UIobj[5]->SetPosition({ -50,0,300 });

			UIobj[12]->Update();
			UIobj[5]->Update();
		}
		

#pragma endregion

#pragma region  当たり判定

		if (scene == 1) {
			XMFLOAT3 posA, posB;

			//自弾リストの取得
			const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

			for (int i = 0; i < 3; i++) {
				posA = enemys[i]->GetWorldPos();

				for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
					//敵弾の座標
					posB = bullet->GetWorldPos();

					//AとBの距離
					float r1 = 7.0f;	//敵のスケール
					float r2 = 1.0f;	//弾のスケール
					float r = r1 + r2;

					XMFLOAT3 dis;
					dis.x = posB.x - posA.x;
					dis.y = posB.y - posA.y;
					dis.z = posB.z - posA.z;


					if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {
						enemys[i]->OnCollision();
						bullet->isDead_ = true;
						SoundPlayWave(xAudio2.Get(), soundData2);
					}
				}
			}

			posA = player->GetWorldPos();

			//自機と敵の弾の当たり判定
			for (const std::unique_ptr<Enemy>& enemy : enemys_) {
				for (const std::unique_ptr<EnemyBullet>& bullet : enemy->GetBullets()) {
					//敵弾の座標
					posB = bullet->GetWorldPos();

					//AとBの距離
					float r1 = 7.0f;	//敵のスケール
					float r2 = 1.0f;	//弾のスケール
					float r = r1 + r2;

					XMFLOAT3 dis;
					dis.x = posB.x - posA.x;
					dis.y = posB.y - posA.y;
					dis.z = posB.z - posA.z;

					if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {
						
						if (isPlayerDamege == 0 && invincible == 0) {
							bullet->isDead_ = true;
							isPlayerDamege = 1;
							player->OnCollision();
							SoundPlayWave(xAudio2.Get(), soundData5);
						}
					}
				}
			}

			for (const std::unique_ptr<Enemy>& enemy : enemys_) {
				posA = enemy->GetWorldPos();

				//自機の弾と敵の当たり判定
				for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
					//自弾の座標
					posB = bullet->GetWorldPos();

					//AとBの距離
					float r1 = 7.0f;	//敵のスケール
					float r2 = 1.0f;	//弾のスケール
					float r = r1 + r2;

					XMFLOAT3 dis;
					dis.x = posB.x - posA.x;
					dis.y = posB.y - posA.y;
					dis.z = posB.z - posA.z;


					if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {
						bullet->isDead_ = true;
						enemy->OnCollision();

						if (enemy->IsDead() == true) {
							knockDownNum++;
						}
						SoundPlayWave(xAudio2.Get(), soundData2);
					}
				}
			}
		}
		

#pragma endregion

		//ここから描画処理

		if (scene == 0) {
			// 描画前処理
			dxBasis->PreDraw();

			Object3D::PreDraw(dxBasis->GetCommandList());

			//天球
			object3d3->Draw();

			UIobj[10]->Draw();
			UIobj[11]->Draw();
			UIobj[4]->Draw();

			Object3D::PostDraw();

			//描画後処理
			dxBasis->PostDraw();
		}
		else if (scene == 1) {
#pragma region DirectX毎フレーム処理

			// 描画前処理
			dxBasis->PreDraw();

			Object3D::PreDraw(dxBasis->GetCommandList());

#pragma region  3Dモデル描画処理
			object3d3->Draw();

			floorObject->Draw();

			fieldblock->Draw();
			fieldblock2->Draw();
			fieldblock3->Draw();
			fieldblock4->Draw();
			fieldblock5->Draw();
			fieldblock6->Draw();
			fieldblock7->Draw();
			fieldblock8->Draw();
			fieldblock9->Draw();
			fieldblock10->Draw();
			fieldblock11->Draw();
			fieldblock12->Draw();
			fieldblock13->Draw();
			fieldblock14->Draw();
			fieldblock15->Draw();

			player->Draw();

			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->Draw();
			}

			for (int i = 0; i < 3; i++) {
				if (enemys[i]->IsDead() == false) {
					enemys[i]->Draw();
				}
				
			}

			if (wave == 0) {
				UIobj[1]->Draw();
				UIobj[2]->Draw();
				UIobj[3]->Draw();
			}
			else if (wave == 1) {
				UIobj[6]->Draw();
			}
			else if (wave == 2) {
				UIobj[7]->Draw();
			}
			else if (wave == 3) {
				UIobj[8]->Draw();
			}
#pragma endregion

			Object3D::PostDraw();

			//描画後処理
			dxBasis->PostDraw();

#pragma endregion
		}
		else if (scene == 2) {
			// 描画前処理
			dxBasis->PreDraw();

			Object3D::PreDraw(dxBasis->GetCommandList());

			//天球
			object3d3->Draw();

			floorObject->Draw();

			fieldblock->Draw();
			fieldblock2->Draw();
			fieldblock3->Draw();
			fieldblock4->Draw();
			fieldblock5->Draw();
			fieldblock6->Draw();
			fieldblock7->Draw();
			fieldblock8->Draw();
			fieldblock9->Draw();
			fieldblock10->Draw();
			fieldblock11->Draw();
			fieldblock12->Draw();
			fieldblock13->Draw();
			fieldblock14->Draw();
			fieldblock15->Draw();

			for (std::unique_ptr<Enemy>& enemy : enemys_) {
				enemy->Draw();
			}

			UIobj[0]->Draw();
			UIobj[9]->Draw();
			UIobj[5]->Draw();

			Object3D::PostDraw();

			//描画後処理
			dxBasis->PostDraw();
		}
		else if (scene == 3) {
		// 描画前処理
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		//天球
		object3d3->Draw();

		floorObject->Draw();

		fieldblock->Draw();
		fieldblock2->Draw();
		fieldblock3->Draw();
		fieldblock4->Draw();
		fieldblock5->Draw();
		fieldblock6->Draw();
		fieldblock7->Draw();
		fieldblock8->Draw();
		fieldblock9->Draw();
		fieldblock10->Draw();
		fieldblock11->Draw();
		fieldblock12->Draw();
		fieldblock13->Draw();
		fieldblock14->Draw();
		fieldblock15->Draw();

		player->Draw();

		UIobj[12]->Draw();
		UIobj[5]->Draw();

		Object3D::PostDraw();

		//描画後処理
		dxBasis->PostDraw();
		}
	}

	delete model;
	delete model2;
	delete playerModel;
	delete floorModel;
	delete fieldBlock;
	delete fieldBlock2;

	delete enemyObject;
	delete enemyObject2;
	delete enemyObject3;

	delete floorObject;
	delete playerObject;
	delete object3d3;
	delete player;
	delete fieldblock;
	delete fieldblock2;
	delete fieldblock3;
	delete fieldblock4;
	delete fieldblock5;
	delete fieldblock6;
	delete fieldblock7;
	delete fieldblock8;
	delete fieldblock9;
	delete fieldblock10;
	delete fieldblock11;
	delete fieldblock12;
	delete fieldblock13;
	delete fieldblock14;
	delete fieldblock15;

	xAudio2.Reset();
	SoundUnload(&soundData1);
	SoundUnload(&soundData2);
	SoundUnload(&soundData3);
	SoundUnload(&soundData4);

	//ウィンドウクラスを登録解除
	winApp->Release();

	return 0;
}
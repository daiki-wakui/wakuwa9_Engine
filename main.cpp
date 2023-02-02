#include "WindowsApp.h"
#include "KeyBoard.h"
#include "DirectXBasis.h"
#include "Object3D.h"
#include "Model.h"
#include "CollisonPrimitive.h"
#include "Collison.h"

#include <memory>
#include <string>
#include <DirectXTex.h>

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


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

	//DirectX初期化
	dxBasis->Initialize(winApp);
	DirectX.reset(dxBasis);

	Object3D::StaticInitialize(dxBasis->GetDevice(), winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//keyborad初期化
	input_->Initialize(winApp->GetHInstancee(), winApp->GetHwnd());
	keyboard.reset(input_);

	//OBJからモデルを読み込む
	Model* model = Model::LoadFromObj("sphere");
	Model* model3 = Model::LoadFromObj("floor");
	Model* model2 = Model::LoadFromObj("world");
	Model* model4 = Model::LoadFromObj("player");
	Model* modelHit = Model::LoadFromObj("sphere2");
	Model* modeltri = Model::LoadFromObj("tria");
	Model* modelCude = Model::LoadFromObj("Cube");
	Model* modelCudeHit = Model::LoadFromObj("Cube2");

	//3Dオブジェクト生成
	Object3D* object3d = Object3D::Create(5.0f,5,5);
	Object3D* objectHit = Object3D::Create(5.0f,5,5);
	Object3D* object3d2 = Object3D::Create(2.0f,2,2);
	Object3D* object3d3 = Object3D::Create(100.0f,100,100);

	Object3D* objectFloor = Object3D::Create(5.0f,5,5);
	Object3D* objectTri = Object3D::Create(3.0f,3,3);
	Object3D* objectCude = Object3D::Create(1.0f,100,1);
	Object3D* objectCudeHit = Object3D::Create(1.0f,100,1);

	//3Dオブジェクトに3Dモデルを紐づけ
	object3d->SetModel(model);
	object3d2->SetModel(model4);
	object3d3->SetModel(model2);

	objectTri->SetModel(modeltri);
	objectCude->SetModel(modelCude);
	objectCudeHit->SetModel(modelCudeHit);

	object3d->SetPosition({ -10,15,+5 });
	object3d2->SetPosition({ +10,0,+5 });

	objectFloor->SetModel(model3);
	objectFloor->SetPosition({ 0,-4,0 });
	objectTri->SetPosition({ 0,-7,-10 });

	objectHit->SetModel(modelHit);

	bool hit = false;

	Sphere sphere;
	Plane plane;
	Vector3 spherePos;
	Vector3 planePos;
	Vector3 triPos;
	Vector3 normalVec = { 0,1,0 };

	Triangle triangle;

	triPos.x = objectTri->GetPosition().x;
	triPos.y = objectTri->GetPosition().y;
	triPos.z = objectTri->GetPosition().z;

	//レイと三角形
	triangle.p0 = Vector3(triPos.x - 30, triPos.y, triPos.z);
	triangle.p1 = Vector3(triPos.x, triPos.y, triPos.z + 120);
	triangle.p2 = Vector3(triPos.x + 30, triPos.y, triPos.z);

	//球と三角形
	triangle.normal = Vector3(0, 1, 0);

	Ray ray;
	Vector3 raypos;
	ray.dir = { 0,-1,0 };
	int State = 0;

	int timer = 0;
	XMFLOAT3 move;
	XMFLOAT3 rayMove;
	bool down = true;

	//ゲームループ
	while (true) {
		//×ボタンで終了メッセージがきたら
		if (winApp->gameloopExit(msg) == true) {
			break;	//ゲームループ終了
		}
		object3d->Update();
		object3d3->Update();
		objectFloor->Update();
		objectHit->Update();
		objectTri->Update();
		objectCude->Update();
		objectCudeHit->Update();

		//keyborad更新処理
		input_->Update();

		XMFLOAT3 pos3d;
		XMFLOAT3 pos3d2;
		XMFLOAT3 pos3dCube;

		pos3d = object3d->GetRotation();
		pos3d2 = object3d->GetPosition();
		pos3dCube = objectCude->GetPosition();
		move = object3d->GetPosition();	
		rayMove = objectCude->GetPosition();

		if (State == 1) {
			if (down == true) {
				move.y -= 0.5f;
				timer++;
				if (timer >= 50) {
					timer = 0;
					down = false;
				}
			}
			else {
				move.y += 0.5f;
				timer++;
				if (timer >= 50) {
					timer = 0;
					down = true;
				}
			}
		}

		if (State == 2) {
			if (down == true) {
				move.x++;
				timer++;
				if (timer >= 50) {
					timer = 0;
					down = false;
				}
			}
			else {
				move.x--;
				timer++;
				if (timer >= 50) {
					timer = 0;
					down = true;
				}
			}
		}

		if (State == 3||State==5) {
			if (down == true) {
				rayMove.y++;
				timer++;
				if (timer >= 50) {
					timer = 0;
					down = false;
				}
			}
			else {
				rayMove.y--;
				timer++;
				if (timer >= 50) {
					timer = 0;
					down = true;
				}
			}
		}

		if (State == 4) {
			if (down == true) {
				rayMove.x++;
				timer++;
				if (timer >= 50) {
					timer = 0;
					down = false;
				}
			}
			else {
				rayMove.x--;
				timer++;
				if (timer >= 50) {
					timer = 0;
					down = true;
				}
			}
		}

		if (State == 1 || State == 2) {
			object3d->SetPosition(move);
		}
		if (State == 3 || State == 4 || State == 5) {
			objectCude->SetPosition(rayMove);
		}
		objectCudeHit->SetPosition(objectCude->GetPosition());
		objectHit->SetPosition(object3d->GetPosition());

		if (input_->keyInstantPush(DIK_1)) {
			if (State != 1) {
				objectFloor->SetPosition({ 0,-4,0 });
				State = 1;
				Object3D::CameraEyeMoveVector(XMFLOAT3(0, 10, -100));
				object3d->SetPosition({ -10,20,+5 });
				timer = 0;
				down = true;
			}
			
		}

		if (input_->keyInstantPush(DIK_2)) {
			if (State != 2) {
				State = 2;
				Object3D::CameraEyeMoveVector(XMFLOAT3(0, 20, -60));
				object3d->SetPosition({ -25,-10,+35 });
				triangle.p0 = Vector3(triPos.x + 1, triPos.y, triPos.z + 1);
				triangle.p1 = Vector3(triPos.x - 1, triPos.y, triPos.z);
				triangle.p2 = Vector3(triPos.x + 1, triPos.y, triPos.z - 1);
				timer = 0;
				down = true;
			}
		}

		if (input_->keyInstantPush(DIK_3)) {
			if (State != 3) {
				State = 3;
				Object3D::CameraEyeMoveVector(XMFLOAT3(0, 5, -100));
				objectCude->SetPosition({ 0,-125,0 });
				objectFloor->SetPosition({ 0,0,0 });
				timer = 0;
				down = true;
			}
			
		}

		if (input_->keyInstantPush(DIK_4)) {
			if (State != 4) {
				State = 4;
				Object3D::CameraEyeMoveVector(XMFLOAT3(0, 20, -100));
				triangle.p0 = Vector3(triPos.x - 30, triPos.y, triPos.z);
				triangle.p1 = Vector3(triPos.x, triPos.y, triPos.z + 120);
				triangle.p2 = Vector3(triPos.x + 30, triPos.y, triPos.z);
				objectCude->SetPosition({ -25,50,15 });
				timer = 0;
				down = true;
			}
			
		}

		if (input_->keyInstantPush(DIK_5)) {
			if (State != 5) {
				State = 5;
				Object3D::CameraEyeMoveVector(XMFLOAT3(0, 20, -100));
				object3d->SetPosition({ 0,15,+5 });
				objectCude->SetPosition({ 0,-115,+5 });
				timer = 0;
				down = true;
			}
			
		}

		spherePos.x = object3d->GetPosition().x;
		spherePos.y = object3d->GetPosition().y;
		spherePos.z = object3d->GetPosition().z;

		planePos.x = objectFloor->GetPosition().x;
		planePos.y = objectFloor->GetPosition().y;
		planePos.z = objectFloor->GetPosition().z;

		sphere.center = spherePos;
		sphere.radius = 5.0f/2;

		plane.normal = normalVec;
		plane.distance = 0.0f;

		raypos.x = objectCude->GetPosition().x;
		raypos.y = objectCude->GetPosition().y + 100;
		raypos.z = objectCude->GetPosition().z;

		ray.start = raypos;

		if (State == 1) {
			hit = Collison::CheckSphere2Plane(sphere, plane);
		}
		if (State == 2) {
			hit = Collison::CheckSphere2Triangle(sphere, triangle);
		}
		if (State == 3) {
			hit = Collison::CheckRay2Plane(ray, plane);
		}
		if (State == 4) {
			hit = Collison::CheckRay2Triangle(ray, triangle);
		}
		if (State == 5) {
			hit = Collison::CheckRay2Sphere(ray, sphere);
		}

#pragma region DirectX毎フレーム処理

		// 描画前処理
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		object3d3->Draw();

		if (hit) {

			if (State == 1) {
				objectHit->Draw();
				objectFloor->Draw();
			}
			if (State == 2) {
				objectHit->Draw();
				objectTri->Draw();
			}
			if (State == 3) {
				objectCudeHit->Draw();
				objectFloor->Draw();
			}
			if (State == 4) {
				objectCudeHit->Draw();
				objectTri->Draw();
			}
			if (State == 5) {
				objectHit->Draw();
				objectCudeHit->Draw();
			}
		}
		else {

			if (State == 1) {
				object3d->Draw();
				objectFloor->Draw();
			}
			if (State == 2) {
				object3d->Draw();
				objectTri->Draw();
			}
			if (State == 3) {
				objectCude->Draw();
				objectFloor->Draw();
			}
			if (State == 4) {
				objectCude->Draw();
				objectTri->Draw();
			}
			if (State == 5) {
				object3d->Draw();
				objectCude->Draw();
			}
		}

		Object3D::PostDraw();

		//描画後処理
		dxBasis->PostDraw();
		
#pragma endregion

	}

	delete model;
	delete object3d;
	delete object3d2;
	delete object3d3;
	//ウィンドウクラスを登録解除
	winApp->Release();

	return 0;
}

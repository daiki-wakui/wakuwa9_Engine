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

	object3d->SetPosition({ -10,0,+5 });
	object3d2->SetPosition({ +10,0,+5 });

	objectFloor->SetModel(model3);
	objectFloor->SetPosition({ 0,-7,0 });
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
	/*triangle.p0 = Vector3(triPos.x + 1, triPos.y, triPos.z + 1);
	triangle.p1 = Vector3(triPos.x - 1, triPos.y, triPos.z);
	triangle.p2 = Vector3(triPos.x + 1, triPos.y, triPos.z - 1);*/

	triangle.normal = Vector3(0, 1, 0);

	Ray ray;
	Vector3 raypos;

	//ray.start = { 0,1,0 };
	ray.dir = { 0,-1,0 };
	
	//ray.dir = Vector3(-1, 0, 0);

	int State = 0;

	//ゲームループ
	while (true) {
		//×ボタンで終了メッセージがきたら
		if (winApp->gameloopExit(msg) == true) {
			break;	//ゲームループ終了
		}
		object3d->Update();
		//object3d2->Update();
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

		pos3d = object3d->GetRotation();
		pos3d2 = objectCude->GetPosition();

		if (input_->keyInstantPush(DIK_SPACE)) {
			if (hit == false) {
				hit = true;
			}
			else {
				hit = false;
			}
		}

		if (input_->keyPush(DIK_D)) {
			pos3d.y++;
		}
		if (input_->keyPush(DIK_A)) {
			pos3d.y--;
		}
		if (input_->keyPush(DIK_S)) {
			pos3d.x--;
		}
		if (input_->keyPush(DIK_W)) {
			pos3d.x++;
		}

		if (input_->keyPush(DIK_RIGHT)) {
			pos3d2.x++;
		}
		if (input_->keyPush(DIK_LEFT)) {
			pos3d2.x--;
		}
		if (input_->keyPush(DIK_UP)) {
			pos3d2.y++;
		}
		if (input_->keyPush(DIK_DOWN)) {
			pos3d2.y--;
		}
		if (input_->keyPush(DIK_A)) {
			pos3d2.z++;
		}
		if (input_->keyPush(DIK_Z)) {
			pos3d2.z--;
		}

		object3d->SetRotation(pos3d);
		//object3d->SetPosition(pos3d2);
		objectCude->SetPosition(pos3d2);
		//objectHit->SetPosition(pos3d2);
		objectCudeHit->SetPosition(pos3d2);

		objectHit->SetPosition(object3d->GetPosition());

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

		//hit = Collison::CheckSphere2Plane(sphere, plane);
		//hit = Collison::CheckSphere2Triangle(sphere, triangle);
		//hit = Collison::CheckRay2Plane(ray, plane);
		//hit = Collison::CheckRay2Triangle(ray, triangle);
		hit = Collison::CheckRay2Sphere(ray, sphere);

#pragma region DirectX毎フレーム処理

		// 描画前処理
		dxBasis->PreDraw();

		Object3D::PreDraw(dxBasis->GetCommandList());

		
		//object3d2->Draw();
		object3d3->Draw();
		
		//objectFloor->Draw();
		//objectTri->Draw();

		if (hit) {
			objectHit->Draw();
			objectCudeHit->Draw();
		}
		else {
			object3d->Draw();
			objectCude->Draw();
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

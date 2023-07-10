#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include "Vector3.h"

class Camera
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	// ビュー行列
	static XMMATRIX sMatView;
	// 射影行列
	static XMMATRIX sMatProjection;
	// 視点座標
	static XMFLOAT3 sEye;
	// 注視点座標
	static XMFLOAT3 sTarget;
	// 上方向ベクトル
	static XMFLOAT3 sUp;

public:

	//初期化
	void Initialize(int32_t window_width, int32_t window_height);

	static void Update();

	void SetCamera(XMFLOAT3& eye, XMFLOAT3& terget);

	static void CameraMoveVector(XMFLOAT3& move);
	static void CameraEyeMoveVector(Vector3& eye);

	static const XMFLOAT3& GetEye() { return sEye; }
	static const XMFLOAT3& GetTarget() { return sTarget; }
	static const XMFLOAT3& GetUp() { return sUp; }

	static void SetEye(XMFLOAT3& eye);
	static void SetTarget(XMFLOAT3& target);
};


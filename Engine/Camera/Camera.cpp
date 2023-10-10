#include "Camera.h"

using namespace DirectX;
using namespace Microsoft::WRL;

XMMATRIX Camera::sMatView{};
XMMATRIX Camera::sMatProjection{};
XMFLOAT3 Camera::sEye = { 0, 20, -30.0f };
XMFLOAT3 Camera::sTarget = { 0, 10, 0 };
XMFLOAT3 Camera::sUp = { 0, 1, 0 };

void Camera::Initialize(int32_t window_width, int32_t window_height)
{
	// ビュー行列の生成
	sMatView = XMMatrixLookAtLH(
		XMLoadFloat3(&sEye),
		XMLoadFloat3(&sTarget),
		XMLoadFloat3(&sUp));


	// 透視投影による射影行列の生成
	sMatProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
}

void Camera::Update()
{
	// ビュー行列の更新
	sMatView = XMMatrixLookAtLH(XMLoadFloat3(&sEye), XMLoadFloat3(&sTarget), XMLoadFloat3(&sUp));
}

void Camera::CameraMoveVector(XMFLOAT3& move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::CameraEyeMoveVector(Vector3& eye)
{
	XMFLOAT3 eye_ = GetEye();
	Vector3 VecEye_;
	VecEye_.x = eye_.x;
	//VecEye_.y = eye_.y;
	VecEye_.z = eye_.z;

	VecEye_.x += eye.x;
	//VecEye_.y += eye.y;
	VecEye_.z += eye.z;

	eye_.x = VecEye_.x;
	//eye_.y = VecEye_.y;
	eye_.z = VecEye_.z;

	SetEye(eye_);
}

void Camera::SetEye(XMFLOAT3& eye)
{
	Camera::sEye = eye;

	Update();
}

void Camera::SetTarget(XMFLOAT3& target)
{
	Camera::sTarget = target;

	Update();
}

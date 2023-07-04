#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include "Vector3.h"

class Camera
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	// �r���[�s��
	static XMMATRIX sMatView;
	// �ˉe�s��
	static XMMATRIX sMatProjection;
	// ���_���W
	static XMFLOAT3 sEye;
	// �����_���W
	static XMFLOAT3 sTarget;
	// ������x�N�g��
	static XMFLOAT3 sUp;

public:

	//������
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


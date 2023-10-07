#pragma once

class Vector3;

class Matrix4
{
public:
	// �sx��
	float m[4][4];

	// �R���X�g���N�^
	Matrix4();

	// ������Z�q�I�[�o�[���[�h
	//Matrix4& operator*=(const Matrix4& m2);

	Matrix4 operator*(const Matrix4& m2);

	void ScaleSet(Vector3& v);
	void RotXSet(float& x);
	void RotYSet(float& y);
	void RotZSet(float& z);
	void TransSet(Vector3& v);
};



#pragma once

class Vector3;

class Matrix4
{
public:
	// 行x列
	float m[4][4];

	// コンストラクタ
	Matrix4();

	// 代入演算子オーバーロード
	//Matrix4& operator*=(const Matrix4& m2);

	Matrix4 operator*(const Matrix4& m2);

	void ScaleSet(Vector3& v);
	void RotXSet(float& x);
	void RotYSet(float& y);
	void RotZSet(float& z);
	void TransSet(Vector3& v);
};



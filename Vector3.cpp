#include "Vector3.h"
#include <cmath>

//�[���x�N�g���̃R���X�g���N�^
Vector3::Vector3()
	:x(0), y(0), z(0)
{
}

//������̃R���X�g���N�^
Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

//�x�N�g���̒���
float Vector3::length() const {
	return sqrtf((x * x) + (y * y) + (z * z));
}

//�x�N�g���̐��K��
Vector3& Vector3::normalize() {
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

//����
float Vector3::dot(const Vector3& v) const {
	return  (x * v.x) + (y * v.y) + (z + v.z);
}

//�O��
Vector3 Vector3::cross(const Vector3& v) const {
	Vector3 v1;
	v1.x = (y * v.z) - (z * v.y);	//x����;
	v1.y = (z * v.x) - (x * v.z);	//y����;
	v1.z = (x * v.y) - (y * v.x);	//z����;

	return v1;
}

//���`��Ԋ֐�
const Vector3 Vector3::lerp(const Vector3& start, const Vector3& end, const float t)
{
	return start * (1.0f - t) + end * t;
}

void Vector3::multiplyMat4(DirectX::XMMATRIX& mat)
{
	x = x * mat.r[0].m128_f32[0] + y * mat.r[1].m128_f32[0] + z * mat.r[2].m128_f32[0];
	y = x * mat.r[0].m128_f32[1] + y * mat.r[1].m128_f32[1] + z * mat.r[2].m128_f32[1];
	z = x * mat.r[0].m128_f32[2] + y * mat.r[1].m128_f32[2] + z * mat.r[2].m128_f32[2];

	/*x = x * mat.m[0][0] + y * mat.m[1][0] + z * mat.m[2][0];
	y = x * mat.m[0][1] + y * mat.m[1][1] + z * mat.m[2][1];
	z = x * mat.m[0][2] + y * mat.m[1][2] + z * mat.m[2][2];*/
}

////�P�����Z�q�I�[�o�[���[�h////
Vector3 Vector3::operator+()const {
	return *this;
}

Vector3 Vector3::operator-()const {
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

////2�����Z�q�I�[�o�[���[�h////
const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}


const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s) {

	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v) {
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s) {
	Vector3 temp(v);
	temp.x /= s;
	temp.y /= s;
	temp.z /= s;
	return temp;
}



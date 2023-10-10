#include "Vector2.h"

#include <cmath>

//ゼロベクトルのコンストラクタ
Vector2::Vector2()
	:x(0), y(0)
{
}

//引数二つのコンストラクタ
Vector2::Vector2(float x, float y)
	:x(x), y(y)
{
}

//ベクトルの長さ
float Vector2::length() const {
	return sqrtf((x * x) + (y * y));
}

//ベクトルの正規化
Vector2& Vector2::normalize() {
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

//内積
float Vector2::dot(const Vector2& v) const {
	return  (x * v.x) + (y * v.y);
}

//外積
float Vector2::cross(const Vector2& v) const {
	return (x * v.y) - (y * v.x);
}

////単項演算子オーバーロード////
Vector2 Vector2::operator+()const {
	return *this;
}

Vector2 Vector2::operator-()const {
	return Vector2(-x, -y);
}

Vector2& Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2& Vector2::operator*=(float s) {
	x *= s;
	y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s) {
	x /= s;
	y /= s;
	return *this;
}

////2項演算子オーバーロード////
const Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	Vector2 temp(v1);
	return temp += v2;
}


const Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	Vector2 temp(v1);
	return temp -= v2;
}

const Vector2 operator*(const Vector2& v, float s) {

	Vector2 temp(v);
	return temp *= s;
}

const Vector2 operator*(float s, const Vector2& v) {
	return v * s;
}


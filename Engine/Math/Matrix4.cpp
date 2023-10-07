#include "Matrix4.h"
#include "Vector3.h"
#include <DirectXMath.h>

Matrix4::Matrix4()
{
}

Matrix4 Matrix4::operator*(const Matrix4& m2)
{
	Matrix4 answer;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				answer.m[i][j] += m[i][k] * m2.m[k][j];
			}
		}
	}

	return answer;
}



void Matrix4::ScaleSet(Vector3& v) {
	m[0][0] = v.x;
	m[1][1] = v.y;
	m[2][2] = v.z;
	m[3][3] = 1.0f;
}

void Matrix4::RotXSet(float& x) {
	//x
	m[1][1] = (float)cos(x);
	m[1][2] = (float)sin(x);
	m[2][1] = (float)-sin(x);
	m[2][2] = (float)cos(x);
	/*m[0][0] = 1.0f;
	m[3][3] = 1.0f;*/
}

void Matrix4::RotYSet(float& y) {
	//y
	m[0][0] = (float)cos(y);
	m[0][2] = (float)-sin(y);
	m[2][0] = (float)sin(y);
	m[2][2] = (float)cos(y);
	/*m[1][1] = 1.0f;
	m[3][3] = 1.0f;*/
}

void Matrix4::RotZSet(float& z) {
	//z
	m[0][0] = (float)cos(z);
	m[0][1] = (float)sin(z);
	m[1][0] = (float)-sin(z);
	m[1][1] = (float)cos(z);
	/*m[2][2] = 1.0f;
	m[3][3] = 1.0f;*/
}

void Matrix4::TransSet(Vector3& v) {
	m[3][0] = v.x;
	m[3][1] = v.y;
	m[3][2] = v.z;
	m[3][3] = 1.0f;
}

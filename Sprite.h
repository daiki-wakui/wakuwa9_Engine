#pragma once
#include "SpriteBasis.h"
#include <DirectXMath.h>
using namespace DirectX;

class Sprite
{
private:
	HRESULT result;

	SpriteBasis* spBasis;


public:
	void Initialize(SpriteBasis* spBasis);
	void Update();
	void Draw();

	void VertexData();
};


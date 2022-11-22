#pragma once
#include "DirectXBasis.h"
#include <string>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

class SpriteBasis
{
private:
	HRESULT result;

	DirectXBasis* dxBasis = nullptr;

public:
	void Initialize(DirectXBasis* dxBasis);
	void Update();
	void Draw();

	void LoadShader();

	DirectXBasis* GetDxBasis() const { return dxBasis; };
};


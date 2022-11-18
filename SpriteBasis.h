#pragma once
#include "DirectXBasis.h"

class SpriteBasis
{
private:
	DirectXBasis* dxBasis = nullptr;

public:
	void Initialize(DirectXBasis* dxBasis);
	void Update();
	void Draw();

	DirectXBasis* GetDxBasis() const { return dxBasis; };
};


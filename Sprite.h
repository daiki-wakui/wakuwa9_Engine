#pragma once
#include "SpriteBasis.h"

class Sprite
{
private:
	SpriteBasis* spBasis;


public:
	void Initialize(SpriteBasis* spBasis);
	void Update();
	void Draw();
};


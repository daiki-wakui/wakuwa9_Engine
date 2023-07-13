#pragma once
#include "WindowsApp.h"
#include "DirectXBasis.h"
#include "ImGuiManager.h"
#include "KeyBoard.h"
#include "GamePad.h"
#include "LightGroup.h"
#include "SpriteBasis.h"
#include "Sound.h"

class BaseScene
{
public:

	~BaseScene() = default;

	//������
	virtual void Initialize() = 0;

	//�I��
	virtual void Finalize() = 0;

	//���t���[��
	virtual void Update() = 0;

	//�`��
	virtual void Draw() = 0;

};


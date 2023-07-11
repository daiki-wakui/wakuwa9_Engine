#include "KeyBoard.h"

void KeyBoard::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result;

	//DirectInput�̏�����
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard_->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

}

void KeyBoard::Update()
{
	//�L�[�{�[�h���̎擾�J�n
	keyboard_->Acquire();

	for (int32_t i = 0; i < 256; i++) {
		oldkey_[i] = key_[i];
	}

	//�S�L�[�̓��͏�Ԃ��擾����
	keyboard_->GetDeviceState(sizeof(key_), key_);

}

bool KeyBoard::keyPush(uint8_t key)
{
	return key_[key];
}

bool KeyBoard::keyRelease(uint8_t key)
{
	return !key_[key];
}

bool KeyBoard::keyInstantPush(uint8_t key)
{
	return key_[key] && !oldkey_[key];
}

bool KeyBoard::keyInstantRelease(uint8_t key)
{
	return !key_[key] && oldkey_[key];
}

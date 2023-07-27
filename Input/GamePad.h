#pragma once

#include "Vector2.h"

#include <Windows.h>
#include <Xinput.h>
#include <cstdint>

class GamePad
{
private:

    //���͏��
    XINPUT_STATE state_;
    XINPUT_STATE oldState_;

    const double PI = 3.141492;

private:
    void SetDeadZone(short& sThumb, const short& deaadzone);
public:

    //�X�V����
    void Update();

    //�{�^������(�P����)
    bool PushInstantA();
    bool PushInstantB();
    bool PushInstantX();
    bool PushInstantY();
    //�{�^������
    bool PushButtonA();
    bool PushButtonB();
    bool PushButtonX();
    bool PushButtonY();
    bool PushButtonLB();
    bool PushButtonRB();
    //L�X�e�B�b�N����
    bool InputLStickLeft();
    bool InputLStickRight();
    bool InputLStickUp();
    bool InputLStickDown();
    bool InputLStick();

    bool LTrigger();
    bool RTrigger();
    //R�X�e�B�b�N����
    bool InputRStickLeft();
    bool InputRStickRight();
    bool InputRStickUp();
    bool InputRStickDown();
    bool InputRStick();

    //L�X�e�B�b�N�̊p�x�擾
    float GetLStickAngle(float angle);
    //R�X�e�B�b�N�̊p�x�擾
    float GetRStickAngle(float angle);

    float GetInputPadLX() const { return state_.Gamepad.sThumbLX; }
    float GetInputPadLY() const { return state_.Gamepad.sThumbLY; }
    float GetInputPadRX() const { return state_.Gamepad.sThumbRX; }
    float GetInputPadRY() const { return state_.Gamepad.sThumbRY; }
};


#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <cstdint>
#include "Vector2.h"


class GamePad
{
private:
    XINPUT_STATE state;
    XINPUT_STATE oldState;

    const double PI = 3.141492;
private:
    void SetDeadZone(short& sThumb, const short& deaadzone);
public:

    void Update();
    bool PushButtonA();
    bool PushButtonB();
    bool PushButtonX();
    bool PushButtonY();

    bool InputLStickLeft();
    bool InputLStickRight();
    bool InputLStickUp();
    bool InputLStickDown();

    bool InputLStick();

    float GetLStickAngle();

    float GetInputPadLX() const { return state.Gamepad.sThumbLX; }
    float GetInputPadLY() const { return state.Gamepad.sThumbLY; }
};


#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <cstdint>



class GamePad
{
private:
    XINPUT_STATE state;
    XINPUT_STATE oldState;

    int32_t te = 0;
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

    float GetInputPadLX() const { return state.Gamepad.sThumbLX; }
    float GetInputPadLY() const { return state.Gamepad.sThumbLY; }
};


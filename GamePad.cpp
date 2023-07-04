#include "GamePad.h"
#pragma comment (lib, "xinput.lib")

void GamePad::SetDeadZone(short& sThumb, const short& deaadzone)
{
    if ((sThumb < deaadzone) && sThumb > -deaadzone) {
        sThumb = 0;
    }
}

void GamePad::Update()
{

    DWORD dwResult;

    dwResult = XInputGetState(0, &state);

    if (dwResult == ERROR_SUCCESS)
    {
        // Controller is connected
        te++;
    }
    else
    {
        // Controller is not connected
        te = 100;
    }

    SetDeadZone(state.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    SetDeadZone(state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    SetDeadZone(state.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    SetDeadZone(state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

bool GamePad::PushButtonA()
{
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
        return true;
    }

    return false;
}

bool GamePad::PushButtonB()
{
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
        return true;
    }

    return false;
}

bool GamePad::PushButtonX()
{
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
        return true;
    }

    return false;
}

bool GamePad::PushButtonY()
{
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
        return true;
    }

    return false;
}

bool GamePad::InputLStickLeft()
{

    if (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        return true;
    }

    return false;
}

bool GamePad::InputLStickRight()
{
    if (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        return true;
    }

    return false;
}

bool GamePad::InputLStickUp()
{
    if (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
       return true;
    }

    return false;
}

bool GamePad::InputLStickDown()
{
    if (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        return true;
    }

    return false;
}

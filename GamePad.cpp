#include "GamePad.h"
#pragma comment (lib, "xinput.lib")

#include <cmath>

void GamePad::SetDeadZone(short& sThumb, const short& deaadzone)
{
    if ((sThumb < deaadzone) && sThumb > -deaadzone) {
        sThumb = 0;
    }
}

void GamePad::Update()
{
    //ëOâÒÇÃì¸óÕèÓïÒ
    oldState = state;

    XInputGetState(0, &state);

    SetDeadZone(state.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    SetDeadZone(state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    SetDeadZone(state.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    SetDeadZone(state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

bool GamePad::PushButtonA(){
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) {return true;}
    return false;
}

bool GamePad::PushButtonB(){
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) {return true;}
    return false;
}

bool GamePad::PushButtonX(){
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) {return true;}
    return false;
}

bool GamePad::PushButtonY(){
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {return true;}
    return false;
}

bool GamePad::InputLStickLeft(){
    if (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {return true;}
    return false;
}

bool GamePad::InputLStickRight(){
    if (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {return true;}
    return false;
}

bool GamePad::InputLStickUp(){
    if (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {return true;}
    return false;
}

bool GamePad::InputLStickDown(){
    if (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {return true;}
    return false;
}

bool GamePad::InputLStick()
{
    if (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { return true; }
    if (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { return true; }
    if (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { return true; }
    if (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { return true; }
    return false;
}

float GamePad::GetLStickAngle(float angle)
{
    if (InputLStick() == false) {
        return angle;
    }

    float inputX = GetInputPadLX();
    float inputY = GetInputPadLY();

    float radian;
    float resultAngle;

    Vector2 stickL = { inputX,inputY };
    stickL.normalize();

    radian = std::atan2(stickL.cross({ 0,1 }), -stickL.dot({ 0,-1 }));
    resultAngle = radian * (180 / (float)PI);

    return resultAngle;
}

bool GamePad::InputRStickLeft()
{
    if (state.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    return false;
}

bool GamePad::InputRStickRight()
{
    if (state.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    return false;
}

bool GamePad::InputRStickUp()
{
    if (state.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    return false;
}

bool GamePad::InputRStickDown()
{
    if (state.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    return false;
}

bool GamePad::InputRStick()
{
    if (state.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    if (state.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    if (state.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    if (state.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    return false;
}

float GamePad::GetRStickAngle(float angle)
{
    if (InputRStick() == false) {
        return angle;
    }

    float inputX = GetInputPadRX();
    float inputY = GetInputPadRY();

    float radian;
    float resultAngle;

    Vector2 stickR = { inputX,inputY };
    stickR.normalize();

    radian = std::atan2(stickR.cross({ 0,1 }), -stickR.dot({ 0,-1 }));
    resultAngle = radian * (180 / (float)PI);

    return resultAngle;
}

bool GamePad::LTrigger()
{
    if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
        return true;
    }

    return false;
}

bool GamePad::RTrigger()
{
    if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
        return true;
    }

    return false;
}

bool GamePad::PushButtonLB()
{
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
        return true;
    }

    return false;
}

bool GamePad::PushButtonRB()
{
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
        return true;
    }

    return false;
}

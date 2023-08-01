#include "GamePad.h"

#include <cmath>

#pragma comment (lib, "xinput.lib")

void GamePad::SetDeadZone(short& sThumb, const short& deaadzone)
{
    if ((sThumb < deaadzone) && sThumb > -deaadzone) {
        sThumb = 0;
    }
}

void GamePad::Update()
{
    //ëOâÒÇÃì¸óÕèÓïÒ
    oldState_ = state_;

    XInputGetState(0, &state_);

    SetDeadZone(state_.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    SetDeadZone(state_.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    SetDeadZone(state_.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    SetDeadZone(state_.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

bool GamePad::PushInstantA()
{
    if (state_.Gamepad.wButtons & XINPUT_GAMEPAD_A && oldState_.Gamepad.wButtons != XINPUT_GAMEPAD_A) {
        return true;
    }

    return false;
}

bool GamePad::PushInstantB()
{
    if (state_.Gamepad.wButtons & XINPUT_GAMEPAD_B && oldState_.Gamepad.wButtons != XINPUT_GAMEPAD_B) {
        return true;
    }

    return false;
}

bool GamePad::PushInstantX()
{
    if (state_.Gamepad.wButtons & XINPUT_GAMEPAD_X && oldState_.Gamepad.wButtons != XINPUT_GAMEPAD_X) {
        return true;
    }

    return false;
}

bool GamePad::PushInstantY()
{
    if (state_.Gamepad.wButtons & XINPUT_GAMEPAD_Y && oldState_.Gamepad.wButtons != XINPUT_GAMEPAD_Y) {
        return true;
    }

    return false;
}

bool GamePad::PushButtonA(){
    if (state_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {return true;}
    return false;
}

bool GamePad::PushButtonB(){
    if (state_.Gamepad.wButtons & XINPUT_GAMEPAD_B) {return true;}
    return false;
}

bool GamePad::PushButtonX(){
    if (state_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {return true;}
    return false;
}

bool GamePad::PushButtonY(){
    if (state_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {return true;}
    return false;
}

bool GamePad::InputLStickLeft(){
    if (state_.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {return true;}
    return false;
}

bool GamePad::InputLStickRight(){
    if (state_.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {return true;}
    return false;
}

bool GamePad::InputLStickUp(){
    if (state_.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {return true;}
    return false;
}

bool GamePad::InputLStickDown(){
    if (state_.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {return true;}
    return false;
}

bool GamePad::InputLStick()
{
    if (state_.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { return true; }
    if (state_.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { return true; }
    if (state_.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { return true; }
    if (state_.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) { return true; }
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
    if (state_.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    return false;
}

bool GamePad::InputRStickRight()
{
    if (state_.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    return false;
}

bool GamePad::InputRStickUp()
{
    if (state_.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    return false;
}

bool GamePad::InputRStickDown()
{
    if (state_.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    return false;
}

bool GamePad::InputRStick()
{
    if (state_.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    if (state_.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    if (state_.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
    if (state_.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) { return true; }
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
    if (state_.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
        return true;
    }

    return false;
}

bool GamePad::RTrigger()
{
    if (state_.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
        return true;
    }

    return false;
}

bool GamePad::PushButtonLB()
{
    if (state_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
        return true;
    }

    return false;
}

bool GamePad::PushButtonRB()
{
    if (state_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
        return true;
    }

    return false;
}

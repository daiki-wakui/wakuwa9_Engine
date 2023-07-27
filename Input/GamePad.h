#pragma once

#include "Vector2.h"

#include <Windows.h>
#include <Xinput.h>
#include <cstdint>

class GamePad
{
private:

    //入力状態
    XINPUT_STATE state_;
    XINPUT_STATE oldState_;

    const double PI = 3.141492;

private:
    void SetDeadZone(short& sThumb, const short& deaadzone);
public:

    //更新処理
    void Update();

    //ボタン入力(単押し)
    bool PushInstantA();
    bool PushInstantB();
    bool PushInstantX();
    bool PushInstantY();
    //ボタン入力
    bool PushButtonA();
    bool PushButtonB();
    bool PushButtonX();
    bool PushButtonY();
    bool PushButtonLB();
    bool PushButtonRB();
    //Lスティック入力
    bool InputLStickLeft();
    bool InputLStickRight();
    bool InputLStickUp();
    bool InputLStickDown();
    bool InputLStick();

    bool LTrigger();
    bool RTrigger();
    //Rスティック入力
    bool InputRStickLeft();
    bool InputRStickRight();
    bool InputRStickUp();
    bool InputRStickDown();
    bool InputRStick();

    //Lスティックの角度取得
    float GetLStickAngle(float angle);
    //Rスティックの角度取得
    float GetRStickAngle(float angle);

    float GetInputPadLX() const { return state_.Gamepad.sThumbLX; }
    float GetInputPadLY() const { return state_.Gamepad.sThumbLY; }
    float GetInputPadRX() const { return state_.Gamepad.sThumbRX; }
    float GetInputPadRY() const { return state_.Gamepad.sThumbRY; }
};


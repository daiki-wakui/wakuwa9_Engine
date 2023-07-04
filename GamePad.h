#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <cstdint>

#pragma comment (lib, "xinput.lib")

class GamePad
{
private:
    XINPUT_STATE state;
    XINPUT_STATE prevState;

    int32_t te = 0;
private:
    void SetDeadZone(short& sThumb, const short& deaadzone);
public:

    void Update();
    bool PushButtonA();
    bool PushButtonB();
    bool PushButtonX();
    bool PushButtonY();
};


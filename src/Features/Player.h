#pragma once
#include <CPed.h>

namespace Player {
    extern CPed *g_p_ped;
    extern bool bExitVehicle;
    extern ULONGLONG dwExitTick;

    void
    Update();

    void
    AirBreak();

    void
    AntiStun();
}

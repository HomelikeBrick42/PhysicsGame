#pragma once

#include "Defines.h"

typedef struct Clock Clock;

Clock* Clock_Create();
void Clock_Destroy(Clock* clock);

void Clock_Start(Clock* clock);
void Clock_Update(Clock* clock);
f64 Clock_GetTime(Clock* clock);

#if defined(CLOCK_PRIVATE)
    #if defined(_WIN32) || defined(_WIN64)
        #include <windows.h>

typedef struct Clock {
    f64 InverseFrequency;
    LARGE_INTEGER StartTime;
    LARGE_INTEGER CurrentTime;
} Clock;
    #endif
#endif

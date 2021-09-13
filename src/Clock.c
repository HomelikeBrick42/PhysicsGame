#define CLOCK_PRIVATE
#include "Clock.h"

#if defined(_WIN32) || defined(_WIN64)

Clock* Clock_Create() {
    Clock* clock = malloc(sizeof(Clock));
    LARGE_INTEGER frequency;
    if (!QueryPerformanceFrequency(&frequency)) {
        free(clock);
        return nil;
    }
    clock->InverseFrequency = 1.0 / cast(f64) frequency.QuadPart;
    return clock;
}

void Clock_Destroy(Clock* clock) {
    free(clock);
}

void Clock_Start(Clock* clock) {
    QueryPerformanceCounter(&clock->StartTime);
    QueryPerformanceCounter(&clock->CurrentTime);
}

void Clock_Update(Clock* clock) {
    QueryPerformanceCounter(&clock->CurrentTime);
}

f64 Clock_GetTime(Clock* clock) {
    return cast(f64) clock->CurrentTime.QuadPart * clock->InverseFrequency;
}

#endif

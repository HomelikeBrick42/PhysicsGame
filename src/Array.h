#pragma once

#include "Defines.h"

#define Array(type) type*

#define Array_Create(type) (cast(Array(type)) Array_Create_(sizeof(type)))

#define Array_Push(array, value)                               \
    do {                                                       \
        __typeof__(*(array)) temp = value;                     \
        (array)                   = Array_Push_(array, &temp); \
    } while (0)

void* Array_Create_(u64 elementSize);
void Array_Destroy(void* array);

void* Array_Push_(void* array, void* value);
u64 Array_GetLength(void* array);

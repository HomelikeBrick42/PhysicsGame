#include "Array.h"

#include <stdlib.h>
#include <string.h>

enum {
    ELEMENT_SIZE,
    CAPACITY,
    LENGTH,
    COUNT,
};

void* Array_Create_(u64 elementSize) {
    u64* header = malloc(COUNT * sizeof(u64));
    header[ELEMENT_SIZE] = elementSize;
    header[CAPACITY] = 0;
    header[LENGTH] = 0;
    return header + COUNT;
}

void Array_Destroy(void* array) {
    u64* header = cast(u64*) array - COUNT;
    free(header);
}

void* Array_Push_(void* array, void* value) {
    u64* header = cast(u64*) array - COUNT;

    if (header[LENGTH] >= header[CAPACITY]) {
        u64 newCapacity = header[CAPACITY] == 0 ? 1 : header[CAPACITY] * 2;
        u64* newHeader = malloc(COUNT * sizeof(u64) + newCapacity * header[ELEMENT_SIZE]);

        memcpy(newHeader, header, COUNT * sizeof(u64) + header[LENGTH] * header[ELEMENT_SIZE]);
        newHeader[CAPACITY] = newCapacity;
        free(header);
        header = newHeader;
        array = header + COUNT;
    }

    memcpy(&(cast(u8*) array)[header[LENGTH]++ * header[ELEMENT_SIZE]], value, header[ELEMENT_SIZE]);
    return array;
}

u64 Array_GetLength(void* array) {
    u64* header = cast(u64*) array - COUNT;
    return header[LENGTH];
}

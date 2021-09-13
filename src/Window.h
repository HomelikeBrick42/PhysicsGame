#pragma once

#include "Defines.h"

typedef struct Window Window;

typedef enum KeyCode {
    KeyCode_Unknown = 0,

    KeyCode_0 = '0',
    KeyCode_1 = '1',
    KeyCode_2 = '2',
    KeyCode_3 = '3',
    KeyCode_4 = '4',
    KeyCode_5 = '5',
    KeyCode_6 = '6',
    KeyCode_7 = '7',
    KeyCode_8 = '8',
    KeyCode_9 = '9',

    KeyCode_A = 'A',
    KeyCode_B = 'B',
    KeyCode_C = 'C',
    KeyCode_D = 'D',
    KeyCode_E = 'E',
    KeyCode_F = 'F',
    KeyCode_G = 'G',
    KeyCode_H = 'H',
    KeyCode_I = 'I',
    KeyCode_J = 'J',
    KeyCode_K = 'K',
    KeyCode_L = 'L',
    KeyCode_M = 'M',
    KeyCode_N = 'N',
    KeyCode_O = 'O',
    KeyCode_P = 'P',
    KeyCode_Q = 'Q',
    KeyCode_R = 'R',
    KeyCode_S = 'S',
    KeyCode_T = 'T',
    KeyCode_U = 'U',
    KeyCode_V = 'V',
    KeyCode_W = 'W',
    KeyCode_X = 'X',
    KeyCode_Y = 'Y',
    KeyCode_Z = 'Z',
} KeyCode;

typedef void Window_CloseCallbackFunc(Window* window);
typedef void Window_ResizeCallbackFunc(Window* window, u32 width, u32 height);
typedef void Window_MouseMoveCallbackFunc(Window* window, s32 xDelta, s32 yDelta);
typedef void Window_KeyCallbackFunc(Window* window, KeyCode key, b8 pressed);
typedef void Window_DrawCallbackFunc(Window* window);

Window* Window_Create(u32 width, u32 height);
void Window_Destroy(Window* window);

void Window_Show(Window* window);
void Window_Hide(Window* window);
void Window_EnableMouse(Window* window);
void Window_DisableMouse(Window* window);
void Window_PollEvents(Window* window);
void Window_InvalidatePixels(Window* window);

void Window_SetCloseCallback(Window* window, Window_CloseCallbackFunc* callback);
void Window_SetResizeCallback(Window* window, Window_ResizeCallbackFunc* callback);
void Window_SetMouseMoveCallback(Window* window, Window_MouseMoveCallbackFunc* callback);
void Window_SetKeyCallback(Window* window, Window_KeyCallbackFunc* callback);
void Window_SetDrawCallback(Window* window, Window_DrawCallbackFunc* callback);
void Window_SetUserData(Window* window, void* userData);

void* Window_GetUserData(Window* window);

#if defined(WINDOW_PRIVATE)
    #if defined(_WIN32) || defined(_WIN64)
        #include <windows.h>
    #endif

typedef struct Window {
    #if defined(_WIN32) || defined(_WIN64)
    HINSTANCE Instance;
    HWND Handle;
    HDC DeviceContext;
    #endif
    Window_CloseCallbackFunc* CloseCallback;
    Window_ResizeCallbackFunc* ResizeCallback;
    Window_MouseMoveCallbackFunc* MouseMoveCallback;
    Window_KeyCallbackFunc* KeyCallback;
    Window_DrawCallbackFunc* DrawCallback;
    b8 MouseDisabled;
    void* UserData;
} Window;
#endif

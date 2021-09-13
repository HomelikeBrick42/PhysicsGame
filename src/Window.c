#define WINDOW_PRIVATE
#include "Window.h"

#include <stdlib.h>
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)

    #include <windowsx.h>

static const char* WindowClassName = "GameWindowClass";
static const DWORD WindowStyle     = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
static const DWORD WindowStyleEx   = WS_EX_APPWINDOW;

static b8 WindowClassRegistered = FALSE;

static LRESULT CALLBACK WindowCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    if (message == WM_NCCREATE) {
        CREATESTRUCTA* createStruct = cast(CREATESTRUCTA*) lParam;
        SetWindowLongPtrA(windowHandle, GWLP_USERDATA, cast(LONG_PTR) createStruct->lpCreateParams);
        result = DefWindowProcA(windowHandle, message, wParam, lParam);
    } else {
        Window* window = cast(Window*) GetWindowLongPtrA(windowHandle, GWLP_USERDATA);
        switch (message) {
            case WM_QUIT:
            case WM_CLOSE:
            case WM_DESTROY: {
                if (window->CloseCallback != nil) {
                    window->CloseCallback(window);
                }
            } break;

            case WM_SIZE: {
                RECT windowRect;
                if (GetClientRect(window->Handle, &windowRect)) {
                    int width  = windowRect.right - windowRect.left;
                    int height = windowRect.bottom - windowRect.top;
                    if (width > 0 && height > 0) {
                        window->Width  = width;
                        window->Height = height;
                        if (window->ResizeCallback != nil) {
                            window->ResizeCallback(window, cast(u32) width, cast(u32) height);
                        }
                    }
                }
                goto Default;
            } break;

            case WM_NCPAINT:
            case WM_PAINT: {
                if (window->DrawCallback != nil) {
                    window->DrawCallback(window);
                }
                goto Default;
            } break;

            case WM_MOVE: {
                if (window->MouseDisabled) {
                    Window_DisableMouse(window);
                }
            } break;

            case WM_MOUSEWHEEL: {
                if (window->MouseScrollCallback != nil) {
                    s32 xDelta = GET_X_LPARAM(wParam);
                    xDelta     = xDelta == 0 ? 0 : xDelta < 0 ? -1 : 1;
                    s32 yDelta = GET_Y_LPARAM(wParam);
                    yDelta     = yDelta == 0 ? 0 : yDelta < 0 ? -1 : 1;
                    window->MouseScrollCallback(window, xDelta, yDelta);
                }
            } break;

            case WM_KILLFOCUS: {
                if (window->MouseDisabled) {
                    while (ShowCursor(TRUE) < 0) {
                    }
                    ClipCursor(nil);
                }
            } break;

            case WM_SETFOCUS: {
                if (window->MouseDisabled) {
                    while (ShowCursor(FALSE) >= 0) {
                    }
                    RECT rect;
                    GetClientRect(window->Handle, &rect);
                    MapWindowPoints(window->Handle, nil, cast(POINT*) & rect, sizeof(RECT) / sizeof(POINT));
                    ClipCursor(&rect);
                }
            } break;

            case WM_MOUSEMOVE: {
                if (!window->MouseDisabled && window->MousePositionCallback != nil) {
                    s32 mouseX = GET_X_LPARAM(lParam);
                    s32 mouseY = GET_X_LPARAM(lParam);
                    window->MousePositionCallback(window, mouseX, mouseY);
                }
            } break;

            case WM_INPUT: {
                if (window->MouseDisabled && window->MouseMoveCallback != nil) {
                    HRAWINPUT inputHandle = cast(HRAWINPUT) lParam;
                    UINT size;
                    GetRawInputData(inputHandle, RID_INPUT, nil, &size, sizeof(RAWINPUTHEADER));
                    BYTE bytes[size];
                    GetRawInputData(inputHandle, RID_INPUT, bytes, &size, sizeof(RAWINPUTHEADER));
                    RAWINPUT* input = cast(RAWINPUT*) bytes;

                    s32 mouseX = input->data.mouse.lLastX;
                    s32 mouseY = -input->data.mouse.lLastY;
                    window->MouseMoveCallback(window, mouseX, mouseY);
                }
            } break;

            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP: {
                if (window->KeyCallback != nil) {
                    b8 pressed = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;

                    KeyCode key;
                    switch (wParam) {
    #define KEY(name, value) \
        case value: {        \
            key = name;      \
        } break;
                        KEY(KeyCode_Escape, VK_ESCAPE)

                        KEY(KeyCode_0, 0x30)
                        KEY(KeyCode_1, 0x31)
                        KEY(KeyCode_2, 0x32)
                        KEY(KeyCode_3, 0x33)
                        KEY(KeyCode_4, 0x34)
                        KEY(KeyCode_5, 0x35)
                        KEY(KeyCode_6, 0x36)
                        KEY(KeyCode_7, 0x37)
                        KEY(KeyCode_8, 0x38)
                        KEY(KeyCode_9, 0x39)

                        KEY(KeyCode_A, 0x41)
                        KEY(KeyCode_B, 0x42)
                        KEY(KeyCode_C, 0x43)
                        KEY(KeyCode_D, 0x44)
                        KEY(KeyCode_E, 0x45)
                        KEY(KeyCode_F, 0x46)
                        KEY(KeyCode_G, 0x47)
                        KEY(KeyCode_H, 0x48)
                        KEY(KeyCode_I, 0x49)
                        KEY(KeyCode_J, 0x4A)
                        KEY(KeyCode_K, 0x4B)
                        KEY(KeyCode_L, 0x4C)
                        KEY(KeyCode_M, 0x4D)
                        KEY(KeyCode_N, 0x4E)
                        KEY(KeyCode_O, 0x4F)
                        KEY(KeyCode_P, 0x50)
                        KEY(KeyCode_Q, 0x51)
                        KEY(KeyCode_R, 0x52)
                        KEY(KeyCode_S, 0x53)
                        KEY(KeyCode_T, 0x54)
                        KEY(KeyCode_U, 0x55)
                        KEY(KeyCode_V, 0x56)
                        KEY(KeyCode_W, 0x57)
                        KEY(KeyCode_X, 0x58)
                        KEY(KeyCode_Y, 0x59)
                        KEY(KeyCode_Z, 0x5A)
    #undef KEY

                        default: {
                            key = KeyCode_Unknown;
                        } break;
                    }

                    u16 repeatCount = lParam & 0xFFFF;
                    for (u16 i = 0; i < repeatCount; i++) {
                        window->KeyCallback(window, key, pressed);
                    }
                }
                goto Default;
            } break;

            default:
Default:
                result = DefWindowProcA(windowHandle, message, wParam, lParam);
                break;
        }
    }

    return result;
}

Window* Window_Create(u32 width, u32 height) {
    Window* window = calloc(1, sizeof(Window));
    if (window == nil) {
        return nil;
    }

    window->Instance = GetModuleHandleA(nil);
    if (window->Instance == nil) {
        goto Error;
    }

    if (!WindowClassRegistered) {
        if (!RegisterClassExA(&(WNDCLASSEXA){
                .cbSize        = sizeof(WNDCLASSEXA),
                .style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
                .lpfnWndProc   = WindowCallback,
                .hInstance     = window->Instance,
                .hCursor       = LoadCursor(nil, IDC_ARROW),
                .lpszClassName = WindowClassName,
            })) {
            goto Error;
        }
        WindowClassRegistered = TRUE;
    }

    RECT windowRect   = {};
    windowRect.left   = 100;
    windowRect.right  = windowRect.left + (LONG)width;
    windowRect.top    = 100;
    windowRect.bottom = windowRect.top + (LONG)height;
    if (!AdjustWindowRectEx(&windowRect, WindowStyle, FALSE, WindowStyleEx)) {
        goto Error;
    }

    int windowWidth  = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    window->Handle = CreateWindowExA(WindowStyleEx,
                                     WindowClassName,
                                     "Game Window",
                                     WindowStyle,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     windowWidth,
                                     windowHeight,
                                     nil,
                                     nil,
                                     window->Instance,
                                     window);
    if (window->Handle == nil) {
        goto Error;
    }

    window->DeviceContext = GetDC(window->Handle);
    if (window->DeviceContext == nil) {
        goto Error;
    }

    if (!RegisterRawInputDevices(
            &(RAWINPUTDEVICE){
                .usUsagePage = 0x01,
                .usUsage     = 0x02,
            },
            1,
            sizeof(RAWINPUTDEVICE))) {
        goto Error;
    }

    return window;
Error:
    free(window);
    return nil;
}

void Window_Destroy(Window* window) {
    ReleaseDC(window->Handle, window->DeviceContext);
    DestroyWindow(window->Handle);
    free(window);
}

void Window_Show(Window* window) {
    ShowWindow(window->Handle, SW_SHOW);
}

void Window_Hide(Window* window) {
    ShowWindow(window->Handle, SW_HIDE);
}

void Window_EnableMouse(Window* window) {
    while (ShowCursor(TRUE) < 0) {
    }
    ClipCursor(nil);
    window->MouseDisabled = FALSE;
}

void Window_DisableMouse(Window* window) {
    while (ShowCursor(FALSE) >= 0) {
    }
    RECT rect;
    GetClientRect(window->Handle, &rect);
    MapWindowPoints(window->Handle, nil, cast(POINT*) & rect, sizeof(RECT) / sizeof(POINT));
    ClipCursor(&rect);
    window->MouseDisabled = TRUE;
}

void Window_PollEvents(Window* window) {
    MSG message;
    while (PeekMessageA(&message, window->Handle, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

void Window_InvalidatePixels(Window* window) {
    InvalidateRect(window->Handle, nil, TRUE);
}

void Window_SetUserData(Window* window, void* userData) {
    window->UserData = userData;
}

void Window_SetCloseCallback(Window* window, Window_CloseCallbackFunc* callback) {
    window->CloseCallback = callback;
}

void Window_SetResizeCallback(Window* window, Window_ResizeCallbackFunc* callback) {
    window->ResizeCallback = callback;
    RECT windowRect;
    if (GetClientRect(window->Handle, &windowRect)) {
        int width  = windowRect.right - windowRect.left;
        int height = windowRect.bottom - windowRect.top;
        if (width > 0 && height > 0) {
            window->ResizeCallback(window, cast(u32) width, cast(u32) height);
        }
    }
}

void Window_SetMousePositionCallback(Window* window, Window_MousePositionCallbackFunc* callback) {
    window->MousePositionCallback = callback;
}

void Window_SetMouseMoveCallback(Window* window, Window_MouseMoveCallbackFunc* callback) {
    window->MouseMoveCallback = callback;
}

void Window_SetMouseScrollCallback(Window* window, Window_MouseScrollCallbackFunc* callback) {
    window->MouseScrollCallback = callback;
}

void Window_SetKeyCallback(Window* window, Window_KeyCallbackFunc* callback) {
    window->KeyCallback = callback;
}

void Window_SetDrawCallback(Window* window, Window_DrawCallbackFunc* callback) {
    window->DrawCallback = callback;
}

void* Window_GetUserData(Window* window) {
    return window->UserData;
}

void Window_GetSize(Window* window, u32* width, u32* height) {
    if (width != nil) {
        *width = window->Width;
    }
    if (height != nil) {
        *height = window->Height;
    }
}

b8 Window_IsMouseDisabled(Window* window) {
    return window->MouseDisabled;
}

#endif

#pragma once

// tell the compiler to not include many unneeded header files
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "resource.h"
#include "framework.h"

#define WINDOWCLASS L"FallingBlockGame"
#define WINDOWTITLE L"A Falling Block Game!"

HINSTANCE hInstMain = NULL; //main app handle
HWND hWndMain = NULL; //main window handle
LRESULT CALLBACK TheWindowProc(HWND, UINT, WPARAM, LPARAM);
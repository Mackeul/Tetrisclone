// TetrisClone.cpp : Defines the entry point for the application.
//
#include "Game.h"
#include "TetrisClone.h"

static Game* theGame = Game::Instance();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    //assign instance to global variable
    hInstMain = hInstance;

    //create window class
    WNDCLASSEX wcx;

    //set the size of the structure
    wcx.cbSize = sizeof(WNDCLASSEX);

    //class style
    wcx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

    //window procedure
    wcx.lpfnWndProc = TheWindowProc;

    //class extra
    wcx.cbClsExtra = 0;

    //window extra
    wcx.cbWndExtra = 0;

    //application handle
    wcx.hInstance = hInstMain;

    //icon
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    //cursor
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);

    //background color
    wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    //menu
    wcx.lpszMenuName = NULL;

    //class name
    wcx.lpszClassName = WINDOWCLASS;

    //small icon
    wcx.hIconSm = NULL;

    //register the window class, return 0 if not successful
    if (!RegisterClassEx(&wcx)) {
        return(0);
    }

    //create main window
    hWndMain = CreateWindowEx(0, WINDOWCLASS, WINDOWTITLE, WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_VISIBLE, 0, 0, 320, 240, NULL, NULL, hInstMain, NULL);

    //error check
    if (!hWndMain) {
        return(0);
    }

    //if program initialization failed, return 0
    if (!theGame->Init(hWndMain)) {
        return(0);
    }
    //invalidate the window rect
    InvalidateRect(hWndMain, NULL, FALSE);

    //Message structure
    MSG msg;

    //message pump  
    while (true) {

        //look for message
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

            // there was a message

            //check that we arent quitting
            if (msg.message == WM_QUIT) {
                break;  // break out of for loop message pump
            }

            //translate message
            TranslateMessage(&msg);

            //dispatch message
            DispatchMessage(&msg);
        }

        //run the main game loop
        theGame->Tick();

        //invalidate the window rect
        InvalidateRect(hWndMain, NULL, FALSE);
    }

    //clean up program data
    theGame->Done();

    //return the wparam from the WM_QUIT message
    return(msg.wParam);

}

LRESULT CALLBACK TheWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    //which message did we get?

    if (theGame->IsPaused()) {
        if (uMsg == WM_KEYDOWN && wParam == VK_PAUSE) {
            theGame->TogglePause();
            return(0);
        }
    }
    switch (uMsg) {
    case WM_KEYDOWN: {
        //check for escape key
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hWndMain);
            return(0); //handled message
        }
        else if (wParam == VK_DOWN) { // check for down arrow key
            theGame->MovePiece(0, 1);
            return(0); //handled message
        }
        else if (wParam == VK_UP || wParam == VK_SPACE) { // check for up arrow key or space bar for block rotation.
            theGame->RotatePiece();
            return(0); //handled message
        }
        else if (wParam == VK_LEFT) { // check for left arrow key
            theGame->MovePiece(-1,0);
            return(0); //handled message
        }
        else if (wParam == VK_RIGHT) { // check for right arrow key
            theGame->MovePiece(1,0);
            return(0); //handled message
        }
        else if (wParam == VK_PAUSE) {
            theGame->TogglePause();
            return(0);
        }
    }
        break;
    case WM_DESTROY: {  //window is being destroyed.

        //tell the aplication we are quitting
        PostQuitMessage(0);

        //handled message
        return(0);
    }
        break;
    case WM_PAINT: { // the window needs repainting
        
        theGame->PaintMap(hwnd);

        //handled message
        return(0);
    }
        break;

    }//end switch

    //pass along any other message to default message handler
    return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}



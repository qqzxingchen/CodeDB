#include <windows.h>

LPCTSTR szAppName = TEXT("PEViewerAppName");
LPCTSTR szWndName = TEXT("PEViewer");

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
bool registerWndClass( HINSTANCE hInstance ){
    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = GetModuleHandle(NULL);
    wndclass.hIcon = NULL;
    wndclass.hCursor = NULL;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    return RegisterClass(&wndclass);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR szCmdLine, int iCmdShow)
{
    HWND hwnd;
    MSG msg;
    registerWndClass( hInstance );
    hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST,
        szAppName, 
        szWndName,
        WS_THICKFRAME,
        100,100,400,400,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT pt;

    switch (message)
    {
    case WM_CREATE:
        for ( int i = 100 ; i < 200 ; i ++ )
            for ( int j = 100 ; j < 200 ; j ++ )
                SetLayeredWindowAttributes(hwnd,RGB(i,j,100),200,LWA_COLORKEY|LWA_ALPHA);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd,&pt);
        for ( int i = 0 ; i < 255 ; i ++ )
            for ( int j = 0 ; j < 255 ; j ++ )
                SetPixel(hdc,i,j,RGB(i,j,100));

        EndPaint(hwnd,&pt);
        break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}



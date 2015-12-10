#include <windows.h>
#include <stdio.h>

int i=0;
char szchar[200]={'\0'};

LRESULT CALLBACK WinSunProc
(
    HWND   hwnd, 
    UINT   umsg,
	WPARAM wparam,
    LPARAM lparam
)
{
	HDC hdc;
    switch(umsg)
    {
	case WM_CHAR:
		szchar[i]=wparam;
		i++;
		MessageBox(hwnd,szchar,"xingchen",MB_YESNO);
        break;
    case WM_LBUTTONDOWN:
        MessageBox(hwnd,"left button down","xingchen",0);
		hdc=GetDC(hwnd);
        TextOut(hdc,0,50,"xingchen",strlen("xingchen"));
		ReleaseDC(hwnd,hdc);
		break;
    case WM_PAINT:
		PAINTSTRUCT ps;
		hdc=BeginPaint(hwnd,&ps);
		TextOut(hdc,20,20,"jiujie",strlen("jiujie"));
		EndPaint(hwnd,&ps);
		break;
    case WM_CLOSE:
		if(IDYES==MessageBox(hwnd,"你真的要关闭吗？？？","close",MB_YESNO))
		{
            DestroyWindow(hwnd);		
		}
		break;
    case WM_DESTROY:
		PostQuitMessage(0);
		break;
    default:
		return DefWindowProc(hwnd,umsg,wparam,lparam);
	}
  return 0;
}


int WINAPI WinMain
(
    HINSTANCE  hInstance, 
    HINSTANCE  bPrevInstance, 
    LPSTR      lpCmdLine,
    int        nCmdShow
)
{
    WNDCLASS wndclass;
    wndclass.cbClsExtra=0;
	wndclass.cbWndExtra=0;
	wndclass.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
    wndclass.hIcon=LoadIcon(NULL,IDI_ERROR);
    wndclass.hInstance=hInstance;
    wndclass.lpfnWndProc=WinSunProc;
	wndclass.lpszClassName="xingchen";
    wndclass.lpszMenuName=NULL;
	wndclass.style=CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wndclass);
    
	HWND hwnd;
    hwnd=CreateWindow
		(
           "xingchen",
		   "xingchen",
		   WS_OVERLAPPEDWINDOW,
		   50,
		   50,
		   300,
		   400,
           NULL,
		   NULL,
		   hInstance,
		   NULL
		);
   ShowWindow(hwnd,SW_SHOWNORMAL);
   UpdateWindow(hwnd);
   
   MSG msg;
   while(GetMessage(&msg,NULL,0,0))
   {
         TranslateMessage(&msg);
		 DispatchMessage(&msg);
   }
   return 0;
}
#include <windows.h>
#include <stdio.h>

#define xscreen GetSystemMetrics(SM_CXSCREEN)
#define yscreen GetSystemMetrics(SM_CYSCREEN)
#define font_high 15
#define font_width 5
unsigned int ID_TIME=1000;

TCHAR ch[]="生日快乐";
int i=0;

void draw(HDC hdcmem);

LRESULT CALLBACK WinSunProc
(
    HWND   hwnd, 
    UINT   umsg,
	WPARAM wparam,
    LPARAM lparam
)
{
    HDC    hdc;
	HFONT  hfont;
	static HDC hdcmem;
    static HBITMAP hbitmap;
    


	switch(umsg)
    {

    case WM_CREATE:
		 SetTimer(hwnd,ID_TIME,100,NULL);
		 
		 hdc=GetDC(hwnd);
         hdcmem=CreateCompatibleDC(hdc);
         hbitmap=CreateCompatibleBitmap(hdc,xscreen,yscreen);
         SelectObject(hdcmem,hbitmap);
		 
		 ReleaseDC(hwnd,hdc);

		 hfont=CreateFont(
			              3*font_high,               //新字体的高 
			              3*font_width,                //        宽
				    	  0,0, 
					      FW_BOLD,          //笔画粗细，0-1000,400为标准，700为粗体
					      0,                //TRUE 是 斜体
					      0,                //TRUE 是 下划线
					      0,                //TRUE 是 删除线
                          DEFAULT_CHARSET,  //字符集
					      OUT_DEFAULT_PRECIS, 
					      CLIP_DEFAULT_PRECIS, 
                          DRAFT_QUALITY, 
					      FIXED_PITCH | FF_SWISS, 
					      NULL
					     );
         SelectObject(hdcmem,hfont);   DeleteObject(hfont);
	     SetBkMode(hdcmem,TRANSPARENT);
         break;

    case WM_TIMER:
		 hdc=GetDC(hwnd);
         PatBlt(hdcmem,0,0,xscreen,yscreen,BLACKNESS);
         
		 draw(hdcmem);
		 
		 BitBlt(hdc,0,0,xscreen,yscreen,hdcmem,0,0,SRCCOPY);
		 ReleaseDC(hwnd,hdc);
		
		 break;

    case WM_LBUTTONDOWN:
	case WM_DESTROY:

		 KillTimer(hwnd,ID_TIME);
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
		   NULL,
		   WS_DLGFRAME | WS_THICKFRAME | WS_POPUP,
		   50,
		   50,
		   300,
		   400,
           NULL,
		   NULL,
		   hInstance,
		   NULL
		);
   ShowWindow(hwnd,SW_SHOWMAXIMIZED);
   UpdateWindow(hwnd);
   
   MSG msg;
   while(GetMessage(&msg,NULL,0,0))
   {
         TranslateMessage(&msg);
		 DispatchMessage(&msg);
   }
   return msg.wParam;
}


void draw(HDC hdcmem)
{

}
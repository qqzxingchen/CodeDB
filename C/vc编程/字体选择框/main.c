#include <windows.h>
#include <stdio.h>
int i=0;

LRESULT CALLBACK WinSunProc
(
    HWND   hwnd,
    UINT   umsg,
	WPARAM wparam,
    LPARAM lparam
)
{
    LOGFONT lf = {0};  //结构体全部赋值为零，表示是默认字体
    CHOOSEFONT cf = {0};
    TEXTMETRIC tm;
    HDC hdc;
    switch(umsg)
    {
    case WM_RBUTTONDOWN:
	    hdc=GetDC(hwnd);
        cf.lStructSize = sizeof(CHOOSEFONT);
           //结构体大小
        cf.lpLogFont = &lf;
           //当打开字体选择框，选择字体后，就将新的字体参数放进lf中
        cf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT;
           //字体选择框的属性

		ChooseFont(&cf);
		   //创建一个使用户选择逻辑字体属性的对话框

		SelectObject(hdc,CreateFontIndirect(cf.lpLogFont));
        TextOut(hdc,100,i,"Hello World",strlen("Hello World"));

		GetTextMetrics(hdc,&tm);
		   //取得当前字体的属性
		i+=tm.tmHeight;
		   //当前字体的高
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
    MSG msg;
    HWND hwnd;

    wndclass.cbClsExtra=0;
	wndclass.cbWndExtra=0;
	wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
    wndclass.hIcon=LoadIcon(NULL,IDI_ERROR);
    wndclass.hInstance=hInstance;
    wndclass.lpfnWndProc=WinSunProc;
	wndclass.lpszClassName="xingchen";
    wndclass.lpszMenuName=NULL;
	wndclass.style=CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wndclass);


    hwnd=CreateWindow
		(
           "xingchen",
		   "xingchen",
		   WS_OVERLAPPEDWINDOW,
		   50,
		   50,
		   1000,
		   600,
           NULL,
		   NULL,
		   hInstance,
		   NULL
		);
   ShowWindow(hwnd,SW_SHOWNORMAL);
   UpdateWindow(hwnd);

   while(GetMessage(&msg,NULL,0,0))
   {
         TranslateMessage(&msg);
		 DispatchMessage(&msg);
   }
   return 0;
}

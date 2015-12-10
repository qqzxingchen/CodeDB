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
    LOGFONT lf = {0};  //�ṹ��ȫ����ֵΪ�㣬��ʾ��Ĭ������
    CHOOSEFONT cf = {0};
    TEXTMETRIC tm;
    HDC hdc;
    switch(umsg)
    {
    case WM_RBUTTONDOWN:
	    hdc=GetDC(hwnd);
        cf.lStructSize = sizeof(CHOOSEFONT);
           //�ṹ���С
        cf.lpLogFont = &lf;
           //��������ѡ���ѡ������󣬾ͽ��µ���������Ž�lf��
        cf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT;
           //����ѡ��������

		ChooseFont(&cf);
		   //����һ��ʹ�û�ѡ���߼��������ԵĶԻ���

		SelectObject(hdc,CreateFontIndirect(cf.lpLogFont));
        TextOut(hdc,100,i,"Hello World",strlen("Hello World"));

		GetTextMetrics(hdc,&tm);
		   //ȡ�õ�ǰ���������
		i+=tm.tmHeight;
		   //��ǰ����ĸ�
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

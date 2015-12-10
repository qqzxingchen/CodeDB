#include <windows.h>

LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY WinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance,
					 LPTSTR lpCmdLine, 
					 int nCmdShow)
{   
	MSG msg;
	HWND hWnd; 
	WNDCLASS wc;

	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=(WNDPROC)WndProc;       
	wc.cbClsExtra=0;                      
	wc.cbWndExtra=0;                      
	wc.hInstance=hInstance;              
	wc.hIcon=LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor=LoadCursor(NULL,IDC_CROSS);
	wc.hbrBackground=0;				//�����ֵ��Ϊ0�������ڴ򿪵�ʱ��Ͳ����ػ棬��˴��ڵĿͻ����ͻ��ǵ�ǰ����
									//ֻ��xp����Ч
	wc.lpszMenuName=lpszAppName;              
	wc.lpszClassName=lpszAppName;
	
	if(!RegisterClass(&wc))
		return(FALSE);
	hWnd=CreateWindow(lpszAppName,  
		lpszTitle,
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT,
		0, 
		CW_USEDEFAULT, 
		0,  
		NULL, 
		NULL,  
		hInstance, 
		NULL);               
	if (!hWnd) 
		return(FALSE);
	ShowWindow(hWnd, SW_MAXIMIZE); 
	UpdateWindow(hWnd);         
	
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg); 
		DispatchMessage(&msg);  
	}
	
	return(msg.wParam); 
}


LRESULT CALLBACK WndProc(HWND hWnd, 
						 UINT uMsg, 
						 WPARAM wParam,
						 LPARAM lParam)
{
	static HDC	hdcDesktop;
	HDC			hdc;
	int			x,y;
	COLORREF	color;
	RECT		rect;
	PAINTSTRUCT ps;
	char temp[50];

	switch(uMsg){   
	case WM_CREATE:
		if(MessageBox(NULL,"ʹ�÷������������ͬʱ���£���õ����ɫ��Ϣ�ͻ���ʾ�����Ͻ�",
			"������Ļȡɫ��",MB_OKCANCEL)==IDOK)
		{
			Sleep(1000);
			hdcDesktop=CreateDC("DISPLAY",NULL,NULL,NULL);
			
			break;
		}
		else
		{
			PostQuitMessage(0);
		}
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		BitBlt(hdc,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),hdcDesktop,0,0,SRCCOPY);
		EndPaint(hWnd,&ps);
		break;
	case WM_LBUTTONDOWN:
		if((wParam&MK_RBUTTON)==MK_RBUTTON)
		{
			x=LOWORD(lParam);
			y=HIWORD(lParam);
			
			hdc=GetDC(hWnd);
			color=GetPixel(hdc,x,y);
			GetWindowRect(hWnd,&rect);
			rect.right-=10;
			rect.top+=5;

			wsprintf(temp,"x=%d,y=%d;Red:%d;Green:%d;Blue:%d",x,y,GetRValue(color),GetGValue(color),GetBValue(color));
			DrawText(hdc,temp,-1,&rect,DT_SINGLELINE|DT_LEFT);
		}
		return 0;
	case WM_RBUTTONDOWN:		
		if((wParam&MK_LBUTTON)==MK_LBUTTON)
		{
			x=LOWORD(lParam);
			y=HIWORD(lParam);
			
			hdc=GetDC(hWnd);
			color=GetPixel(hdc,x,y);			
			GetWindowRect(hWnd,&rect);
			rect.right-=10;
			rect.top+=5;

			wsprintf(temp,"x=%d,y=%d;Red:%d;Green:%d;Blue:%d",x,y,GetRValue(color),GetGValue(color),GetBValue(color));
			DrawText(hdc,temp,-1,&rect,DT_SINGLELINE|DT_LEFT);
		}
		return 0;

	case WM_DESTROY :
		PostQuitMessage(0);
		return 0;
		
    }
	return  DefWindowProc(hWnd, uMsg, wParam, lParam);
}


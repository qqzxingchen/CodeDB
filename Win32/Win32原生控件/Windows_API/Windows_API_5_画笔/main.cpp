#include <windows.h>

HWND desktop_hwnd;
LPCTSTR str;
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
	
	str=GetCommandLine();
	desktop_hwnd=GetDesktopWindow();
	
	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=(WNDPROC)WndProc;       
	wc.cbClsExtra=0;                      
	wc.cbWndExtra=0;                      
	wc.hInstance=hInstance;              
	wc.hIcon=LoadIcon(NULL, IDI_APPLICATION); 
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
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
	ShowWindow(hWnd, nCmdShow); 
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
	RECT rect;
	HDC hdc;
	PAINTSTRUCT ps;
	static int uRed=0, uGreen=0, uBlue=0;

	static HPEN oldPen, cosPen[4], geoPen[3];
	LOGPEN lp;
	LOGBRUSH lb;

	
	switch(uMsg){   
	case WM_PAINT :
		hdc=BeginPaint(hWnd, &ps);
		//DrawText (hdc, TEXT ("Hello, You Are Welcoming!"), -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;	
		
		LOGBRUSH logbrush;
		char tempp[10];
		int i;
		for(i=0;i<=6;i++){
			logbrush.lbStyle = PS_SOLID;
			logbrush.lbColor = RGB(i*uRed,i*uBlue,i*uGreen);
			logbrush.lbHatch = 0;

			SelectObject(hdc,ExtCreatePen(PS_GEOMETRIC|PS_DASH|PS_ENDCAP_ROUND|PS_JOIN_ROUND
							,i*10+1,&logbrush,0,NULL));
			MoveToEx(hdc,10*i,60*i,NULL);
			LineTo(hdc,100*i,60*i);
		}

		GetClientRect(hWnd, &rect);
		SetTextColor(hdc, RGB(uRed, uGreen, uBlue));
		DrawText (hdc, str, -1, &rect,DT_SINGLELINE | DT_CENTER) ;	
		
		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN :
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		uRed=rand() % 255;
		uGreen=rand() % 255;
		uBlue=rand() % 255;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}

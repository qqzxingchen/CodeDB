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
	static int add=0;
	RECT rect;
	HDC hdc;
	PAINTSTRUCT ps;
	static int uRed=0, uGreen=0, uBlue=0;
	POINT point[10];
	DWORD s[10];
	
	switch(uMsg){   
	case WM_PAINT :
		add++;
		hdc=BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		SetTextColor(hdc, RGB(uRed, uGreen, uBlue));
		//DrawText (hdc, TEXT ("Hello, You Are Welcoming!"), -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;		
		DrawText (hdc, str, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;	

		point[0].x=10;
		point[0].y=10;
		point[1].x=100;
		point[1].y=10;
		point[2].x=10;
		point[2].y=100;
		point[3].x=100;
		point[3].y=100;
		point[4].x=20;
		point[4].y=20;
		point[5].x=200;
		point[5].y=20;
		point[6].x=20;
		point[6].y=200;
		point[7].x=200;
		point[7].y=200;
		int i,j;
		
		SetBkMode(hdc,MM_ISOTROPIC);
		SetWindowExtEx(hdc, 1000, -1000, NULL); 
		SetViewportExtEx(hdc,100,100,NULL);
		SetViewportOrgEx(hdc,add,add%100,NULL);

		for(i=0;i<10;i++)
			for(j=0;j<10;j++)
				SetPixel(hdc,200+i,200+j,RGB(uRed,uBlue,uGreen));

		SelectObject(hdc,CreateHatchBrush(HS_DIAGCROSS,RGB(uRed,uGreen,uBlue)));
		
		//Polygon(hdc,point,8);

		Pie(hdc,0,0,200,200,150,50,150,150);

		//MoveToEx(hdc,100,100,NULL);
		//ArcTo(hdc,0,0,200,200,0,0,0,0);

		//for(i=0;i<8;i++)
		//PolyBezier(hdc,point,4);

		//Ellipse(hdc,100,100,200,400);

		EndPaint(hWnd, &ps);
		break;
	case WM_RBUTTONDOWN:
		SendMessage(hWnd,WM_DESTROY,wParam,lParam);
		break;
	case WM_KEYDOWN :
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

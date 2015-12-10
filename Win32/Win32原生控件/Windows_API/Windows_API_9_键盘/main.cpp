#include <windows.h>

HBITMAP CreateBitmapFile (HDC hdc, PTSTR szFileName);

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
	
	//SwapMouseButton(TRUE);
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg); 
		DispatchMessage(&msg);  
	}
	ReleaseCapture();
	//SwapMouseButton(false);

	return(msg.wParam); 
}


LRESULT CALLBACK WndProc(HWND hWnd, 
						 UINT uMsg, 
						 WPARAM wParam,
						 LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HDC hdcDeskTop,hdcMem;
	static int cxScreen,cyScreen;
	static int cxClient,cyClient;
	static HBITMAP hBitmap,hbitmap;
	static HWND hwndDeskTop;

	switch(uMsg){   
	case WM_CREATE:
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		SetCapture(hWnd);

		// ��ȡ���洰�ڵĴ��ھ��
		hwndDeskTop = GetDesktopWindow();
		// �����������洰�ڵ��豸������
		hdcDeskTop = GetDC(hwndDeskTop);
		// ��������ʾ�豸��������ݵ��ڴ��豸������
		hdcMem = CreateCompatibleDC(hdcDeskTop);
		// ��ȡ���洰�ڵĴ�С
		cxScreen = GetDeviceCaps(hdcDeskTop, HORZRES);
		cyScreen = GetDeviceCaps(hdcDeskTop, VERTRES);
		hBitmap = CreateCompatibleBitmap(hdcDeskTop, cxScreen, cyScreen);
		SelectObject(hdcMem, hBitmap);
		// ������λͼ�����ش��͵����ݵ��ڴ��豸�������λͼ��
		//ShowWindow(hWnd, SW_HIDE);
		//BitBlt(hdcMem, 0, 0, cxScreen, cyScreen,hdcDeskTop, 0, 0 ,SRCCOPY);
		StretchBlt(hdcMem, 0, 0, cxScreen, cyScreen,hdcDeskTop, 0, 0 , cxScreen, cyScreen,SRCCOPY);
		//ShowWindow(hWnd, SW_SHOW);
		DeleteDC(hdcDeskTop);
		return 0 ;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint (hWnd, &ps) ;
		// ���ڴ��豸�������λͼѹ����ʾ�����򴰿���
		SetStretchBltMode(hdc, COLORONCOLOR);
		StretchBlt(hdc, 0, 0, cxClient, cyClient, 
			hdcMem, 0 ,0 , cxScreen, cyScreen, SRCCOPY); 
		EndPaint (hWnd, &ps) ;
		return 0;
	case WM_LBUTTONDOWN:
		MessageBox(NULL,"left","left",MB_OK);
		break;
		
	case WM_RBUTTONDOWN:
		MessageBox(NULL,"right","right",MB_OK);
		break;
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}


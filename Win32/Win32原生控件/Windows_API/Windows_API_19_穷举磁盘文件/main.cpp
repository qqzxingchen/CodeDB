#include <windows.h>

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
	char file[1000];
	LPTSTR filetemp;
	int n;
	WIN32_FIND_DATA fd;
	HANDLE han;

	switch(uMsg){   
	case WM_CREATE:
// 		n=SearchPath("c:\\","kernel32",".dll",1000,file,&filetemp);
// 		if(n==0)
// 			MessageBox(NULL,"wrong","wrong",MB_OK);
// 		//file[n]='\0';
// 		MessageBox(NULL,file,file,MB_OK);

		han=FindFirstFile("c:\\*",&fd);
		MessageBox(NULL,fd.cFileName,fd.cFileName,MB_OK);
		while(FindNextFile(han,&fd)!=ERROR_NO_MORE_FILES){
			MessageBox(NULL,fd.cAlternateFileName,fd.cAlternateFileName,MB_OK);

		}



		break;
	case WM_PAINT :
		hdc=BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		SetTextColor(hdc, RGB(uRed, uGreen, uBlue));
		DrawText (hdc, str, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;		
		EndPaint(hWnd, &ps);
		break;
	case WM_RBUTTONDOWN:
		PostQuitMessage(0);
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

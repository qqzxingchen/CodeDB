// #include <windows.h>
// #include "PEClass.h"
// 
// LPCTSTR lpszAppName=TEXT("MyApp");
// LPCTSTR lpszTitle=TEXT("My Application"); 
// 
// LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// 
// int APIENTRY WinMain(HINSTANCE hInstance, 
// 					 HINSTANCE hPrevInstance,
// 					 LPTSTR lpCmdLine, 
// 					 int nCmdShow)
// {   
// 	MSG msg;
// 	HWND hWnd; 
// 	WNDCLASS wc;
// 		
// 	wc.style=CS_HREDRAW|CS_VREDRAW;
// 	wc.lpfnWndProc=(WNDPROC)WndProc;       
// 	wc.cbClsExtra=0;                      
// 	wc.cbWndExtra=0;                      
// 	wc.hInstance=hInstance;              
// 	wc.hIcon=LoadIcon(NULL, IDI_APPLICATION); 
// 	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
// 	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
// 	wc.lpszMenuName=lpszAppName;              
// 	wc.lpszClassName=lpszAppName;
// 	
// 	if(!RegisterClass(&wc))
// 		return(FALSE);
// 	hWnd=CreateWindow(lpszAppName,  
// 		lpszTitle,
// 		WS_OVERLAPPEDWINDOW, 
// 		CW_USEDEFAULT,
// 		0, 
// 		CW_USEDEFAULT, 
// 		0,  
// 		NULL, 
// 		NULL,  
// 		hInstance, 
// 		NULL);               
// 	if (!hWnd) 
// 		return(FALSE);
// 	ShowWindow(hWnd, nCmdShow); 
// 	UpdateWindow(hWnd);         
// 	
// 	while(GetMessage(&msg, NULL, 0, 0)){
// 		TranslateMessage(&msg); 
// 		DispatchMessage(&msg);  
// 	}
// 	
// 	return(msg.wParam); 
// }
// 
// 
// LRESULT CALLBACK WndProc(HWND hWnd, 
// 						 UINT uMsg, 
// 						 WPARAM wParam,
// 						 LPARAM lParam)
// {
// 	RECT rect;
// 	HDC hdc;
// 	PAINTSTRUCT ps;
// 	static PEBase pebase;
// 	static char str[100];
// 	int	i=0;
// 	static int j=0;
// 
// 	switch(uMsg){   
// 	case WM_PAINT :
// 		hdc=BeginPaint(hWnd, &ps);
// 		GetClientRect(hWnd, &rect);
// 		
// 		j++;
// 		if(j%2==0)
// 		{
// 			if(!pebase.Init("C:\\Documents and Settings\\Administrator\\×ÀÃæ\\winmine.exe","exe"))
// 				MessageBox(NULL,pebase.ErrorString,NULL,NULL);
// 		}
// 		else
// 		{	
// 			if(!pebase.Init("C:\\WINDOWS\\system32\\user32.dll","dll"))
// 				MessageBox(NULL,pebase.ErrorString,NULL,NULL);		
// 		}
// 		
// 		rect.top+=100;
// 		for(i=0;i<pebase.ntHeader.FileHeader.NumberOfSections;i++)
// 		{
// 			DrawText (hdc, (pebase.getPEName(str),str),-1, &rect,DT_SINGLELINE | DT_CENTER );
// 			rect.top+=20;
// 		}
// 
// 		EndPaint(hWnd, &ps);
// 		break;
// 	case WM_RBUTTONDOWN:
// 		PostQuitMessage(0);
// 		break;
// 	case WM_KEYDOWN :
// 	case WM_LBUTTONDOWN:
// 		InvalidateRect(hWnd, NULL, FALSE);
// 		break;
// 	case WM_DESTROY :
// 		PostQuitMessage(0);
// 		break;
// 	default :
// 		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
//     }
// 	return 0;
// }

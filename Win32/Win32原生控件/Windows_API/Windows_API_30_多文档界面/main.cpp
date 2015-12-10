#include <windows.h>
#include "resource.h"

#define INIT_MENU_POS 	0
#define HELLO_MENU_POS	2
#define RECT_MENU_POS	1

#define IDM_FIRSTCHILD	50000

LRESULT CALLBACK FrameWndProc	(HWND,UINT,WPARAM,LPARAM);
BOOL	CALLBACK CloseEnumProc	(HWND,LPARAM);
LRESULT	CALLBACK HelloWndProc	(HWND,UINT,WPARAM,LPARAM);
LRESULT	CALLBACK RectWndProc	(HWND,UINT,WPARAM,LPARAM);

typedef struct tagHELLODATA{
	UINT		iColor;
	COLORREF	clrText;
}HELLODATA,*PHELLODATA;
typedef struct tagRECTDATA{
	short		cxClient;
	short		cyClient;
}RECTDATA,*PRECTDATA;

TCHAR	szAppName[]		=TEXT("MDIDemo");
TCHAR	szFrameClass[]	=TEXT("MdiFrame");
TCHAR	szHelloClass[]	=TEXT("MdiHelloChild");
TCHAR	szRectClass[]	=TEXT("MdiRectChild");

HINSTANCE hInst;
HMENU	hMenuInit,hMenuHello,hMenuRect;
HMENU	hMenuInitWindow,hMenuHelloWindow,hMenuRectWindow;

int WINAPI WinMain(
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int iCmdShow)
{
	HWND		hwndFrame;
	MSG			msg;
	WNDCLASS	wc;

	hInst=hInstance;

	//register frame class
	wc.style		=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc	=FrameWndProc;
	wc.cbClsExtra	=0;
	wc.cbWndExtra	=0;
	wc.hInstance	=hInstance;
	wc.hIcon		=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor		=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)(COLOR_APPWORKSPACE+1);
	wc.lpszMenuName	=NULL;
	wc.lpszClassName=szFrameClass;
	if(!RegisterClass(&wc))
		return 0;

	//register hello child window
	wc.style		=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc	=HelloWndProc;
	wc.cbClsExtra	=0;
	wc.cbWndExtra	=sizeof(HANDLE);
	wc.hInstance	=hInstance;
	wc.hIcon		=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor		=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName	=NULL;
	wc.lpszClassName=szHelloClass;
	RegisterClass(&wc);

	//register rect child window
	wc.style		=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc	=RectWndProc;
	wc.cbClsExtra	=0;

	wc.cbWndExtra	=sizeof(HANDLE);
	wc.hInstance	=hInstance;
	wc.hIcon		=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor		=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName	=NULL;
	wc.lpszClassName=szRectClass;
	RegisterClass(&wc);

	//menu
	hMenuInit	=LoadMenu(hInstance,TEXT("MdiMenuInit"));
	hMenuHello	=LoadMenu(hInstance,TEXT("MdiMenuHello"));	
	hMenuRect	=LoadMenu(hInstance,TEXT("MdiMenuRect"));
	
	hMenuInitWindow	=GetSubMenu(hMenuInit,INIT_MENU_POS);
	hMenuHelloWindow=GetSubMenu(hMenuHello,HELLO_MENU_POS);
	hMenuRectWindow	=GetSubMenu(hMenuRect,RECT_MENU_POS);

	hwndFrame=CreateWindow(
		szFrameClass,
		TEXT("MDI Demonstration"),
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		CW_USEDEFAULT,CW_USEDEFAULT,
		CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,
		hMenuInit,hInstance,NULL);

	ShowWindow(hwndFrame,iCmdShow);
	UpdateWindow(hwndFrame);

	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DestroyMenu(hMenuHello);
	DestroyMenu(hMenuRect);
	return msg.wParam;
}

LRESULT CALLBACK FrameWndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static HWND			hwndClient;
	CLIENTCREATESTRUCT	clientcreate;
	HWND				hwndChild;
	MDICREATESTRUCT		mdicreate;

	switch (message)
	{
	case WM_CREATE:
		clientcreate.hWindowMenu=hMenuInitWindow;
		clientcreate.idFirstChild=IDM_FIRSTCHILD;

		hwndClient=CreateWindow(
			TEXT("MDICLIENT"),
			NULL,
			WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,
			0,0,0,0,
			hwnd,
			(HMENU)1,
			hInst,
			(PSTR)&clientcreate);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_NEWHELLO:
			mdicreate.szClass	=szHelloClass;
			mdicreate.szTitle	=TEXT("Hello");
			mdicreate.hOwner	=hInst;
			mdicreate.x			=CW_USEDEFAULT;
			mdicreate.y			=CW_USEDEFAULT;
			mdicreate.cx		=CW_USEDEFAULT;
			mdicreate.cx		=CW_USEDEFAULT;
			mdicreate.style		=0;
			mdicreate.lParam	=0;
			hwndChild=(HWND)SendMessage(hwndClient,
				WM_MDICREATE,0,(LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			return 0;
		case IDM_FILE_NEWRECT:
			mdicreate.szClass	=szRectClass;
			mdicreate.szTitle	=TEXT("Rectangles");
			mdicreate.hOwner	=hInst;
			mdicreate.x			=CW_USEDEFAULT;
			mdicreate.y			=CW_USEDEFAULT;
			mdicreate.cx		=CW_USEDEFAULT;
			mdicreate.cx		=CW_USEDEFAULT;
			mdicreate.style		=0;
			mdicreate.lParam	=0;
			hwndChild=(HWND)SendMessage(hwndClient,
				WM_MDICREATE,0,(LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			return 0;
		case IDM_FILE_CLOSE:
			hwndChild=(HWND)SendMessage(hwndClient,WM_MDIGETACTIVE,0,0);
			if(SendMessage(hwndChild,WM_QUERYENDSESSION,0,0))
				SendMessage(hwndClient,WM_MDIDESTROY,(WPARAM)hwndChild,0);
			return 0;
		case IDM_APP_EXIT:
			SendMessage(hwnd,WM_CLOSE,0,0);
			return 0;
		case IDM_WINDOW_TILE:
			SendMessage(hwndClient,WM_MDITILE,0,0);
			return 0;
		case IDM_WINDOW_ARRANGE:
			SendMessage(hwndClient,WM_MDIICONARRANGE,0,0);	//������С���Ĵ��ڣ����ǲ����û�г�ʼ���Ĵ�����Ӱ��
			return 0;
		case IDM_WINDOW_CASCADE:
			SendMessage(hwndClient,WM_MDICASCADE,0,0);		
			return 0;
		case IDM_WINDOW_CLOSEALL:
			EnumChildWindows(hwndClient,CloseEnumProc,0);	//ö��hwndclient�������Ӵ��ڲ����ε���CloseEnumProc
			return 0;
		default:
			hwndChild=(HWND)SendMessage(hwndClient,WM_MDIGETACTIVE,0,0);
			if(IsWindow(hwndChild))
				SendMessage(hwndChild,WM_COMMAND,wParam,lParam);
			break;
		}
		break;
	case WM_QUERYENDSESSION:
	case WM_CLOSE:
		SendMessage(hwnd,WM_COMMAND,IDM_WINDOW_CLOSEALL,0);
		if(NULL!=GetWindow(hwndClient,GW_CHILD))
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefFrameProc(hwnd,hwndClient,message,wParam,lParam);
}


//ÿ���Ӵ��ڽ�Ҫ������ʱ����������������
//To continue enumeration, the callback function must return TRUE; to stop enumeration, it must return FALSE. 
BOOL	CALLBACK CloseEnumProc	(HWND hwnd,LPARAM lParam)
{
	if(GetWindow(hwnd,GW_OWNER))
		return TRUE;
	SendMessage(GetParent(hwnd),WM_MDIRESTORE,(WPARAM)hwnd,0);
 	if(!SendMessage(hwnd,WM_QUERYENDSESSION,0,0))
 		return TRUE;
	SendMessage(GetParent(hwnd),WM_MDIDESTROY,(WPARAM)hwnd,0);
	return TRUE;
}



LRESULT	CALLBACK HelloWndProc	(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static COLORREF clrTextArray[]=
			{RGB(0,0,0),
			RGB(255,0,0),
			RGB(0,255,0),
			RGB(0,0,255),
			RGB(255,255,255)};
	static HWND hwndClient,hwndFrame;
	HDC		hdc;
	HMENU	hMenu;
	PHELLODATA	pHelloData;
	PAINTSTRUCT	ps;
	RECT	rect;

	switch (message)
	{
	case WM_CREATE:	//ÿ�δ��������͵��Ӵ��ڣ��ú�������������һ��
		pHelloData=(PHELLODATA)HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY,sizeof(HELLODATA));
		pHelloData->iColor=IDM_COLOR_BLACK;
		pHelloData->clrText=RGB(0,0,0);

		//Ҫʹ��SetWindowLong��������������ע���ʱ��ָ��Ҫ��Ķ���ռ��С����wc.cbWndExtra�����ֽ�Ϊ��λ��
		SetWindowLong(hwnd,GWL_USERDATA,(long)pHelloData);
		hwndClient=GetParent(hwnd);
		hwndFrame=GetParent(hwndClient);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_COLOR_BLACK:
		case IDM_COLOR_RED:
		case IDM_COLOR_GREEN:
		case IDM_COLOR_BLUE:
		case IDM_COLOR_WHITE:
			pHelloData=(PHELLODATA)GetWindowLong(hwnd,GWL_USERDATA);
			hMenu=GetMenu(hwndFrame);
			
			CheckMenuItem(hMenu,pHelloData->iColor,MF_UNCHECKED);	//��Frame�����Ͻ���ɫѡ��ǰ��ĶԹ�ȥ��
			
			pHelloData->iColor=wParam;								//��ȡ��ǰ�������ɫ
			CheckMenuItem(hMenu,pHelloData->iColor,MF_CHECKED);		//��Frame�����Ͻ��������ɫѡ��ǰ����϶Թ�
			
			pHelloData->clrText=clrTextArray[wParam-IDM_COLOR_BLACK];	//���µ�ǰ��ɫΪ��ѡ�е���ɫ
			InvalidateRect(hwnd,NULL,FALSE);
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hwnd,&ps);
		pHelloData=(PHELLODATA)GetWindowLong(hwnd,GWL_USERDATA);
		SetTextColor(hdc,pHelloData->clrText);
		GetClientRect(hwnd,&rect);
		DrawText(hdc,TEXT("hello world!"),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		EndPaint(hwnd,&ps);
		return 0;
	
		//WM_MDIACTIVATE�Ƿ��͸�MDI�ͻ����ڵ���Ϣ,����ǿ�Ƽ�������һ��MDI�Ӵ���
		//��Ϣ�Ĳ���wParamΪ��Ҫ�����MDI�Ӵ��ڵĴ��ھ����
		//����lParam�ڷ�����Ϣʱ��Ϊ��.
		//MDI�Ӵ����յ���Ϣ�󣬸ò���������ȡ�������MDI�Ӵ��ھ��������ֵ����Ϊ�㡣
	case WM_MDIACTIVATE:
		SendMessage(hwndClient,WM_MDISETMENU,
			(lParam==(LPARAM)hwnd)?(WPARAM)hMenuHello:(WPARAM)hMenuInit,
			NULL);
			//(LPARAM)hMenuInitWindow);

		//������ͬ���͵���ͬ���Ӵ��ڼ��л�ʱ����Ҫ��Frame���ڵ���ɫѡ�����Ϊ��ǰ�Ӵ��ڵ���ɫ
		pHelloData=(PHELLODATA)GetWindowLong(hwnd,GWL_USERDATA);
		CheckMenuItem(hMenuHello,pHelloData->iColor,
			(lParam==(LPARAM)hwnd)?MF_CHECKED:MF_UNCHECKED);

		DrawMenuBar(hwndFrame);		//�ػ�Frame���ڵĲ˵�
									//������˵���Ϊĳһ���ʹ��ڷ����
									//����˵��ǵ���ǰ�����ڲ�ͬ���͵��Ӵ������л�ʱ�����ı䣬
									//����ͬ�����Ӵ������л�ʱ�������ı�
		return 0;

//	case WM_QUERYENDSESSION:		//
//	case WM_CLOSE:					//�˳�ǰ�����ѶԻ���
//		if(IDOK!=MessageBox(hwnd,TEXT("OK to close window?"),TEXT("hello"),MB_ICONQUESTION|MB_OKCANCEL))
//			return 0;
//		else
//			break;

	case WM_DESTROY:
		pHelloData=(PHELLODATA)GetWindowLong(hwnd,0);
		HeapFree(GetProcessHeap(),0,pHelloData);
		return 0;
	}
	return DefMDIChildProc(hwnd,message,wParam,lParam);
}



LRESULT	CALLBACK RectWndProc	(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static HWND hwndClient,hwndFrame;
	HDC			hdc;
	PRECTDATA	pRectData;
	PAINTSTRUCT	ps;
	
	switch (message)
	{
	case WM_CREATE:
		pRectData=(PRECTDATA)HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY,sizeof(RECTDATA));
		SetWindowLong(hwnd,GWL_USERDATA,(long)pRectData);
		hwndClient=GetParent(hwnd);
		hwndFrame=GetParent(hwndClient);
		return 0;
	case WM_SIZE:
		if(wParam!=SIZE_MINIMIZED)
		{
			pRectData=(PRECTDATA)GetWindowLong(hwnd,GWL_USERDATA);
			pRectData->cxClient=LOWORD(lParam);
			pRectData->cyClient=HIWORD(lParam);
		}
		break;
	case WM_PAINT:
		hdc=BeginPaint(hwnd,&ps);
		EndPaint(hwnd,&ps);
		return 0;
	case WM_MDIACTIVATE:
		SendMessage(hwndClient,WM_MDISETMENU,
			(lParam==(LPARAM)hwnd)?(WPARAM)hMenuRect:(WPARAM)hMenuInit,(LPARAM)hMenuRectWindow);	
		DrawMenuBar(hwndFrame);
		return 0;
	case WM_DESTROY:
		pRectData=(PRECTDATA)GetWindowLong(hwnd,GWL_USERDATA);
		HeapFree(GetProcessHeap(),0,pRectData);
		return 0;
	}
	return DefMDIChildProc(hwnd,message,wParam,lParam);
}
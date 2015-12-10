#include <windows.h>
#include <winnt.h>
#include <winuser.h>
#include "PEInfoFunc.h"
#include "resource.h"

#define	XC_SB_MOUSEWHEEL WM_USER+9		//lParam为一个int表示向上或者向下的行数

#define XC_OPENWINDOW   WM_USER+10		//wParam存储将要创建子窗口的标题
#define XC_FILENAME		WM_USER+11		//当文件名符合要求时，对文件内PE要求的MZ字段和PE字段进行审核

#define XC_CREATEBITMAP WM_USER+12		//wParam表示窗口的ID号，即sign数组中的值

#define IDM_FIRSTCHILD	50000
#define XC_SCROLL_MAX	300
#define XC_SCROLL_PAGE	100
#define WM_MOUSEWHEEL	0x020A			//因为系统版本过低，所以需要在这里手动定义

int		sign[]={XC_NT_INFO,XC_SECTION_INFO};
TCHAR	title[][40]={TEXT("NT Header Info"),TEXT("Section And Data Directory Info")};

TCHAR	szAppName[]		=TEXT("PEInfo");
TCHAR	szFrameClass[]	=TEXT("MdiFrame");
TCHAR	szInfoClass[]	=TEXT("Info");


HINSTANCE	hInst;
HMENU		hMenuInit;
HMENU		hMenu;

PEINFO		peInfo;

LRESULT CALLBACK FrameWndProc	(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
BOOL	CALLBACK CloseEnumProc	(HWND hWnd,LPARAM lParam);
LRESULT	CALLBACK InfoWndProc	(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int iCmdShow)
{
	HWND		hwndFrame;
	MSG			msg;
	WNDCLASS	wc;

	ZeroMemory(&peInfo,sizeof(PEINFO));

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

	//register PE info child window
	wc.style		=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc	=InfoWndProc;
	wc.cbClsExtra	=0;
	wc.cbWndExtra	=0;
	wc.hInstance	=hInstance;
	wc.hIcon		=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor		=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName	=NULL;
	wc.lpszClassName=szInfoClass;
	RegisterClass(&wc);

	hMenu			=LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MENU1));
	hMenuInit		=GetSubMenu(hMenu,0);

	hwndFrame=CreateWindow(
		szFrameClass,
		TEXT("PEInfo"),
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		CW_USEDEFAULT,CW_USEDEFAULT,
		CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,
		hMenu,hInstance,NULL);
	
	ShowWindow(hwndFrame,iCmdShow);
	UpdateWindow(hwndFrame);
	
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	DestroyMenu(hMenu);
	DestroyMenu(hMenuInit);
	return msg.wParam;
}



LRESULT CALLBACK FrameWndProc	(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static HWND			hWndClient;
	CLIENTCREATESTRUCT	clientcreate;
	HWND				hwndChild;
	MDICREATESTRUCT		mdicreate;
	OPENFILENAME		ofn;
	char				temp[256];
	char				tempename[10];
	static HANDLE		hFile;
	DWORD				byteNum;
	int					i;

	switch (message)
	{
	case WM_CREATE:
		clientcreate.hWindowMenu=hMenuInit;
		clientcreate.idFirstChild=IDM_FIRSTCHILD;
		
		hWndClient=CreateWindow(
			TEXT("MDICLIENT"),
			NULL,
			WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,
			0,0,300,300,
			hWnd,
			hMenu,
			hInst,
			(PSTR)&clientcreate);
		return 0;
	case XC_OPENWINDOW:		//wParam存储将要创建窗口的标题
		mdicreate.szClass	=szInfoClass;
		mdicreate.szTitle	=(char*)wParam;
		mdicreate.hOwner	=hInst;
		mdicreate.x			=CW_USEDEFAULT;
		mdicreate.y			=CW_USEDEFAULT;
		mdicreate.cx		=CW_USEDEFAULT;
		mdicreate.cx		=CW_USEDEFAULT;
		mdicreate.style		=0;
		mdicreate.lParam	=0;
		
		hwndChild=(HWND)SendMessage(hWndClient,
			WM_MDICREATE,0,(LPARAM)(LPMDICREATESTRUCT)&mdicreate);

		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_OPENFILE:
			ZeroMemory(&ofn,sizeof(OPENFILENAME));
			ZeroMemory(temp,sizeof(temp));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = temp;
			ofn.nMaxFile = sizeof(temp);
			ofn.lpstrFilter = "*.exe;*.dll\0\0";
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.lpstrTitle = "Please Choose PE file";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			
			if(GetOpenFileName(&ofn)==TRUE)
			{
				wsprintf(tempename,"%s",&(ofn.lpstrFile)[ofn.nFileExtension]);

				if( (tempename[0]=='E' || tempename[0]=='e')&&
					(tempename[1]=='X' || tempename[1]=='x')&&
					(tempename[2]=='E' || tempename[2]=='e') )
				{	
					SendMessage(hWnd,XC_FILENAME,(WPARAM)ofn.lpstrFile,(LPARAM)NULL);
					return 0;
				}
				
				if( (tempename[0]=='D' || tempename[0]=='d')&&
					(tempename[1]=='L' || tempename[1]=='l')&&
					(tempename[2]=='L' || tempename[2]=='l') )
				{
					SendMessage(hWnd,XC_FILENAME,(WPARAM)ofn.lpstrFile,(LPARAM)NULL);
					return 0;
				}

				if( (tempename[0]=='S' || tempename[0]=='s')&&
					(tempename[1]=='Y' || tempename[1]=='y')&&
					(tempename[2]=='S' || tempename[2]=='s') )
				{
					SendMessage(hWnd,XC_FILENAME,(WPARAM)ofn.lpstrFile,(LPARAM)NULL);
					return 0;
				}

				MessageBox(NULL,"错误的文件格式","File Type Error",MB_OK);
			}
			else
			{
				switch (CommDlgExtendedError())
				{
				case FNERR_BUFFERTOOSMALL:	MessageBox(NULL,"文件路径缓存过小","FNERR_BUFFERTOOSMALL",MB_OK);break;
				case FNERR_INVALIDFILENAME:	MessageBox(NULL,"文件名无效","FNERR_INVALIDFILENAME",MB_OK);break;
				case FNERR_SUBCLASSFAILURE:	MessageBox(NULL,"内存无法读取","FNERR_SUBCLASSFAILURE",MB_OK);break;
				}
			}
			return 0;

		case IDM_EXIT:
			SendMessage(hWnd,WM_CLOSE,0,0);
			return 0;
		case IDM_WINDOWTILE:
			SendMessage(hWndClient,WM_MDITILE,0,0);
			return 0;
		case IDM_WINDOWARRANGE:
			SendMessage(hWndClient,WM_MDIICONARRANGE,0,0);	//排列最小化的窗口，但是不会对没有初始化的窗口有影响
			return 0;
		case IDM_WINDOWCASCADE:
			SendMessage(hWndClient,WM_MDICASCADE,0,0);		
			return 0;
		case IDM_WINDOWCLOSEALL:
			EnumChildWindows(hWndClient,CloseEnumProc,0);	//枚举hwndclient函数的子窗口并依次调用CloseEnumProc
			return 0;
		default:
			hwndChild=(HWND)SendMessage(hWndClient,WM_MDIGETACTIVE,0,0);
			if(IsWindow(hwndChild))
				SendMessage(hwndChild,WM_COMMAND,wParam,lParam);
			break;
		}
		break;
	case XC_FILENAME:

		hFile=CreateFile((char*)wParam,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hFile!=INVALID_HANDLE_VALUE)
		{
			SetFilePointer(hFile,0,NULL,FILE_BEGIN);
			ReadFile(hFile,&peInfo.dosHeader,sizeof(IMAGE_DOS_HEADER),&byteNum,NULL);
			if(strncmp((char*)&peInfo.dosHeader.e_magic,"MZ",2)!=0)
			{	
				CloseHandle(hFile);
				MessageBox(NULL,"该文件不是一个有效的win32程序","MZ字段无法匹配",MB_OK);
				return 0;
			}

			SetFilePointer(hFile,peInfo.dosHeader.e_lfanew,NULL,FILE_BEGIN);
			ReadFile(hFile,&peInfo.ntHeader,sizeof(IMAGE_NT_HEADERS),&byteNum,NULL);
			if(strncmp((char*)&peInfo.ntHeader.Signature,"PE\0\0",4)!=0)
			{	
				CloseHandle(hFile);
				MessageBox(NULL,"该文件不是一个有效的win32程序","PE字段无法匹配",MB_OK);
				return 0;
			}
			
			if(peInfo.pSections!=NULL)
				HeapFree(GetProcessHeap(),0,peInfo.pSections);

			for (i=0;i<CHILDWINDOWNUM;i++)
			{
				peInfo.hWnd[i]=0;
				SendMessage(hWnd,WM_COMMAND,IDM_WINDOWCLOSEALL,NULL);
			}

			peInfo.pSections=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,
				peInfo.ntHeader.FileHeader.NumberOfSections*sizeof(IMAGE_SECTION_HEADER)+2);
			SetFilePointer(hFile,peInfo.dosHeader.e_lfanew+0x78+16*8,NULL,FILE_BEGIN);
			ReadFile(hFile,peInfo.pSections,
				peInfo.ntHeader.FileHeader.NumberOfSections*sizeof(IMAGE_SECTION_HEADER),&byteNum,NULL);

			for(i=CHILDWINDOWNUM-1;i>=0;i--)
				SendMessage(hWnd,XC_OPENWINDOW,(WPARAM)title[i],NULL);
			SendMessage(hWndClient,WM_MDITILE,0,0);
		}
		else
		{
			MessageBox(NULL,"打开文件失败",NULL,MB_OK);
		}
		return 0;
	case WM_QUERYENDSESSION:
	case WM_CLOSE:
		SendMessage(hWnd,WM_COMMAND,IDM_WINDOWCLOSEALL,0);
		if(NULL!=GetWindow(hWndClient,GW_CHILD))
			return 0;
		break;
	case WM_DESTROY:
		if(peInfo.pSections!=NULL)
			HeapFree(GetProcessHeap(),0,peInfo.pSections);
		PostQuitMessage(0);
		return 0;
	}
	return DefFrameProc(hWnd,hWndClient,message,wParam,lParam);
}


LRESULT	CALLBACK InfoWndProc	(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static HWND hWndClient,hwndFrame;
	static HDC	hdcMem[CHILDWINDOWNUM];
	static HBITMAP	hBitmap[CHILDWINDOWNUM];
	PAINTSTRUCT	ps;
	RECT		rect;
	HDC			hdc;
	int			i;
	int			nowpos;

	switch (message)
	{
	case WM_MOUSEWHEEL:
		if((int)wParam>0)
			PostMessage(hWnd,WM_VSCROLL,(WPARAM)((WORD)XC_SB_MOUSEWHEEL),(LPARAM)-7);
		else
			PostMessage(hWnd,WM_VSCROLL,(WPARAM)((WORD)XC_SB_MOUSEWHEEL),(LPARAM)7);
		break;
	case WM_CREATE:
		hWndClient=GetParent(hWnd);
		hwndFrame=GetParent(hWndClient);

		hdc=GetDC(hWnd);
		for(i=0;i<CHILDWINDOWNUM;i++)
		{
			if(strcmp(((LPCREATESTRUCT)lParam)->lpszName,title[i])==0)
			{
				peInfo.hWnd[i]=hWnd;
				PostMessage(hWnd,XC_CREATEBITMAP,(WPARAM)i,NULL);
				break;
			}
		}
		return 0;
	case XC_CREATEBITMAP:
		hdc=GetDC(hWnd);
		hdcMem[(int)wParam] = CreateCompatibleDC(hdc);     //创建一个与指定设备兼容的内存设备上下文环境（DC）。
		hBitmap[(int)wParam] = CreateCompatibleBitmap(hdc,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)*3); 
		SelectObject(hdcMem[(int)wParam], hBitmap[(int)wParam]);
		SetTextColor(hdcMem[(int)wParam],RGB(200,0,0));
		SetBkMode(hdcMem[(int)wParam], TRANSPARENT); 
		InvalidateRect(hWnd,NULL,TRUE);
		ReleaseDC(hWnd, hdc);
		
		rect.left=0;
		rect.top=0;
		rect.right=GetSystemMetrics(SM_CXSCREEN);
		rect.bottom=GetSystemMetrics(SM_CYSCREEN)*3;
		DisplayInfo(hdcMem[(int)wParam],&rect,(int)wParam,peInfo);
		return 0;
	case WM_PAINT:
		for(i=0;i<CHILDWINDOWNUM;i++)
			if(peInfo.hWnd[i]==hWnd)
				break;
		
		hdc=BeginPaint(hWnd,&ps);

		BitBlt(hdc, 0, 0, peInfo.windowSize[i].x,peInfo.windowSize[i].y, hdcMem[i], 
			0, (int)(((float)peInfo.scrollInfo[i].nPos/peInfo.scrollInfo[i].nMax)*peInfo.windowSize[i].y*3), 
			SRCCOPY); 
			
		EndPaint(hWnd,&ps);
		return 0;
	case WM_CLOSE: 
		ShowWindow(hWnd,SW_MINIMIZE);
		return 0;
	case WM_SIZE:
		for (i=0;i<CHILDWINDOWNUM;i++)
		{
			if(peInfo.hWnd[i]==hWnd)
				break;
		}
		peInfo.windowSize[i].x=LOWORD(lParam);
		peInfo.windowSize[i].y=HIWORD(lParam);
		
		peInfo.scrollInfo[i].cbSize=sizeof(SCROLLINFO);
		peInfo.scrollInfo[i].fMask=SIF_PAGE|SIF_POS|SIF_RANGE;
		peInfo.scrollInfo[i].nMin=0;
		peInfo.scrollInfo[i].nMax=XC_SCROLL_MAX-1;
		peInfo.scrollInfo[i].nPos=0;
		peInfo.scrollInfo[i].nPage=XC_SCROLL_PAGE;
		SetScrollInfo(hWnd,SB_VERT,&peInfo.scrollInfo[i],TRUE);
		SendMessage(hWnd,WM_VSCROLL,NULL,NULL);
		InvalidateRect(hWnd,NULL,false);
		break;
	case WM_VSCROLL:
		for (i=0;i<CHILDWINDOWNUM;i++)
		{
			if(peInfo.hWnd[i]==hWnd)
				break;
		}

		peInfo.scrollInfo[i].cbSize=sizeof(SCROLLINFO);
		peInfo.scrollInfo[i].fMask=SIF_ALL;
		GetScrollInfo(hWnd,SB_VERT,&peInfo.scrollInfo[i]);
		nowpos=peInfo.scrollInfo[i].nPos;
		switch(LOWORD(wParam))
		{
		case SB_TOP:                    // 滚动条处于最上端
			peInfo.scrollInfo[i].nPos=peInfo.scrollInfo[i].nMin;
			break;
		case SB_BOTTOM:                 // 滚动条处于最下端
			peInfo.scrollInfo[i].nPos = peInfo.scrollInfo[i].nMax;
			break;
		case SB_LINEUP:                 // 向上滚动一行
			peInfo.scrollInfo[i].nPos -= 1;		
			break;	
		case SB_LINEDOWN:                // 向下滚动一行		
			peInfo.scrollInfo[i].nPos +=1;
			break;	
		case SB_PAGEUP:                 // 向上滚动一页		
			peInfo.scrollInfo[i].nPos -=peInfo.scrollInfo[i].nPage;		
			break;	
		case SB_PAGEDOWN:               // 向下滚动一页	
			peInfo.scrollInfo[i].nPos +=peInfo.scrollInfo[i].nPage;		
			break;	
		case SB_THUMBTRACK:              // 拖曳滚动框	
			peInfo.scrollInfo[i].nPos = peInfo.scrollInfo[i].nTrackPos;		
			break;	
		case XC_SB_MOUSEWHEEL:			// 该消息是手动添加的，为了响应鼠标滚轮的动作
			peInfo.scrollInfo[i].nPos += (int)lParam;
			break;
		default:
			break;		
		}

		if(peInfo.scrollInfo[i].nPos>peInfo.scrollInfo[i].nMax)   // 确保滚动条位于滚动范围内
			peInfo.scrollInfo[i].nPos=peInfo.scrollInfo[i].nMax;
		if(peInfo.scrollInfo[i].nPos<peInfo.scrollInfo[i].nMin)
			peInfo.scrollInfo[i].nPos=peInfo.scrollInfo[i].nMin;
		peInfo.scrollInfo[i].fMask=SIF_POS;
		SetScrollInfo(hWnd, SB_VERT, &peInfo.scrollInfo[i], TRUE);
		GetScrollInfo(hWnd, SB_VERT, &peInfo.scrollInfo[i]);
		ScrollWindowEx(hWnd, 0, peInfo.windowSize[i].y*(nowpos-peInfo.scrollInfo[i].nPos)/peInfo.scrollInfo[i].nPage,
			NULL, NULL, NULL, NULL, SW_INVALIDATE|SW_ERASE );
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_ERASEBKGND:		//当接收到这个消息后并返回true，即告诉系统你已经重绘界面，不需要它再次重绘
							//这个消息主要作用是消除闪烁
		return TRUE;
	}
	return DefMDIChildProc(hWnd,message,wParam,lParam);
}

BOOL CALLBACK CloseEnumProc	(HWND hWnd,LPARAM lParam)
{
	if(GetWindow(hWnd,GW_OWNER))
		return TRUE;
	SendMessage(GetParent(hWnd),WM_MDIRESTORE,(WPARAM)hWnd,0);
	if(!SendMessage(hWnd,WM_QUERYENDSESSION,0,0))
		return TRUE;
	SendMessage(GetParent(hWnd),WM_MDIDESTROY,(WPARAM)hWnd,0);
	return TRUE;
}



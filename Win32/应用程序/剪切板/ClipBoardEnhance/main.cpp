#include <windows.h>

#define IDC_MYBUTTON 10001

static char dstDirectory[1000];
BOOL open_or_close=FALSE;
int screenX,screenY;
HANDLE buttonHandle;

LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void XC_moveFile(char *sourceFile,char *dstFile);

int APIENTRY WinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance,
					 LPTSTR lpCmdLine, 
					 int nCmdShow)
{   
	MSG msg;
	HWND hWnd; 
	WNDCLASS wc;

	screenX=GetSystemMetrics(SM_CXSCREEN);
	screenY=GetSystemMetrics(SM_CYSCREEN);

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
		NULL,
		WS_OVERLAPPEDWINDOW,
		screenX-130,0,130,130,
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
	static HWND hWndNextViewer;
	HDROP hdrop;
	RECT rect;
	POINT point[2];
	HRGN hRgn;
	char temp[1000];
	int i;

	switch(uMsg){  
	case WM_CREATE:
		RECT rect2;
		
		GetWindowRect(hWnd,&rect);
		GetClientRect(hWnd,&rect2);
		
		point[0].x=0;
		point[0].y=0;
		ClientToScreen(hWnd,&point[0]);
		
		point[1].x=rect2.right;
		point[1].y=rect2.bottom;
		ClientToScreen(hWnd,&point[1]);
		
		hRgn=CreateRectRgn(
			point[0].x-rect.left,
			point[0].y-rect.top,
			point[0].x-rect.left+70,
			point[0].y-rect.top+40);
		SetWindowRgn(hWnd,hRgn,TRUE);

		buttonHandle=CreateWindow(
			"BUTTON",NULL,
			WS_CHILD|WS_VISIBLE,
			0,0,70,40,
			hWnd, (HMENU)IDC_MYBUTTON,GetModuleHandle(NULL),NULL);

		open_or_close=false;

		i=0;
		while(1){
			wsprintf(temp,"%c:",'C'+i);
			if(GetDriveType(temp)==DRIVE_REMOTE){
				break;
			}
			i++;
		}//找到远程磁盘，即VirtualBox的共享磁盘。
		
		//在远程磁盘上创建ClipBorderShare文件夹，用以存放临时文件。
		wsprintf(&temp[strlen(temp)],"\\ClipBorderShare");
		wsprintf(dstDirectory,"%s",temp);
		CreateDirectory(dstDirectory,NULL);

		break;
	case WM_COMMAND:
		if(LOWORD(wParam)==IDC_MYBUTTON){
			if(open_or_close==false){
				hWndNextViewer=SetClipboardViewer(hWnd);
				open_or_close=TRUE;
			}else{
				ChangeClipboardChain (hWnd, hWndNextViewer);
				open_or_close=FALSE;
			}
		}
		break;
	case WM_CHANGECBCHAIN:
		if((HWND)wParam==hWndNextViewer){
			hWndNextViewer=(HWND)lParam;
		}else if(hWndNextViewer){
			SendMessage(hWndNextViewer,uMsg,wParam,lParam);
		}
		break;
	case WM_DRAWCLIPBOARD://更新对剪贴板数据的显示
		char s[1000];
		if (hWndNextViewer)
			SendMessage (hWndNextViewer, uMsg, wParam, lParam);
		if(open_or_close==TRUE){
			OpenClipboard (hWnd);
			hdrop=(HDROP)GetClipboardData(CF_HDROP);
			if(hdrop!=NULL){
				DragQueryFile(hdrop,0,temp,sizeof(temp));
				XC_moveFile(temp,dstDirectory);
				wsprintf(s,"mov file from  \"%s\"  to  \"%s\"",temp,dstDirectory);
				MessageBox(NULL,s,NULL,MB_OK);
			}
			CloseClipboard ();
		}
		return 0;
	case WM_DESTROY:
		if(open_or_close==TRUE)
			ChangeClipboardChain (hWnd, hWndNextViewer);
		PostQuitMessage(0);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}

/*	sourceFile路径名格式为：A:\B
	dstFile路径名格式为：C:\D
*/
void XC_moveFile(char *sourceFile,char *dstFile){
	WIN32_FIND_DATA fd;
	HANDLE handle;
	char temp[1000];
	char dst[1000];
	char rootfilename[1000];

	FindFirstFile(sourceFile,&fd);
	if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY){
		wsprintf(rootfilename,"%s",fd.cFileName);

		wsprintf(temp,"%s\\%s",dstFile,rootfilename);
		CreateDirectory(temp,NULL);

		wsprintf(temp,"%s\\*",sourceFile);
		handle=FindFirstFile(temp,&fd);
		if(handle==INVALID_HANDLE_VALUE){
			MessageBox(NULL,"Error invalid handle",NULL,MB_OK);
		}else{
			while(1){
				if(strcmp(fd.cFileName,".")==0 || strcmp(fd.cFileName,"..")==0){
					if(FindNextFile(handle,&fd)==0)
						return ;
					else 
						continue;
				}
				if((fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY){
					wsprintf(temp,"%s\\%s\\%s",dstFile,rootfilename,fd.cFileName);
					CreateDirectory(temp,NULL);
					
					wsprintf(temp,"%s\\%s",sourceFile,fd.cFileName);
					wsprintf(dst,"%s\\%s",dstFile,rootfilename);
					XC_moveFile(temp,dst);
				}else{
					wsprintf(temp,"%s\\%s",sourceFile,fd.cFileName);
					wsprintf(dst,"%s\\%s\\%s",dstFile,rootfilename,fd.cFileName);
					CopyFile(temp,dst,FALSE);
				}		
				if(FindNextFile(handle,&fd)==0)
					return ;
			}
		}
	}else{
		wsprintf(temp,"%s\\%s",dstFile,fd.cFileName);
		CopyFile(sourceFile,temp,FALSE);
	}
}
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <commctrl.h>
#include <commdlg.h>
#include "resource.h"
#include <mmsystem.h>

#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"winmm.lib")

LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application");

HWND hTreeView;
HWND hStatic;
HWND hGroup;
HANDLE handle_driver;
TVINSERTSTRUCT treeinsert;
TVITEM treeitem;
HIMAGELIST himagelist;
HIMAGELIST hdragimagelist;
HICON hicon[4];
RECT rect;

HANDLE handle_play,handle_reset,handle_stop,handle_speed,handle_rew;
char nowfile[1000];

BOOL dragmode=FALSE;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void find_file(HANDLE handle,char * addr,int n);
void list_file_info(char * fileaddr);

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
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName=lpszAppName;              
	wc.lpszClassName=lpszAppName;

	if(!RegisterClass(&wc))
		return(FALSE);
	hWnd=CreateWindow(lpszAppName,lpszTitle,
		WS_OVERLAPPEDWINDOW, 
		100,100,1100,600,
		NULL,NULL,hInstance,NULL);               
	if (!hWnd)
		return(FALSE);
	ShowWindow(hWnd, SW_SHOW);
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

	int i,j;
	char temp[100];
	char str_file_to_find[1000]="\0";
	char file_temp[100][100];
	NMHDR nmhdr;
	NMTREEVIEW nmtreeview;
	TV_HITTESTINFO tvhit;
	char commandstring[1000];

	switch(uMsg){   
	case WM_CREATE:
		InitCommonControls();
		GetClientRect(hWnd,&rect);
		hTreeView=CreateWindow("SysTreeView32","xingchen_treeview",
			//TVS_HASBUTTONS|	
			TVS_HASLINES|TVS_LINESATROOT|WS_CHILD|WS_VISIBLE,
			rect.left,rect.top,600,rect.bottom-rect.top-60,
			hWnd,(HMENU)IDC_XC_TREEVIEW,NULL,NULL);
		hStatic=CreateWindow("STATIC","xingchen_static",WS_VISIBLE|WS_CHILD,
				rect.left+600,rect.top,rect.right-rect.left,rect.bottom-rect.top,
				hWnd,(HMENU)IDC_XC_STATIC,NULL,NULL);
		handle_play	=CreateWindow("BUTTON", "播放", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
			100+rect.left,rect.bottom-45, 60, 30, 
			GetParent(hTreeView),(HMENU)MAKEINTRESOURCE(IDC_BUTTON1), GetModuleHandle(NULL), NULL); 
		handle_stop	=CreateWindow("BUTTON", "暂停", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
			185+rect.left,rect.bottom-45, 60, 30, 
			GetParent(hTreeView),(HMENU)MAKEINTRESOURCE(IDC_BUTTON2), GetModuleHandle(NULL), NULL); 
		handle_reset=CreateWindow("BUTTON", "结束", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
			270+rect.left,rect.bottom-45, 60, 30, 
			GetParent(hTreeView),(HMENU)MAKEINTRESOURCE(IDC_BUTTON3), GetModuleHandle(NULL), NULL); 
		handle_rew	=CreateWindow("BUTTON", "快退", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
			355+rect.left,rect.bottom-45, 60, 30, 
			GetParent(hTreeView),(HMENU)MAKEINTRESOURCE(IDC_BUTTON4), GetModuleHandle(NULL), NULL); 
		handle_speed=CreateWindow("BUTTON", "快进", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
			440+rect.left,rect.bottom-45, 60, 30, 
		GetParent(hTreeView),(HMENU)MAKEINTRESOURCE(IDC_BUTTON5), GetModuleHandle(NULL), NULL); 


		
		//构造图形列表
		himagelist=ImageList_Create(32,32,ILC_COLOR32,4,10);

		hicon[0]=LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_XCICON_remote));
		hicon[1]=LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_XCICON_folder));
		hicon[2]=LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_XCICON_disk));
		hicon[3]=LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_XCICON_file));
		

		//为图形列表加载图标
		ImageList_AddIcon(himagelist,hicon[0]);
		ImageList_AddIcon(himagelist,hicon[1]);
		ImageList_AddIcon(himagelist,hicon[2]);
		ImageList_AddIcon(himagelist,hicon[3]);


		//将图形列表与树形列表绑定
		SendMessage(hTreeView,TVM_SETIMAGELIST,TVSIL_NORMAL,(LPARAM)himagelist);

		for(i=0;i<26;i++){
			wsprintf(temp,"%c:\\",i+'A');
			j=GetDriveType(temp);
			if(j==DRIVE_FIXED     || j==DRIVE_REMOTE || 
			   j==DRIVE_REMOVABLE || j==DRIVE_RAMDISK|| j==DRIVE_CDROM) 
			{
				treeinsert.hParent=NULL;
				treeinsert.hInsertAfter=TVI_LAST;
				treeitem.mask=TCIF_TEXT|TCIF_IMAGE|TVIF_SELECTEDIMAGE;
				//引用第三个图形列表中的图标
				treeitem.iImage=2;
				treeitem.iSelectedImage=2;
				treeitem.pszText=temp;
				treeinsert.item=treeitem;
				handle_driver=(HANDLE)SendMessage(hTreeView,TVM_INSERTITEM,0,(LPARAM)&treeinsert);
				wsprintf(temp,"%c:\\",i+'A');
				find_file(handle_driver,temp,0);
			}
		}
		break;
	case WM_NOTIFY:
		int i,j,num;
		HTREEITEM htemp;
		HTREEITEM htemp2;
		HTREEITEM htemp3;
		TVITEM tvitem_temp;
		nmhdr=*((NMHDR*)lParam);
		if(nmhdr.hwndFrom==hTreeView){
			if(nmhdr.code==TVN_BEGINDRAG){
				nmtreeview=*((NMTREEVIEW*)lParam);
				hdragimagelist=(HIMAGELIST)SendMessage(hTreeView,TVM_CREATEDRAGIMAGE,0,(LPARAM)(nmtreeview.itemNew.hItem));
				ImageList_BeginDrag(hdragimagelist,0,0,0);
				ImageList_DragEnter(hTreeView,nmtreeview.ptDrag.x,nmtreeview.ptDrag.y);
				SetCapture(hWnd);
                dragmode = TRUE;
			}else if(nmhdr.code==TVN_SELCHANGED){
				htemp=TreeView_GetSelection(hTreeView);
				if(htemp==NULL){
					//MessageBox(NULL,"wrong",NULL,MB_OK);
				}else{
					tvitem_temp.mask=TVIF_TEXT;
					tvitem_temp.hItem=htemp;
					tvitem_temp.pszText=temp;
					tvitem_temp.cchTextMax=100;
					TreeView_GetItem(hTreeView,&tvitem_temp);

					while(1){
						htemp2=TreeView_GetChild(hTreeView,htemp);
						if(htemp2==NULL)
							break;
						TreeView_DeleteItem(hTreeView,htemp2);
					}


					htemp3=htemp;
					for(i=0;;i++){
						htemp2=TreeView_GetParent(hTreeView,htemp3);
						tvitem_temp.mask=TVIF_TEXT;
						tvitem_temp.hItem=htemp3;
						tvitem_temp.pszText=file_temp[i];
						tvitem_temp.cchTextMax=100;
						TreeView_GetItem(hTreeView,&tvitem_temp);	
						if(htemp2==NULL)
							break;
						htemp3=htemp2;
					}
					num=0;
					for(;i>=0;--i){
						j=0;
						while(file_temp[i][j]!='\0'){
							str_file_to_find[num]=file_temp[i][j];
							j++;
							num++;
						}
					}
					list_file_info(str_file_to_find);
					wsprintf(nowfile,"%s",str_file_to_find);
					find_file(htemp,str_file_to_find,0);
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		if(dragmode){
            ImageList_DragLeave(hTreeView);
            ImageList_EndDrag();
            ImageList_Destroy(hdragimagelist);
            HTREEITEM hTmp = (HTREEITEM)SendMessage(hTreeView,TVM_GETNEXTITEM,TVGN_DROPHILITE,0);
            SendMessage(hTreeView,TVM_SELECTITEM,TVGN_CARET,(LPARAM)hTmp);
            SendMessage(hTreeView,TVM_SELECTITEM,TVGN_DROPHILITE,0);
            ReleaseCapture();
            dragmode = FALSE;
		}
		break;
	case WM_MOUSEMOVE:
		if(dragmode){
            tvhit.pt.x = LOWORD(lParam);
            tvhit.pt.y = HIWORD(lParam);
            ImageList_DragMove(tvhit.pt.x,tvhit.pt.y);
            ImageList_DragShowNolock(FALSE);
            HTREEITEM hTmp = (HTREEITEM)SendMessage(hTreeView,TVM_HITTEST,NULL,(LPARAM)&tvhit);
            if(hTmp != NULL)
                SendMessage(hTreeView,TVM_SELECTITEM,TVGN_DROPHILITE,(LPARAM)hTmp);
			
            ImageList_DragShowNolock(TRUE);
        }
		break;

	case WM_COMMAND:
		if((HANDLE)lParam==handle_reset){
			wsprintf(sign_struct.mp3_playing_song_when_stop,"\0");
			sign_struct.mp3_playing_when_stop=FALSE;
			
			wsprintf(commandstring,"close all");
			mciSendString(commandstring,NULL,NULL,NULL);
		}

		if(strcmp(&nowfile[strlen(nowfile)-3],"mp3")==0){
			if((HANDLE)lParam==handle_play){
				if(sign_struct.mp3_playing_when_stop==FALSE || 
					(sign_struct.mp3_playing_when_stop==TRUE && 
					strcmp(sign_struct.mp3_playing_song_when_stop,nowfile)!=0)){
					wsprintf(commandstring,"close all");
					mciSendString(commandstring,NULL,NULL,NULL);
					
					wsprintf(commandstring,"open \"%s\" alias mysong notify",nowfile);
					mciSendString(commandstring,NULL,NULL,(HWND)GetParent(hTreeView));
				}

				wsprintf(commandstring,"play mysong");
				mciSendString(commandstring,NULL,NULL,NULL);
				
			}else if((HANDLE)lParam==handle_stop){
				wsprintf(sign_struct.mp3_playing_song_when_stop,"%s",nowfile);
				sign_struct.mp3_playing_when_stop=TRUE;

				wsprintf(commandstring,"pause mysong");
				mciSendString(commandstring,NULL,NULL,NULL);
			}	
		}
		break;
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}


void find_file(HANDLE handle,char * addr,int n){
	if(n>=2)
		return ;
	n++;

	HANDLE han,hantemp;
	TVINSERTSTRUCT treeinsert;
	TVITEM treeitem;
	WIN32_FIND_DATA fd;
	char temp[1000];
	char nametemp[100];

	//addr[strlen(addr)-1]='\0';
	wsprintf(temp,"%s\\*",addr);

	han=FindFirstFile(temp,&fd);
	if(han==INVALID_HANDLE_VALUE)
		return ;
	while(1){
		if(strcmp(fd.cFileName,".")==0 || strcmp(fd.cFileName,"..")==0){
			if(FindNextFile(han,&fd)==0)
				return ;
			else 
				continue;
		}

		if((fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY){
			wsprintf(nametemp,"%s\\",fd.cFileName);
			treeitem.pszText=nametemp;
			treeitem.mask=TVIF_TEXT|TCIF_IMAGE|TVIF_SELECTEDIMAGE;
			treeitem.iImage=1;
			treeitem.iSelectedImage=1;
			treeinsert.hInsertAfter=TVI_FIRST;
		}else{
			treeitem.pszText=fd.cFileName;
			treeitem.mask=TVIF_TEXT|TCIF_IMAGE|TVIF_SELECTEDIMAGE;
			treeitem.iImage=3;
			treeitem.iSelectedImage=3;
			treeinsert.hInsertAfter=TVI_LAST;
		}
			
		treeinsert.item=treeitem;
		treeinsert.hParent=(HTREEITEM)handle;
		hantemp=(HANDLE)SendMessage(hTreeView,TVM_INSERTITEM,0,(LPARAM)&treeinsert);

		if(((fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY)
			&&((fd.dwFileAttributes&FILE_ATTRIBUTE_SYSTEM)!=FILE_ATTRIBUTE_SYSTEM))
		{
			wsprintf(temp,"%s\\%s",addr,fd.cFileName);
			find_file(hantemp,temp,n);
		}
		if(FindNextFile(han,&fd)==0)
			return ;
	}
}

void list_file_info(char *fileaddr){
	HDC hdc;
	char temp[1000];
	int size=0;
	WIN32_FIND_DATA fd;
	SYSTEMTIME systemtime;
	DWORD num[4];

	if(fileaddr[strlen(fileaddr)-1]=='\\')
		fileaddr[strlen(fileaddr)-1]='\0';

	wsprintf(temp,"%s\n\n\n",fileaddr);
	size=strlen(temp);

	//根目录，这里可以获取一些驱动器信息
	if(strlen(fileaddr)==2){
		int type;


		wsprintf(&temp[size],"磁盘驱动器类型：");
		size=strlen(temp);
		
		type=GetDriveType(fileaddr);
		if(type==DRIVE_UNKNOWN){
			wsprintf(&temp[size],"不明驱动器\n");
			size=strlen(temp);
		}else if(type==DRIVE_REMOVABLE){
			wsprintf(&temp[size],"可移动磁盘\n");
			size=strlen(temp);
		}else if(type==DRIVE_FIXED){
			wsprintf(&temp[size],"固定磁盘\n");
			size=strlen(temp);
		}else if(type==DRIVE_REMOTE){
			wsprintf(&temp[size],"网络驱动器\n");
			size=strlen(temp);
		}else if(type==DRIVE_CDROM){
			wsprintf(&temp[size],"CD_ROM驱动器\n");
			size=strlen(temp);
		}else if(type==DRIVE_RAMDISK){
			wsprintf(&temp[size],"RAM DISK\n");
			size=strlen(temp);
		}


		GetDiskFreeSpace(fileaddr,&num[0],&num[1],&num[2],&num[3]);
		wsprintf(&temp[size],"\n\n在这个驱动器上：\n");
		size=strlen(temp);
		wsprintf(&temp[size],"每簇的扇区数为：     %u\n",num[0]);
		size=strlen(temp);
		wsprintf(&temp[size],"每个扇区的字节数： %u\n",num[1]);
		size=strlen(temp);
		wsprintf(&temp[size],"空闲的簇数：            %-10u  (  %dM  )\n",num[2],num[0]*num[1]/1024*num[2]/1024);
		size=strlen(temp);
		wsprintf(&temp[size],"总共的簇数：            %-10u  (  %dM  )\n",num[3],num[0]*num[1]/1024*num[3]/1024);
		size=strlen(temp);

	}else{
		FindFirstFile(fileaddr,&fd);

		FileTimeToSystemTime(&fd.ftCreationTime,&systemtime);
		wsprintf(&temp[size],"文件创建时间           ：%-4d-%-2d-%-2d %-2d:%-2d:%-2d\n",
						systemtime.wYear,systemtime.wMonth,systemtime.wDay,
						systemtime.wHour,systemtime.wMinute,systemtime.wSecond);
		size=strlen(temp);

		FileTimeToSystemTime(&fd.ftLastAccessTime,&systemtime);
		wsprintf(&temp[size],"文件最后访问时间    ：%-4d-%-2d-%-2d %-2d:%-2d:%-2d\n",
			systemtime.wYear,systemtime.wMonth,systemtime.wDay,
			systemtime.wHour,systemtime.wMinute,systemtime.wSecond);
		size=strlen(temp);

		FileTimeToSystemTime(&fd.ftLastWriteTime,&systemtime);
		wsprintf(&temp[size],"文件最后修改时间    ：%-4d-%-2d-%-2d %-2d:%-2d:%-2d\n\n",
						systemtime.wYear,systemtime.wMonth,systemtime.wDay,
						systemtime.wHour,systemtime.wMinute,systemtime.wSecond);
		size=strlen(temp);
		
		wsprintf(&temp[size],"文件大小：   %-10d字节\n\n", (fd.nFileSizeHigh * (MAXDWORD+1)) + fd.nFileSizeLow);
		size=strlen(temp);
		
		wsprintf(&temp[size],"文件类型：  ");
		size=strlen(temp);
		
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY){
			wsprintf(&temp[size],"目录文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)==FILE_ATTRIBUTE_ARCHIVE){
			wsprintf(&temp[size],"存档文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)==FILE_ATTRIBUTE_COMPRESSED){
			wsprintf(&temp[size],"压缩文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)==FILE_ATTRIBUTE_ENCRYPTED){
			wsprintf(&temp[size],"加密文件 ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==FILE_ATTRIBUTE_HIDDEN){
			wsprintf(&temp[size],"隐藏文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)==FILE_ATTRIBUTE_NORMAL){
			wsprintf(&temp[size],"普通文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)==FILE_ATTRIBUTE_OFFLINE){
			wsprintf(&temp[size],"脱机文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)==FILE_ATTRIBUTE_READONLY){
			wsprintf(&temp[size],"只读文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)==FILE_ATTRIBUTE_REPARSE_POINT){
			wsprintf(&temp[size],"重解析文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)==FILE_ATTRIBUTE_SPARSE_FILE){
			wsprintf(&temp[size],"稀疏文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)==FILE_ATTRIBUTE_SYSTEM){
			wsprintf(&temp[size],"系统文件  ");
			size=strlen(temp);
		}
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)==FILE_ATTRIBUTE_TEMPORARY){
			wsprintf(&temp[size],"临时文件  ");
			size=strlen(temp);
		}


	}
	SetWindowText(hStatic,temp);
	

	if(strlen(fileaddr)==2){
		double i=(double)num[2]/num[3];
		wsprintf(temp,"%f",i);
		//MessageBox(NULL,temp,NULL,MB_OK);

		hdc=GetDC(hStatic);
		SelectObject(hdc,CreateSolidBrush(RGB(200,0,0)));
		Ellipse(hdc, 100, 200, 300, 400);
		SelectObject(hdc,CreateSolidBrush(RGB(0,0,200)));
		Pie(hdc, 100, 200, 300, 400,300,299,(int)(200+100*cos(i*2*PI)),(int)(300-100*sin(i*2*PI)));
		
		ReleaseDC(hStatic,hdc);
	}else{

	}
}

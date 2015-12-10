#include <windows.h>
#include "resource.h"
#include "WndProc.h"
#include "ListFile.h"
#include <commctrl.h>
#include <commdlg.h>
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"winmm.lib")

XC_sign_struct sign_struct={FALSE,FALSE,NULL,"00000\0",FALSE,NULL};

LRESULT CALLBACK WndProc(HWND hWnd, 
						 UINT uMsg, 
						 WPARAM wParam,
						 LPARAM lParam){

	int i,j;
	int driverType;
	char temp[1000];
	char commandstring[1000];
	
	HANDLE hDriveRoot;
	RECT rect;
	HIMAGELIST himagelist;
	TVINSERTSTRUCT treeinsert;
	TVITEM treeitem;

	switch(uMsg){
	case WM_CREATE:
		InitCommonControls();
		GetClientRect(hWnd,&rect);
		hTreeView=CreateWindow("SysTreeView32","xingchen_treeview",
			TVS_HASLINES|TVS_LINESATROOT|WS_CHILD|WS_VISIBLE,
			rect.left,rect.top,600,rect.bottom-rect.top-60,
			hWnd,(HMENU)IDC_XC_TREEVIEW,NULL,NULL);
		hStatic=CreateWindow("STATIC","xingchen_static",WS_VISIBLE|WS_CHILD,
			rect.left+600,rect.top,rect.right-rect.left,rect.bottom-rect.top,
			hWnd,(HMENU)IDC_XC_STATIC,NULL,NULL);

		//创建图形列表
		himagelist=ImageList_Create(32,32,ILC_COLOR32,5,10);
		//为图形列表加载图标
		ImageList_AddIcon(himagelist,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON1)));
		ImageList_AddIcon(himagelist,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON2)));
		ImageList_AddIcon(himagelist,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON3)));
		ImageList_AddIcon(himagelist,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON4)));
		ImageList_AddIcon(himagelist,LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON5)));
		//将图形列表与树形列表绑定
		SendMessage(hTreeView,TVM_SETIMAGELIST,TVSIL_NORMAL,(LPARAM)himagelist);
		
		for(i=0;i<26;i++){
			wsprintf(temp,"%c:\\",i+'A');
			driverType=GetDriveType(temp);
			if(	driverType==DRIVE_FIXED     || driverType==DRIVE_REMOTE || 
				driverType==DRIVE_REMOVABLE || driverType==DRIVE_RAMDISK|| 
				driverType==DRIVE_CDROM) {

				treeinsert.hParent=NULL;
				treeinsert.hInsertAfter=TVI_LAST;
				treeitem.mask=TCIF_TEXT|TCIF_IMAGE|TVIF_SELECTEDIMAGE;
				treeitem.iImage=1;
				treeitem.iSelectedImage=1;
				treeitem.pszText=temp;
				treeinsert.item=treeitem;
				hDriveRoot=(HANDLE)SendMessage(hTreeView,TVM_INSERTITEM,0,(LPARAM)&treeinsert);
 				wsprintf(temp,"%c:\\",i+'A');
 				ListFile(hDriveRoot,temp);
			}
		}
		break;
	case WM_COMMAND: 
		if((HANDLE)lParam==handle_reset){
			sign_struct.myMultimediaIsPlaying=FALSE;
			mciSendString("close all",NULL,NULL,NULL);
			if(strcmp(&nowFile[strlen(nowFile)-3],"mp3")!=0){
				DestroyWindow((HWND)handle_reset);
				sign_struct.buttonSign[3]='0';
			}

			//当按下结束键时，应刷新Static窗口以显示最新信息
			//主要是那个 “MyMultiMedia is Playing”
			ListFileInfo(hStatic,nowFile);
		}
		if(strcmp(&nowFile[strlen(nowFile)-3],"mp3")==0){
			if((HANDLE)lParam==handle_play){
				sign_struct.myMultimediaIsPlaying=TRUE;

				//当按下结束键时，应刷新Static窗口以显示最新信息
				//主要是那个 “MyMultiMedia is Playing”
				ListFileInfo(hStatic,nowFile);

				if(sign_struct.mp3_playing_when_stop==FALSE || 
					(sign_struct.mp3_playing_when_stop==TRUE && 
					strcmp(sign_struct.mp3_playing_song_when_stop,nowFile)!=0)){
					wsprintf(commandstring,"close all");
					mciSendString(commandstring,NULL,NULL,NULL);
					
					wsprintf(commandstring,"open \"%s\" alias myMultimedia notify",nowFile);
					mciSendString(commandstring,NULL,NULL,(HWND)GetParent(hTreeView));
				}
				
				wsprintf(commandstring,"play myMultimedia");
				mciSendString(commandstring,NULL,NULL,NULL);
				
			}else if((HANDLE)lParam==handle_stop){
				wsprintf(sign_struct.mp3_playing_song_when_stop,"%s",nowFile);
				sign_struct.mp3_playing_when_stop=TRUE;
				
				wsprintf(commandstring,"pause myMultimedia");
				mciSendString(commandstring,NULL,NULL,NULL);
			}	
		}

		break;
	case WM_NOTIFY:
		char temp1[10000];
		NMHDR nmhdr;
		HTREEITEM hSelected;
		HTREEITEM htemp;
		TVITEM tvitem_temp;
		char str_to_find_file[10000];

		nmhdr=*((NMHDR*)lParam);

		//若是双击事件
		if(nmhdr.hwndFrom==hTreeView && nmhdr.code==NM_DBLCLK){
			//得到选中项
			hSelected=TreeView_GetSelection(hTreeView);

			tvitem_temp.mask=TVIF_TEXT;
			tvitem_temp.hItem=hSelected;
			tvitem_temp.pszText=temp;
			tvitem_temp.cchTextMax=100;
			TreeView_GetItem(hTreeView,&tvitem_temp);

			if(tvitem_temp.pszText[strlen(tvitem_temp.pszText)-1]=='\\'){
				htemp=TreeView_GetChild(hTreeView,hSelected);
				if(htemp==NULL){
					getFullTreePath(hTreeView,hSelected,str_to_find_file);
					wsprintf(nowFile,"%s",str_to_find_file);
					ListFile(hSelected,str_to_find_file);
				}
			}
		}
		if(nmhdr.hwndFrom==hTreeView && nmhdr.code==TVN_SELCHANGED){
			hSelected=TreeView_GetSelection(hTreeView);
			getFullTreePath(hTreeView,hSelected,str_to_find_file);
			wsprintf(nowFile,"%s",str_to_find_file);

			if(strcmp(&str_to_find_file[strlen(str_to_find_file)-3],"mp3")==0){
				if(sign_struct.buttonSign[0]=='0'){
					GetClientRect(hWnd,&rect);
					handle_play	=CreateWindow("BUTTON", "播放", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
						100+rect.left,rect.bottom-45, 60, 30, 
						GetParent(hTreeView),(HMENU)MAKEINTRESOURCE(IDC_BUTTON1), GetModuleHandle(NULL), NULL); 
					handle_stop	=CreateWindow("BUTTON", "暂停", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
						185+rect.left,rect.bottom-45, 60, 30, 
						GetParent(hTreeView),(HMENU)MAKEINTRESOURCE(IDC_BUTTON2), GetModuleHandle(NULL), NULL); 
					
					if(sign_struct.buttonSign[3]=='0'){
						handle_reset=CreateWindow("BUTTON", "结束", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
							270+rect.left,rect.bottom-45, 60, 30, 
							GetParent(hTreeView),(HMENU)MAKEINTRESOURCE(IDC_BUTTON3), GetModuleHandle(NULL), NULL); 
					}

					handle_rew	=CreateWindow("BUTTON", "快退", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
						355+rect.left,rect.bottom-45, 60, 30, 
						GetParent(hTreeView),(HMENU)MAKEINTRESOURCE(IDC_BUTTON4), GetModuleHandle(NULL), NULL); 
					handle_speed=CreateWindow("BUTTON", "快进", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
						440+rect.left,rect.bottom-45, 60, 30, 
						GetParent(hTreeView),(HMENU)MAKEINTRESOURCE(IDC_BUTTON5), GetModuleHandle(NULL), NULL); 
					sign_struct.buttonSign[0]='1';
					sign_struct.buttonSign[1]='1';
					sign_struct.buttonSign[2]='1';
					sign_struct.buttonSign[4]='1';
					sign_struct.buttonSign[5]='1';
			
				}
			}else{
				if(sign_struct.myMultimediaIsPlaying==FALSE){
					DestroyWindow((HWND)handle_reset);
					sign_struct.buttonSign[3]='0';
				}
				DestroyWindow((HWND)handle_play);
				DestroyWindow((HWND)handle_stop);
				DestroyWindow((HWND)handle_rew);
				DestroyWindow((HWND)handle_speed);
				sign_struct.buttonSign[0]='0';
				sign_struct.buttonSign[1]='0';
				sign_struct.buttonSign[2]='0';
				sign_struct.buttonSign[4]='0';
				sign_struct.buttonSign[5]='0';
			}

			ListFileInfo(hStatic,str_to_find_file);
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



void getFullTreePath(HWND hTreeView,HTREEITEM hSelected,char*str_to_find_file){
	HTREEITEM htemp;
	TVITEM tvitem_temp;
	char temp1[10000];
	char temp[100];
	
	tvitem_temp.mask=TVIF_TEXT;
	tvitem_temp.hItem=hSelected;
	tvitem_temp.pszText=temp;
	tvitem_temp.cchTextMax=100;
	TreeView_GetItem(hTreeView,&tvitem_temp);
	if(tvitem_temp.pszText[strlen(tvitem_temp.pszText)-1]=='\\')
		wsprintf(str_to_find_file,"%s\\",tvitem_temp.pszText);
	else 
		wsprintf(str_to_find_file,"%s",tvitem_temp.pszText);
	
	htemp=hSelected;
	while(1){
		htemp=TreeView_GetParent(hTreeView,htemp);
		if(htemp==NULL)
			break;
		tvitem_temp.mask=TVIF_TEXT;
		tvitem_temp.hItem=htemp;
		tvitem_temp.pszText=temp;
		tvitem_temp.cchTextMax=100;
		TreeView_GetItem(hTreeView,&tvitem_temp);
		wsprintf(temp1,"%s",str_to_find_file);
		wsprintf(str_to_find_file,"%s\\%s",tvitem_temp.pszText,temp1);					
	}
}
#ifndef MAINFRAME__H 
#define MAINFRAME__H

#include <winuser.h>


///////////////////////////////////////////////////
//message
#define WM_MOUSEWHEEL	0x020A			//因为系统版本过低，所以需要在这里手动定义

#define XC_OPENWINDOW   WM_USER+10		//wParam存储将要创建子窗口的标题
#define	XC_SB_MOUSEWHEEL WM_USER+9		//lParam为一个int表示向上或者向下的行数
#define XC_INITBITMAP	WM_USER+12		//初始化绘图板，只在打开正确文件格式的PE文件之后会被发送一次

#define XC_REDRAWTIMER		10001
#define XC_REDRAWTIME		500		
#define IDM_WINDOWCLOSEALL	40012	
//////////////////////////////////////////////////
//constant
#define IDM_FIRSTCHILD	50000


/////////////////////////////////////////////////
//用来区分子窗口
#define	CHILDWINDOWNUM			5


//子窗口的ID号
#define XC_NT_INFO				0
#define XC_SECTION_INFO			1
#define XC_IMPORT_DESCRIPTOR	2
#define XC_EXPORT_DESCRIPTOR	3
#define XC_DELAY_IMPORT_DESCRIPTOR	4

#define XC_SCROLL_PAGE	30

static TCHAR	title[][50]=
				{	
					TEXT("NT Header Info"),
					TEXT("Section And Data Directory Info"),
 					TEXT("Import Descriptor Info"),
 					TEXT("Export Descriptor Info"),
					TEXT("Delay Import Descriptor Info")
				};


typedef struct tagCHILDWININFO		//用来存储每个子窗口的一些信息
{
	int				ID;				//用来存储子窗口的ID号
	POINT			windowSize;		//用来存储子窗口的当前窗口大小
	SCROLLINFO		scrollInfo;		//用来存储子窗口的滚动条状态
	int				scrollPageNum;	
	int				drawBoardSize;	//用来存储子窗口需要申请的绘图板大小，以屏幕y轴的分辨率为单位
}CHILDWININFO,*PCHILDWININFO;


///////////////////////////////////////////////
//function
LRESULT CALLBACK FrameWndProc	(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
BOOL	CALLBACK CloseEnumProc	(HWND hWnd,LPARAM lParam);
LRESULT	CALLBACK InfoWndProc	(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);



INT_PTR CALLBACK DialogProc(
		HWND hwndDlg,  // handle to dialog box
		UINT uMsg,     // message
		WPARAM wParam, // first message parameter
		LPARAM lParam  // second message parameter
		);
bool getValueFromString(char *temp,DWORD *value);

#endif


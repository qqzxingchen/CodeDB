#ifndef MAINFRAME__H 
#define MAINFRAME__H

#include <winuser.h>


///////////////////////////////////////////////////
//message
#define WM_MOUSEWHEEL	0x020A			//��Ϊϵͳ�汾���ͣ�������Ҫ�������ֶ�����

#define XC_OPENWINDOW   WM_USER+10		//wParam�洢��Ҫ�����Ӵ��ڵı���
#define	XC_SB_MOUSEWHEEL WM_USER+9		//lParamΪһ��int��ʾ���ϻ������µ�����
#define XC_INITBITMAP	WM_USER+12		//��ʼ����ͼ�壬ֻ�ڴ���ȷ�ļ���ʽ��PE�ļ�֮��ᱻ����һ��

#define XC_REDRAWTIMER		10001
#define XC_REDRAWTIME		500		
#define IDM_WINDOWCLOSEALL	40012	
//////////////////////////////////////////////////
//constant
#define IDM_FIRSTCHILD	50000


/////////////////////////////////////////////////
//���������Ӵ���
#define	CHILDWINDOWNUM			5


//�Ӵ��ڵ�ID��
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


typedef struct tagCHILDWININFO		//�����洢ÿ���Ӵ��ڵ�һЩ��Ϣ
{
	int				ID;				//�����洢�Ӵ��ڵ�ID��
	POINT			windowSize;		//�����洢�Ӵ��ڵĵ�ǰ���ڴ�С
	SCROLLINFO		scrollInfo;		//�����洢�Ӵ��ڵĹ�����״̬
	int				scrollPageNum;	
	int				drawBoardSize;	//�����洢�Ӵ�����Ҫ����Ļ�ͼ���С������Ļy��ķֱ���Ϊ��λ
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


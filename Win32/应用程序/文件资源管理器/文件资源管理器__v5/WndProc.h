#ifndef WndProc__H
#define WndProc__H

#include <commctrl.h>

#define IDC_XC_TREEVIEW 4001
#define IDC_XC_STATIC   4002
#define IDC_XC_AVI_WINDOWS 4003


#define IDC_BUTTON1 2001 
#define IDC_BUTTON2 2002 
#define IDC_BUTTON3 2003 
#define IDC_BUTTON4 2004 
#define IDC_BUTTON5 2005 

typedef struct XC_sign_struct{
	BOOL myMultimediaIsPlaying;	//��ǰ�Ƿ��ڲ���Ӱ��
	BOOL mp3_playing_when_stop;	//��������ͣʱ����ǰ�Ƿ��ڲ�������
	char mp3_playing_song_when_stop[1000];
								//��������ͣʱ����ǰ���ڲ��ŵ�����
	char buttonSign[7];			//���߸�char��ֻ��ȡ  '0' '1'
								//buttonSign[0]����������ť����״̬
								//buttonSign[1~5]�ֱ����  ���ţ���ͣ�����������������  ������ť�Ƿ����

}XC_sign_struct;




extern HWND hTreeView;
extern HWND hStatic;
extern HANDLE handle_play,handle_reset,handle_stop,handle_speed,handle_rew;
extern HANDLE handle_avi_window;

extern char nowFile[1000];

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void getFullTreePath(HWND hTreeView,HTREEITEM hSelected,char * str_to_find_file);



#endif
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
	BOOL myMultimediaIsPlaying;	//当前是否在播放影音
	BOOL mp3_playing_when_stop;	//当按下暂停时，当前是否在播放音乐
	char mp3_playing_song_when_stop[1000];
								//当按下暂停时，当前正在播放的音乐
	char buttonSign[7];			//这七个char型只能取  '0' '1'
								//buttonSign[0]代表六个按钮的总状态
								//buttonSign[1~5]分别代表  播放，暂停，结束，快进，快退  六个按钮是否存在

}XC_sign_struct;




extern HWND hTreeView;
extern HWND hStatic;
extern HANDLE handle_play,handle_reset,handle_stop,handle_speed,handle_rew;
extern HANDLE handle_avi_window;

extern char nowFile[1000];

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void getFullTreePath(HWND hTreeView,HTREEITEM hSelected,char * str_to_find_file);



#endif
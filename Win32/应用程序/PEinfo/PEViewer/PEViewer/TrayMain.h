#ifndef TRAYMAIN__H
#define TRAYMAIN__H

#include <windows.h>


// 静态ID
const UINT XC_ANALYSEPEFILE = WM_USER + 0x1;    // 自定义消息，其中lParam(TCHAR *)中存储着PE文件的路径信息
const UINT XC_DIALOG_SHOWHIDE = WM_USER + 0x2;  

const UINT IDM_ITEM_QUIT = 0x2000;          // 在托盘图标上右键点击后消息的ID
const UINT IDM_ITEM_INDEXSTART = 0x3000;    // 托盘右键菜单上每项的ID都是 IDM_ITEM_INDEXSTART + index

const UINT ID_HOTKEY_ALT_R = 0x4000;        // 当按下Alt加R时

const int OLDFILEMAXNUMBER = 10;            // 每次程序运行时读取config.txt文件中历史数据的个数

const int wndWidth = 300;
const int wndHeight = 200;

const TCHAR scriptFilePath[] = TEXT("resources\\script.js");
const TCHAR styleFilePath[] = TEXT("resources\\style.css");
const TCHAR histroyFilePath[] = TEXT("resources\\histroy.txt");   // 该文件是用来存储历史查询记录的
const TCHAR webbrowserCommandFilePath[] = TEXT("resources\\webbrowsercommand.txt");

// 函数声明

void writeToHtml( const TCHAR peFilePath[],TCHAR htmlFilePath[] );
void showHtml( const TCHAR htmlFilePath[] );
bool registerWndClass( HINSTANCE hInstance );
void initTrayIcon( HWND hwnd,NOTIFYICONDATA * nid );


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);




#endif // TRAYMAIN__H



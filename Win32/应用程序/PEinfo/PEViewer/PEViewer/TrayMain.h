#ifndef TRAYMAIN__H
#define TRAYMAIN__H

#include <windows.h>


// ��̬ID
const UINT XC_ANALYSEPEFILE = WM_USER + 0x1;    // �Զ�����Ϣ������lParam(TCHAR *)�д洢��PE�ļ���·����Ϣ
const UINT XC_DIALOG_SHOWHIDE = WM_USER + 0x2;  

const UINT IDM_ITEM_QUIT = 0x2000;          // ������ͼ�����Ҽ��������Ϣ��ID
const UINT IDM_ITEM_INDEXSTART = 0x3000;    // �����Ҽ��˵���ÿ���ID���� IDM_ITEM_INDEXSTART + index

const UINT ID_HOTKEY_ALT_R = 0x4000;        // ������Alt��Rʱ

const int OLDFILEMAXNUMBER = 10;            // ÿ�γ�������ʱ��ȡconfig.txt�ļ�����ʷ���ݵĸ���

const int wndWidth = 300;
const int wndHeight = 200;

const TCHAR scriptFilePath[] = TEXT("resources\\script.js");
const TCHAR styleFilePath[] = TEXT("resources\\style.css");
const TCHAR histroyFilePath[] = TEXT("resources\\histroy.txt");   // ���ļ��������洢��ʷ��ѯ��¼��
const TCHAR webbrowserCommandFilePath[] = TEXT("resources\\webbrowsercommand.txt");

// ��������

void writeToHtml( const TCHAR peFilePath[],TCHAR htmlFilePath[] );
void showHtml( const TCHAR htmlFilePath[] );
bool registerWndClass( HINSTANCE hInstance );
void initTrayIcon( HWND hwnd,NOTIFYICONDATA * nid );


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);




#endif // TRAYMAIN__H



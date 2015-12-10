#include <windows.h>
#include "resource.h"
#include "PEBaseClass.h"
#include "PEImportClass.h"
#include "PEDelayImportClass.h"
#include "PEExportClass.h"
#include "PERelocation.h"
#include "global.h"

#include "TrayMain.h"

PEBase * peBase;
PEImportClass * importObj;
PEDelayImportClass * delayImportObj;
PEExportClass * exportObj;
PERelocation * relocationObj;

LPCTSTR szAppName = TEXT("PEViewerAppName");
LPCTSTR szWndName = TEXT("PEViewer");

char szOldFilePath[OLDFILEMAXNUMBER][MAX_PATH];

void readOldFilePathDataFromFile(HANDLE pathDataFile,char pathDataArray[][MAX_PATH])
{
    int index = 0;
    int offset = 0;
    char tempStr[MAX_PATH];
    DWORD byteNum;
    for ( int i = 0 ; i < OLDFILEMAXNUMBER ; i ++ )
        pathDataArray[i][0] = '\0';

    while(index < OLDFILEMAXNUMBER){
        SetFilePointer(pathDataFile,offset,NULL,FILE_BEGIN);
        ReadFile(pathDataFile,tempStr,MAX_PATH,&byteNum,NULL);

        if ( byteNum == 0 )
            break;    

        for ( int i = 0 ; tempStr[i]!='\0' ; i ++ ){
            if ( tempStr[i] != '\n' ){
                pathDataArray[index][i] = tempStr[i];
            }else{
                offset += i+1;
                break;
            }
        }
        index ++;
    }
}
void writeOldFilePathDataIntoFile(
    HANDLE pathDataFile,
    char pathDataArray[][MAX_PATH])
{
    char tempStr[MAX_PATH];
    DWORD byteNum;
    int index = 0;
    while ( index < OLDFILEMAXNUMBER ){
        if ( pathDataArray[index][0] == '\0' )
            break;
        wsprintfA(tempStr,"%s\n",pathDataArray[index]);
        WriteFile(pathDataFile,tempStr,strlen(tempStr),&byteNum,NULL);
        int i = GetLastError();
        index ++;
    }
}

// 返回值：如果确实插入了，则返回true；如果已经存在，则返回false
bool insertOneFilePath(
    TCHAR * pathDataT,
    char pathDataArray[][MAX_PATH])
{
    char tempStr[MAX_PATH];
    wCharToMChar(pathDataT,tempStr);
    for ( int i = 0 ; i < OLDFILEMAXNUMBER ; i ++ ){
        if ( strcmp(tempStr,pathDataArray[i]) == 0 )
            return false;
        if ( pathDataArray[i][0] == '\0' ){
            wsprintfA(pathDataArray[i],"%s",tempStr);
            return true;
        }
    }
    for ( int i = 0 ; i < OLDFILEMAXNUMBER-1 ; i ++ )
        wsprintfA(pathDataArray[i],"%s",pathDataArray[i+1]);    
    wsprintfA(pathDataArray[OLDFILEMAXNUMBER-1],"%s",tempStr);
    return true;
}


// 重建托盘图标的右键菜单
void recreateTrayMenu(HMENU * menu,char pathDataArray[][MAX_PATH]){
    if ( *menu != (HMENU)INVALID_HANDLE_VALUE ){
        DestroyMenu(*menu);
        *menu = (HMENU)INVALID_HANDLE_VALUE;
    }
        
    *menu = CreatePopupMenu();
    int index = 0;
    while( index < OLDFILEMAXNUMBER ){
        if ( szOldFilePath[index][0] == '\0' )
            break;
        AppendMenuA(*menu,MF_STRING, IDM_ITEM_INDEXSTART+index,szOldFilePath[index]);
        index ++;
    }
    AppendMenu(*menu,MF_STRING, IDM_ITEM_QUIT,TEXT("退出"));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR szCmdLine, int iCmdShow)
{
    const TCHAR SIGNALMUTEXTNAME[] = TEXT("XCMUTEXT_PEViewer");

    HANDLE singleMutexHandle = OpenMutex(MUTEX_ALL_ACCESS,false,SIGNALMUTEXTNAME);
    if ( singleMutexHandle != NULL ){
        CloseHandle(singleMutexHandle);
        return 0;
    }else{
        CreateMutex(NULL,false,SIGNALMUTEXTNAME);
    }

    HWND hwnd;
    MSG msg;

    if (!registerWndClass(hInstance))
    {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_ACCEPTFILES | WS_EX_LAYERED,
        szAppName, 
        szWndName,
        WS_THICKFRAME ,
        GetSystemMetrics(SM_CXSCREEN) - wndWidth - 40,
        GetSystemMetrics(SM_CYSCREEN) - wndHeight - 60,
        wndWidth,
        wndHeight,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static UINT WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));  // 不要修改TaskbarCreated，这是系统任务栏自定义的消息

    static TCHAR szFileName[MAX_PATH] = TEXT("");  // 用来存储exe文件名

    static HMENU hMenuPopup;            // 右键菜单
    static NOTIFYICONDATA nid;
    HANDLE histroyFileHandle;
    HDC hdc;
    POINT point;
    PAINTSTRUCT pt;
    HDROP hDropInfo;
    
    static TCHAR wndDisText[256] = TEXT("");

    static bool hotKeyOnline = false;

    switch (message)
    {
    case WM_CREATE:
        initTrayIcon( hwnd,&nid );

        if ( hotKeyOnline == false )
            if (RegisterHotKey(hwnd,ID_HOTKEY_ALT_R,MOD_ALT,'R') != 0)
                hotKeyOnline = true;
        
        histroyFileHandle = CreateFile( 
            histroyFilePath,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL );
        if ( histroyFileHandle != INVALID_HANDLE_VALUE ){
            readOldFilePathDataFromFile(histroyFileHandle,szOldFilePath);
            CloseHandle(histroyFileHandle);
        }
        recreateTrayMenu(&hMenuPopup,szOldFilePath);

        SendMessage(hwnd,WM_KILLFOCUS,NULL,NULL);
        SetLayeredWindowAttributes(hwnd,NULL,240,LWA_ALPHA);
        break;
    case WM_DROPFILES:      // 获取拖拽过来的文件名
        hDropInfo = (HDROP) wParam; 
        DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
        SendMessage(hwnd,XC_ANALYSEPEFILE,LPARAM(szFileName),WPARAM(NULL));
        break;
    case WM_COMMAND:        // 托盘菜单响应
        switch(LOWORD(wParam)){
        case IDM_ITEM_QUIT:
            SendMessage(hwnd,WM_DESTROY,NULL,NULL);
        default:
            if ( LOWORD(wParam) >= IDM_ITEM_INDEXSTART && LOWORD(wParam) < IDM_ITEM_INDEXSTART + OLDFILEMAXNUMBER ){
                mCharToWChar(szOldFilePath[LOWORD(wParam) - IDM_ITEM_INDEXSTART],szFileName);
                SendMessage(hwnd,XC_ANALYSEPEFILE,LPARAM(szFileName),WPARAM(NULL));
            }
        }
        break;
    case WM_HOTKEY:         // 热键
        if ( wParam == ID_HOTKEY_ALT_R )
            SendMessage(hwnd,XC_DIALOG_SHOWHIDE,NULL,NULL);
        break;
    case WM_USER:           // 托盘图标按键消息
        if (lParam == WM_RBUTTONDOWN){
            GetCursorPos(&point);
            SetForegroundWindow(hwnd);
            TrackPopupMenu (hMenuPopup, TPM_RIGHTBUTTON,point.x,point.y, 0, hwnd, NULL) ;  
        }else if(lParam == WM_LBUTTONDOWN || lParam == WM_LBUTTONDBLCLK){
            SendMessage(hwnd,XC_DIALOG_SHOWHIDE,NULL,NULL);
        }
        break;
    case WM_SETFOCUS:       // 当窗口取得焦点时
        SetLayeredWindowAttributes(hwnd,NULL,240,LWA_ALPHA);
        return 0;
    case WM_KILLFOCUS:      // 当窗口失去焦点时
        SetLayeredWindowAttributes(hwnd,NULL,100,LWA_ALPHA);
        return 0;
    case XC_DIALOG_SHOWHIDE:
        ShowWindow(hwnd,IsWindowVisible(hwnd) ? SW_HIDE : SW_SHOW);
        break;
    case WM_DESTROY:
        histroyFileHandle = CreateFile( histroyFilePath,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL );
        if ( histroyFileHandle != INVALID_HANDLE_VALUE ){
            writeOldFilePathDataIntoFile(histroyFileHandle,szOldFilePath);
            CloseHandle(histroyFileHandle);
        }
        if (hotKeyOnline)
            UnregisterHotKey(hwnd,ID_HOTKEY_ALT_R);
        DestroyMenu (hMenuPopup);
        Shell_NotifyIcon(NIM_DELETE, &nid);
        PostQuitMessage(0);
        break;

    case XC_ANALYSEPEFILE:
        peBase = new PEBase( szFileName );
        if ( ! peBase->getDosNTSection() ){
            MessageBox( NULL,peBase->ErrorString,TEXT("Error"),MB_OK );
            wsprintf(szFileName,TEXT("\0\0"));
            return 0;
        }
        if ( ! peBase->writeToTempFile() ){		
            MessageBox( NULL,peBase->ErrorString,TEXT("Error"),MB_OK );
            wsprintf(szFileName,TEXT("\0\0"));
            return 0;
        }
        importObj = new PEImportClass( peBase );
        importObj->writeToTempFile();
        delayImportObj = new PEDelayImportClass( peBase );
        delayImportObj->writeToTempFile();
        exportObj = new PEExportClass( peBase );
        exportObj->writeToTempFile();
        relocationObj = new PERelocation( peBase );
        relocationObj->writeToTempFile();

        TCHAR htmlFilePath [MAX_PATH];
        writeToHtml(peBase->getPEFilePath(),htmlFilePath);
        showHtml( htmlFilePath );

        if(insertOneFilePath(szFileName,szOldFilePath)){
            recreateTrayMenu(&hMenuPopup,szOldFilePath);
        }

        wsprintf(wndDisText,peBase->optionHeader->isWin32PEFile()?TEXT("Win32"):TEXT("Win64"));

        InvalidateRect(hwnd,NULL,true);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd,&pt);
        TextOut(hdc,0,0,szFileName,wcslen(szFileName));
        TextOut(hdc,0,50,wndDisText,wcslen(wndDisText));
        EndPaint(hwnd,&pt);
        break;
    default:
        /*
        * 防止当Explorer.exe 崩溃以后，程序在系统系统托盘中的图标就消失
        *
        * 原理：Explorer.exe 重新载入后会重建系统任务栏。当系统任务栏建立的时候会向系统内所有
        * 注册接收TaskbarCreated 消息的顶级窗口发送一条消息，我们只需要捕捉这个消息，并重建系
        * 统托盘的图标即可。
        */
        if (message == WM_TASKBARCREATED)
            SendMessage(hwnd,WM_CREATE,NULL,NULL);
        break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

bool registerWndClass( HINSTANCE hInstance ){
    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = GetModuleHandle(NULL);
    wndclass.hIcon = LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TRAYICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    return RegisterClass(&wndclass);
}
void initTrayIcon( HWND hwnd,NOTIFYICONDATA * nid ){
    nid->cbSize = sizeof(nid);
    nid->hWnd = hwnd;
    nid->uID = 0;
    nid->uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid->uCallbackMessage = WM_USER;
    nid->hIcon = LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TRAYICON));
    lstrcpy(nid->szTip, szAppName);
    Shell_NotifyIcon(NIM_ADD, nid);
}

void writeToHtml( const TCHAR peFilePath[],TCHAR htmlFilePath[] ){
    // 根据文件的全路径名得到文件名，并为其后缀上_index.html，作为html的文件名
    TCHAR fileName[MAX_PATH];   // 不包括全路径，仅有文件名和后缀民
    getFileNameFromFullPath(peFilePath,fileName);
    for ( int i = wcslen(fileName)-1 ; i >= 0 ; i -- ){
        if ( fileName[i] == '.' ){
            fileName[i] = '_';
            break;
        }
    }

    wsprintf( &fileName[wcslen(fileName)],TEXT("_index.html"),fileName );
    if ( htmlFilePath != NULL ) 
        wsprintf( htmlFilePath,TEXT("%s"),fileName );

    HANDLE dataFile,
            importDataFile,
            delayImportDataFile,
            exportDataFile,
            relocationDataFile,
            scriptFile,
            styleFile,
            outFile;
    dataFile = CreateFile( peBase->GetTempFileName(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL );
    importDataFile = CreateFile( importObj->GetTempFileName(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL );
    delayImportDataFile = CreateFile( delayImportObj->GetTempFileName(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL );
    exportDataFile = CreateFile( exportObj->GetTempFileName(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL );
    relocationDataFile = CreateFile( relocationObj->GetTempFileName(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL );
    styleFile = CreateFile( styleFilePath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL );
    scriptFile = CreateFile( scriptFilePath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL );
    outFile = CreateFile( fileName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL );

    DWORD byteNum;
    char filePath[MAX_PATH*2];

    writeStringToFile(outFile,"<!DOCTYPE html>\n<html>\n<head>\n<style>\n");
    writeFileDataToFile( outFile,styleFile );
    writeStringToFile(outFile,"\n</style>\n<script>\n");

    writeStringToFile(outFile,"var PEFilePath = '");

    memset(filePath,0,MAX_PATH);
    wCharToMChar(peBase->getPEFilePath(),filePath);
    addTranslate(filePath);
    WriteFile(outFile,(LPCVOID)filePath,strlen(filePath),&byteNum,NULL);

    writeStringToFile(outFile,"';\n");

    writeFileDataToFile( outFile,dataFile );
    writeFileDataToFile( outFile,importDataFile );
    writeFileDataToFile( outFile,delayImportDataFile);
    writeFileDataToFile( outFile,exportDataFile );
    writeFileDataToFile( outFile,relocationDataFile );
    writeFileDataToFile( outFile,scriptFile );

    writeStringToFile(outFile,"\n</script>\n<head>\n<body>\n</body>\n</html>");

    CloseHandle( dataFile );
    CloseHandle( importDataFile );
    CloseHandle( delayImportDataFile );
    CloseHandle( exportDataFile );
    CloseHandle( relocationDataFile );
    CloseHandle( scriptFile );
    CloseHandle( styleFile );
    CloseHandle( outFile );
}



void showHtml( const TCHAR htmlFilePath[] ){
    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    TCHAR commandStr[1024] = TEXT("\0");

    HANDLE webbrowserCommandFileHandle = CreateFile( webbrowserCommandFilePath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL );
    if ( webbrowserCommandFileHandle == INVALID_HANDLE_VALUE ){
        return ;
    }
    DWORD byteNum;
    char commands[1024];
    ReadFile(webbrowserCommandFileHandle,commands,1024,&byteNum,NULL);
    CloseHandle(webbrowserCommandFileHandle);

    wsprintfA( &commands[byteNum],"\0" );

    mCharToWChar(commands,commandStr);

    wsprintf(&commandStr[wcslen(commandStr)],TEXT(" %s"),htmlFilePath);

    CreateProcess( NULL,commandStr,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi );
}

#include <windows.h>
#include <stdio.h>
#define STRMAXLEN 25
#define STRMINLEN 8 
char string[]="生日快乐";

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ; 
////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////// 
typedef struct tagCharChain 
{ 
   struct tagCharChain *prev;   //链表的前个元素
 
   TCHAR ch;                     //一个显示列中的一个字符
   struct tagCharChain *next;    //链表的后个元素
}CharChain, *pCharChain; 

typedef struct tagCharColumn 
{ 
   CharChain *head, *current, *point; 
   int x, y, iStrLen;                      //显示列的开始显示的x,y坐标,iStrLen是这个列的长度
   int iStopTimes, iMustStopTimes;         //已经停滞的次数和必须停滞的次数,必须停滞的次数是随机的
}CharColumn, *pCharColumn; 

int WINAPI WinMain(
		 HINSTANCE hInstance, 
		 HINSTANCE hPrevInstance, 
         PSTR szCmdLine, 
		 int iCmdShow
		) 
{ 
   static TCHAR szAppName[] = TEXT ("matrix") ; 
   HWND hwnd ; 
   MSG msg ; 
   WNDCLASS wndclass ; 

   wndclass.style = CS_HREDRAW | CS_VREDRAW ; 
   wndclass.lpfnWndProc = WndProc ; 
   wndclass.cbClsExtra = 0 ; 
   wndclass.cbWndExtra = 0 ; 
   wndclass.hInstance = hInstance ; 
   wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION) ; 
   wndclass.hCursor = LoadCursor (NULL, IDC_ARROW) ; 
   wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH) ; 
   wndclass.lpszMenuName = NULL ; 
   wndclass.lpszClassName = szAppName ; 

   if(!RegisterClass (&wndclass)) 
   { 
       MessageBox (NULL, TEXT ("此程序必须运行在NT下!"), szAppName, MB_ICONERROR) ; 
       return 0; 
   } 

   hwnd = CreateWindow (
	                    szAppName, 
	                    NULL, 
                        WS_DLGFRAME | WS_THICKFRAME | WS_POPUP, 
						0,0,                               //显示窗口的左上点坐标 
                        GetSystemMetrics(SM_CXSCREEN),        //显示窗口的右下点坐标    
						GetSystemMetrics(SM_CYSCREEN), 
                   
						NULL, 
						NULL, 
						hInstance, 
                        NULL
					  ) ; 

   ShowWindow (hwnd,SW_SHOWMAXIMIZED) ;    //最大化显示 
   UpdateWindow (hwnd) ; 
   ShowCursor(FALSE); 

   srand ((int) GetCurrentTime ()) ; 
   while (GetMessage (&msg, NULL, 0, 0)) 
   { 
       TranslateMessage (&msg) ; 
       DispatchMessage (&msg) ; 
   } 
   ShowCursor(TRUE); 
   return msg.wParam ; 
} 

TCHAR randomChar() 
{ 
    return (TCHAR)(rand()%(126-33)+33); //ascii码表中，随机显示33到126的字符（这些字符是可见的）
} 

int init(CharColumn *cc, int cyScreen, int x)  
{ 
   int j; 
   cc->iStrLen = rand()%(STRMAXLEN-STRMINLEN) + STRMINLEN; 
   //cc->x = x+100;
   cc->x = x+3 ;
   
   cc->y =rand()%3?rand()%cyScreen:0;   //每列字符的起点 
   cc->iMustStopTimes = rand()%6 ; 
   cc->iStopTimes = 0 ; 
   cc->head = cc->current = (pCharChain)calloc(cc->iStrLen, sizeof(CharChain)); 
   for(j=0; j<cc->iStrLen-1; j++) 
   { 
       cc->current->prev = cc->point;  
       cc->current->ch = '\0'; 
       cc->current->next = cc->current+1; 
       cc->point = cc->current; cc->current++; 
   } 
   cc->current->prev = cc->point; 
   cc->current->ch = '\0'; 
   cc->current->next = cc->head; 
   cc->head->prev = cc->current;
 
   cc->current = cc->point = cc->head; 
   cc->head->ch = randomChar(); 
   return 0; 
} 

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
   HDC hdc ; 

   int i, j, temp, ctn; 
   unsigned int ID_TIMER=1000; 
   static HDC hdcMem; 
   HFONT hFont; 
   static HBITMAP hBitmap; 
   static int cxScreen, cyScreen;
   static int iFontWidth=10, iFontHeight=15, iColumnCount;
   static CharColumn *ccChain; 
   
   switch (message) 
   { 
      case WM_CREATE: 
           cxScreen = GetSystemMetrics(SM_CXSCREEN) ;     //显示字符雨的窗口范围  x
           cyScreen = GetSystemMetrics(SM_CYSCREEN) ;     //                      y
           
		   SetTimer (hwnd, ID_TIMER, 10, NULL) ; 

           hdc = GetDC(hwnd); 
           hdcMem = CreateCompatibleDC(hdc);     //创建一个与指定设备兼容的内存设备上下文环境（DC）。
           hBitmap = CreateCompatibleBitmap(hdc, cxScreen, cyScreen); 
           SelectObject(hdcMem, hBitmap); 
           ReleaseDC(hwnd, hdc);
          

           hFont = CreateFont(
			                  iFontHeight,      //新字体的高 
			                  
							  iFontWidth-5,     //        宽
							  0, 
							  0, 
							  FW_BOLD,          //笔画粗细，0-1000,400为标准，700为粗体
							  0,                //TRUE 是 斜体
							  0,                //TRUE 是 下划线
							  0,                //TRUE 是 删除线
                              DEFAULT_CHARSET,  //字符集
							  OUT_DEFAULT_PRECIS, 
							  
							  CLIP_DEFAULT_PRECIS, 
                              DRAFT_QUALITY, 
							  FIXED_PITCH | FF_SWISS, 
							  TEXT("Fixedsys")
							 ); 
           SelectObject(hdcMem, hFont); 
           DeleteObject (hFont) ;              //删除字体
		   SetBkMode(hdcMem, TRANSPARENT);     //OPAQUE  为破坏背景色输出
		                                       //TRANSPARENT 为透明输出，即输出后不破坏背景
           iColumnCount = cxScreen/(iFontWidth*3/2); 

           ccChain = (pCharColumn)calloc(iColumnCount, sizeof(CharColumn)); 
           for(i=0; i<iColumnCount; i++) 
		   { 
               init(ccChain+i, cyScreen, (iFontWidth*3/2)*i); 
		   } 
           return 0 ; 
     
      case WM_TIMER: 
           hdc = GetDC(hwnd); 
           PatBlt (hdcMem, 0, 0, cxScreen, cyScreen, BLACKNESS) ; //显示字符雨背景颜色  BLACKNESS，WHITENESS
           for(i=0; i<iColumnCount; i++) 
		   {  
               ctn = (ccChain+i)->iStopTimes++ > (ccChain+i)->iMustStopTimes; 

               (ccChain+i)->point = (ccChain+i)->head; 


               SetTextColor(hdcMem, RGB(255, 255, 255));                                       //设置字符雨头部的字符颜色
                                                                                               //RGB(255,255,255)为白色
			   TextOut(hdcMem, (ccChain+i)->x, (ccChain+i)->y, &((ccChain+i)->point->ch), 1);  //显示字符雨头部的字符 
               j = (ccChain+i)->y; 
               (ccChain+i)->point = (ccChain+i)->point->next; 

               temp = 0 ; 
               while((ccChain+i)->point != (ccChain+i)->head && (ccChain+i)->point->ch) 
			   { 
                   SetTextColor(hdcMem, RGB(0,0,0));  
                   SetTextColor(hdcMem, RGB(0, 255-(255*(temp++)/(ccChain+i)->iStrLen), 0));     //以互补色消除字符
                   TextOut(hdcMem, (ccChain+i)->x, j-=iFontHeight, &((ccChain+i)->point->ch), 1); //显示字符雨身体部分
                   (ccChain+i)->point = (ccChain+i)->point->next; 
			   } 
               if(ctn) 
                   (ccChain+i)->iStopTimes = 0 ; 
               else continue; 
                   (ccChain+i)->y += iFontHeight; 

               if( (ccChain+i)->y-(ccChain+i)->iStrLen*iFontHeight > cyScreen) 
			   { 
                   free( (ccChain+i)->current ); 
                   init(ccChain+i, cyScreen, (iFontWidth*3/2)*i); 
			   } 

               (ccChain+i)->head = (ccChain+i)->head->prev; 
               (ccChain+i)->head->ch = randomChar(); 
		   } 

           BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY); 
           ReleaseDC(hwnd, hdc); 
           return 0; 
     
      case WM_RBUTTONDOWN:             //鼠标右键按下，bios中断 
           KillTimer (hwnd, ID_TIMER) ; 
           return 0; 
       
      case WM_RBUTTONUP:                 //鼠标右键抬起，bios中断结束
           SetTimer (hwnd, ID_TIMER, 10, NULL) ; 
           return 0; 
       

      //case WM_KEYDOWN: 
      case WM_LBUTTONDOWN: 
      case WM_DESTROY:                  //销毁窗口
           KillTimer (hwnd, ID_TIMER) ; 
           DeleteObject(hBitmap); 
           DeleteDC(hdcMem); 
           for(i=0; i<iColumnCount; i++) 
		   {  
               free( (ccChain+i)->current ); 
		   } 
           free(ccChain); 
           PostQuitMessage (0) ; 
           return 0 ; 
   }     
   return DefWindowProc (hwnd, message, wParam, lParam) ;        //缺省 
}


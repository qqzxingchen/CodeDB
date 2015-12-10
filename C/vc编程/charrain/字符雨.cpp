#include <windows.h>
#include <stdio.h>
#define STRMAXLEN 25
#define STRMINLEN 8 
char string[]="���տ���";

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ; 
////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////// 
typedef struct tagCharChain 
{ 
   struct tagCharChain *prev;   //�����ǰ��Ԫ��
 
   TCHAR ch;                     //һ����ʾ���е�һ���ַ�
   struct tagCharChain *next;    //����ĺ��Ԫ��
}CharChain, *pCharChain; 

typedef struct tagCharColumn 
{ 
   CharChain *head, *current, *point; 
   int x, y, iStrLen;                      //��ʾ�еĿ�ʼ��ʾ��x,y����,iStrLen������еĳ���
   int iStopTimes, iMustStopTimes;         //�Ѿ�ͣ�͵Ĵ����ͱ���ͣ�͵Ĵ���,����ͣ�͵Ĵ����������
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
       MessageBox (NULL, TEXT ("�˳������������NT��!"), szAppName, MB_ICONERROR) ; 
       return 0; 
   } 

   hwnd = CreateWindow (
	                    szAppName, 
	                    NULL, 
                        WS_DLGFRAME | WS_THICKFRAME | WS_POPUP, 
						0,0,                               //��ʾ���ڵ����ϵ����� 
                        GetSystemMetrics(SM_CXSCREEN),        //��ʾ���ڵ����µ�����    
						GetSystemMetrics(SM_CYSCREEN), 
                   
						NULL, 
						NULL, 
						hInstance, 
                        NULL
					  ) ; 

   ShowWindow (hwnd,SW_SHOWMAXIMIZED) ;    //�����ʾ 
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
    return (TCHAR)(rand()%(126-33)+33); //ascii����У������ʾ33��126���ַ�����Щ�ַ��ǿɼ��ģ�
} 

int init(CharColumn *cc, int cyScreen, int x)  
{ 
   int j; 
   cc->iStrLen = rand()%(STRMAXLEN-STRMINLEN) + STRMINLEN; 
   //cc->x = x+100;
   cc->x = x+3 ;
   
   cc->y =rand()%3?rand()%cyScreen:0;   //ÿ���ַ������ 
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
           cxScreen = GetSystemMetrics(SM_CXSCREEN) ;     //��ʾ�ַ���Ĵ��ڷ�Χ  x
           cyScreen = GetSystemMetrics(SM_CYSCREEN) ;     //                      y
           
		   SetTimer (hwnd, ID_TIMER, 10, NULL) ; 

           hdc = GetDC(hwnd); 
           hdcMem = CreateCompatibleDC(hdc);     //����һ����ָ���豸���ݵ��ڴ��豸�����Ļ�����DC����
           hBitmap = CreateCompatibleBitmap(hdc, cxScreen, cyScreen); 
           SelectObject(hdcMem, hBitmap); 
           ReleaseDC(hwnd, hdc);
          

           hFont = CreateFont(
			                  iFontHeight,      //������ĸ� 
			                  
							  iFontWidth-5,     //        ��
							  0, 
							  0, 
							  FW_BOLD,          //�ʻ���ϸ��0-1000,400Ϊ��׼��700Ϊ����
							  0,                //TRUE �� б��
							  0,                //TRUE �� �»���
							  0,                //TRUE �� ɾ����
                              DEFAULT_CHARSET,  //�ַ���
							  OUT_DEFAULT_PRECIS, 
							  
							  CLIP_DEFAULT_PRECIS, 
                              DRAFT_QUALITY, 
							  FIXED_PITCH | FF_SWISS, 
							  TEXT("Fixedsys")
							 ); 
           SelectObject(hdcMem, hFont); 
           DeleteObject (hFont) ;              //ɾ������
		   SetBkMode(hdcMem, TRANSPARENT);     //OPAQUE  Ϊ�ƻ�����ɫ���
		                                       //TRANSPARENT Ϊ͸���������������ƻ�����
           iColumnCount = cxScreen/(iFontWidth*3/2); 

           ccChain = (pCharColumn)calloc(iColumnCount, sizeof(CharColumn)); 
           for(i=0; i<iColumnCount; i++) 
		   { 
               init(ccChain+i, cyScreen, (iFontWidth*3/2)*i); 
		   } 
           return 0 ; 
     
      case WM_TIMER: 
           hdc = GetDC(hwnd); 
           PatBlt (hdcMem, 0, 0, cxScreen, cyScreen, BLACKNESS) ; //��ʾ�ַ��걳����ɫ  BLACKNESS��WHITENESS
           for(i=0; i<iColumnCount; i++) 
		   {  
               ctn = (ccChain+i)->iStopTimes++ > (ccChain+i)->iMustStopTimes; 

               (ccChain+i)->point = (ccChain+i)->head; 


               SetTextColor(hdcMem, RGB(255, 255, 255));                                       //�����ַ���ͷ�����ַ���ɫ
                                                                                               //RGB(255,255,255)Ϊ��ɫ
			   TextOut(hdcMem, (ccChain+i)->x, (ccChain+i)->y, &((ccChain+i)->point->ch), 1);  //��ʾ�ַ���ͷ�����ַ� 
               j = (ccChain+i)->y; 
               (ccChain+i)->point = (ccChain+i)->point->next; 

               temp = 0 ; 
               while((ccChain+i)->point != (ccChain+i)->head && (ccChain+i)->point->ch) 
			   { 
                   SetTextColor(hdcMem, RGB(0,0,0));  
                   SetTextColor(hdcMem, RGB(0, 255-(255*(temp++)/(ccChain+i)->iStrLen), 0));     //�Ի���ɫ�����ַ�
                   TextOut(hdcMem, (ccChain+i)->x, j-=iFontHeight, &((ccChain+i)->point->ch), 1); //��ʾ�ַ������岿��
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
     
      case WM_RBUTTONDOWN:             //����Ҽ����£�bios�ж� 
           KillTimer (hwnd, ID_TIMER) ; 
           return 0; 
       
      case WM_RBUTTONUP:                 //����Ҽ�̧��bios�жϽ���
           SetTimer (hwnd, ID_TIMER, 10, NULL) ; 
           return 0; 
       

      //case WM_KEYDOWN: 
      case WM_LBUTTONDOWN: 
      case WM_DESTROY:                  //���ٴ���
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
   return DefWindowProc (hwnd, message, wParam, lParam) ;        //ȱʡ 
}


#include <windows.h>
#include <stdio.h>

LPCTSTR str;
LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 

FILE *fp;
FILE *fptemp;
char read_buf[1000];
SCROLLINFO scrollinfo;
int windowx;
int windowy;
char int_to_X_retu[18];

void int_to_X(unsigned int temp,int n){
	fprintf(fptemp,"%X\n",temp);
	int i;
	int j;
	for(i=0;i<=17;i++)
		int_to_X_retu[i]='\0';
	i=n-1;
	while(i>=0){
		j=temp%(0x10);
		if(j>=0 && j<=9)
			int_to_X_retu[i]=j+'0';
		if(j>=10 && j<=15)
			int_to_X_retu[i]=j+'A'-10;
		temp=temp/(0x10);
		i--;
	}
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, 
					HINSTANCE hPrevInstance,
					LPTSTR lpCmdLine, 
					int nCmdShow)
{   
	MSG msg;
	HWND hWnd; 
	WNDCLASS wc;

	str=GetCommandLine();

	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=(WNDPROC)WndProc;       
	wc.cbClsExtra=0;                      
	wc.cbWndExtra=0;                      
	wc.hInstance=hInstance;              
	wc.hIcon=LoadIcon(NULL, IDI_APPLICATION); 
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName=lpszAppName;              
	wc.lpszClassName=lpszAppName;
	
	if(!RegisterClass(&wc))
		return(FALSE);
		hWnd=CreateWindow(lpszAppName,  
						lpszTitle,
						WS_OVERLAPPEDWINDOW | WS_VSCROLL, 
						CW_USEDEFAULT,
						0, 
						CW_USEDEFAULT, 
						0,  
						NULL, 
						NULL,  
						hInstance, 
						NULL);               
	if (!hWnd) 
		return(FALSE);
	fp=fopen("main.dll","rb");
	fread(read_buf,sizeof(read_buf),1,fp);
	fptemp=fopen("out.txt","w");

	ShowWindow(hWnd, nCmdShow); 
	UpdateWindow(hWnd);         

	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg); 
		DispatchMessage(&msg);  
	}
	fclose(fp);
	fclose(fptemp);
	return(msg.wParam); 
}


LRESULT CALLBACK WndProc(HWND hWnd, 
						 UINT uMsg, 
						WPARAM wParam,
						LPARAM lParam)
{
	RECT rect;
	HDC hdc;
	PAINTSTRUCT ps;
	static int uRed=0, uGreen=0, uBlue=0;

	hdc=GetDC(hWnd);
	switch(uMsg){   
		case WM_SIZE:
			windowx=LOWORD(lParam);
			windowy=HIWORD(lParam);

			scrollinfo.cbSize=sizeof(SCROLLINFO);
			scrollinfo.fMask=SIF_PAGE|SIF_POS|SIF_RANGE;
			scrollinfo.nMin=0;
			scrollinfo.nMax=100-1;
			scrollinfo.nPos=0;
			scrollinfo.nPage=10;
			SetScrollInfo(hWnd,SB_VERT,&scrollinfo,TRUE);
			SendMessage(hWnd,WM_VSCROLL,NULL,NULL);
			break;
		case WM_VSCROLL:
			int nowpos;
			scrollinfo.cbSize=sizeof(SCROLLINFO);
			scrollinfo.fMask=SIF_ALL;
			GetScrollInfo(hWnd,SB_VERT,&scrollinfo);
			nowpos=scrollinfo.nPos;
			switch(LOWORD(wParam)){
			case SB_TOP:                    // 滚动条处于最上端
				scrollinfo.nPos=scrollinfo.nMin;
				break;
			case SB_BOTTOM:                 // 滚动条处于最下端
				scrollinfo.nPos = scrollinfo.nMax;
				break;
			case SB_LINEUP:                 // 向上滚动一行
				scrollinfo.nPos -= 1;		
				break;	
			case SB_LINEDOWN:                // 向下滚动一行		
				scrollinfo.nPos +=1;
				break;	
			case SB_PAGEUP:                 // 向上滚动一页		
				scrollinfo.nPos -=scrollinfo.nPage;		
				break;	
			case SB_PAGEDOWN:               // 向下滚动一页	
				scrollinfo.nPos +=scrollinfo.nPage;		
				break;	
			case SB_THUMBTRACK:              // 拖曳滚动框	
				scrollinfo.nPos=scrollinfo.nTrackPos;		
				break;	
			default:
				break;		
			}
			if(scrollinfo.nPos>scrollinfo.nMax)   // 确保滚动条位于滚动范围内
				scrollinfo.nPos=scrollinfo.nMax;
			if(scrollinfo.nPos<scrollinfo.nMin)
				scrollinfo.nPos=scrollinfo.nMin;
			scrollinfo.fMask=SIF_POS;
			SetScrollInfo(hWnd, SB_VERT, &scrollinfo, TRUE);
			GetScrollInfo(hWnd, SB_VERT, &scrollinfo);
			ScrollWindowEx(hWnd, 0, windowy*(nowpos-scrollinfo.nPos)/scrollinfo.nPage,
					NULL, NULL, NULL, NULL, SW_INVALIDATE|SW_ERASE );
            InvalidateRect(hWnd, NULL, FALSE);
			break;
		case WM_CREATE:
			break;
		case WM_PAINT :
			int i;
			char temp[100];

			hdc=BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rect);
			SetTextColor(hdc, RGB(uRed, uGreen, uBlue));
			DrawText (hdc, str, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;		

			for(i=scrollinfo.nPos;i<(int)scrollinfo.nPage+(int)scrollinfo.nPos;i++){
				sprintf(temp,"now is %s\0",(int_to_X((unsigned int)read_buf[i],2),int_to_X_retu));
				rect.top+=15;
				DrawText(hdc,temp,-1,&rect,DT_CENTER | DT_SINGLELINE);
			}

			EndPaint(hWnd, &ps);
			break;
		case WM_RBUTTONDOWN:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
		case WM_LBUTTONDOWN:
			uRed=rand() % 255;
			uGreen=rand() % 255;
			uBlue=rand() % 255;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case WM_DESTROY :
			PostQuitMessage(0);
			break;
		default :
			return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}



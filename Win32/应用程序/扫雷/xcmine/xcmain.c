#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "xc.h"
#include "resource.h"

extern HBITMAP hBitmap;
extern HDC hBitmapDC;

int APIENTRY WinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance,
					 LPTSTR lpCmdLine, 
					 int nCmdShow)
{   
	MSG msg;
	HWND hWnd; 
	WNDCLASS wc;
	
	hBitmap=LoadImage(hInstance,MAKEINTRESOURCE(IDB_BITMAP1),IMAGE_BITMAP,16,256,LR_DEFAULTCOLOR);

	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=(WNDPROC)WndProc;       
	wc.cbClsExtra=0;                      
	wc.cbWndExtra=0;                      
	wc.hInstance=hInstance;              
	wc.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName=lpszAppName;              
	wc.lpszClassName=lpszAppName;
	
	if(!RegisterClass(&wc))
		return(FALSE);
	hWnd=CreateWindow(lpszAppName,  
		lpszTitle,
		WS_OVERLAPPEDWINDOW, 
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
	ShowWindow(hWnd, nCmdShow); 
	UpdateWindow(hWnd);         
	
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg); 
		DispatchMessage(&msg);  
	}
	
	return(msg.wParam); 
}


LRESULT CALLBACK WndProc(HWND hWnd, 
						 UINT uMsg, 
						 WPARAM wParam,
						 LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static XC_mine mine;
	RECT rect;
	SYSTEMTIME st;

	int i,j;
	char temp[1000];
	int tempnum,tempx,tempy;

	switch(uMsg){
	case XC_CreateCompatibleWindow:
		
		mine.length=16;
		mine.xnum=HIBYTE(wParam);
		mine.ynum=LOBYTE(wParam);
		mine.minenum=(int)lParam;
		MoveWindow(hWnd,10,10,200,200,FALSE);
		GetClientRect(hWnd,&rect);	

		MoveWindow(hWnd,100,100,
			200-(rect.right-rect.left)+mine.length*mine.xnum,
			200-(rect.bottom-rect.top)+mine.length*mine.ynum,
			TRUE
		);
		
		for(i=0;i<=mine.xnum+1;i++){
			for(j=0;j<=mine.ynum+1;j++){
				mine.s[i][j]=0;
				mine.sign[i][j]=0;
			}
		}
		
		tempnum=mine.minenum;
		while(tempnum){
			while(1){
				tempx=rand()%mine.xnum;
				tempy=rand()%mine.ynum;
				if(mine.s[tempx+1][tempy+1]!=-1){
					mine.s[tempx+1][tempy+1]=-1;
					break;
				}
			}
			tempnum--;
		}
		
		for(i=1;i<=mine.xnum;i++){
			for(j=1;j<=mine.ynum;j++){
				if(mine.s[i][j]==0){
					if(mine.s[i+1][j+1]==-1) mine.s[i][j]-=mine.s[i+1][j+1];
					if(mine.s[i+1][j  ]==-1) mine.s[i][j]-=mine.s[i+1][j  ];
					if(mine.s[i+1][j-1]==-1) mine.s[i][j]-=mine.s[i+1][j-1];
					if(mine.s[i  ][j+1]==-1) mine.s[i][j]-=mine.s[i  ][j+1];
					if(mine.s[i  ][j-1]==-1) mine.s[i][j]-=mine.s[i  ][j-1];
					if(mine.s[i-1][j+1]==-1) mine.s[i][j]-=mine.s[i-1][j+1];
					if(mine.s[i-1][j  ]==-1) mine.s[i][j]-=mine.s[i-1][j  ];
					if(mine.s[i-1][j-1]==-1) mine.s[i][j]-=mine.s[i-1][j-1];
				}
			}
		}
		InvalidateRect(hWnd,NULL,TRUE);
		break;

	case WM_CREATE:
		hdc=GetDC(hWnd);
		hBitmapDC=CreateCompatibleDC(hdc);
		SelectObject(hBitmapDC,hBitmap);

		GetSystemTime(&st);
		srand((int)(st.wSecond*st.wMilliseconds));
		SendMessage(hWnd,XC_CreateCompatibleWindow,(WPARAM)MAKEWORD(40,70),(LPARAM)1000);
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		
		for(i=1;i<=mine.xnum;i++){
			for(j=1;j<=mine.ynum;j++){
				if(mine.sign[i][j]==0){
					BitBlt(hdc,
						(i-1)*mine.length,
						(j-1)*mine.length,
						mine.length,
						mine.length,
						hBitmapDC,
						0,
						mine.length*0,
						SRCCOPY
					);
				}else if(mine.sign[i][j]==1){
					if(mine.s[i][j]==0){
						BitBlt(hdc,
							(i-1)*mine.length,
							(j-1)*mine.length,
							mine.length,
							mine.length,
							hBitmapDC,
							0,
							mine.length*15,
							SRCCOPY
						);
					}else if(mine.s[i][j]>=1 && mine.s[i][j]<=8){
						BitBlt(hdc,
							(i-1)*mine.length,
							(j-1)*mine.length,
							mine.length,
							mine.length,
							hBitmapDC,
							0,
							mine.length*(15-mine.s[i][j]),
							SRCCOPY
						);
					} 
				}else if(mine.sign[i][j]==2){
					BitBlt(hdc,
						(i-1)*mine.length,
						(j-1)*mine.length,
						mine.length,
						mine.length,
						hBitmapDC,
						0,
						mine.length*1,
						SRCCOPY
					);
				}
			}
		}



		EndPaint(hWnd,&ps);
		break;
	case WM_LBUTTONDOWN:
		tempx=LOWORD(lParam);
		tempy=HIWORD(lParam);
		if(tempx%mine.length!=0 && tempy%mine.length!=0){
			tempx=tempx/mine.length+1;
			tempy=tempy/mine.length+1;
		}else{
			break;
		}

		hdc=GetDC(hWnd);
		if(mine.sign[tempx][tempy]==0){
			mine.sign[tempx][tempy]=1;
			if(mine.s[tempx][tempy]==-1){
				for(i=1;i<=mine.xnum;i++){
					for(j=1;j<=mine.ynum;j++){
						if(mine.s[i][j]==-1){
							BitBlt(hdc,
								(i-1)*mine.length,
								(j-1)*mine.length,
								mine.length,
								mine.length,
								hBitmapDC,
								0,
								mine.length*4,
								SRCCOPY
							);
						}
					}
				}
				if(IDYES==MessageBox(hWnd,"您要再来一局么???","你输了!!!",MB_YESNO)){
					PostMessage(hWnd,XC_CreateCompatibleWindow,(WPARAM)MAKEWORD(30,50),(LPARAM)100);
				}else{
					PostQuitMessage(0);
				}
			}else if(mine.s[tempx][tempy]>=1 && mine.s[tempx][tempy]<=8){
				BitBlt(hdc,
					(tempx-1)*mine.length,
					(tempy-1)*mine.length,
					mine.length,
					mine.length,
					hBitmapDC,
					0,
					mine.length*(15-mine.s[tempx][tempy]),
					SRCCOPY
				);
				findzero(&mine,hdc,tempx,tempy);
			}else if(mine.s[tempx][tempy]==0){
				BitBlt(hdc,
					(tempx-1)*mine.length,
					(tempy-1)*mine.length,
					mine.length,
					mine.length,
					hBitmapDC,
					0,
					mine.length*15,
					SRCCOPY
				);
				findzero(&mine,hdc,tempx,tempy);
			}
		}

		tempnum=0;
		for(i=1;i<=mine.xnum;i++){
			for(j=1;j<=mine.ynum;j++){
				if(mine.sign[i][j]!=1){
					tempnum++;
				}
			}
		}
		if(tempnum==mine.minenum){
			if(IDYES==MessageBox(hWnd,"您要再来一局么???","你赢了!!!",MB_YESNO)){
				PostMessage(hWnd,XC_CreateCompatibleWindow,(WPARAM)MAKEWORD(30,50),(LPARAM)100);
			}else{
				PostQuitMessage(0);
			}
		}

		break;
	case WM_RBUTTONDOWN:
		tempx=LOWORD(lParam);
		tempy=HIWORD(lParam);
		if(tempx%mine.length!=0 && tempy%mine.length!=0){
			tempx=tempx/mine.length+1;
			tempy=tempy/mine.length+1;
		}else{
			break;
		}
		if(mine.sign[tempx][tempy]==2){
			mine.sign[tempx][tempy]=0;
			hdc=GetDC(hWnd);
			BitBlt(hdc,
				(tempx-1)*mine.length,
				(tempy-1)*mine.length,
				mine.length,
				mine.length,
				hBitmapDC,
				0,
				mine.length*0,
				SRCCOPY
			);
		}else if(mine.sign[tempx][tempy]==0){
			mine.sign[tempx][tempy]=2;
			hdc=GetDC(hWnd);
			BitBlt(hdc,
				(tempx-1)*mine.length,
				(tempy-1)*mine.length,
				mine.length,
				mine.length,
				hBitmapDC,
				0,
				mine.length*1,
				SRCCOPY
			);
		}

		break;
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	default :
		return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}

int findzero(XC_mine *mine,HDC hdc,int startx,int starty){
	POINT point[10000];
	int sign[100][100];
	int pointnumstart=0;
	int pointnumend=0;
	int i,j;
	int x,y;

	for(i=1;i<=mine->xnum;i++)
		for(j=1;j<=mine->ynum;j++)
			sign[i][j]=0;

	if(mine->s[startx][starty]==0){
		point[0].x=startx;
		point[0].y=starty;
		pointnumend=1;
		sign[startx][starty]=1;
	}


	for(i=pointnumstart;i<pointnumend;i++){
		x=point[i].x;
		y=point[i].y;
		
		if(x-1>=1 && mine->s[x-1][y]!=-1 && sign[x-1][y]==0){
			mine->sign[x-1][y]=1;
			sign[x-1][y]=1;
			BitBlt(hdc,
				(x-2)*mine->length,
				(y-1)*mine->length,
				mine->length,
				mine->length,
				hBitmapDC,
				0,
				mine->length*(15-mine->s[x-1][y]),
				SRCCOPY
			);
			if(mine->s[x-1][y]==0){
				point[pointnumend].x=x-1;
				point[pointnumend].y=y;
				pointnumend++;
			}
		}
		if(x+1<=mine->xnum && mine->s[x+1][y]!=-1 && sign[x+1][y]==0){
			mine->sign[x+1][y]=1;
			sign[x+1][y]=1;
			BitBlt(hdc,
				(x)*mine->length,
				(y-1)*mine->length,
				mine->length,
				mine->length,
				hBitmapDC,
				0,
				mine->length*(15-mine->s[x+1][y]),
				SRCCOPY
				);
			if(mine->s[x+1][y]==0){
				point[pointnumend].x=x+1;
				point[pointnumend].y=y;
				pointnumend++;
			}
		}			
		if(y-1>=1 && mine->s[x][y-1]!=-1 && sign[x][y-1]==0){
			mine->sign[x][y-1]=1;
			sign[x][y-1]=1;
			BitBlt(hdc,
				(x-1)*mine->length,
				(y-2)*mine->length,
				mine->length,
				mine->length,
				hBitmapDC,
				0,
				mine->length*(15-mine->s[x][y-1]),
				SRCCOPY
			);
			if(mine->s[x][y-1]==0){
				point[pointnumend].x=x;
				point[pointnumend].y=y-1;
				pointnumend++;
			}
		}			
		if(y+1<=mine->ynum && mine->s[x][y+1]!=-1 && sign[x][y+1]==0){
			mine->sign[x][y+1]=1;
			sign[x][y+1]=1;
			BitBlt(hdc,
				(x-1)*mine->length,
				(y)*mine->length,
				mine->length,
				mine->length,
				hBitmapDC,
				0,
				mine->length*(15-mine->s[x][y+1]),
				SRCCOPY
			);
			if(mine->s[x][y+1]==0){
				point[pointnumend].x=x;
				point[pointnumend].y=y+1;
				pointnumend++;
			}
		}
	}

	return 0;
}

//#define WINVER 0x0500
#include <windows.h>
#include <winbase.h>

#define START 400
#define ID_TIMER 10001

LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, 
					HINSTANCE hPrevInstance,
					LPTSTR lpCmdLine, 
					int nCmdShow)
{   
	MSG msg;
	HWND hWnd; 
	WNDCLASS wc;

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
	RECT rect;
	HDC hdc;
	PAINTSTRUCT ps;
	static SYSTEM_INFO si;
	char str[100]={"\0"};
	MEMORYSTATUS ms;


	switch(uMsg){   
		case WM_CREATE:
			SetTimer(hWnd,ID_TIMER,1000,NULL);
			GetSystemInfo(&si);
			break;
		case WM_TIMER:
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case WM_PAINT:
			int i;
			hdc=BeginPaint(hWnd, &ps);

			if(*(WORD*)&si==PROCESSOR_ARCHITECTURE_UNKNOWN)
				wsprintf(str,"unknow");
			else if(*(WORD*)&si==PROCESSOR_ARCHITECTURE_INTEL)
				wsprintf(str,"Windows 95/98/Me");
			else if(*(WORD*)&si==PROCESSOR_ARCHITECTURE_MIPS)
				wsprintf(str,"Windows NT 3.51");
			else if(*(WORD*)&si==PROCESSOR_ARCHITECTURE_ALPHA || 
				*(WORD*)&si==PROCESSOR_ARCHITECTURE_PPC)
				wsprintf(str,"Windows NT 4.0 and earlier");
			else if(*(WORD*)&si==PROCESSOR_ARCHITECTURE_IA64  )
				wsprintf(str,"64-bit Windows");
			TextOut(hdc,0,0,TEXT("processor architecture:"),strlen(TEXT("processor architecture:")));
			TextOut(hdc,START,0,str,strlen(str));


			wsprintf(str,"0x%X(used for VirtualAlloc function)",si.dwPageSize);
			TextOut(hdc,0,16,TEXT("Page Size:"),strlen(TEXT("Page Size:")));
			TextOut(hdc,START,16,str,strlen(str));

			wsprintf(str,"0x%X",si.lpMinimumApplicationAddress);
			TextOut(hdc,0,32,TEXT("Lowest Memory Address:"),strlen(TEXT("Lowest Memory Address:")));
			TextOut(hdc,START,32,str,strlen(str));

			wsprintf(str,"0x%X",si.lpMaximumApplicationAddress);
			TextOut(hdc,0,48,TEXT("Highest Memory Address:"),strlen(TEXT("Highest Memory Address:")));
			TextOut(hdc,START,48,str,strlen(str));
			
			wsprintf(str,"%d",si.dwNumberOfProcessors);
			TextOut(hdc,0,64,TEXT("Processor Number:"),strlen(TEXT("Processor Number:")));
			TextOut(hdc,START,64,str,strlen(str));
			
			wsprintf(str,"%X",si.dwActiveProcessorMask);
			TextOut(hdc,0,5*16,TEXT("Active Process:"),strlen(TEXT("Active Process:")));
			TextOut(hdc,START,5*16,str,strlen(str));
			
			if(si.dwProcessorType==PROCESSOR_INTEL_386)
				wsprintf(str,"Intel_386");
			else if(si.dwProcessorType==PROCESSOR_INTEL_486)
				wsprintf(str,"Intel_486");
			else if(si.dwProcessorType==PROCESSOR_INTEL_PENTIUM)
				wsprintf(str,"Intel_pentium");
			TextOut(hdc,0,6*16,TEXT("Process Type:"),strlen(TEXT("Process Type:")));
			TextOut(hdc,START,6*16,str,strlen(str));
			
			wsprintf(str,"0x%X",si.dwAllocationGranularity);	
			TextOut(hdc,0,7*16,TEXT("Allocation Granularity:"),strlen(TEXT("Allocation Granularity:")));
			TextOut(hdc,START,7*16,str,strlen(str));
			
			if(*(WORD*)&si==PROCESSOR_ARCHITECTURE_INTEL){
				if(si.wProcessorLevel==3)
					wsprintf(str,"Intel 80386");
				else if(si.wProcessorLevel==4)
					wsprintf(str,"Intel 80486");
				else if(si.wProcessorLevel==5)
					wsprintf(str,"Intel Pentium");
				else if(si.wProcessorLevel==6)
					wsprintf(str,"Intel Pentium Pro or Pentium II");
			}else if(*(WORD*)&si==PROCESSOR_ARCHITECTURE_MIPS){
				if(si.wProcessorLevel==4)
					wsprintf(str,"MIPS R4000");
			}else if(*(WORD*)&si==PROCESSOR_ARCHITECTURE_ALPHA){
				wsprintf(str,"Alpha %d",si.wProcessorLevel);
			}else if(*(WORD*)&si==PROCESSOR_ARCHITECTURE_PPC){
				if(si.wProcessorLevel==6 || si.wProcessorLevel==9){
					wsprintf(str,"PPC 60%d+",si.wProcessorLevel==6?3:4);
				}else{
					wsprintf(str,"PPC 60%d",si.wProcessorLevel);
				}
			}
			TextOut(hdc,0,8*16,TEXT("Process Level:"),strlen(TEXT("Process Level:")));
			TextOut(hdc,START,8*16,str,strlen(str));

			GlobalMemoryStatus(&ms);

			wsprintf(str,"%d percents",ms.dwMemoryLoad);
			TextOut(hdc,0,11*16,TEXT("Memory Load:"),strlen(TEXT("Memory Load:")));
			TextOut(hdc,START,11*16,str,strlen(str));
			
			wsprintf(str,"0x%X bytes",ms.dwTotalPhys);
			TextOut(hdc,0,12*16,TEXT("Total size of physical memory:"),strlen(TEXT("Total size of physical memory:")));
			TextOut(hdc,START,12*16,str,strlen(str));

			wsprintf(str,"0x%X bytes",ms.dwAvailPhys);
			TextOut(hdc,0,13*16,TEXT("Avail size of physical memory:"),strlen(TEXT("Avail size of physical memory:")));
			TextOut(hdc,START,13*16,str,strlen(str));

			wsprintf(str,"0x%X bytes",ms.dwTotalPageFile);
			TextOut(hdc,0,14*16,TEXT("Total size of committed memory limit:"),strlen(TEXT("Total size of committed memory limit:")));
			TextOut(hdc,START,14*16,str,strlen(str));
			
			wsprintf(str,"0x%X bytes",ms.dwAvailPageFile);
			TextOut(hdc,0,15*16,TEXT("Avail size of committed memory limit:"),strlen(TEXT("Avail size of committed memory limit:")));
			TextOut(hdc,START,15*16,str,strlen(str));

			wsprintf(str,"0x%X bytes",ms.dwTotalVirtual);
			TextOut(hdc,0,16*16,TEXT("Total size of virtual address space:"),strlen(TEXT("Total size of virtual address space:")));
			TextOut(hdc,START,16*16,str,strlen(str));
			
			wsprintf(str,"0x%X bytes",ms.dwAvailVirtual);
			TextOut(hdc,0,17*16,TEXT("Avail size of virtual address space:"),strlen(TEXT("Avail size of virtual address space:")));
			TextOut(hdc,START,17*16,str,strlen(str));


			
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY :
			PostQuitMessage(0);
			break;
		default :
			return  DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0;
}

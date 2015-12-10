// hello world.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include<windows.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	int i;
	char n[2]="0";
	n[0]+=65;
 	for(i=0;i<=10;i++)
	{
		n[0]++;
		MessageBox(NULL,TEXT(n),TEXT(""),1);
	}
	return 0;
}




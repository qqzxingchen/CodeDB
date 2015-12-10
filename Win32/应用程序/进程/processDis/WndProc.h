#ifndef WndProc__H
#define WndProc__H

	#include <windows.h>
	#include <commctrl.h>
	#include <Tlhelp32.h>
	#include <stdio.h>

	#define ProcessControlTimer 10051
	#define IDC_XC_LISTBOX 1001
	#define IDC_XC_STATIC 1002
	#define IDC_XC_LISTBOX2 1003
	extern HWND hListBox;
	extern HWND hStatic;
	extern HWND hListBox2;

	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
#endif
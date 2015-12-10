#ifndef _xingchen_xingchen_ 0x153714
#define _xingchen_xingchen_ 0x153714

#define WS_EX_LAYERED 0x00080000
#define LWA_ALPHA     0x00000002
#define LWA_COLORKEY  0x00000001
typedef BOOL ( WINAPI *LPFNSETLAYEREDWINDOWATTRIBUTES)
                  (   HWND     hwnd,
                      COLORREF crKey,
                      BYTE     bAlpha,
                      DWORD    dwFlags) ;
BOOL TransparentWnd(HWND hwnd,COLORREF crkey,BYTE byAlpha,DWORD dwflags)
{
	long lStyle;
	HMODULE hModule;
	LPFNSETLAYEREDWINDOWATTRIBUTES lpSetLayeredWindowAttributes;
	hModule = (HMODULE)LoadLibrary("user32.dll") ;
	lpSetLayeredWindowAttributes = (LPFNSETLAYEREDWINDOWATTRIBUTES)
			GetProcAddress(hModule, TEXT("SetLayeredWindowAttributes")) ;
	
	lStyle = GetWindowLong(hwnd, GWL_EXSTYLE) ;
    lStyle = lStyle | WS_EX_LAYERED ;
	SetWindowLong(hwnd, GWL_EXSTYLE, lStyle) ;
	lpSetLayeredWindowAttributes(hwnd,crkey,byAlpha,dwflags) ;
	RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN) ;
	FreeLibrary(hModule) ;
	return TRUE ;
}
#endif

#define XC_Dialog_width  218
#define XC_Dialog_height 144

LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);


//ErroorCode [in]	为 GetLastError() 得到的错误代码
//explian    [out]	为 中文解释
//lang       [int]  0为中文，1为英文
//返回值：若该错误代码被定义，则返回TRUE，否则，返回FALSE
BOOL XC_getwrongtext(int ErrorCode,char * explian,int lang);
int lang=0;
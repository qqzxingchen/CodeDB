#define XC_Dialog_width  218
#define XC_Dialog_height 144

LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);


//ErroorCode [in]	Ϊ GetLastError() �õ��Ĵ������
//explian    [out]	Ϊ ���Ľ���
//lang       [int]  0Ϊ���ģ�1ΪӢ��
//����ֵ�����ô�����뱻���壬�򷵻�TRUE�����򣬷���FALSE
BOOL XC_getwrongtext(int ErrorCode,char * explian,int lang);
int lang=0;


#define XC_CreateCompatibleWindow WM_USER+10
#define XC_DrawMap WM_USER+11
#define XC_INIT WM_USER+12


LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("ɨ��"); 


typedef struct XC_mine{
	int length;		//����ͼ��Ĵ�С���˴���16
	int xnum;		//���
	int ynum;		//�߶�
	int minenum;	//��������
	int s[100][100];//��ʾ��λ���Ƿ��е��ף���Ϊ-1����ʾ�е��ף���Ϊ��������ʾ��Χ�������׵ĸ���
	int sign[100][100];//��ʾ�õ�״̬��0��ʾδ���㿪��1��ʾ���㿪��2��ʾ���ױ�ʾ��
					//s��sign�������Ч��Χ��1--xnum��1--ynum
}XC_mine;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);

/************************************************************************/
/*��ĳ������Χ�˸�����û�е��ף��������������ĸ�����Ѱ����ͬ���͵ĵ㣬��Ҫ���� 
startx ��Ӧ������ϵ��mine.s����Ч����
                                                                     */
/************************************************************************/
int findzero(XC_mine * mine,HDC hdc,int startx,int starty);

HBITMAP hBitmap;
HDC hBitmapDC;
XC_mine mine;


#define XC_CreateCompatibleWindow WM_USER+10
#define XC_DrawMap WM_USER+11



LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 


HBITMAP hBitmap;
HDC hBitmapDC;

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

/************************************************************************/
/*��ĳ������Χ�˸�����û�е��ף��������������ĸ�����Ѱ����ͬ���͵ĵ㣬��Ҫ���� 
startx ��Ӧ������ϵ��mine.s����Ч����
                                                                     */
/************************************************************************/
int findzero(XC_mine * mine,HDC hdc,int startx,int starty);
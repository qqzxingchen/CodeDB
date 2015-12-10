

#define XC_CreateCompatibleWindow WM_USER+10
#define XC_DrawMap WM_USER+11



LPCTSTR lpszAppName=TEXT("MyApp");
LPCTSTR lpszTitle=TEXT("My Application"); 


HBITMAP hBitmap;
HDC hBitmapDC;

typedef struct XC_mine{
	int length;		//地雷图标的大小，此处是16
	int xnum;		//宽度
	int ynum;		//高度
	int minenum;	//地雷数量
	int s[100][100];//表示该位置是否有地雷，若为-1，表示有地雷，若为正数，表示周围方块中雷的个数
	int sign[100][100];//表示该点状态。0表示未被点开，1表示被点开，2表示地雷表示。
					//s和sign数组的有效范围是1--xnum，1--ynum
}XC_mine;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/************************************************************************/
/*若某个点周围八个方格都没有地雷，则在上下左右四个方向寻找相同类型的点，需要迭代 
startx 对应的坐标系是mine.s的有效坐标
                                                                     */
/************************************************************************/
int findzero(XC_mine * mine,HDC hdc,int startx,int starty);
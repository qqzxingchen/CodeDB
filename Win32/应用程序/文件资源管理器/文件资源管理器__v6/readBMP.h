#ifndef XC_readBMP__H
#define XC_readBMP__H

#include <stdio.h>

typedef struct XC_bmp{
	BITMAPFILEHEADER bmpfileheader;
	BITMAPINFOHEADER bmpinfoheader;
	RGBQUAD rgb[256];		//若使用了调色板，则这里面存储调色板的颜色
	int rgbnum;				//若使用了调色板，则这里面存储调色板的颜色的数量
	BOOL quadsign;			//标志是否使用调色板
}XC_bmp;

//该函数将bmp文件中的内容读入xc_bmp结构体中，并标志一些标志位
//FILE *fp	bmp文件的指针。当前指针位置必须指向BITMAPFILEHEADER的开始处
//返回值：若为0，表示该文件为BMP文件，且未加密
//        若为1，表示该文件不是BMP文件
//        若为2，表示该文件是BMP文件，但是已经被压缩
//        若为3，表示该bmp文件像素数不为1,4,8,24中的一种
int readBMPFile(FILE *fp,XC_bmp *xcbmp);


//该函数将bmp图像显示到窗口中
//FILE *fp			bmp文件的指针。当前指针位置必须指向图像数据字节阵列
//xstart,ystart		表示从xstart和ystart处显示图像
int disBMP(HDC hdc,FILE *fp,XC_bmp *xcbmp,int xstart,int ystart);


#endif
#ifndef XC_readBMP__H
#define XC_readBMP__H

#include <stdio.h>

typedef struct XC_bmp{
	BITMAPFILEHEADER bmpfileheader;
	BITMAPINFOHEADER bmpinfoheader;
	RGBQUAD rgb[256];		//��ʹ���˵�ɫ�壬��������洢��ɫ�����ɫ
	int rgbnum;				//��ʹ���˵�ɫ�壬��������洢��ɫ�����ɫ������
	BOOL quadsign;			//��־�Ƿ�ʹ�õ�ɫ��
}XC_bmp;

//�ú�����bmp�ļ��е����ݶ���xc_bmp�ṹ���У�����־һЩ��־λ
//FILE *fp	bmp�ļ���ָ�롣��ǰָ��λ�ñ���ָ��BITMAPFILEHEADER�Ŀ�ʼ��
//����ֵ����Ϊ0����ʾ���ļ�ΪBMP�ļ�����δ����
//        ��Ϊ1����ʾ���ļ�����BMP�ļ�
//        ��Ϊ2����ʾ���ļ���BMP�ļ��������Ѿ���ѹ��
//        ��Ϊ3����ʾ��bmp�ļ���������Ϊ1,4,8,24�е�һ��
int readBMPFile(FILE *fp,XC_bmp *xcbmp);


//�ú�����bmpͼ����ʾ��������
//FILE *fp			bmp�ļ���ָ�롣��ǰָ��λ�ñ���ָ��ͼ�������ֽ�����
//xstart,ystart		��ʾ��xstart��ystart����ʾͼ��
int disBMP(HDC hdc,FILE *fp,XC_bmp *xcbmp,int xstart,int ystart);


#endif
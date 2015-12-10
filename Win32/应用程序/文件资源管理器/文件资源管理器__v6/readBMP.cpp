#include <stdio.h>
#include <windows.h>
#include "readBMP.h"

int readBMPFile(FILE *fp,XC_bmp *xcbmp){
	WORD temp;
	int i,j;

	fread((char*)&xcbmp->bmpfileheader,2*sizeof(DWORD)+3*sizeof(WORD),1,fp);
	temp=xcbmp->bmpfileheader.bfType;

	if(strncmp((char*)&temp,"BM",2)==0){
		fread((char*)&xcbmp->bmpinfoheader,sizeof(BITMAPINFOHEADER),1,fp);

		if(xcbmp->bmpinfoheader.biCompression==0){
			if(xcbmp->bmpinfoheader.biBitCount==1 ||
			   xcbmp->bmpinfoheader.biBitCount==4 ||
			   xcbmp->bmpinfoheader.biBitCount==8){
				
				xcbmp->quadsign=TRUE;
				xcbmp->rgbnum=1<<(int)xcbmp->bmpinfoheader.biBitCount;
				for(i=0;i<xcbmp->rgbnum;i++){
					fread((char*)(&xcbmp->rgb[i]),sizeof(RGBQUAD),1,fp);			
				}
			}else if(xcbmp->bmpinfoheader.biBitCount==24){
				
			}else{
				return 3;
			}
		}else{
			return 2;
		}
	}else{
		return 1;
	}
	return 0;
}


int disBMP(HDC hdc,FILE *fp,XC_bmp *xcbmp,int xstart,int ystart){
	int i,j,k;
	char temp1,temp2,temp3;
	int  temp;
	int width=xcbmp->bmpinfoheader.biWidth;
	int height=xcbmp->bmpinfoheader.biHeight;


	if(xcbmp->bmpinfoheader.biBitCount==24){
		for(j=0;j<height;j++){
			for(i=0;i<width;i++){
				fread(&temp1,sizeof(char),1,fp);
				fread(&temp2,sizeof(char),1,fp);
				fread(&temp3,sizeof(char),1,fp);
				SetPixel(hdc,xstart+i,ystart+height-j-1,RGB(temp3,temp2,temp1));
			}

			//因为bmp格式中每次扫描一个扫描行（4字节的倍数），因此需要
			k=3*width%4;
			k=(4-k)%4;
			fseek(fp,k,1);
		}
	}else if(xcbmp->bmpinfoheader.biBitCount==1){
		for(j=0;j<height;j++){
			for(i=0;i<width;i+=8){
				fread(&temp1,sizeof(char),1,fp);
				for(k=0;k<8;k++){
					SetPixel(hdc,xstart+i,ystart+height-j-1,
						RGB(xcbmp->rgb[(temp1&(1<<k))>>k].rgbRed,
						xcbmp->rgb[(temp1&(1<<k))>>k].rgbGreen,
						xcbmp->rgb[(temp1&(1<<k))>>k].rgbBlue));
				}
			}
			k=(width/8)%4;
			k=(4-k)%4;
			fseek(fp,k,1);
		}

	}else if(xcbmp->bmpinfoheader.biBitCount==4){
		for(j=0;j<height;j++){
			for(i=0;i<width;i+=2){
				fread(&temp1,sizeof(char),1,fp);
				SetPixel(hdc,xstart+i,ystart+height-j-1,
					RGB(xcbmp->rgb[(temp1&0xF0)>>4].rgbRed,
					    xcbmp->rgb[(temp1&0xF0)>>4].rgbGreen,
						xcbmp->rgb[(temp1&0xF0)>>4].rgbBlue));
				SetPixel(hdc,xstart+i+1,ystart+height-j-1,
					RGB(xcbmp->rgb[temp1&0x0F].rgbRed,
					    xcbmp->rgb[temp1&0x0F].rgbGreen,
					    xcbmp->rgb[temp1&0x0F].rgbBlue));
			}
			k=(width/2)%4;
			k=(4-k)%4;
			fseek(fp,k,1);
		}

	}else if(xcbmp->bmpinfoheader.biBitCount==8){
		for(j=0;j<height;j++){
			for(i=0;i<width;i++){
				fread(&temp1,sizeof(char),1,fp);
				SetPixel(hdc,xstart+i,ystart+height-j-1,
					RGB(xcbmp->rgb[temp1].rgbRed,xcbmp->rgb[temp1].rgbGreen,xcbmp->rgb[temp1].rgbBlue));
			}
			k=width%4;
			k=(4-k)%4;
			fseek(fp,k,1);
		}
	}


	return 0;
}
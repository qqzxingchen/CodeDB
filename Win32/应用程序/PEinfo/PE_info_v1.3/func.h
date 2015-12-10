#include <stdio.h>
#include <string.h>
#include <direct.h>

struct XC_IMAGE_DOS_HEADER dos_header;
struct XC_IMAGE_NT_HEADERS nt_header;
struct XC_IMAGE_DATA_DIRECTORY data_dir[XC_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
struct XC_IMAGE_SECTION_HEADER section_header[1000];
struct XC_IMAGE_IMPORT_DESCRIPTOR import_descriptor[1000];
struct XC_IMAGE_IMPORT_BY_NAME import_by_name[5000];
struct XC_IMAGE_THUNK_DATA thunk_data[5000];
struct XC_IMAGE_EXPORT_DIRECTORY export_directory;
struct XC_IMAGE_BASE_RELOCATION base_relocation;
struct XC_IMAGE_DELAY_IMPORT_DESCRIPTOR delay_import;
struct XC_RESOURCE_TYPE_AND_OFFSET type_offset[10000];
struct XC_IMAGE_TLS_DIRECTORY tls_directory;
struct XC_IMAGE_POINTER_TO_TLS_CALLBACK pointer_to_tls_callback;

const char preDefine[16][16]={
	"光标\0","位图\0","图标\0","菜单\0","对话框\0",
		"字符串\0","字体目录\0","字体\0","加速键\0",
		"未格式化资源\0","消息表\0","光标组\0","\0","图标组\0","\0","版本信息\0"
								}; 
int Menu_num;
int Icon_num;
int Dialog_num;

int type_offset_num;
int import_by_name_num;
int import_descriptor_num;

void list_dos_header();
void list_nt_header();
void list_nt_header_Data_directory();
void list_section_header();
void list_import_descriptor();
void list_export_directory();
void list_base_relocation();
void list_delay_import_descriptor();
void list_resource_directory();
void list_tls();

int RVA_to_FOA(int temp);
void getrealname();
int init();
void fp_read(int n,int order);
void get_Resource();
void get_MultiByte(char *from,int from_length);
void read_Unicode_and_Change(char *name_string);


char name[100]={'\0'};
char realname[20]={'\0'};
char dirname[100]={'\0'};
char int_to_X_retu[17];
char fp_read_retu[100];
FILE *fp;
FILE *outfp;


//将name[]里面的全路径名分割：
//dirname[]里面保存目录
//realname[]里面保存文件名(无后缀)
void getrealname(){
	int i,j,k;
	char temp[100]={'\0'};
	for(i=0;;i++){
		temp[i]=name[i];
		if(name[i]=='\0')
			break;
	}
	for(j=i-1;j>=0;j--)
		if(temp[j]=='\\')
			break;
	for(k=0;k<=j;k++)
		dirname[k]=temp[k];
	if(dirname[0]=='\\')
		dirname[0]='\0';

	for(k=j+1;k<i;k++){
		realname[k-j-1]=temp[k];
	}
	for(k=i-j-2;k>=0;k--){
		if(realname[k]=='.'){
			realname[k]='\0';
			break;
		}
	}
}


//将一个整数变换格式为n位十六进制数（不够则前面补零）
//将得到的字符串放到int_to_X_retu[9]里面
void int_to_X(int temp,int n){
	
	int i;
	int j;
	for(i=0;i<=16;i++)
		int_to_X_retu[i]='\0';
	i=n-1;
	while(i>=0){
		j=temp%(0x10);
		if(j>=0 && j<=9)
			int_to_X_retu[i]=j+'0';
		if(j>=10 && j<=15)
			int_to_X_retu[i]=j+'A'-10;
		temp=temp/(0x10);
		i--;
	}
}


int RVA_to_FOA(int temp){
	int i;
	for(i=0;i<nt_header.FileHeader.NumberOfSections;i++){
		if(temp>=(int)section_header[i].VirtualAddress 
			&& (temp<(int)section_header[i].VirtualAddress+(int)section_header[i].Misc.VirtualSize)){
			return (temp-(int)section_header[i].VirtualAddress
						+(int)section_header[i].PointerToRawData);
		}
	}
	return 0;
}

//从文件读取n个字节的数据，放到fp_read_retu[]中
//若n为0，则往后读一直到 '\0'
//order为-1，则逆序存储（数据）
//order为1，则顺序存储
void fp_read(int n,int order){
	int i;
	char s;
	for(i=0;i<100;i++)
		fp_read_retu[i]='\0';
	if(n==0){
		i=0;
		do{
			fscanf(fp,"%c",&s);
			fp_read_retu[i]=s;
			i++;
		}while(s!='\0');
	}else if(order==-1){
		for(i=0;i<n;i++){
			fscanf(fp,"%c",&s);
			fp_read_retu[n-i-1]=s;
		}
	}else if(order==1){
		for(i=0;i<n;i++){
			fscanf(fp,"%c",&s);
			fp_read_retu[i]=s;
		}
	}
}


int init(){

	char temp[100]={'\0'};
	printf("请输入PE文件的全路径:\n");
	
	scanf("%s",name);
	
	fp=fopen(name,"rb");
	if(fp==NULL){
		printf("wrong\n");
		return -1;
	}
	getrealname();


	Menu_num=1;
	Dialog_num=1;
	return 0;
}

void get_Resource(){
	int i,j,k,m;
	char temp[100];
	
	//菜单用到的变量
	char name_string[100];
	int fItemFlag;
	WORD ID;
	XC_MenuHeader menu_header;

	//图标用到的变量
	struct XC_ICO_DIR icon_dir;
	struct XC_ICO_DIR_ENTRY icon_dir_entry[100];
	WORD word_temp;
	DWORD dword_temp;
	BYTE byte_temp;
	char char_temp;

	//对话框用到的变量
	struct XC_DIALOGBOXHEADER dialogboxheader;
	struct XC_DIALOGCONTROLDATA dialogcontroldata;
	int fp_station_now;

	FILE *resourceFp;
	for(i=0;i<type_offset_num;i++){
		//printf("%X\n",type_offset[i].offset);

		if(type_offset[i].ID_1==4){		
			sprintf(temp,"%s%s_PEinfo\\Menu\\%s_%d_%d.txt",dirname,realname,preDefine[4-1],type_offset[i].ID_2,Menu_num);
		
			resourceFp=fopen(temp,"w");		
			fprintf(resourceFp,"%s\n\n",name);
			fprintf(resourceFp,"ID:%-4d   Num:%-4d\n\n",type_offset[i].ID_2,Menu_num);
			fseek(fp,type_offset[i].offset,0);
			fread((char*)&menu_header,sizeof(XC_MenuHeader),1,fp);
			while(1){
				fItemFlag=0;
				fread(&fItemFlag,sizeof(WORD),1,fp);

				if(fItemFlag==0x0080){
					fread(&ID,sizeof(WORD),1,fp);
					read_Unicode_and_Change(name_string);
					fprintf(resourceFp,"菜单结束    :  ID:%s   %s\n",(int_to_X(ID,4),int_to_X_retu),name_string);
					break;
				}else if(fItemFlag==0x10){
					read_Unicode_and_Change(name_string);
					fprintf(resourceFp,"弹出式菜单项:  ID:NULL   %s\n",name_string);
				}else if(fItemFlag==0){
					fread(&ID,sizeof(WORD),1,fp);
					if(ID!=0){
						read_Unicode_and_Change(name_string);
						fprintf(resourceFp,"普通菜单项  :  ID:%s   %s\n",(int_to_X(ID,4),int_to_X_retu),name_string);
					}else{
						fread(&ID,sizeof(WORD),1,fp);
						if(ID==0)
							fprintf(resourceFp,"菜单分隔符  :  ID:NULL   -----------\n");
					}
				}else{
					fread(&ID,sizeof(WORD),1,fp);
					read_Unicode_and_Change(name_string);
					if(fItemFlag==1)
						fprintf(resourceFp,"灰色        ");
					else if(fItemFlag==2)
						fprintf(resourceFp,"未激活      ");
					else if(fItemFlag==4)
						fprintf(resourceFp,"位图        ");
					else if(fItemFlag==0x0100)
						fprintf(resourceFp,"自画        ");
					else if(fItemFlag==8)
						fprintf(resourceFp,"已选中      ");
					else if(fItemFlag==0x20)
						fprintf(resourceFp,"菜单条分隔  ");
					else if(fItemFlag==0x40)
						fprintf(resourceFp,"菜单分隔    ");
					else 
						fprintf(resourceFp,"不明        ");
					fprintf(resourceFp,":  ID:%s   %s\n",(int_to_X(ID,4),int_to_X_retu),name_string);
				}
			}

			Menu_num++;
			fclose(resourceFp);
		}else if(type_offset[i].ID_1==14){
			fseek(fp,type_offset[i].offset,0);
			fread((char*)&icon_dir,sizeof(struct XC_ICO_DIR),1,fp);
			if((int)icon_dir.idreserved==0 && (int)icon_dir.idtype==1){
				for(j=0;j<(int)icon_dir.idcount;j++){
					fread((char*)&icon_dir_entry[j],4*sizeof(BYTE)+3*sizeof(WORD)+sizeof(DWORD),1,fp);
				}
				for(j=0;j<icon_dir.idcount;j++){
					for(k=0;k<type_offset_num;k++){
						if(type_offset[k].ID_2==(int)icon_dir_entry[j].dwImageOffset
							&& type_offset[k].ID_1==3	){	

							sprintf(temp,"%s%s_PEinfo\\Icon\\ID_%d.ico",dirname,realname,type_offset[k].ID_2);
							resourceFp=fopen(temp,"wb");	
							
							word_temp=(WORD)0;
							fwrite(&word_temp,sizeof(WORD),1,resourceFp);
							word_temp=(WORD)1;
							fwrite(&word_temp,sizeof(WORD),1,resourceFp);
							fwrite(&word_temp,sizeof(WORD),1,resourceFp);

							fwrite(&icon_dir_entry[j].bWidth,4*sizeof(BYTE)+2*sizeof(WORD)+sizeof(DWORD),1,resourceFp);
							dword_temp=(DWORD)(3*sizeof(WORD)+4*sizeof(BYTE)+2*sizeof(WORD)+2*sizeof(DWORD));
							fwrite(&dword_temp,sizeof(DWORD),1,resourceFp);

							fseek(fp,type_offset[k].offset,0);
							for(m=0;m<type_offset[k].size;m+=sizeof(BYTE)){
								fread(&byte_temp,sizeof(BYTE),1,fp);
								fwrite(&byte_temp,sizeof(BYTE),1,resourceFp);
							}							
							fclose(resourceFp);
						}
					}
				}
			}
		}else if(type_offset[i].ID_1==5){
			//printf("%X,size==%X\n",type_offset[i].offset,type_offset[i].size);

			sprintf(temp,"%s%s_PEinfo\\Dialog\\Dialog_ID_%d.txt",dirname,realname,type_offset[i].ID_2);
			resourceFp=fopen(temp,"w");
			fprintf(resourceFp,"%s\n\n",name);

			fseek(fp,type_offset[i].offset,0);
			fread((char*)&dialogboxheader,2*sizeof(DWORD)+5*sizeof(WORD),1,fp);
			fprintf(resourceFp,"标准窗口样式：%X\n",dialogboxheader.lStyle);
			fprintf(resourceFp,"扩展窗口样式：%X\n",dialogboxheader.lExtendedStyle);
			fprintf(resourceFp,"窗口起点坐标：x ==%-5d,y ==%-5d\n",(int)dialogboxheader.rect.x,(int)dialogboxheader.rect.y);
			fprintf(resourceFp,"窗口大小    ：cx==%-5d,cy==%-5d\n",(int)dialogboxheader.rect.cx,(int)dialogboxheader.rect.cy);
			fprintf(resourceFp,"控件数量    ：%d\n",dialogboxheader.NumberOfItems);
			
			fread((char*)&word_temp,sizeof(WORD),1,fp);
			if(word_temp==0xFFFF){
				fread((char*)&word_temp,sizeof(WORD),1,fp);
				fprintf(resourceFp,"菜单ID      ：%d\n",word_temp);
			}else if(word_temp==0){	
				fprintf(resourceFp,"菜单名      ：NULL\n");
			}

			fread((char*)&word_temp,sizeof(WORD),1,fp);
			if(word_temp==0xFFFF){
				fread((char*)&word_temp,sizeof(WORD),1,fp);
				fprintf(resourceFp,"类ID        ：%d\n",word_temp);
			}else if(word_temp==0){	
				fprintf(resourceFp,"类名        ：NULL\n");
			}
			read_Unicode_and_Change(temp);
			fprintf(resourceFp,"对话框标题  ：%s\n",temp);
			if(dialogboxheader.lStyle & DS_SETFONT){
				fread((char*)&word_temp,sizeof(WORD),1,fp);
				read_Unicode_and_Change(temp);
				fprintf(resourceFp,"字体        ：%-2d,%s\n\n",word_temp,temp);
			}else{
				fread((char*)&word_temp,sizeof(WORD),1,fp);
				fprintf(resourceFp,"字体        ：NULL\n\n");
				fread((char*)&dword_temp,sizeof(DWORD),1,fp);
			}

			fp_station_now=ftell(fp);
			m=fp_station_now%sizeof(DWORD);
			fseek(fp,(sizeof(DWORD)-m)%sizeof(DWORD),1);
			fp_station_now=ftell(fp);
			for(j=0;j<(int)dialogboxheader.NumberOfItems;j++){
				//每个控件结构体都会DWORD对齐
				m=ftell(fp);
				m=m-fp_station_now;
				m=m%sizeof(DWORD);
				fseek(fp,(sizeof(DWORD)-m)%sizeof(DWORD),1);
				
				fread((char*)&dialogcontroldata,2*sizeof(DWORD)+5*sizeof(WORD),1,fp);
				fprintf(resourceFp,"\n第%-2d个控件：\n",j+1);
				fprintf(resourceFp,"控件标准窗口样式：%X\n",dialogcontroldata.lStyle);
				fprintf(resourceFp,"控件扩展窗口样式：%X\n",dialogcontroldata.lExtendedStyle);
				fprintf(resourceFp,"控件窗口起始坐标：x ==%-5d,y ==%-5d\n",
						dialogcontroldata.rect.x,dialogcontroldata.rect.y);
				fprintf(resourceFp,"控件窗口大小    ：cx==%-5d,cy==%-5d\n",
						dialogcontroldata.rect.cx,dialogcontroldata.rect.cy);
				fprintf(resourceFp,"控件ID          ：%d\n",dialogcontroldata.wID);
				
				fread((char*)&word_temp,sizeof(WORD),1,fp);
				if(word_temp==0xFFFF){
					fread((char*)&word_temp,sizeof(WORD),1,fp);
					fprintf(resourceFp,"控件类型        ：%X\n",word_temp);
					
					read_Unicode_and_Change(temp);
				
					if(strlen(temp)==0)
						fprintf(resourceFp,"控件名          ：NULL\n");
					else 
						fprintf(resourceFp,"控件名          ：%s\n",temp);			
				}else{	
					fseek(fp,-1*sizeof(WORD),1);
					fprintf(resourceFp,"控件类型        ：NULL\n");
					read_Unicode_and_Change(temp);
					if(strlen(temp)==0)
						fprintf(resourceFp,"控件名          ：NULL\n");
					else 
						fprintf(resourceFp,"控件名          ：%s\n",temp);
				
					fread((char*)&word_temp,sizeof(WORD),1,fp);
				}
				fread((char*)&word_temp,sizeof(WORD),1,fp);	
				
			}






			fclose(resourceFp);
		}else if(type_offset[i].ID_1==2){
			sprintf(temp,"%s%s_PEinfo\\Bitmap\\Bitmap_ID_%d.bmp",dirname,realname,type_offset[i].ID_2);
			resourceFp=fopen(temp,"wb");

			fseek(fp,type_offset[i].offset,0);
			//fread((char*)&bitmap,sizeof(XC_BITMAPINFOHEADER),1,fp);

			char_temp='B';
			fwrite(&char_temp,sizeof(char),1,resourceFp);			
			char_temp='M';
			fwrite(&char_temp,sizeof(char),1,resourceFp);
			dword_temp=(DWORD)(type_offset[i].size+14);
			fwrite(&dword_temp,sizeof(DWORD),1,resourceFp);
			word_temp=0;
			fwrite(&word_temp,sizeof(WORD),1,resourceFp);
			fwrite(&word_temp,sizeof(WORD),1,resourceFp);
			m=40+14;
			fwrite(&m,sizeof(DWORD),1,resourceFp);

// 			printf("%X\n",type_offset[i].offset);
// 			printf("%X\n",type_offset[i].size);

			fseek(fp,type_offset[i].offset,0);
			for(m=0;m<type_offset[i].size;m+=sizeof(BYTE)){
				fread(&char_temp,sizeof(BYTE),1,fp);
				fwrite(&char_temp,sizeof(BYTE),1,resourceFp);
			}

			fclose(resourceFp);
		}
	}
}

void read_Unicode_and_Change(char *name_string){
	int name_length=0;
	WORD name_temp;
	
	while(1){
		fread((char*)&name_temp,sizeof(WORD),1,fp);
		if((int)name_temp==0){
			break;
		}
		((WORD*)name_string)[name_length]=name_temp;
		name_length++;
	}
	get_MultiByte(name_string,name_length);
}


void get_MultiByte(char *from,int from_length){
	char temp[100];
	int num;
	num=WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,(LPCWSTR)from,(int)from_length,temp,100,NULL,NULL);
	temp[num]='\0';
	wsprintf(from,"%s",temp);
}


void list_dos_header(){
	fprintf(outfp,"IMAGE_DOS_HEADER   struct\n");
	fprintf(outfp,"e_magic       DWORD   MZ\n");	
	fprintf(outfp,"e_lfanew      DWORD   %s\n",(int_to_X((int)dos_header.e_lfanew,8),int_to_X_retu));	
	fprintf(outfp,"IMAGE_DOS_HEADER   ends\n");
	fprintf(outfp,"\n");
}


void list_nt_header(){
	fseek(fp,(int)dos_header.e_lfanew,0);
		fread(&nt_header,sizeof(struct XC_IMAGE_NT_HEADERS),1,fp);


	fprintf(outfp,"IMAGE_NT_HEADER   struct\n");
	
	if(strncmp((char *)&nt_header,"PE",2)==0)
		fprintf(outfp,"signature        DWORD   PE\n");

	fprintf(outfp,"\n   IMAGE_FILE_HEADER   struct\n");
	
	fprintf(outfp,"      Machine                WORD   %8s\n",(int_to_X((int)nt_header.FileHeader.Machine,4),int_to_X_retu));
	fprintf(outfp,"      NumberOfSections       WORD   %8X\n",(int)nt_header.FileHeader.NumberOfSections);
	fprintf(outfp,"      SizeOfOptionalHeader   WORD   %8X\n",(int)nt_header.FileHeader.SizeOfOptionalHeader);
	fprintf(outfp,"      Characteristics        WORD   %8s\n",(int_to_X((int)nt_header.FileHeader.Characteristics,4),int_to_X_retu));

	fprintf(outfp,"   IMAGE_FILE_HEADER   ends\n");
	fprintf(outfp,"\n   IMAGE_OPTIONAL_HEADER32   struct\n");

	fprintf(outfp,"      SizeOfImage           DWORD   %8X\n",(int)nt_header.OptionalHeader.SizeOfImage);
	fprintf(outfp,"      SizeOfHeaders         DWORD   %8X\n",(int)nt_header.OptionalHeader.SizeOfHeaders);
	fprintf(outfp,"      CheckSum              DWORD   %8s\n",(int_to_X((int)nt_header.OptionalHeader.CheckSum,8),int_to_X_retu));
	fprintf(outfp,"      Subsystem              WORD   %8s",(int_to_X((int)nt_header.OptionalHeader.Subsystem,4),int_to_X_retu));
		if(nt_header.OptionalHeader.Subsystem==2)
			fprintf(outfp,"   Windows图形用户界面\n");
		else if(nt_header.OptionalHeader.Subsystem==3)
			fprintf(outfp,"   Windows字符模式(console)\n");
		else 
			fprintf(outfp,"   暂时未知，需XC扩展才可识别\n");
	fprintf(outfp,"      NumberOfRVAAndSize    DWORD   %8X\n",(int)nt_header.OptionalHeader.NumberOfRvaAndSizes);
	fprintf(outfp,"      ImageBase             DWORD   %8s\n",(int_to_X((int)nt_header.OptionalHeader.ImageBase,8),int_to_X_retu));
	fprintf(outfp,"      FileAligment          DWORD   %8X\n",(int)nt_header.OptionalHeader.FileAlignment);
	fprintf(outfp,"      SectionAligment       DWORD   %8X\n",(int)nt_header.OptionalHeader.SectionAlignment);
	
	fprintf(outfp,"      AddressOfEntryPoint   DWORD   %s   RVA\n",(int_to_X((int)nt_header.OptionalHeader.AddressOfEntryPoint,8),int_to_X_retu));
 	int temp=(int)nt_header.OptionalHeader.AddressOfEntryPoint;
	temp=RVA_to_FOA(temp);
	fprintf(outfp,"                                    %8X   FOA\n",temp);

	fseek(fp,dos_header.e_lfanew+0x78,0);
	int i;
	for(i=0;i<(int)nt_header.OptionalHeader.NumberOfRvaAndSizes;i++){
		fread((char*)&data_dir[i],sizeof(XC_IMAGE_DATA_DIRECTORY),1,fp);
	}	
	list_nt_header_Data_directory();

	fprintf(outfp,"   IMAGE_OPTIONAL_HEADER32   ends\n\n");
	fprintf(outfp,"IMAGE_NT_HEADER   ends\n\n");
}

void list_nt_header_Data_directory(){

	char s[16][25]={".edata:  导出表RVA",".idata:  导入表RVA",".pdata:  资源表RVA",".rsrc:   异常表RVA","(文件偏移):安全表",
		".reloc:  重定位表RVA",".debug:  调试表RVA","(必须为0):版权表RVA","全局指针表RVA",".tls:    线程本地存储RVA",
					"加载配置表RVA","绑定导入表RVA","IAT表RVA","延迟导入表RVA",".cormeta:CLR表RVA","预留类型RVA"};

	int i;
	fprintf(outfp,"\n");
	for(i=0;i<(int)nt_header.OptionalHeader.NumberOfRvaAndSizes;i++){
		fprintf(outfp,"      ID:%-2d   ADDR:%s -- SIZE:%8X   %s\n",i,(int_to_X((int)data_dir[i].VirtualAddress,8),int_to_X_retu),(int)data_dir[i].Size,s[i]);
	}
	fprintf(outfp,"\n");
}

void list_section_header(){
	int i;
	fseek(fp,dos_header.e_lfanew+0x78+16*8,0);
	for(i=0;i<nt_header.FileHeader.NumberOfSections;i++)
			fread((char*)&section_header[i],sizeof(XC_IMAGE_SECTION_HEADER),1,fp);

	fprintf(outfp,"\nIMAGE_SECTION_HEADER   struct\n\n");
	
	fprintf(outfp,"--------------------------------------------------------------------\n");
	fprintf(outfp,"Name        真实尺寸  对齐后的大小  文件中的偏移   RVA地址  节的属性\n");
	fprintf(outfp,"--------------------------------------------------------------------\n");
	for(i=0;i<nt_header.FileHeader.NumberOfSections;i++){
		fprintf(outfp,"%-10s  "	,(char*)&section_header[i].Name);
		fprintf(outfp,"%8X      ",(int)section_header[i].Misc.PhysicalAddress);				
		fprintf(outfp,"%8X     ",(int)section_header[i].SizeOfRawData);				
		fprintf(outfp,"%s   "	,(int_to_X((int)section_header[i].PointerToRawData,8),int_to_X_retu));				
		fprintf(outfp,"%s  "	,(int_to_X((int)section_header[i].VirtualAddress,8),int_to_X_retu));				
		fprintf(outfp,"%8X  \n"	,(int)section_header[i].Characteristics);															
	}
	fprintf(outfp,"--------------------------------------------------------------------\n");
	fprintf(outfp,"\nIMAGE_SECTION_HEADER   ends\n");
}

void list_import_descriptor(){
	if((int)nt_header.OptionalHeader.DataDirectory[1].Size==0){
		fprintf(outfp,"\n该PE文件无导入表\n");
		return ;
	}


	int i;
	import_by_name_num=0;
	int addr;
	addr=RVA_to_FOA(data_dir[1].VirtualAddress);
	fseek(fp,addr,0);
	for(i=0;;i++){
		fread((char*)&import_descriptor[i],sizeof(XC_IMAGE_IMPORT_DESCRIPTOR),1,fp);	
		if((int)import_descriptor[i].Name==0)
			break;
	}
	import_descriptor_num=i;

	int temp;
	char tempstr[100];
	int temp2;

	temp=(int)nt_header.OptionalHeader.DataDirectory[1].VirtualAddress;
// 	for(i=0;i<nt_header.FileHeader.NumberOfSections;i++){
// 		if(temp>=(int)section_header[i].VirtualAddress && temp<=(int)section_header[i].VirtualAddress+(int)section_header[i].Misc.PhysicalAddress){
// 			break;
// 		}
// 	}

	for(i=0;i<import_descriptor_num;i++){

		temp=(int)import_descriptor[i].Name;
		temp=RVA_to_FOA(temp);
		fseek(fp,temp,0);
		fread(tempstr,100,1,fp);

		fprintf(outfp,"\n--------------------------------------------\n");			
		fprintf(outfp,"导入表%-4d: %s\n",i+1,tempstr);
		fprintf(outfp,"OriginalFirstThunk   :%s\n",(int_to_X((int)import_descriptor[i].OriginalFirstThunk,8),int_to_X_retu));
		fprintf(outfp,"TimeDateStamp        :%s\n",(int_to_X((int)import_descriptor[i].TimeDateStamp,8),int_to_X_retu));
		fprintf(outfp,"ForwarderChain       :%s\n",(int_to_X((int)import_descriptor[i].ForwarderChain,8),int_to_X_retu));
		fprintf(outfp,"FirstThunk           :%s\n",(int_to_X((int)import_descriptor[i].FirstThunk,8),int_to_X_retu));
		fprintf(outfp,"--------------------------------------------\n");			

		if((int)import_descriptor[i].OriginalFirstThunk!=0)
			temp=(int)import_descriptor[i].OriginalFirstThunk;
		else 
			temp=(int)import_descriptor[i].FirstThunk;
		temp=RVA_to_FOA(temp);
		fseek(fp,temp,0);
		temp2=temp;
		while(1){
			fread((char*)&thunk_data[import_by_name_num],sizeof(struct XC_IMAGE_THUNK_DATA),1,fp);
			
			if((int)thunk_data[import_by_name_num].u1.AddressOfData==0)
				break;
			if(((int)thunk_data[import_by_name_num].u1.AddressOfData & 0x80000000)==0 ){
				temp=RVA_to_FOA((int)thunk_data[import_by_name_num].u1.AddressOfData);
				fseek(fp,temp,0);
				fread((char*)&import_by_name[import_by_name_num],sizeof(WORD),1,fp);
				fread(tempstr,50,1,fp);
				fprintf(outfp,"HINT:%s   NAME:%s\n",
						(int_to_X((int)import_by_name[import_by_name_num].Hint,4),int_to_X_retu),tempstr);
			}
			temp2+=sizeof(struct XC_IMAGE_THUNK_DATA);
			fseek(fp,temp2,0);
			import_by_name_num++;
		}		
	}
}

void list_export_directory(){
	int i,j;
	
	if((int)nt_header.OptionalHeader.DataDirectory[0].Size==0){
		fprintf(outfp,"\n该PE文件无导出表\n");
		return ;
	}

	i=RVA_to_FOA((int)nt_header.OptionalHeader.DataDirectory[0].VirtualAddress);
	fseek(fp,i,0);
	fread((char*)&export_directory,sizeof(struct XC_IMAGE_EXPORT_DIRECTORY),1,fp);

	char temp[100]={'\0'};
	
	int ID[10000]={0};
	int IDnum;
	int export_func_addr;
	int export_func_name;

	fseek(fp,RVA_to_FOA((int)export_directory.Name),0);
	fread(temp,100*sizeof(char),1,fp);

	fprintf(outfp,"NumberOfFunctions    :%-8d\n",(int)export_directory.NumberOfFunctions);
	fprintf(outfp,"NumberOfNames        :%-8d\n",(int)export_directory.NumberOfNames);
	fprintf(outfp,"nBase                :%-8d\n",(int)export_directory.Base);
	fprintf(outfp,"AddressOfFunctions   :%8s\n",(int_to_X((int)export_directory.AddressOfFunctions,8),int_to_X_retu));
	fprintf(outfp,"AddressOfNames       :%8s\n",(int_to_X((int)export_directory.AddressOfNames,8),int_to_X_retu));
	fprintf(outfp,"AddressOfNameOrdinals:%8s\n\n",(int_to_X((int)export_directory.AddressOfNameOrdinals,8),int_to_X_retu));

	fseek(fp,RVA_to_FOA((int)export_directory.AddressOfNameOrdinals),0);

	for(i=0;i<(int)export_directory.NumberOfNames;i++){		
		fread(&ID[i],sizeof(WORD),1,fp);
	}


	for(i=0;i<(int)export_directory.NumberOfFunctions;i++){		
		export_func_addr=RVA_to_FOA((int)export_directory.AddressOfFunctions);
		export_func_name=RVA_to_FOA((int)export_directory.AddressOfNames);

		fprintf(outfp,"NUM:%-4d  ID:%s  ",i,(int_to_X((int)export_directory.Base+i,4),int_to_X_retu));

		fseek(fp,export_func_addr+i*sizeof(DWORD),0);
		fread(&export_func_addr,sizeof(DWORD),1,fp);
		fprintf(outfp,"ADDR:%s  ",(int_to_X(export_func_addr,8),int_to_X_retu));
		fprintf(outfp,"ADDR_in_Memory:%s  ",(int_to_X(export_func_addr+nt_header.OptionalHeader.ImageBase,8),int_to_X_retu));	

		IDnum=-1;
		for(j=0;j<(int)export_directory.NumberOfNames;j++){
			if(ID[j]==i){
				IDnum=j;
				break;
			}
		}
		if(IDnum==-1){
			fprintf(outfp,"NAME:(按照序号导出)\n");
		}else{
			fseek(fp,export_func_name+IDnum*sizeof(DWORD),0);
			fread(&export_func_name,sizeof(DWORD),1,fp);
			export_func_name=RVA_to_FOA(export_func_name);
			fseek(fp,export_func_name,0);
			fread(temp,100,1,fp);
			fprintf(outfp,"NAME:%s\n",temp);
		}
	}
}

void list_base_relocation(){
	if((int)nt_header.OptionalHeader.DataDirectory[5].Size==8 || (int)nt_header.OptionalHeader.DataDirectory[5].Size==0){
		fprintf(outfp,"\n该PE文件无重定位表\n");
		return ;
	}

	int i;
	int temp;
	int sign;

	fprintf(outfp,"(若修正项为FFF,则表示不需修正)\n\n");

	fseek(fp,RVA_to_FOA((int)nt_header.OptionalHeader.DataDirectory[5].VirtualAddress),0);
	while(1){
		fread((char*)&base_relocation,sizeof(struct XC_IMAGE_BASE_RELOCATION),1,fp);
		if((int)base_relocation.VirtualAddress==0){
			fprintf(outfp,"\n\n重定位表结束\n");
			return ;
		}

		fprintf(outfp,"\n");
		fprintf(outfp,"SizeOfBlock        :%d\n",(int)base_relocation.SizeOfBlock);
		fprintf(outfp,"VirtualAddress(RVA):%s\n",(int_to_X((int)base_relocation.VirtualAddress,8),int_to_X_retu));
		fprintf(outfp,"VAbase             :%s\n",(int_to_X((int)(base_relocation.VirtualAddress+nt_header.OptionalHeader.ImageBase),8),int_to_X_retu));
		fprintf(outfp,"---------------------------------------------------------------");
		for(i=0;i<((int)base_relocation.SizeOfBlock-8)/2;i++){
			fread(&temp,sizeof(WORD),1,fp);

			temp=temp & 0xFFFF;
			sign=temp >> 12;
			temp=temp & 0xFFF;

			if(i%16==0)
				fprintf(outfp,"\n");

			if(sign==0){
				fprintf(outfp,"FFF ");
			}else if(sign==3){
				fprintf(outfp,"%s ",(int_to_X(temp,3),int_to_X_retu));
			}
		}
		fprintf(outfp,"\n");
	}
}

void list_delay_import_descriptor(){
	if((int)nt_header.OptionalHeader.DataDirectory[13].Size==0){
		fprintf(outfp,"该PE文件无延迟导入表\n");
		return ;
	}
	
	int i;
	int temp;
	int temp2;
	char tempstr[100];
	int RVA_FOA_SIGN=-1;

	i=0;
	while(1){
		temp=RVA_to_FOA((int)nt_header.OptionalHeader.DataDirectory[13].VirtualAddress);

		fseek(fp,temp+i*sizeof(struct XC_IMAGE_DELAY_IMPORT_DESCRIPTOR),0);
		fread((char*)&delay_import,sizeof(struct XC_IMAGE_DELAY_IMPORT_DESCRIPTOR),1,fp);

		if((int)delay_import.Name==0)
			return ;
		
		if((int)delay_import.Attributes==1)
			RVA_FOA_SIGN=0;
		else if((int)delay_import.Attributes==0)
			RVA_FOA_SIGN=1;

		temp=RVA_to_FOA((int)delay_import.Name-RVA_FOA_SIGN*nt_header.OptionalHeader.ImageBase);
		fseek(fp,temp,0);
		fread(tempstr,100,1,fp);
		fprintf(outfp,"DLL_name:%s\n",tempstr);

		temp2=RVA_to_FOA((int)delay_import.DelayINT-RVA_FOA_SIGN*nt_header.OptionalHeader.ImageBase);			
		while(1){
			
			fseek(fp,temp2,0);
			fread(&temp,sizeof(DWORD),1,fp);
			if(temp==0)
				break;
			
			fseek(fp,RVA_to_FOA(temp-RVA_FOA_SIGN*nt_header.OptionalHeader.ImageBase),0);
			fread((char*)&temp,sizeof(WORD),1,fp);
			fread(tempstr,100,1,fp);
			fprintf(outfp,"    %s\n",tempstr);


			temp2+=sizeof(DWORD);
		}
		i++;
	}
}

void list_resource_directory(){
	if((int)nt_header.OptionalHeader.DataDirectory[2].Size==0){
		fprintf(outfp,"该PE文件无资源表\n");
		return ;
	}
	type_offset_num=0;

	int i,j,k;
	int addr;
	WORD temp_length;
	int temp_num;
	char temp[100];
	char temp2[100];

	struct XC_IMAGE_RESOURCE_DIRECTORY resource_directory;
	struct XC_IMAGE_RESOURCE_DIRECTORY resource_directory_2;
	struct XC_IMAGE_RESOURCE_DIRECTORY resource_directory_3;

	struct XC_IMAGE_RESOURCE_DIRECTORY_ENTRY resource_directory_entry;
	struct XC_IMAGE_RESOURCE_DIRECTORY_ENTRY resource_directory_entry_2;
	struct XC_IMAGE_RESOURCE_DIRECTORY_ENTRY resource_directory_entry_3;

	struct XC_IMAGE_RESOURCE_DATA_ENTRY resource_data_entry;

	addr=RVA_to_FOA((int)nt_header.OptionalHeader.DataDirectory[2].VirtualAddress);

	fseek(fp,addr,0);
	fread((char*)&resource_directory,sizeof(XC_IMAGE_RESOURCE_DIRECTORY),1,fp);

	fprintf(outfp,"NumberOfNamedEntries :%d\n",(int)resource_directory.NumberOfNamedEntries);
	fprintf(outfp,"NumberOfIdEntries    :%d\n\n",(int)resource_directory.NumberOfIdEntries);

	for(i=0;i<(int)resource_directory.NumberOfNamedEntries+(int)resource_directory.NumberOfIdEntries;i++){
		fseek(fp,addr	+sizeof(struct XC_IMAGE_RESOURCE_DIRECTORY)
						+sizeof(struct XC_IMAGE_RESOURCE_DIRECTORY_ENTRY)*i,0);
		fread((char*)&resource_directory_entry,sizeof(XC_IMAGE_RESOURCE_DIRECTORY_ENTRY),1,fp);

		if(resource_directory_entry.bit1.NameIsString==1){
			fseek(fp,(int)resource_directory_entry.bit1.NameOffset+addr,0);
			fread((char*)&temp_length,sizeof(WORD),1,fp);
			fread(temp,(int)temp_length*sizeof(WORD),1,fp);
			temp_num=WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,(LPCWSTR)temp,(int)temp_length,temp2,100,NULL,NULL);
			temp2[temp_num]='\0';
				fprintf(outfp,"\nNAME  -->%s\n",temp2);
		}else if(resource_directory_entry.bit1.NameIsString==0){
			if((int)resource_directory_entry.Id<=16)
				fprintf(outfp,"\nID:%-3X-->%s\n",(int)resource_directory_entry.Id,
							preDefine[(int)resource_directory_entry.Id-1]);
			else 
				fprintf(outfp,"\nID:%-3X-->自定义编号\n",(int)resource_directory_entry.Id);
		}

		if(resource_directory_entry.bit2.DataIsDirectory==1){
			fseek(fp,addr+(int)resource_directory_entry.bit2.OffsetToDirectory,0);
			fread(&resource_directory_2,sizeof(XC_IMAGE_RESOURCE_DIRECTORY),1,fp);
			for(j=0;j<(int)resource_directory_2.NumberOfNamedEntries+(int)resource_directory_2.NumberOfIdEntries;j++){
				fseek(fp,addr+(int)resource_directory_entry.bit2.OffsetToDirectory
							+sizeof(XC_IMAGE_RESOURCE_DIRECTORY)
							+j*sizeof(XC_IMAGE_RESOURCE_DIRECTORY_ENTRY),0);
				fread((char*)&resource_directory_entry_2,sizeof(XC_IMAGE_RESOURCE_DIRECTORY_ENTRY),1,fp);
					
				if(resource_directory_entry_2.bit1.NameIsString==1){
					fseek(fp,(int)resource_directory_entry_2.bit1.NameOffset+addr,0);
					fread((char*)&temp_length,sizeof(WORD),1,fp);
					fread(temp,(int)temp_length*sizeof(WORD),1,fp);
					temp_num=WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,(LPCWSTR)temp,(int)temp_length,temp2,100,NULL,NULL);
					temp2[temp_num]='\0';
					fprintf(outfp,"   NAME:%s\n",temp2);
				}else if(resource_directory_entry_2.bit1.NameIsString==0){
					fprintf(outfp,"   ID:%-6X\n",(int)resource_directory_entry_2.Id);
				}

				if(resource_directory_entry_2.bit2.DataIsDirectory==1){
					fseek(fp,addr+resource_directory_entry_2.bit2.OffsetToDirectory,0);
					fread((char*)&resource_directory_3,sizeof(XC_IMAGE_RESOURCE_DIRECTORY),1,fp);
					for(k=0;k<(int)resource_directory_3.NumberOfIdEntries
							+(int)resource_directory_3.NumberOfNamedEntries;k++){
						fseek(fp,addr+resource_directory_entry_2.bit2.OffsetToDirectory
									 +sizeof(XC_IMAGE_RESOURCE_DIRECTORY)
									 +k*sizeof(XC_IMAGE_RESOURCE_DIRECTORY_ENTRY),0);
						fread((char*)&resource_directory_entry_3,sizeof(struct XC_IMAGE_RESOURCE_DIRECTORY_ENTRY),1,fp);
						fseek(fp,addr+(int)resource_directory_entry_3.bit2.OffsetToDirectory,0);						
						fread((char*)&resource_data_entry,sizeof(XC_IMAGE_RESOURCE_DATA_ENTRY),1,fp);

						fprintf(outfp,"      offset(RVA)=%-8X  offset(FOA)=%-8X  size=%-5X\n",
									(int)resource_data_entry.OffsetToData,
									RVA_to_FOA((int)resource_data_entry.OffsetToData),						
									(int)resource_data_entry.Size);

						type_offset[type_offset_num].ID_1=(int)resource_directory_entry.Id;
						type_offset[type_offset_num].ID_2=(int)resource_directory_entry_2.Id;
						type_offset[type_offset_num].offset=RVA_to_FOA((int)resource_data_entry.OffsetToData);						
						type_offset[type_offset_num].size=(int)resource_data_entry.Size;
						type_offset_num++;
					}
				}else if(resource_directory_entry_2.bit2.DataIsDirectory==0){
					fprintf(outfp,"ERROR!!\n");
					fprintf(outfp,"find in function list_resource_directory!!\n");
				}
			}
		}else if(resource_directory_entry.bit2.DataIsDirectory==0){
			fprintf(outfp,"ERROR!!!\n");
			fprintf(outfp,"find in function list_resource_directory!!!\n");
		}	
		fprintf(outfp,"\n");
	}
	get_Resource();
}


void list_tls(){
	if((int)nt_header.OptionalHeader.DataDirectory[9].Size==0){
		fprintf(outfp,"\n该PE文件无TLS(线程局部存储表)\n");
		return ;
	}
	
	//int start,end;
	int i;
	int addr_callback;
	//char temp_char;
	char temp[100];

	fseek(fp,RVA_to_FOA((int)nt_header.OptionalHeader.DataDirectory[9].VirtualAddress),0);
	fread((char*)&tls_directory,sizeof(XC_IMAGE_TLS_DIRECTORY),1,fp);

	fseek(fp,RVA_to_FOA((int)tls_directory.AddressOfIndex-nt_header.OptionalHeader.ImageBase),0);
	fread(temp,100,1,fp);
	if(temp[0]==0)
		fprintf(outfp,"TLS模板的名称: NULL\n");
	else
		fprintf(outfp,"TLS模板的名称: %s\n",temp);


	fprintf(outfp,"\n模板内容暂时没有显示，若以后需要可以添加\n");
// 	start=RVA_to_FOA((int)tls_directory.StartAddressOfRawData-nt_header.OptionalHeader.ImageBase);
// 	end	 =RVA_to_FOA((int)tls_directory.EndAddressOfRawData-nt_header.OptionalHeader.ImageBase);
// 	fprintf(outfp,"TLS模板的内容：");
// 	fseek(fp,start,0);
// 	for(i=0;i<end-start;i++){
// 		fscanf(fp,"%c",&temp_char);
// 		fprintf(outfp," %c",temp_char);
// 	}


	fprintf(outfp,"\nTLS模板中未初始化的数据量：%-4X\n\n",(int)tls_directory.SizeOfZeroFill);
	
	//fprintf(outfp,"回调函数：");
	addr_callback=RVA_to_FOA((int)tls_directory.AddressOfCallBacks-nt_header.OptionalHeader.ImageBase);
	fseek(fp,addr_callback,0);
	fread((char*)&pointer_to_tls_callback,2*sizeof(PVOID)+sizeof(DWORD),1,fp);
	i=1;
	while((int)pointer_to_tls_callback.DllHandle!=0){
		fprintf(outfp,"回调函数%-2d:",i);
		if((int)pointer_to_tls_callback.Reason==0){
			fprintf(outfp,"进程将要被终止，包括第一个线程\n");
		}else if((int)pointer_to_tls_callback.Reason==1){
			fprintf(outfp,"启动一个新进程，包括第一个线程\n");
		}else if((int)pointer_to_tls_callback.Reason==2){
			fprintf(outfp,"创建了一个新线程，创建所有线程时都会发送这个通知，除第一个线程外\n");
		}else if((int)pointer_to_tls_callback.Reason==3){
			fprintf(outfp,"线程将要被终止，终止所有线程时都会发送这个通知，除第一个线程外\n");
		}
		fread((char*)&pointer_to_tls_callback,2*sizeof(PVOID)+sizeof(DWORD),1,fp);	
		i++;
	}
}




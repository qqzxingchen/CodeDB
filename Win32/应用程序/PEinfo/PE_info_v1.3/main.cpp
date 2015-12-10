#include "struct.h"
#include "func.h"

extern struct XC_IMAGE_DOS_HEADER dos_header;

extern char name[100];
extern FILE *fp;
extern FILE *outfp;

extern int init();

extern void list_dos_header();
extern void list_nt_header();
extern void list_nt_header_Data_directory();
extern void list_section_header();
extern void list_import_descriptor();
extern void list_export_directory();
extern void list_base_relocation();
extern void list_delay_import_descriptor();
extern void list_resource_directory();
extern void list_tls();

int main(){
	printf("PE小工具系列1---PE文件信息显示器\n");
	printf("功能：显示PE文件的一些基本信息：\n");

	char temp[100]={'\0'};
	if(init()==-1)
		return 0;

	fseek(fp,0,0);
	fread(&dos_header,sizeof(struct XC_IMAGE_DOS_HEADER),1,fp);
	if(strncmp((char *)&dos_header.e_magic,"MZ",2)!=0){
		printf("this file is not a PE file!!!\n");
		return 0;
	}else{	

		sprintf(temp,"%s%s_PEinfo",dirname,realname);
		_mkdir(temp);
		sprintf(temp,"%s%s_PEinfo\\Menu",dirname,realname);
		_mkdir(temp);
		sprintf(temp,"%s%s_PEinfo\\Icon",dirname,realname);
		_mkdir(temp);
		sprintf(temp,"%s%s_PEinfo\\Dialog",dirname,realname);
		_mkdir(temp);
		sprintf(temp,"%s%s_PEinfo\\Bitmap",dirname,realname);
		_mkdir(temp);



		sprintf(temp,"%s%s_PEinfo\\header_info.txt",dirname,realname);
		outfp=fopen(temp,"w");		
		fprintf(outfp,"%s\n\n",name);
		list_dos_header();
		list_nt_header();
		list_section_header();
		

		fclose(outfp);
		sprintf(temp,"%s%s_PEinfo\\import_descriptor.txt",dirname,realname);
		outfp=fopen(temp,"w");		
		fprintf(outfp,"%s\n\n",name);
		list_import_descriptor();


		fclose(outfp);
		sprintf(temp,"%s%s_PEinfo\\export_directory.txt",dirname,realname);
		outfp=fopen(temp,"w");		
		fprintf(outfp,"%s\n\n",name);
		list_export_directory();


		fclose(outfp);
		sprintf(temp,"%s%s_PEinfo\\base_relocation.txt",dirname,realname);
		outfp=fopen(temp,"w");		
		fprintf(outfp,"%s\n\n",name);
		list_base_relocation();

		
		fclose(outfp);
		sprintf(temp,"%s%s_PEinfo\\delay_import.txt",dirname,realname);
		outfp=fopen(temp,"w");		
		fprintf(outfp,"%s\n\n",name);
		list_delay_import_descriptor();


		fclose(outfp);
		sprintf(temp,"%s%s_PEinfo\\resource_directory.txt",dirname,realname);
		outfp=fopen(temp,"w");		
		fprintf(outfp,"%s\n\n",name);
		list_resource_directory();

		fclose(outfp);
		sprintf(temp,"%s%s_PEinfo\\tls.txt",dirname,realname);
		outfp=fopen(temp,"w");		
		fprintf(outfp,"%s\n\n",name);
		list_tls();
	}
	
	fclose(fp);
	fclose(outfp);
	return 0;
}




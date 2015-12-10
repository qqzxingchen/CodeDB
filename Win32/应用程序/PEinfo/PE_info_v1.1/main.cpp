#include<stdio.h>

char trans[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};	

FILE *fp;
FILE *outfp;
char name[100];

int read_int[100];

char out[3];

int PEstart=0;
int NumberOfSections;

//一些变量的初始化
int init();

//将s数组里面的int型格式化转换（16进制），转换成功后存入out数组里面
int changedis(int temp);

//读入i个字节，并将其放入s数组里面
int get(int i);


int find_IMAGE_DOS_HEADER();
int find_IMAGE_NT_HEADER();
int find_IMAGE_FILE_HEADER();
int find_IMAGE_OPTIONAL_HEADER32();
int find_16_IMAGE_DATA_DIRECTORY();
int find_IMAGE_SECTION_HEADER();


int main(){

	init();
	get(4);
	find_IMAGE_DOS_HEADER();
	find_IMAGE_NT_HEADER();
	find_IMAGE_SECTION_HEADER();

	fclose(fp);
	fclose(outfp);
	return 0;
}

int init(){
	int i;
	for(i=0;i<100;i++)
		name[i]=0;
	printf("Please input the full dir of the PE file\n");
	scanf("%s",name);
	fp=fopen(name,"rb");
	outfp=fopen("PE_info.txt","w");
	return 0;
}


int find_IMAGE_DOS_HEADER(){
	fprintf(outfp,"IMAGE_DOS_HEADER STRUCT\n");
	fseek(fp,0,0);
	get(2);
		fprintf(outfp,"e_magic       双字节 0000h     %c%c\n",read_int[0],read_int[1]);
	fseek(fp,0x3c,0);
	get(4);
		PEstart+=changedis(read_int[3])*256*256*256;
		fprintf(outfp,"e_lfanew      四字节 003ch     %s",out);
		PEstart+=changedis(read_int[2])*256*256;
		fprintf(outfp,"%s",out);
		PEstart+=changedis(read_int[1])*256;
		fprintf(outfp,"%s",out);
		PEstart+=changedis(read_int[0]);
		fprintf(outfp,"%s\n",out);

	fprintf(outfp,"IMAGE_DOS_HEADER ENDS\n");
	return 0;
}


int find_IMAGE_NT_HEADER(){
fprintf(outfp,"\n\nIMAGE_NT_HEADERS STRUCT\n");
	fseek(fp,PEstart,0);
	get(4);
		fprintf(outfp,"PEsign           四字节 %8X      %c%c%c%c",PEstart,read_int[0],read_int[1],read_int[2],read_int[3]);		

	fprintf(outfp,"\n");
	find_IMAGE_FILE_HEADER();
	fprintf(outfp,"\n");
	find_IMAGE_OPTIONAL_HEADER32();
	fprintf(outfp,"\n");

fprintf(outfp,"\n\nIMAGE_NT_HEADERS ENDS\n");
	return 0;
}

int find_IMAGE_FILE_HEADER(){
	int temp;
fprintf(outfp,"\n   IMAGE_FILE_HEADER STRUCT\n");
	fseek(fp,PEstart+4,0);
	get(2);
		changedis(read_int[1]);
		fprintf(outfp,"   Machine               双字节  0004h   %6s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s\n",out);
	get(2);
		temp=0;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		NumberOfSections=temp;
		fprintf(outfp,"   NumberOfSections      双字节  0006h   %8X H\n",temp);
	get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   TimeDateInit          四字节  0008h   %s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s\n",out);
	get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   PointerToSymbolTable  四字节  000ch   %s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s     (用于调试)\n",out);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   NumberOfSymbols       四字节  0010h   %8X H\n",temp);
	get(2);
		temp=0;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfOptionalHeader  双字节  0014h   %8X H\n",temp);
	get(2);
		changedis(read_int[1]);
		fprintf(outfp,"   Characteristics       双字节  0016h   %6s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s\n",out);
fprintf(outfp,"   IMAGE_FILE_HEADER ENDS\n");
	return 0;
}

int find_IMAGE_OPTIONAL_HEADER32(){
	int temp;
fprintf(outfp,"   IMAGE_OPTIONAL_HEADER32 STRUCT\n");
	fseek(fp,PEstart+24,0);

	get(2);
		temp=0;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   Magic                 双字节  0018h   %8X H",temp);
			if(temp==0x010B)
				fprintf(outfp,"     PE32文件\n");
			else if(temp==0x0107)
				fprintf(outfp,"     ROM映像文件\n");
			else if(temp==0x020B)
				fprintf(outfp,"     PE32+文件(64位下的PE)\n");	
	get(2);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfCode            四字节  001ch   %8X H",temp);
		fprintf(outfp,"   代码是基于文件对齐后的大小\n");
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfInitData        四字节  0020h   %8X H",temp);
		fprintf(outfp,"   已初始化数据的节的总大小\n");
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfUnInitData      四字节  0024h   %8X H",temp);
		fprintf(outfp,"   未初始化数据的节的总大小\n");
	get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   AddrOfEntryPoint      四字节  0028h   %s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s   (RVA)\n",out);
	get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   BaseOfCode            四字节  002Ch   %s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s   (代码节的起始RVA  .txt)\n",out);
	get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   BaseOfData            四字节  0030h   %s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s   (数据节的起始RVA  .data)\n",out);
	get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   ImageBase             四字节  0034h   %s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s   (PE映像的优先装入点)\n",out);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SectionAlignment      四字节  0038h   %8X H\n",temp);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   FileAlignment         四字节  003ch   %8X H\n",temp);
	get(4);
	get(4);
	get(4);
	get(4);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfImage           四字节  0050h   %8X H\n",temp);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfHeaders         四字节  0054h   %8X H\n",temp);
	get(4);
	get(2);
		temp=0;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   Subsystem             双字节  005ch   %8X H",temp);
			if(temp==2)
				fprintf(outfp,"   windows图形用户界面\n");
			else if(temp==3)
				fprintf(outfp,"   windows字符模式(console)\n");
			else if(temp==1)
				fprintf(outfp,"   设备驱动程序和Native Windows进程");
	get(2);
		temp=0;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   DllCharacteristics    双字节  005eh   %8X H\n",temp);
			if(temp && 16)
				fprintf(outfp,"         DLL加载时可被重定位\n");
			if(temp && 32)
				fprintf(outfp,"         强制代码实施完整性验证\n");
			if(temp && 64)
				fprintf(outfp,"         该影像兼容DEP\n");
			if(temp && 128)
				fprintf(outfp,"         可以隔离，但是并不隔离此映像\n");
			if(temp && 256)
				fprintf(outfp,"         映像不使用SEH\n");
			if(temp && 512)
				fprintf(outfp,"         不绑定映像\n");
			if(temp && 2048)
				fprintf(outfp,"         该映像是一个WDM driver\n");
			if(temp && 8192)
				fprintf(outfp,"         可用于终端服务器");
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfStackReserve    四字节  0x60h   %8X H  保留栈的大小\n",temp);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfStackCommit     四字节  0x64h   %8X H  初始化时实际提交的栈大小\n",temp);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfHeapReserve     四字节  0x68h   %8X H  初始化时保留堆的大小\n",temp);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfHeapCommit      四字节  0x68h   %8X H  初始化时实际提交堆的大小\n",temp);
	get(4);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   NumberOfRvaAndSize    四字节  0x74h   %8X H  数据目录结构的数量\n",temp);

	fprintf(outfp,"\n");
	fprintf(outfp,"\n");
	find_16_IMAGE_DATA_DIRECTORY();
	fprintf(outfp,"\n");
fprintf(outfp,"   IMAGE_OPTIONAL_HEADER32 ENDS\n");
	return 0;
}

int find_16_IMAGE_DATA_DIRECTORY(){
	int i;

	fseek(fp,PEstart+0x78,0);

	char s[16][25]={".edata:  导出表RVA",".idata:  导入表RVA",".pdata:  资源表RVA",".rsrc:   异常表RVA","(文件偏移):安全表",
					".reloc:  重定位表RVA",".debug:  调试表RVA","(必须为0):版权表RVA","全局指针表RVA",".tls:    线程本地存储RVA",
					"加载配置表RVA","绑定导入表RVA","IAT表RVA","延迟导入表RVA",".cormeta:CLR表RVA","预留类型RVA"};
	
	for(i=0;i<16;i++){
		get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   Addr:%s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s",out);

		get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   Size:%s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s",out);

		fprintf(outfp,"---%s\n",s[i]);

		//fprintf(outfp,"\n");
	}

	return 0;
}

int find_IMAGE_SECTION_HEADER(){
	int i;
	int temp;

fprintf(outfp,"\n");
fprintf(outfp,"IMAGE_SECTION_HEADER STRUCT (个数由NumberOfSections确定)\n");
fprintf(outfp,"\n");
	fseek(fp,PEstart+0x78+16*8,0);
	for(i=0;i<NumberOfSections;i++){
		get(8);
			fprintf(outfp,"第%d个节表项:",i+1);
			fprintf(outfp,"   %c%c%c%c%c%c%c%c\n",read_int[0],read_int[1],
																	 		read_int[2],read_int[3],
																			read_int[4],read_int[5],
																			read_int[6],read_int[7]);
		get(4);
			temp=0;
			temp+=changedis(read_int[3])*256*256*256;
			temp+=changedis(read_int[2])*256*256;
			temp+=changedis(read_int[1])*256;
			temp+=changedis(read_int[0]);
			fprintf(outfp,"Misc<Union>      四字节  0008h   %8X   文件对齐之前的大小\n",temp);
		get(4);
			changedis(read_int[3]);
			fprintf(outfp,"VirtualAddress   四字节  000ch   %s",out);
			changedis(read_int[2]);
			fprintf(outfp,"%s",out);
			changedis(read_int[1]);
			fprintf(outfp,"%s",out);
			changedis(read_int[0]);
			fprintf(outfp,"%s   节区的RVA地址\n",out);
		get(4);
			temp=0;
			temp+=changedis(read_int[3])*256*256*256;
			temp+=changedis(read_int[2])*256*256;
			temp+=changedis(read_int[1])*256;
			temp+=changedis(read_int[0]);
			fprintf(outfp,"SizeOfRawData    四字节  0010h   %8X   节在文件对齐后的尺寸\n",temp);
		get(4);
			changedis(read_int[3]);
			fprintf(outfp,"PointerToRawData 四字节  0014h   %s",out);
			changedis(read_int[2]);
			fprintf(outfp,"%s",out);
			changedis(read_int[1]);
			fprintf(outfp,"%s",out);
			changedis(read_int[0]);
			fprintf(outfp,"%s   节区起始数据在文件中的偏移\n",out);
		get(4);
		get(4);
		get(2);
		get(2);
		get(4);
			temp=0;
			temp+=changedis(read_int[3])*256*256*256;
			temp+=changedis(read_int[2])*256*256;
			temp+=changedis(read_int[1])*256;
			temp+=changedis(read_int[0]);
			fprintf(outfp,"Charactoristics  四字节  0024h   %8X   节的属性\n",temp);



		fprintf(outfp,"\n");
	}
fprintf(outfp,"IMAGE_SECTION_HEADER ENDS");


	return 0;
}



int get(int i){
	int j;
	char c;
	//for(j=0;j<100;j++)
	//	read_int[j]=0;

	for(j=0;j<i;j++){
		fscanf(fp,"%c",&c);
		read_int[j]=(int)c;
	}
	return 0;
}

int changedis(int temp){

	int i=temp & 0xF;
	int j=temp >> 4;
	j=j & 0xF;

	out[0]=trans[j];
	out[1]=trans[i];
	out[2]='\0';

	int retu=0;
	if(out[0]>='A' && out[0]<='F')
		retu+=(out[0]-'A'+10)*16;
	else if(out[0]>='0' && out[0]<='9')
		retu+=(out[0]-'0')*16;

	if(out[1]>='A' && out[1]<='F')
		retu+=(out[1]-'A'+10);
	else if(out[1]>='0' && out[1]<='9')
		retu+=(out[1]-'0');

	return retu;
}



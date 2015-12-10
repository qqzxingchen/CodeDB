#include<stdio.h>

char trans[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};	

FILE *fp;
FILE *outfp;
char name[100];

int read_int[100];

char out[3];

int PEstart=0;
int NumberOfSections;

//һЩ�����ĳ�ʼ��
int init();

//��s���������int�͸�ʽ��ת����16���ƣ���ת���ɹ������out��������
int changedis(int temp);

//����i���ֽڣ����������s��������
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
		fprintf(outfp,"e_magic       ˫�ֽ� 0000h     %c%c\n",read_int[0],read_int[1]);
	fseek(fp,0x3c,0);
	get(4);
		PEstart+=changedis(read_int[3])*256*256*256;
		fprintf(outfp,"e_lfanew      ���ֽ� 003ch     %s",out);
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
		fprintf(outfp,"PEsign           ���ֽ� %8X      %c%c%c%c",PEstart,read_int[0],read_int[1],read_int[2],read_int[3]);		

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
		fprintf(outfp,"   Machine               ˫�ֽ�  0004h   %6s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s\n",out);
	get(2);
		temp=0;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		NumberOfSections=temp;
		fprintf(outfp,"   NumberOfSections      ˫�ֽ�  0006h   %8X H\n",temp);
	get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   TimeDateInit          ���ֽ�  0008h   %s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s\n",out);
	get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   PointerToSymbolTable  ���ֽ�  000ch   %s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s     (���ڵ���)\n",out);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   NumberOfSymbols       ���ֽ�  0010h   %8X H\n",temp);
	get(2);
		temp=0;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfOptionalHeader  ˫�ֽ�  0014h   %8X H\n",temp);
	get(2);
		changedis(read_int[1]);
		fprintf(outfp,"   Characteristics       ˫�ֽ�  0016h   %6s",out);
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
		fprintf(outfp,"   Magic                 ˫�ֽ�  0018h   %8X H",temp);
			if(temp==0x010B)
				fprintf(outfp,"     PE32�ļ�\n");
			else if(temp==0x0107)
				fprintf(outfp,"     ROMӳ���ļ�\n");
			else if(temp==0x020B)
				fprintf(outfp,"     PE32+�ļ�(64λ�µ�PE)\n");	
	get(2);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfCode            ���ֽ�  001ch   %8X H",temp);
		fprintf(outfp,"   �����ǻ����ļ������Ĵ�С\n");
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfInitData        ���ֽ�  0020h   %8X H",temp);
		fprintf(outfp,"   �ѳ�ʼ�����ݵĽڵ��ܴ�С\n");
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfUnInitData      ���ֽ�  0024h   %8X H",temp);
		fprintf(outfp,"   δ��ʼ�����ݵĽڵ��ܴ�С\n");
	get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   AddrOfEntryPoint      ���ֽ�  0028h   %s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s   (RVA)\n",out);
	get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   BaseOfCode            ���ֽ�  002Ch   %s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s   (����ڵ���ʼRVA  .txt)\n",out);
	get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   BaseOfData            ���ֽ�  0030h   %s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s   (���ݽڵ���ʼRVA  .data)\n",out);
	get(4);
		changedis(read_int[3]);
		fprintf(outfp,"   ImageBase             ���ֽ�  0034h   %s",out);
		changedis(read_int[2]);
		fprintf(outfp,"%s",out);
		changedis(read_int[1]);
		fprintf(outfp,"%s",out);
		changedis(read_int[0]);
		fprintf(outfp,"%s   (PEӳ�������װ���)\n",out);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SectionAlignment      ���ֽ�  0038h   %8X H\n",temp);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   FileAlignment         ���ֽ�  003ch   %8X H\n",temp);
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
		fprintf(outfp,"   SizeOfImage           ���ֽ�  0050h   %8X H\n",temp);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfHeaders         ���ֽ�  0054h   %8X H\n",temp);
	get(4);
	get(2);
		temp=0;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   Subsystem             ˫�ֽ�  005ch   %8X H",temp);
			if(temp==2)
				fprintf(outfp,"   windowsͼ���û�����\n");
			else if(temp==3)
				fprintf(outfp,"   windows�ַ�ģʽ(console)\n");
			else if(temp==1)
				fprintf(outfp,"   �豸���������Native Windows����");
	get(2);
		temp=0;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   DllCharacteristics    ˫�ֽ�  005eh   %8X H\n",temp);
			if(temp && 16)
				fprintf(outfp,"         DLL����ʱ�ɱ��ض�λ\n");
			if(temp && 32)
				fprintf(outfp,"         ǿ�ƴ���ʵʩ��������֤\n");
			if(temp && 64)
				fprintf(outfp,"         ��Ӱ�����DEP\n");
			if(temp && 128)
				fprintf(outfp,"         ���Ը��룬���ǲ��������ӳ��\n");
			if(temp && 256)
				fprintf(outfp,"         ӳ��ʹ��SEH\n");
			if(temp && 512)
				fprintf(outfp,"         ����ӳ��\n");
			if(temp && 2048)
				fprintf(outfp,"         ��ӳ����һ��WDM driver\n");
			if(temp && 8192)
				fprintf(outfp,"         �������ն˷�����");
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfStackReserve    ���ֽ�  0x60h   %8X H  ����ջ�Ĵ�С\n",temp);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfStackCommit     ���ֽ�  0x64h   %8X H  ��ʼ��ʱʵ���ύ��ջ��С\n",temp);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfHeapReserve     ���ֽ�  0x68h   %8X H  ��ʼ��ʱ�����ѵĴ�С\n",temp);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   SizeOfHeapCommit      ���ֽ�  0x68h   %8X H  ��ʼ��ʱʵ���ύ�ѵĴ�С\n",temp);
	get(4);
	get(4);
		temp=0;
		temp+=changedis(read_int[3])*256*256*256;
		temp+=changedis(read_int[2])*256*256;
		temp+=changedis(read_int[1])*256;
		temp+=changedis(read_int[0]);
		fprintf(outfp,"   NumberOfRvaAndSize    ���ֽ�  0x74h   %8X H  ����Ŀ¼�ṹ������\n",temp);

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

	char s[16][25]={".edata:  ������RVA",".idata:  �����RVA",".pdata:  ��Դ��RVA",".rsrc:   �쳣��RVA","(�ļ�ƫ��):��ȫ��",
					".reloc:  �ض�λ��RVA",".debug:  ���Ա�RVA","(����Ϊ0):��Ȩ��RVA","ȫ��ָ���RVA",".tls:    �̱߳��ش洢RVA",
					"�������ñ�RVA","�󶨵����RVA","IAT��RVA","�ӳٵ����RVA",".cormeta:CLR��RVA","Ԥ������RVA"};
	
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
fprintf(outfp,"IMAGE_SECTION_HEADER STRUCT (������NumberOfSectionsȷ��)\n");
fprintf(outfp,"\n");
	fseek(fp,PEstart+0x78+16*8,0);
	for(i=0;i<NumberOfSections;i++){
		get(8);
			fprintf(outfp,"��%d���ڱ���:",i+1);
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
			fprintf(outfp,"Misc<Union>      ���ֽ�  0008h   %8X   �ļ�����֮ǰ�Ĵ�С\n",temp);
		get(4);
			changedis(read_int[3]);
			fprintf(outfp,"VirtualAddress   ���ֽ�  000ch   %s",out);
			changedis(read_int[2]);
			fprintf(outfp,"%s",out);
			changedis(read_int[1]);
			fprintf(outfp,"%s",out);
			changedis(read_int[0]);
			fprintf(outfp,"%s   ������RVA��ַ\n",out);
		get(4);
			temp=0;
			temp+=changedis(read_int[3])*256*256*256;
			temp+=changedis(read_int[2])*256*256;
			temp+=changedis(read_int[1])*256;
			temp+=changedis(read_int[0]);
			fprintf(outfp,"SizeOfRawData    ���ֽ�  0010h   %8X   �����ļ������ĳߴ�\n",temp);
		get(4);
			changedis(read_int[3]);
			fprintf(outfp,"PointerToRawData ���ֽ�  0014h   %s",out);
			changedis(read_int[2]);
			fprintf(outfp,"%s",out);
			changedis(read_int[1]);
			fprintf(outfp,"%s",out);
			changedis(read_int[0]);
			fprintf(outfp,"%s   ������ʼ�������ļ��е�ƫ��\n",out);
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
			fprintf(outfp,"Charactoristics  ���ֽ�  0024h   %8X   �ڵ�����\n",temp);



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



#include<stdio.h>
#include<string.h>

FILE *fp;
FILE *tempfp;
FILE *outfp;

int nowFp=0;
char keyword[26][10]={"main","if","then","while","do","static", 
					"int","double","struct","break","else","long",
					"switch","case","typedef","char","return",
					"const","float","short","continue","for",
					"void","default","sizeof","do"};
char jiefu[16][3]={
					">=" , ">" , "<=" , "<>" , "<" , "=" ,
					"+"  , "-" , "*"  , "/"  ,
					"("  , ")" , ":=" , ":"  , ";" , "#"
				};
char digit[10]={'0','1','2','3','4','5','6','7','8','9'};
char letter[52];

char s[2000];
int snow;
char now;
char old;

int clear();
int init();
int check();
int stepone();
int indenty();
int indenty_two(int type,char * temps);


int main(){
	if(init()==-1)
		return 0;
	//check();
	indenty();


	fclose(fp);
	fclose(outfp);
	return 0;
}

int clear(){
	int i;
	for(i=0;i<2000;i++)
		s[i]='\0';
	snow=0;
	return 0;
}

int init(){
	
	char s[100];
	char temp[100];
	int temp_num;

	printf("Input File:\n");
	scanf("%s",s);
	
	if((fp=fopen(s,"r"))==NULL){
		printf("File Not Found!!!");
		return -1;
	}
		
	//
	tempfp=fopen("temp.txt","w");
	int sign=0;
	int sign_step;
	int i;
	while(stepone()){
		if(sign==0 && old!='/' && now=='/'){
			sign=1;
		}else if(sign==1 && old=='/' && now=='/'){
			sign=2;
		}else if(sign==1 && old=='/' && now=='*'){
			sign=3;
		}else if(sign==1 && old=='/' && now!='*' && now!='/'){
			fprintf(tempfp,"/");
			sign=0;
		}
		if(sign==0){
			if(now!='\b' && now!='\n' && now!='\r')
				fprintf(tempfp,"%c",now);
			else 
				fprintf(tempfp," ");
		}
		else if(sign==2){
			stepone();
			while(now!='\n'){
				stepone();
			}
			sign=0;
		}
		else if(sign==3){
			temp_num=0;
			sign_step=-1;
			while(!(old=='*' && now=='/')){
				if(stepone()){
					if(now=='\n' || now=='\r' || now=='\b')
						temp[temp_num]=' ';	
					else 
						temp[temp_num]=now;
					temp_num++;
				}else{
					sign_step=1;
					break;
				}
			}
			if(temp_num==1 || temp_num==0 || sign_step==1){
				fprintf(tempfp," /*");
				sign_step=0;
				for(i=0;i<temp_num;i++){
					fprintf(tempfp,"%c",temp[i]);
				}
			}
			sign=0;

		}
	}







	//
// 	tempfp=fopen("input.txt","w");
// 	int sign=0;
// 	while(stepone()){
// 		if(sign==0 && old!='/' && now=='/'){
// 			sign=3;
// 		}else if(sign==3 && old=='/' && now=='/'){
// 			sign=4;
// 
// 		}else if(sign==3 && old=='/' && now=='*'){
// 			sign=1;
// 		}else if(sign==1 && old=='*' && now=='/')
// 			sign=5;
// 
// 		//if(old==' ' && now==' ')
// 		//	continue;
// 
// 		//printf("sign=%d,old=%c,now=%c\n",sign,old,now);
// 		
// 		if(sign==0){
// 			if(!(now=='\n' || now=='\r' || now=='\b'))
// 				fprintf(tempfp,"%c",now);
// 			else 
// 				fprintf(tempfp," ");
// 		}else if(sign==4 || sign==5){
// 			fprintf(tempfp," ");
// 			sign=0;
// 		}
// 	}
	

	
	fclose(tempfp);
	fclose(fp);
	fp=fopen("temp.txt","rb");
	//

	outfp=fopen("out.txt","w");

	for(i=0;i<26;i++)
		letter[i]='A'+i;
	for(i=0;i<26;i++)
		letter[i+26]='a'+i;

	for(i=0;i<26;i++)
		fprintf(outfp,"%-3d   %s\n",i,keyword[i]);
	for(i=0;i<16;i++)
		fprintf(outfp,"%-3d   %s\n",i+26,jiefu[i]);
	fprintf(outfp,"42    空格\n");

	fprintf(outfp,"\n\n\n");
	return 0;
}

int stepone(){
	nowFp++;
	old=now;
	int i=fscanf(fp,"%c",&now);
	if(i!=EOF)
		return 1;
	else 
		return 0;
}

int check(){
	int i;
	for(i=0;i<26;i++)
		printf("%s\n",keyword[i]);
	for(i=0;i<16;i++)
		printf("%3s",jiefu[i]);
	for(i=0;i<52;i++)
		printf("%2c",letter[i]);
	printf("\n");
	return 0;
}

int indenty(){

	//数字,字母为1
	//界符为3
	//空格为4
	int sign=4;
	int signnow=-1;
	while(stepone()){
		//printf("%c",now);
		if(now==' ')
			signnow=4;
		else if((now>='A' && now<='Z')||(now>='a' && now<='z'))
			signnow=1;
		else if(now>='0' && now<='9')
			signnow=1;
		else if(now=='+' || now=='-' || now=='*' || now=='/' || now=='(' 
						 || now==')' || now==';' || now=='>' || now=='<'
						 || now=='#' || now==':' || now=='=' )
			signnow=3;

		//printf("sign==%d,signnow==%d",sign,signnow);
		//printf("   %c--",now);
		//printf("   %s--\n",s);

		if(signnow==4){
			if(sign!=4){
				indenty_two(sign,s);
				fprintf(outfp,"42    空格\n");
				sign=4;
			}
		}
		if(signnow==3){
			if(sign==3){
				s[snow]=now;
				snow++;
			}else if(sign==1){
				indenty_two(sign,s);
				s[snow]=now;
				snow++;
				sign=3;
			}else if(sign==4){
				s[snow]=now;
				snow++;
				sign=3;
			}
		}
		if(signnow==1){
			if(sign==1){
				s[snow]=now;
				snow++;
				sign=signnow;
			}else if(sign==3){
				indenty_two(sign,s);
				s[snow]=now;
				snow++;
				sign=signnow;
			}else if(sign==4){
				s[snow]=now;
				snow++;
				sign=signnow;
			}
		}
	}
	indenty_two(sign,s);
	return 0;
}
int indenty_two(int type,char * temps){
	//printf("%s\n",temps);

	int i;
	char temp[100];

	if(type==1 && (   (temps[0]>='A' && temps[0]<='Z')||
					  (temps[0]>='a' && temps[0]<='z'))   ){
		for(i=0;i<26;i++){
			if(strcmp(keyword[i],temps)==0){
				fprintf(outfp,"%-3d   %s\n",i,temps);
				clear();
				return 0;
			}
		}
		fprintf(outfp,"var   %s\n",temps);
	}else if(type==1 && (temps[0]>='0' && temps[0]<='9')){
		for(i=0;i<100;i++)
			temp[i]='\0';

		i=0;
		while(temps[i]>='0' && temps[i]<='9'){
			temp[i]=temps[i];
			i++;
		}
		
		int j=0;
		while(temp[j]=='0'){
			j++;
		}

		fprintf(outfp,"int   %s\n",&temp[j]);
		indenty_two(type,&temps[i]);

	}else if(type==3){
		if(temps[0]=='>'){
			if(temps[1]=='='){
				fprintf(outfp,"26    >=\n");
				indenty_two(type,&temps[2]);
			}else{
				fprintf(outfp,"27    >\n");
				indenty_two(type,&temps[1]);
			}
		}else if(temps[0]=='<'){
			if(temps[1]=='='){
				fprintf(outfp,"28    <=\n");
				indenty_two(type,&temps[2]);
			}else if(temps[1]=='>'){
				fprintf(outfp,"29    <>\n");
				indenty_two(type,&temps[2]);
			}else{
				fprintf(outfp,"30    <\n");
				indenty_two(type,&temps[1]);
			}
		}else if(temps[0]=='='){
			fprintf(outfp,"31    =\n");
			indenty_two(type,&temps[1]);
		}else if(temps[0]=='+'){
			fprintf(outfp,"32    +\n");
			indenty_two(type,&temps[1]);
		}else if(temps[0]=='-'){
			fprintf(outfp,"33    -\n");
			indenty_two(type,&temps[1]);
		}else if(temps[0]=='*'){
			fprintf(outfp,"34    *\n");
			indenty_two(type,&temps[1]);
		}else if(temps[0]=='/'){
			fprintf(outfp,"35    /\n");
			indenty_two(type,&temps[1]);
		}else if(temps[0]=='('){
			fprintf(outfp,"36    (\n");
			indenty_two(type,&temps[1]);
		}else if(temps[0]==')'){
			fprintf(outfp,"37    )\n");
			indenty_two(type,&temps[1]);
		}else if(temps[0]==':'){
			if(temps[1]=='='){
				fprintf(outfp,"38    :=\n");
				indenty_two(type,&temps[2]);
			}else{
				fprintf(outfp,"39    :\n");
				indenty_two(type,&temps[1]);
			}
		}else if(temps[0]==';'){
			fprintf(outfp,"40    ;\n");
			indenty_two(type,&temps[1]);
		}else if(temps[0]=='#'){
			fprintf(outfp,"41    #\n");
			indenty_two(type,&temps[1]);
		}
	}


	clear();
	return 0;
}
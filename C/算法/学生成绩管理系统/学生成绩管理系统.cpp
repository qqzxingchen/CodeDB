#include<stdio.h>
#include<windows.h>

struct student
{
  unsigned long id;
  char name[11];     
  char sex;            //w or m     
  int age;     
  float score[4];      //平均成绩 语 数 外 
}stud[100];


int n;               //学生的个数 
int main1();
int jiemian1();
int jiemian2();
int jiemian3();
int jiemian4();
int jiemian5();

int main1()
{ 
  system("cls");
  int i=0;
  printf("                   1：录入学生的基本信息。\n");  
  printf("                   2：按ID查询或修改某位学生的信息。\n");  
  printf("                   3：输出所有学生的信息。\n");  
  printf("                   4：查看学生的成绩情况。\n");
  printf("                   5：保存并退出。\n");  
  loop:
  printf("**********************************************************************\n");
  printf("请输入您的选择：\n");
  scanf("%d",&i);
  switch (i)
     {
       case 1:jiemian1();break;  
       case 2:jiemian2();break;  
       case 3:jiemian3();break;  
       case 4:jiemian4();break;
       case 5:jiemian5();break;
       default:{
                 printf("输入错误，请按照提示输入！\n");
                 goto loop;
               }  
     }  
}



int main()
{
  FILE *fp;  
  int i=1;
  fp=fopen("学生成绩记录","rb");
  if(fp==NULL)
     n=0;
  else 
     {
       while(fread(&stud[i],sizeof(struct student),1,fp)!=0)
         {
            fseek(fp,i*sizeof(struct student),0);
            i++; 
         }
       n=i-1;  
     }
   
  fclose(fp);
  main1();  
}



int jiemian1()
{
  system("cls");  
  int i=0,j=0;
  float sum=0;  
  printf("请输入学生的个数：\n");
  scanf("%d",&i);  
  for(j=1;j<=i;j++)
     {
       sum=0;
       printf("请输入第%d个学生的ID：\n",j);            
        scanf("%d",&stud[n+j].id);         
       printf("请输入第%d个学生的名字：\n",j);
       getchar();         
         gets(stud[n+j].name);         
       printf("请输入第%d个学生的性别：\n",j);
        scanf("%c",&stud[n+j].sex);
       printf("请输入第%d个学生的年龄：\n",j);
        scanf("%d",&stud[n+j].age);
       printf("请输入第%d个学生的语文成绩：\n",j);            
        scanf("%f",&stud[n+j].score[1]); 
        sum=sum+stud[n+j].score[1];
       printf("请输入第%d个学生的数学成绩：\n",j);
        scanf("%f",&stud[n+j].score[2]); 
        sum=sum+stud[n+j].score[2];
       printf("请输入第%d个学生的英语成绩：\n",j);            
        scanf("%f",&stud[n+j].score[3]); 
        sum=sum+stud[n+j].score[3];
       stud[n+j].score[0]=sum/3; 
       printf("第%d个学生成绩输入完成。\n",j); 
     }  
  n=n+i;
  getchar();
  main1();  
}



int jiemian2()
{
  system("cls");
  long int k;
  int c=0;
  int m=0;
  float sum=0;
  int i=1,j=0;  
  printf("请输入要查询学生的ID：\n");  
  scanf("%d",&k);  
  for(i=1;i<=n;i++) 
     {
       if(stud[i].id==k)
          {
            j=1;            
            break;            
          }         
     } 
  if(j==0)
    {
       printf("没有您要查询的ID。\n");
       getchar();
       getchar();
       main1();
    }       
  else if(j==1)    
     {
       printf("姓名     性别      年龄       语文成绩  数学成绩  英语成绩  平均成绩\n");            
       printf("%-10s%-10c%-10d%-10f%-10f%-10f%-10f\n",stud[i].name,stud[i].sex,stud[i].age,stud[i].score[1],stud[i].score[2],stud[i].score[3],stud[i].score[0]);            
       printf("          1：修改此人信息。\n");
       printf("          2：删除此人信息。\n");
       printf("          3：返回上级菜单。\n");
       printf("***********************************************\n");
       printf("请输入您的选择：\n");
       scanf("%d",&c);
       switch(c)
          {
            case 1:{    
                       sum=0;
                       printf("请输入学生的ID：\n");            
                       scanf("%d",&stud[i].id);         
                       printf("请输入学生的名字：\n");         
                       gets(stud[i].name);         
                       printf("请输入学生的性别：\n");
                       scanf("%c",&stud[i].sex);
                       printf("请输入学生的年龄：\n");
                       scanf("%d",&stud[i].age);
                       printf("请输入学生的语文成绩：\n");            
                       scanf("%f",&stud[i].score[1]); 
                       sum=sum+stud[i].score[1];
                       printf("请输入学生的数学成绩：\n");
                       scanf("%f",&stud[i].score[2]); 
                       sum=sum+stud[i].score[2];
                       printf("请输入学生的英语成绩：\n");            
                       scanf("%f",&stud[i].score[3]); 
                       sum=sum+stud[i].score[3];
                       stud[i].score[0]=sum/3;
                       printf("修改完成。\n");
                       getchar();
                       main1();
                       break; 
                   }        
            case 2:{
                      stud[i].id=stud[n].id;
                      stud[i].age=stud[n].age;
                      stud[i].sex=stud[n].sex;
                      stud[i].score[0]=stud[n].score[0];
                      stud[i].score[1]=stud[n].score[1];
                      stud[i].score[2]=stud[n].score[2];
                      stud[i].score[3]=stud[n].score[3];
                      for(m=0;m<=10;m++)
                         {
                          stud[i].name[m]=stud[n].name[m];              
                         }
                      n--;
                      getchar();
                      main1();
                      break;
                   } 
            case 3:main1();break; 
                
          }
     }
  getchar();

}



int jiemian3()
{
  system("cls");
  int i=0;
  printf("ID        姓名     性别      年龄       语文成绩  数学成绩  英语成绩  平均成绩\n");            
  for(i=1;i<=n;i++)    
     printf("%-10d%-10s%-10c%-10d%-10f%-10f%-10f%-10f\n",stud[i].id,stud[i].name,stud[i].sex,stud[i].age,stud[i].score[1],stud[i].score[2],stud[i].score[3],stud[i].score[0]);            
  getchar();
  getchar();
  main1();  
    
} 



int jiemian4()
{
  system("cls");
  int i=-1,j=0,k=0,m=0;
  int count1=0,count2=0,count3=0,count4=0,count5=0;
  printf("          0：按照平均分降序输出，并统计个分数段人数。\n");  
  printf("          1：按照语文降序输出，并统计个分数段人数。\n");  
  printf("          2：按照数学降序输出，并统计个分数段人数。\n");  
  printf("          3：按照英语降序输出，并统计个分数段人数。\n");  
  printf("***************************************************************\n");
  printf("请输入您的选择：\n");
  scanf("%d",&i);
  for(j=1;j<n;j++)
      {
        for(k=j+1;k<=n;k++)          
            {
               if(stud[j].score[i]<stud[j].score[i])
                   {
                      stud[0].id=stud[j].id;
                      stud[j].id=stud[k].id;                           
                      stud[k].id=stud[0].id;                              
                      stud[0].age=stud[j].age;
                      stud[j].age=stud[k].age;                           
                      stud[k].age=stud[0].age;
                      for(m=0;m<=10;m++)                              
                      {
                         stud[0].name[m]=stud[j].name[m];
                         stud[j].name[m]=stud[k].name[m];                           
                         stud[k].name[m]=stud[0].name[m];                              
                      }
                      stud[0].sex=stud[j].sex;
                      stud[j].sex=stud[k].sex;                           
                      stud[k].sex=stud[0].sex;
                      stud[0].score[0]=stud[j].score[0];
                      stud[j].score[0]=stud[k].score[0];                           
                      stud[k].score[0]=stud[0].score[0];                              
                      stud[0].score[1]=stud[j].score[1];
                      stud[j].score[1]=stud[k].score[1];                           
                      stud[k].score[1]=stud[0].score[1];
                      stud[0].score[2]=stud[j].score[2];
                      stud[j].score[2]=stud[k].score[2];                           
                      stud[k].score[2]=stud[0].score[2];
                      stud[0].score[3]=stud[j].score[3];
                      stud[j].score[3]=stud[k].score[3];                           
                      stud[k].score[3]=stud[0].score[3];                              
                   }                      
            }
      }
  for(j=1;j<=n;j++)
     {
        if(stud[j].score[i]<=100&&stud[j].score[i]>90)
             count1++;          
        else if(stud[j].score[i]<=90&&stud[j].score[i]>80)
             count2++;           
        else if(stud[j].score[i]<=80&&stud[j].score[i]>70)
             count3++;           
        else if(stud[j].score[i]<=70&&stud[j].score[i]>60)
             count4++;           
        else if(stud[j].score[i]<=60)
             count5++;           
     }
  printf("ID        姓名     性别      年龄       语文成绩  数学成绩  英语成绩  平均成绩\n");               
  for(j=1;j<=n;j++)    
     printf("%-10d%-10s%-10c%-10d%-10f%-10f%-10f%-10f\n",stud[j].id,stud[j].name,stud[j].sex,stud[j].age,stud[j].score[1],stud[j].score[2],stud[j].score[3],stud[j].score[0]); 
  printf("分数在90——100的有%d人\n",count1);
  printf("分数在80——90的有%d人\n",count2);  
  printf("分数在70——80的有%d人\n",count3);
  printf("分数在60——70的有%d人\n",count4);
  printf("分数在60分以下的有%d人\n",count5);
  getchar();
  getchar();
  main1();  
}



int jiemian5()
{   
    FILE *fp1;
    int k=0;
    fp1=fopen("学生成绩记录","wb");
    for(k=1;k<=n;k++)
         fwrite(&stud[k],sizeof(struct student),1,fp1);
    fclose(fp1);
    getchar();
    
    
}




































 

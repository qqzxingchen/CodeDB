#include<stdio.h>
#include<stdlib.h>
int func(int num,int *s,int n)
{
  int i;
  if(n==1)
  {
	  for(i=0;i<num;i++)
		  printf("%d",s[i]);
	  printf("\n");
  }
  else 
  {
      int temp=0;
      int *pt=0;
      int j=0;
	  pt=(int *)malloc(num*sizeof(int));
      for(j=0;j<n;j++)
	  {
	     for(i=0;i<num;i++)
	         pt[i]=s[i];
	     temp=pt[num-n];
		 pt[num-n]=pt[num-1-j];
		 pt[num-1-j]=temp;
	     func(num,pt,n-1);
	  
	  }
      free(pt); 
  }

  return 0;
}



int main()
{
  int ncount=0;
  int *s=0;
  int i;
  printf("please input the number\n");
  scanf("%d",&ncount);
  
  s=(int *)malloc(ncount*sizeof(int));
  
  for(i=0;i<ncount;i++)
  {
	  s[i]=i+1;
  }

  func(ncount,s,ncount);
  system("pause"); 
  return 0;
}

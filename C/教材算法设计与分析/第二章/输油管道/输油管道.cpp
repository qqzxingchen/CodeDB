#include<iostream>
using namespace std;
class point
{
public:
   int N;
   int s[10000][2];
   int minsum;
   point()
   {
      N=0;
	  minsum=0;
	  for(int i=0;i<10000;i++)
	  {
		 s[i][0]=0;
		 s[i][1]=0;
	  }
   }
   int input()
   {
      FILE *fp;
	  fp=fopen("input.txt","r");
      fscanf(fp,"%d",&N);
	  for(int i=0;i<N;i++)
          fscanf(fp,"%d %d",&s[i][0],&s[i][1]);
      fclose(fp);
	  return 0;
   }
   int output()
   {
      FILE *fp;
      fp=fopen("output.txt","w");
      fprintf(fp,"%d",minsum);
	  
      printf("%d\n",minsum);
	  for(int i=0;i<N;i++)
		  printf("%d ",s[i][1]);

	  fclose(fp);
	  return 0;
   }
   int quicksort(int low,int high);  
   int partition(int low,int high);
   int min()
   {
       int temp;
	   int i=0;
	   int j=N-1;
	   while(i<j-1)
	   {
	       i++;
	       j--;
	   }
	   temp=s[i][1];
       for(i=0;i<N;i++)
	   {
	       if(s[i][1]>temp)
			   minsum=minsum+s[i][1]-temp;
		   else
			   minsum=minsum+temp-s[i][1];
	   }
	   return 0;
   }
};
int main()
{
   point lol;
   lol.input();
   lol.quicksort(0,lol.N-1);
   lol.min();
   lol.output();
   return 0;
}






int point::quicksort(int low,int high)
{
    if(low<high)
	{
		int q=partition(low,high);
	    quicksort(low,q-1);
	    quicksort(q+1,high);
	}
	return 0;
}
int point::partition(int low,int high)     //->sort::quicksort()
{
    int i=low+1;
	int j=high;
	int temp=s[low][1];
	while(1)
    {
        while(s[i][1]<temp && i<j)	
	       i++;
		while(s[j][1]>temp)
	       j--;
		if(i>=j)  break;
		s[i][1]=s[i][1]+s[j][1];
		s[j][1]=s[i][1]-s[j][1];
		s[i][1]=s[i][1]-s[j][1];
	}
	s[low][1]=s[j][1];
	s[j][1]=temp;
	return j;
}
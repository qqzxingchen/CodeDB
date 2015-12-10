/*
计算机算法设计与分析  p44

 士兵站队
*/

#include<iostream>
using namespace std;
class solider
{
public:
	int N;
	int X[10000];
	int Y[10000];
	int step;
	solider()
	{
	    N=0;
		step=0;
		for(int i=0;i<10000;i++)
		{
		    X[i]=0;
		    Y[i]=0;
		}
	}
	int input()
	{
	    FILE *fp;
		fp=fopen("input.txt","r");
		fscanf(fp,"%d",&N);
		for(int i=0;i<N;i++)
		{
		    fscanf(fp,"%d %d",&X[i],&Y[i]);

			//printf("%d %d",X[i],Y[i]);
		}
		fclose(fp);
		return 0;
	}
	int output()
    {
	    FILE *fp;
		fp=fopen("output.txt","w");
	    fprintf(fp,"%d",step);
	    fclose(fp);
	    return 0;
	}
	int quicksort(int *s,int low,int high);
	int partition(int *s,int low,int high);
    int process()
	{
	    int tempx=X[(N-1)/2];
	    int tempy=Y[(N-1)/2];
	    for(int i=0;i<N;i++)
		{
		   if(tempy>Y[i])  step+=tempy-Y[i];
		   else            step+=Y[i]-tempy;
		}
		for(i=(N-1)/2;i>=0;i--)
		{
		   if(tempx-(N-1)/2+i>X[i])  step+=tempx-(N-1)/2+i-X[i];
		   else                      step+=-tempx+(N-1)/2-i+X[i];
		}
		for(i=(N-1)/2+1;i<N;i++)
		{
		   if(tempx+i-(N-1)/2>X[i])  step+=tempx+i-(N-1)/2-X[i];
		   else                      step+=-(tempx+i-(N-1)/2-X[i]);
		}
	    return 0;
	}

};
int solider::quicksort(int *s,int low,int high)
{
	if(low<high)
	{
		int q=partition(s,low,high);
	    quicksort(s,low,q-1);
	    quicksort(s,q+1,high);
	}
	return 0;
}
int solider::partition(int *s,int low,int high)
{
    int i=low+1;
	int j=high;
	int temp=s[low];
	while(1)
    {
        while(s[i]<temp && i<j)	
	       i++;
		while(s[j]>temp)
	       j--;
		if(i>=j)  break;
		s[i]=s[i]+s[j];
		s[j]=s[i]-s[j];
		s[i]=s[i]-s[j];
	}
	s[low]=s[j];
	s[j]=temp;
	return j;

}
int main()
{
    solider lol;
	lol.input();
	lol.quicksort(lol.X,0,lol.N-1);
	lol.quicksort(lol.Y,0,lol.N-1);
	lol.process();
	lol.output();
    return 0;

}

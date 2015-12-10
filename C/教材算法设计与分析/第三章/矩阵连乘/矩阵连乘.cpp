#include<iostream>
using namespace std;
class point
{
public:
	int s[100];
	int N;
	int step;
	point()
	{
	   step=0;
	   N=0;
	   for(int i=0;i<100;i++)
	      s[i]=0;
	}
	void input()
	{
	   cin>>N;
	   for(int i=0;i<=N;i++)
	   {
		   cin>>s[i];
	   }
	}
	int min(int start,int end)
	{
	   int te=0;
	   int temp=0;
	   if(start==end)
		  return 0;
	   else
	   {
	      for(int i=start;i<end;i++)
		  {
	         temp=min(start,i)+min(i+1,end)+s[start-1]*s[i]*s[end];
	         if( i==start || (i!=start && te>temp) )
				 te=temp;
		  }//for
	      return te;

	   }//else

	}//min()

};//class


int main()
{
    point lol;
	lol.input();
	int k=lol.min(1,lol.N);
	
    cout<<"it is "<<k<<endl;

    return 0;

}
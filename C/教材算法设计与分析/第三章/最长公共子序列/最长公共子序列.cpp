#include<iostream>
using namespace std;
#define M 100
class point
{
public:
    char X[M];      
    char Y[M];      
    char C[M][M];
	char B[M][M];
    int x,y;      
    point()
    {
       x=0;       
       y=0;    
       for(int i=0;i<M;i++)
       {
           X[i]='\0';        
           Y[i]='\0';    
           for(int j=0;j<M;j++)
           {
		      C[i][j]='\0';
		      B[i][j]='\0';
		   }    
       }    
    }  
    int input()
    {
       int i;
       cout<<"please input the first  string"<<endl; 
       gets(X);
       i=0; while(X[i]) {i++;} x=i;  
	        while(i>=1) {X[i]=X[i-1];i--;} 
       cout<<"please input the second string"<<endl; 
       gets(Y);
       i=0; while(Y[i]) {i++;} y=i; 
	        while(i>=1) {Y[i]=Y[i-1];i--;}
       
	   return 0;
    }
	void output()
	{ 
		cout<<endl;
		int i;
	    for(i=1;i<=x;i++)
		{
		   cout<<"i="<<i<<"    ";
		   for(int j=1;j<=y;j++)
		       cout<<(int)C[i][j]<<"  ";
		   cout<<endl;
		}

        cout<<endl<<endl;
		for(i=1;i<=x;i++)
		{
		   cout<<"i="<<i<<"    ";
		   for(int j=1;j<=y;j++)
		       cout<<(int)B[i][j]<<"  ";
		   cout<<endl;
		}
	
	}
    
	void LCSLength()
	{
	    for(int i=1;i<=x;i++)
		   for(int j=1;j<=y;j++)
           {
			   if(X[i]==Y[j])                  { C[i][j]=C[i-1][j-1]+1;B[i][j]=1; }
			   else if(C[i-1][j]>=C[i][j-1])   { C[i][j]=C[i-1][j];    B[i][j]=2; }
			   else                            { C[i][j]=C[i][j-1];    B[i][j]=3; }
		   }	
	}
    void LCS(int i,int j)
	{
	    if( i==0||j==0 )      return ;
		if( B[i][j]==1 )      { LCS(i-1,j-1);cout<<X[i]; }
		else if(B[i][j]==2 )  { LCS(i-1,j);  }
		else                  { LCS(i,j-1);  }
	}

    
      
      
      
}; 
int main()
{
   point lol;
   lol.input();
   lol.LCSLength();
   cout<<"output"<<endl;
   lol.LCS(lol.x,lol.y);
   lol.output();
   return 0;

}

#include<iostream>
#include<stdlib.h>
using namespace std;
class point
{
public:
   int SIZE;
   int step;
   int S[100][100];
   int X,Y;
   point()
   {
      step=0;
      SIZE=0;
      X=0;
      Y=0;
      for(int i=0;i<100;i++)
         for(int j=0;j<10;j++)
         S[i][j]=0;       
   }
   ~point()
   {
      for(int i=0;i<SIZE;i++)
      {
         for(int j=0;j<SIZE;j++)        
             printf("%4d",S[i][j]);
         cout<<endl; 
      }  
      system("pause");         
   }
   int input()
   {
      cout<<"please input the size:"<<endl;
      cin>>SIZE;
      cout<<"please input x and y"<<endl;
      cin>>X>>Y;
	  S[X][Y]=-1;
      return 0;    
   }
   int cover(int startx,int starty,int x,int y,int size)
   {
       if(size==1)
          return 0;
       int t=step++;
       int s=size/2;
       
       
       if( x < startx+s && y < starty+s )cover(startx  ,starty  ,x,y,s);  else{S[startx+s-1][starty+s-1]=t;cover(startx  , starty  , startx+s-1, starty+s-1,s); } 
       if( x < startx+s && y >=starty+s )cover(startx  ,starty+s,x,y,s);  else{S[startx+s-1][starty+s  ]=t;cover(startx  , starty+s, startx+s-1, starty+s  ,s); }
       if( x >=startx+s && y < starty+s )cover(startx+s,starty  ,x,y,s);  else{S[startx+s  ][starty+s-1]=t;cover(startx+s, starty  , startx+s  , starty+s-1,s); }
       if( x >=startx+s && y >=starty+s )cover(startx+s,starty+s,x,y,s);  else{S[startx+s  ][starty+s  ]=t;cover(startx+s, starty+s, startx+s  , starty+s  ,s); }
       
   }      
      
};
int main()
{
    point lol;
    lol.input();
    lol.cover(0,0,lol.X,lol.Y,lol.SIZE);    
    return 0;
}

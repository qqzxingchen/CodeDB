#include<iostream>
using namespace std;
#define max 1000
class sort
{
public:
   int s[max];   
   int n;  
   int ch;
   sort()
   {
	  ch=0;
      n=0;
      for(int i=0;i<max;i++)   
         s[i]=0;
   }
   ~sort()
   {
      for(int i=0;i<n;i++)    
         cout<<s[i]<<"  "; 
   } 
   int input()
   {
      cout<<"please input the num"<<endl; 
      cin>>n;
      for(int i=0;i<n;i++)
         cin>>s[i];
	  return 0;
   }
   int jiemian()
   {
       cout<<"you can choose :"<<endl;  
       cout<<"1: bubble sort"<<endl;
       cout<<"2: quick sort "<<endl;
       

	   cin>>ch;
	   return 0;
   
   }
   int choose()
   {
       switch(ch)
	   {
	      case 1: bubblesort(); break;  
		  case 2: quicksort(0,n-1); break;
		  default : cout<<"please input again"<<endl;
			        jiemian();
					break;
	   }
	   return 0;
   }

   int quicksort(int low,int high);  int partition(int low,int high);  //¿ìÅÅ
   int bubblesort();                                                   //ÆðÅÝ


};


int sort::bubblesort()
{
   int i,j;
   for(i=0;i<n-1;i++)
	  for(j=i+1;j<n;j++)
		  if(s[i]>s[j])
		  {
	          s[i]=s[i]+s[j];
			  s[j]=s[i]-s[j];
			  s[i]=s[i]-s[j];
		  }
   return 0;
}


int sort::quicksort(int low,int high)
{
    if(low<high)
	{
		int q=partition(low,high);
	    quicksort(low,q-1);
	    quicksort(q+1,high);
	}
	return 0;
}
int sort::partition(int low,int high)     //->sort::quicksort()
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
   sort lol;
   lol.input(); 
   lol.jiemian();
   lol.choose();
   return 0; 
}

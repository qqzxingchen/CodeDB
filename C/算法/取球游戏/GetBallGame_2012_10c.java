import java.util.LinkedList;
import java.util.Scanner;
public class GetBallGame_2012_10c {
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Scanner sc=new Scanner(System.in);
		LinkedList<Integer> numListIn=new LinkedList<Integer>();
		int n=sc.nextInt();
numListIn.offer(sc.nextInt());
		for(int i=1;i<n;i++)
		{
			numListIn.offer(sc.nextInt());
		}
		
		for(int i=0;i<n;i++)
		{
			int result=GetBall(numListIn.poll())?1:0;
			System.out.println(result);
		}
		
	}
	
	public static boolean GetBall (int num) 
	{
		if(num>=1)
		{
			switch (num) {
			case 1:
				return false;//当轮到A取球的时候只剩一个球，则必输
				
			case 3:
				return false;//剩3个时 有两种策略 1.A拿1个-B拿1个-A拿一个（输） 2.A拿3个（输）
				
			case 7:
				return false;//同上
				
			case 8:
				return true;
				
			//如果不是不是1，3，7，8则 选择权交给B，B任然调用该函数，不过返回值需要取反
			//而此时A可以选的只有 1 3 7 8 所以用num减去之  逐个测试即可
			default:
				return (!GetBall(num-8)||!GetBall(num-7)||!GetBall(num-3)||!GetBall(num-1));
			}
		}
		else 
			return false;
		
	}

}

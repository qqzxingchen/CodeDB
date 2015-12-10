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
				return false;//���ֵ�Aȡ���ʱ��ֻʣһ���������
				
			case 3:
				return false;//ʣ3��ʱ �����ֲ��� 1.A��1��-B��1��-A��һ�����䣩 2.A��3�����䣩
				
			case 7:
				return false;//ͬ��
				
			case 8:
				return true;
				
			//������ǲ���1��3��7��8�� ѡ��Ȩ����B��B��Ȼ���øú�������������ֵ��Ҫȡ��
			//����ʱA����ѡ��ֻ�� 1 3 7 8 ������num��ȥ֮  ������Լ���
			default:
				return (!GetBall(num-8)||!GetBall(num-7)||!GetBall(num-3)||!GetBall(num-1));
			}
		}
		else 
			return false;
		
	}

}

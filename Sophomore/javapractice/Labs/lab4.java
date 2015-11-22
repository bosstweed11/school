
import java.util.*;

public class lab4
	{
	public static void main(String [] args)
		{
			
			Random R = new Random();
			int a=0,num0=0,num1=0,num2=0,num3=0,num4=0,num5=0,num6=0,num7=0;
			
			for(int i = 0; i <8000; i++)
			{
			int b = R.nextInt(2);
			int c = R.nextInt(2);
			int d = R.nextInt(2);
			System.out.println("The first number is "+ b);
			System.out.println("The second nubmer is " + c);
			System.out.println("The third number is " +d);
			System.out.println("The binary interpretation of this " + b +""+c+""+d);
			int e = b*4 + c*2 + d*1;
			System.out.println("The decimal interpretation of this is " + e);
			
			if( e==0)
			num0++;
			if( e==1)
			num1++;
			if( e==2)
			num2++;
			if( e==3)
			num3++;
			if( e==4)
			num4++;
			if( e==5)
			num5++;
			if( e==6)
			num6++;
			if( e==7)
			num7++;
			
			}
			
			System.out.println("Statistics: \n\t 0 =" + num0 + "\n\t1 =" + num1+ "\n\t 2 =" + num2+ "\n\t 3 =" + num3+ "\n\t 4 =" + num4+ "\n\t 5 =" + num5+ "\n\t 6 =" + num6+ "\n\t 7 =" + num7);
			
			
	
	
		}
	
	}
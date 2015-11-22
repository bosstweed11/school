import java.util.*;

public class lab6
{
	
	public static void main(String [] args)
	{
		
		int a,i =0;
		double[] B;
		double k,j,l,m,n;
		Scanner inScan = new Scanner(System.in);
		
		System.out.println("enter the number of items you'd like: ");
		a = inScan.nextInt();
		B = new double [a];
		for(i=0; i<B.length; i++)
			{
			System.out.println("enter next int" );
			j = inScan.nextDouble();
			B[i]=j;
			
			}
			
		k = max(B);
		l = min(B);
		m = sum(B);
		n = ave(B);
		
		System.out.println("max = " + k+ "min= " +l +"sum=" + m + "ave=" +n);
	}
	
	public static double max(double [] data)
	{
		int i =0;
		double max=data[0];
		for(i=0;i<data.length;i++)
		{
			if(max < data[i])
				max = data[i];
			
		}
		return(max);
	}
	public static double min(double [] data)
	{
		int i =0;
		double min=data[0];
		for(i=0;i<data.length;i++)
		{
			if(min > data[i])
				min = data[i];
			
		}
		return(min);
	}
		public static double sum(double [] data)
	{
		int i =0;
		double sum=0;
		for(i=0;i<data.length;i++)
		{
			
				sum += data[i];
			
		}
		return(sum);
	}
		public static double ave(double [] data)
	{
		int i =0;
		double sum=0;
		for(i=0;i<data.length;i++)
		{
			
				sum += data[i];
			
		}
		return(sum/i);
	}
}
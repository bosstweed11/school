import java.util.*;
public class kmaps
{

	public static void main (String [] args)
	{
		Scanner scan = new Scanner(System.in);
		int a,t=0,c=0,k,l;
		boolean redo =true;
		Random R = new Random();
		System.out.println("How many minterms does your function have?");
		k = scan.nextInt();
		
		a=R.nextInt(k);
		int[] b = new int[a];
		int[] d = new int[a];
		while(t<a)
		{
			b[t] = R.nextInt(k);
		
			if(t!=0)
			{	
				redo = true;
				while(redo)
				{
					redo =false;
					for(int i =0;i<t;i++)
					{
						
						if(b[t] == d[i])
						{
							b[t] =R.nextInt(k);
							
							redo = true;
						}
						
					}	
				}
				
			}	
		
	
		
			d[t]=b[t];
		
			t++;
			
			
		}
		Arrays.sort(b);
		
		System.out.println("Solve K-maps for the following function of minterms: ");
		System.out.print("[");
		for(int i=0; i<b.length;i++)
		{	
			if(i == b.length-1)
			{
				System.out.print(b[i]);
			}
			else
				System.out.print(b[i] + ",");
		}
		System.out.println("]");
	
	}
	
}
public class A
{
	public static void main(String [] args)
	{
		int []i = new int[10];
	
		for(int j =0; j<i.length;j++)
		{
			i[j] = j;
		}
		System.out.println("Before Reverser: ");
		for(int j =0; j<i.length;j++)
		{
			System.out.println(i[j]);
		}
		int[] temp = new int[10];
		for(int j=0; j<i.length;j++)
		{
			temp[j] = i[j];
		
		}
		System.out.println("temp: ");
		for(int j =0; j<i.length;j++)
		{
			System.out.println(temp[j]);
		}
		for(int j=0; j<i.length;j++)
		{
			int k = i.length-(j+1);
			System.out.println("K = " +k);
				i[j] = temp[k];
		}
		System.out.println("After Reverser: ");
		for(int j =0; j<i.length;j++)
		{
			System.out.println(i[j]);
		}
	
	}
}
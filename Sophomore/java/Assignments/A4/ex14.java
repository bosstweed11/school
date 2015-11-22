public class ex14
{
	public static void initData(int [] data, int multVal)
	{
		for (int i=0; i<data.length; i++)
		{
			data[i] = (i*multVal) % data.length;
		}
	}
	
	public static void showData(int [] Ar, int perLine)
	{
		System.out.println("The data is: ");
		for (int i=0; i<Ar.length-1; i++)
		{
			System.out.println(Ar[i] + ", ");
			if((i+1) % perLine == 0)
				System.out.println();
		}
		
		if( Ar.length> 0)
			System.out.println(Ar[Ar.length-1]);
		System.out.println();
	}
	
	public static void main(String [] args)
	{
		int [] A = new int[23];
		int [] B = new int[29];
		initData(A,11);
		initData(B,17);
		showData(A,6); System.out.println();
		showData(B,8); System.out.println();
		
		SortInt.selectionSort(A);
		SortInt.insertionSort(B);
		
		showData(A,6); System.out.println();
		showData(B,8); System.out.println();
	}
}
	
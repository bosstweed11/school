import java.util.*;
 /*  public static void selectionSort(int [] array)
   {
      int startScan, index, minIndex, minValue;

      for (startScan = 0; startScan < (array.length-1); startScan++)
      {
         minIndex = startScan;
         minValue = array[startScan];
         for(index = startScan + 1; index < array.length; index++)
         {
            if (array[index] < minValue)
            {
               minValue = array[index];
               minIndex = index;
            }
         }
         array[minIndex] = array[startScan];
         array[startScan] = minValue;
      }
   }*/
public class Sort
{

	public static void main(String [] args)
	{
		Random R = new Random();
		int [] A = new int[10];
		int [] B = new int[10];
		for(int i = 0 ; i<A.length;i++)
		{
			A[i] = 10-i;
			B[i] = R.nextInt(560);
		}
		System.out.println("Array A(before sort): ");
		for(int i = 0; i<A.length;i++)
		{
				
			System.out.println(A[i]);
		}
		System.out.println("Array B(before sort): ");
			for(int i = 0; i<B.length;i++)
		{
		
			System.out.println(B[i]);
		}
		selectionSort(A);
		insertionSort(B);
	}
	
   
	public static void selectionSort(int [] array)
	{
		int index, minIndex,minValue;
	
		for(int i = 0 ; i < (array.length-1); i++)
		{
			minIndex = i;
			minValue = array[i];
			
			for(index = i +1; index <array.length; index++)
			{
				if(array[index] < minValue)
				{
					minValue = array[index];
					minIndex = index;
				}
			}
			array[minIndex] = array[i];
			array[i] = minValue;
		}
		System.out.println("Array A(after sort): ");
		for(int i = 0; i<array.length;i++)
		{
			
			System.out.println(array[i]);
		}
	}

	public static void insertionSort(int [] array)
	{
	
		for( int i =1; i < array.length; i++)
		{
			int key = array[i];
			int position = i;
		
			while ( position >0 && key <array[position-1])
			{
				array[position] = array[position-1];
				position--;
			}
		array[position] = key;
		}
		System.out.println("Array B(after sort): ");
		for(int i = 0; i<array.length;i++)
		{
			
			System.out.println(array[i]);
		}
	}








}

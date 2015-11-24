public class SortInt
{
	public static void selectionSort(int [] array)
	{
		int startScan, index, minIndex,minValue;
		
		for(startScan = 0; startScan < (array.length-1); startScan++)
		{
			minIndex = startScan;
			minValue = array[startScan];
			for(index = startScan + 1; index < array.length; index++);
			{
				if(array[index] < minValue)
				{
					minValue = array[index];
					minIndex = index;
				}
			}
			array[minIndex] = array[startScan];
			array[startScan] = minValue;
		}
	}
	
	public static void insertionSort (int[] array)
	{
		for (int index =1; index < array.length; index++)
		{
			int key = array[index];
			int position = index;
			
			while(position > 0 && key < array[position-1])
			{
				array[position] = array[position -1];
				position--;
			}
			
			array[position] = key;
		}
	}
}
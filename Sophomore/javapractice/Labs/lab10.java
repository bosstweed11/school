// CS 0401 Fall 2011
// Lab 10
// You must modify this file so that it works as described in the lab handout.
import java.util.*;
import java.io.*;
public class lab10
{
	public static void main(String [] args)
	{
		boolean file = false;
		boolean caught = false;
		Scanner inScan, fScan = null;
		int k = 5;
		int [] A = new int[k];
		inScan = new Scanner(System.in);
		System.out.println("Please enter the file to read from: ");
		String fName = inScan.nextLine();
		while(!file)
		{
			try
			{
				fScan = new Scanner(new File(fName));
				file = true;
			}
			catch(FileNotFoundException d)
			{
				System.out.println("Your file is invalid -- please re-enter");
				System.out.println("Please enter the file to read from: ");
				fName = inScan.nextLine();
			}
		}
		String nextItem;
		int nextInt = 0;
		int i = 0;
		
		while (fScan.hasNextLine())
		{
			
			nextItem = fScan.nextLine();
			try
			{
				nextInt = Integer.parseInt(nextItem);
			}
			catch(NumberFormatException d)
			{
				System.out.println(nextItem + " is not an integer -- ignored.");
				caught = true;
			}
			
			try
			{
				A[i] = nextInt;
			}
			catch(ArrayIndexOutOfBoundsException d)
			{
				
				System.out.println("Resizing array from " + A.length + " to " + 2*A.length);
				A = resize(A);
				A[i] = nextInt;
			}
			if(!caught)
				i++;
			caught = false;
		}

		System.out.println("Here are your " + i + " items:");
		for (int j = 0; j < i; j++)
		{
			System.out.println(A[j] + " ");
		}
	}
	
	private static int[] resize (int[] array) 
	{
	int[] newArray = new int[array.length * 2];

	for (int i=0; i<array.length; i++)
	{
		newArray[i] = array[i];
	}
	return( newArray);
	}
}


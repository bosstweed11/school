// Written By Andrew Beers 3/21

import java.util.*;
import java.io.*;
public class Assignment4 implements Runnable
{
	private static int size,trials;
	private static Integer [] S;
	private static Integer [] Ra;
	private static Integer [] Re;
	private static Integer [] Hold;
	private static Integer [] Hold1;
	
	private static String file;
	private static double d = 1000000000.0,t=0,t1=0,t2=0,t3=0,t4=0,t5=0,t6=0,t7=0,t8=0,t9=0,t10=0,t11=0,t12=0,t13=0,t14=0,t15=0,t16=0,t17=0,t18=0,t19=0,t20=0,t21=0,t22=0,t23=0,time,time1,time2,time3,time4,time5,time6,time7,time8,time9,time10,time11,time12,time13,time14,time15,time16,time17,time18,time19,time20,time21,time22,time23;
	
	//Main program, start new class object
	public static void main(String [] args) throws IOException
	{
		Assignment4 prog = new Assignment4();
	}
	//Run class object through a thread to account for stack manipulation
	public Assignment4() throws IOException
	{
		(new Thread(this)).start();
	}

	
	//Main of the program
	public void run() 
	{
		Scanner scan = new Scanner(System.in);
		
		
		long start,finish,delta;
	
		//Input data
		System.out.print("Enter array size:");
		size = Integer.parseInt(scan.nextLine());
		System.out.print("Enter number of trials: ");
		trials = Integer.parseInt(scan.nextLine());
		System.out.print("Enter the name of the file to write to: ");
		file = scan.nextLine();
		
		S = new Integer[size];
		Ra =new Integer[size];
		Re = new Integer[size];
		
		//Main Trial loop
		for(int i=0;i<trials;i++)
		{
			//Fill arrays for sorting,make a copy of random and reversed sorted data for after they are sorted
			fillArrays();
			copyRandom();
			copyReversed();
			
			//Trace mode 
			if(size<=20)
			{
				System.out.println("Before Simple QuickSort\n");
				showArrays();
				System.out.println();
			}
			//Simple quick sort, sorted data, random data, reversed data
			start = System.nanoTime();
			Quick.quickSort(S,size);
			finish = System.nanoTime();
			delta = finish-start;
			time = delta/d;
			
	
			start = System.nanoTime();
			Quick.quickSort(Ra,size);
			finish = System.nanoTime();
			delta = finish-start;
			time1 = delta/d;
			
			
			start = System.nanoTime();
			Quick.quickSort(Re,size);
			finish = System.nanoTime();
			delta = finish-start;
			time2 = delta/d;
			
			if(size<=20)
			{
				System.out.println("\nAfter Simple QuickSort\n");
				showArrays();
				System.out.println();
				System.out.println("Time(Sorted): " + time);
				System.out.println("Time(Random): " + time1);
				System.out.println("Time(Reversed): " + time2);
			}
			
			//Recover arrays by assigning them to the copy, make new copy
			Ra = Hold;
			Re = Hold1;
			copyReversed();
			copyRandom();
			
			if(size<=20)
			{
				System.out.println("\nBefore QuickSort Median of 3(Base case 5)\n");
				showArrays();
				System.out.println();
			}
			
			//Median of 3(5), sorted data, random data, reversed data
			start = System.nanoTime();
			TextMergeQuick.quickSort(S,size);
			finish = System.nanoTime();
			delta = finish-start;
			time3 = delta/d;
			
	
			start = System.nanoTime();
			TextMergeQuick.quickSort(Ra,size);
			finish = System.nanoTime();
			delta = finish-start;
			time4 = delta/d;
			
			
			start = System.nanoTime();
			TextMergeQuick.quickSort(Re,size);
			finish = System.nanoTime();
			delta = finish-start;
			time5 = delta/d;
			
			if(size<=20)
			{
				System.out.println("\nAfter QuickSort Median of 3(Base case 5)\n");
				showArrays();
				System.out.println();
				System.out.println("Time(Sorted): " + time3);
				System.out.println("Time(Random): " + time4);
				System.out.println("Time(Reversed): " + time5);
			}
			
			
			Ra = Hold;
			copyRandom();
			Re = Hold1;
			copyReversed();
			
			if(size<=20)
			{
				System.out.println("\nBefore QuickSort Median of 3(Base case 10)\n");
				showArrays();
				System.out.println();
			}
			
			//median of 3(10), sorted data, random data, reversed data
			start = System.nanoTime();
			TextMergeQuick1.quickSort(S,size);
			finish = System.nanoTime();
			delta = finish-start;
			time6 = delta/d;
			
	
			start = System.nanoTime();
			TextMergeQuick1.quickSort(Ra,size);
			finish = System.nanoTime();
			delta = finish-start;
			time7 = delta/d;
			
			
			start = System.nanoTime();
			TextMergeQuick1.quickSort(Re,size);
			finish = System.nanoTime();
			delta = finish-start;
			time8 = delta/d;
			
			if(size<=20)
			{
				System.out.println("\nAfter QuickSort Median of 3(Base case 10)\n");
				showArrays();
				System.out.println();
				System.out.println("Time(Sorted): " + time6);
				System.out.println("Time(Random): " + time7);
				System.out.println("Time(Reversed): " + time8);
			}
			
			
			
			Ra = Hold;
			copyRandom();
			Re = Hold1;
			copyReversed();
			
			if(size<=20)
			{
				System.out.println("\nBefore QuickSort Median of 3(Base case 20)\n");
				showArrays();
				System.out.println();
			}
			
			//median of 3(20), sorted data, random data, reversed data
			start = System.nanoTime();
			TextMergeQuick2.quickSort(S,size);
			finish = System.nanoTime();
			delta = finish-start;
			time9 = delta/d;
			
	
			start = System.nanoTime();
			TextMergeQuick2.quickSort(Ra,size);
			finish = System.nanoTime();
			delta = finish-start;
			time10 = delta/d;
			
			
			start = System.nanoTime();
			TextMergeQuick2.quickSort(Re,size);
			finish = System.nanoTime();
			delta = finish-start;
			time11 = delta/d;
			
			if(size<=20)
			{
				System.out.println("\nAfter QuickSort Median of 3(Base case 20)\n");
				showArrays();
				System.out.println();
				System.out.println("Time(Sorted): " + time9);
				System.out.println("Time(Random): " + time10);
				System.out.println("Time(Reversed): " + time11);
			}
			
			
			Ra = Hold;
			//copyRandom();
			Re = Hold1;
			//copyReversed();
			
			if(size<=20)
			{
				System.out.println("\nBefore random QuickSort\n");
				showArrays();
				System.out.println();
			}
			
			
			//Random quick sort, sorted data, random data, reversed data
			start = System.nanoTime();
			Quick1.quickSort(S,size);
			finish = System.nanoTime();
			delta = finish-start;
			time12 = delta/d;
			
	
			start = System.nanoTime();
			Quick1.quickSort(Ra,size);
			finish = System.nanoTime();
			delta = finish-start;
			time13 = delta/d;
			
			
			start = System.nanoTime();
			Quick1.quickSort(Re,size);
			finish = System.nanoTime();
			delta = finish-start;
			time14 = delta/d;
			
			if(size<=20)
			{
				System.out.println("\nAfter random QuickSort\n");
				showArrays();
				System.out.println();
				System.out.println("Time(Sorted): " + time12);
				System.out.println("Time(Random): " + time13);
				System.out.println("Time(Reversed): " + time14);
			}
			
			fillArrays();
			Ra = Hold;
			copyRandom();
			
			if(size<=20)
			{
				System.out.println("\nBefore MergeSort\n");
				showArrays();
				System.out.println();
			}
			
			
			//merge sort, sorted data, random data, reversed data
			start = System.nanoTime();
			TextMergeQuick.mergeSort(S,size);
			finish = System.nanoTime();
			delta = finish-start;
			time15 = delta/d;
			
	
			start = System.nanoTime();
			TextMergeQuick.mergeSort(Ra,size);
			finish = System.nanoTime();
			delta = finish-start;
			time16 = delta/d;
			
			
			start = System.nanoTime();
			TextMergeQuick.mergeSort(Re,size);
			finish = System.nanoTime();
			delta = finish-start;
			time17 = delta/d;
			
			if(size<=20)
			{
				System.out.println("\nAfter MergeSort\n");
				showArrays();
				System.out.println();
				System.out.println("Time(Sorted): " + time15);
				System.out.println("Time(Random): " + time16);
				System.out.println("Time(Reversed): " + time17);
			}
			
			fillArrays();
			Ra = Hold;
			copyRandom();
			
			if(size<=20)
			{
				System.out.println("\nBefore InsertionSort\n");
				showArrays();
				System.out.println();
			}
			
			
			//Insertion sort, sorted data, random data, reversed data
			start = System.nanoTime();
			TextMergeQuick.insertionSort(S,size);
			finish = System.nanoTime();
			delta = finish-start;
			time18 = delta/d;
			
	
			start = System.nanoTime();
			TextMergeQuick.insertionSort(Ra,size);
			finish = System.nanoTime();
			delta = finish-start;
			time19 = delta/d;
			
			
			start = System.nanoTime();
			TextMergeQuick.insertionSort(Re,size);
			finish = System.nanoTime();
			delta = finish-start;
			time20 = delta/d;
			
			if(size<=20)
			{
				System.out.println("\nAfter InsertionSort\n");
				showArrays();
				System.out.println();
				System.out.println("Time(Sorted): " + time18);
				System.out.println("Time(Random): " + time19);
				System.out.println("Time(Reversed): " + time20);
			}
			//Total time variables for all sorts
			t += time;
			t1 += time1;
			t2 += time2;
			t3 += time3;
			t4 += time4;
			t5 += time5;
			t6 += time6;
			t7 += time7;
			t8 += time8;
			t9 += time9;
			t10 += time10;
			t11 += time11;
			t12 += time12;
			t13 += time13;
			t14 += time14;
			t15 += time15;
			t16 += time16;
			t17 += time17;
			t18 += time18;
			t19 += time19;
			t20 += time20;
			t21 += time21;
			t22 += time22;
			t23 += time23;
		
			Hold = null;
		
		}
		//average time for all sorts
		t = t/trials;
		t1 = t1/trials;
		t2 = t2/trials;
		t3 = t3/trials;
		t4 = t4/trials;
		t5 = t5/trials;
		t6 = t6/trials;
		t7 = t7/trials;
		t8 = t8/trials;
		t9 = t9/trials;
		t10 = t10/trials;
		t11 = t11/trials;
		t12 = t12/trials;
		t13 = t13/trials;
		t14 = t14/trials;
		t15 = t15/trials;
		t16 = t16/trials;
		t17 = t17/trials;
		t18 = t18/trials;
		t19 = t19/trials;
		t20 = t20/trials;
		t21 = t21/trials;
		t22 = t22/trials;
		t23 = t23/trials;
		
		try
		{
			
			toFile();
		}
		catch(Exception a)
		{}
		
		//Print out results
		System.out.println("Algorithm: Simple QuickSort\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t + " sec.\n");
		System.out.println("Algorithm: Simple QuickSort\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t1 + " sec.\n");
		System.out.println("Algorithm: Simple QuickSort\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t2 + " sec.\n");
		System.out.println("Algorithm: Median of 3 QuickSort(min 5)\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t3 + " sec.\n");
		System.out.println("Algorithm: Median of 3 QuickSort(min 5)\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t4 + " sec.\n");
		System.out.println("Algorithm: Median of 3 QuickSort(min 5)\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t5 + " sec.\n");
		System.out.println("Algorithm: Median of 3 QuickSort(min 10)\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t6 + " sec.\n");
		System.out.println("Algorithm: Median of 3 QuickSort(min 10)\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t7 + " sec.\n");
		System.out.println("Algorithm: Median of 3 QuickSort(min 10)\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t8 + " sec.\n");
		System.out.println("Algorithm: Median of 3 QuickSort(min 20)\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t9 + " sec.\n");
		System.out.println("Algorithm: Median of 3 QuickSort(min 20)\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t10 + " sec.\n");
		System.out.println("Algorithm: Median of 3 QuickSort(min 20)\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t11 + " sec.\n");
		System.out.println("Algorithm: Random Pivot QuickSort\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t12 + " sec.\n");
		System.out.println("Algorithm: Random Pivot QuickSort\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t13 + " sec.\n");
		System.out.println("Algorithm: Random Pivot QuickSort\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t14 + " sec.\n");
		System.out.println("Algorithm: MergeSort\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t15 + " sec.\n");
		System.out.println("Algorithm: MergeSort\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t16 + " sec.\n");
		System.out.println("Algorithm: MergeSort\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t17 + " sec.\n");
		System.out.println("Algorithm: InsertionSort\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t18 + " sec.\n");
		System.out.println("Algorithm: InsertionSort\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t19 + " sec.\n");
		System.out.println("Algorithm: InsertionSort\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t20 + " sec.\n");
		
	}
	
	//fill arrays method
	public static void fillArrays()
	{
		
		int j=0;
		Random Gen = new Random();
		
		
		//fill sorted array
		for(int i=0;i<size;i++)
		{
			S[i]=i;
		}
		
		//fill reversed array
		for(int i=size-1; i>-1;i--)
		{
			Re[i]=j;
			j++;
		}
		
		//fill random array
		for(int i=0; i<size; i++)
		{
			int a = Gen.nextInt();
			Ra[i]=a;
		}
		
	}
	
	//show arrays method
	public static void showArrays()
	{
		
		//Show array only shown if <=20
		if(size<=20)
		{
		
			//Display sorted, reversed and random arrays
			System.out.println("Array Size: " + size);
			System.out.println("Sorted\n");
			for(int i=0; i<size;i++)
			{				
				System.out.print(S[i]+ " ");
				
			}
			System.out.println("\nReversed\n");
			for(int i=0; i<size;i++)
			{
				System.out.print(Re[i]+ " ");
				
			}
			
			System.out.println("\nRandom\n");
			for(int i=0; i<size; i++)
			{
				System.out.print(Ra[i]+ " ");
				
			}
		}
		
	}
	
	//copy random method
	private static void copyRandom()
	{
		Hold = new Integer[size];
		for(int i=0;i<size;i++)
		{
			Hold[i]=Ra[i];
		}
	}
	
	//copy reversed method
	private static void copyReversed()
	{
		Hold1 = new Integer[size];
		for(int i=0;i<size;i++)
		{
			Hold1[i]=Re[i];
		}
	}
	
	//to file method, print to file with name
	private static void toFile() throws IOException
	{
		//print results to file
		PrintWriter fileOut = new PrintWriter(file);
		fileOut.println("Algorithm: Simple QuickSort\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t + " sec.\n");
		fileOut.println("Algorithm: Simple QuickSort\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t1 + " sec.\n");
		fileOut.println("Algorithm: Simple QuickSort\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t2 + " sec.\n");
		fileOut.println("Algorithm: Median of 3 QuickSort(min 5)\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t3 + " sec.\n");
		fileOut.println("Algorithm: Median of 3 QuickSort(min 5)\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t4 + " sec.\n");
		fileOut.println("Algorithm: Median of 3 QuickSort(min 5)\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t5 + " sec.\n");
		fileOut.println("Algorithm: Median of 3 QuickSort(min 10)\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t6 + " sec.\n");
		fileOut.println("Algorithm: Median of 3 QuickSort(min 10)\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t7 + " sec.\n");
		fileOut.println("Algorithm: Median of 3 QuickSort(min 10)\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t8 + " sec.\n");
		fileOut.println("Algorithm: Median of 3 QuickSort(min 20)\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t9 + " sec.\n");
		fileOut.println("Algorithm: Median of 3 QuickSort(min 20)\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t10 + " sec.\n");
		fileOut.println("Algorithm: Median of 3 QuickSort(min 20)\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t11 + " sec.\n");
		fileOut.println("Algorithm: Random Pivot QuickSort\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t12 + " sec.\n");
		fileOut.println("Algorithm: Random Pivot QuickSort\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t13 + " sec.\n");
		fileOut.println("Algorithm: Random Pivot QuickSort\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t14 + " sec.\n");
		fileOut.println("Algorithm: MergeSort\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t15 + " sec.\n");
		fileOut.println("Algorithm: MergeSort\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t16 + " sec.\n");
		fileOut.println("Algorithm: MergeSort\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t17 + " sec.\n");
		fileOut.println("Algorithm: InsertionSort\nArray Size: " + size + "\nOrder: Sorted\nNumber of Trials: " + trials + "\nAverage Time: " + t18 + " sec.\n");
		fileOut.println("Algorithm: InsertionSort\nArray Size: " + size + "\nOrder: Random\nNumber of Trials: " + trials + "\nAverage Time: " + t19 + " sec.\n");
		fileOut.println("Algorithm: InsertionSort\nArray Size: " + size + "\nOrder: Reversed\nNumber of Trials: " + trials + "\nAverage Time: " + t20 + " sec.\n");
		
		fileOut.close();
	}
		
			
}
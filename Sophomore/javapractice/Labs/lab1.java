// This program was written by Andrew Beers on 9/19/11

import java.util.*;
import java.math.*;

public class lab1
{
	public static void main (String [] args)
	{
	
		Scanner inScan = new Scanner(System.in);
		int base,integer,count=0,num=0;
	
		System.out.println("Please enter a base greater than 1: (if 0 or negative this program will close) ");
		base = inScan.nextInt();
		
		// Enter loop if acceptable base
		if(base >=1)
		{
			// Repeat loop for multiple times through
			do
			{	
				System.out.println("Please enter a positive integer : ");
				integer = inScan.nextInt();
				
				// Error proof invalid integer value
				while(integer <0)
				{	
					System.out.println("Please enter a positive integer: ");
					integer = inScan.nextInt();
				}
				num = integer;
				//  Calculate number of divisions
				while(num >= base)
				{
					num = num/base;
					count++;
				
				}
			
				System.out.println("The number of divisions of " + integer + " is " + count + " in base " + base);
				
				System.out.println("Please enter a base greater than 1: (if 0 or negative this program will close) ");
				base = inScan.nextInt();
				
			}while(base >= 1);
	
		}
	}
	
}
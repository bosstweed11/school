

import java.util.*;
import java.lang.Math.*;

public class binary
{
	public static void main(String [] args)
	{
	Scanner inScan = new Scanner(System.in);
	int c,i;
	float b,x,j=2;
	float[] k;
	k = new float[6];
	
	System.out.print("Please enter an integer less than 128 to convert from decimal to binary: ");
	x = inScan.nextInt();
	
	System.out.println("The number you chose to convert is " + x);
	
	
	for(i=0; i<=1; i++)
	{
	
		k[i] = x%j;
	}
	
	System.out.print("The  number is " + k[1] + k[0]);
	
	
	
	}






}
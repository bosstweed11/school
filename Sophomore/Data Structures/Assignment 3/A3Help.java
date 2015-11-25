// CS 0445 Spring 2012
// Help program for Assignment 3
// This program will process a string consisting of integers separated by single
// blank characters, parsing and adding the numbers to return a single int for 
// each string.

public class A3Help
{
	public static String [] lines = {"123 1000 11111 998877",
								  "10 20 30 40 50 60 70 80 90 100",
								  "1 10 100 1000 10000 100000"};
	public static void main(String [] args)
	{
		// For each line make a new object, then call the method to add
		// the numbers together
		for (String s: lines)
		{
			A3Help A = new A3Help(s);
			int result = A.addNums();
			System.out.println("\nThe result is " + result + "\n");
		}
	}
	
	// instance data is the String containing the numbers and the index
	// I used the same instance variables for Assignment 3
	private int ind;
	private String data;
	
	// Create a new object
	public A3Help(String line)
	{
		ind = 0;
		data = new String(line);
	}
	
	// Recursively add the numbers in the String.  Note carefully how this method
	// is working recursively (no loops!).
	public int addNums()
	{
		if (ind < data.length())  // Base case is we are at the end of the String
		{
			int num = getNum(0);  // Get the next number (also a recursive method)
			ind++; // skip blank  // increment the index
			return num + addNums();  // Add the number to the recursive sum of the
									 // remaining numbers and return
		}
		return 0;  // Sum of no numbers is 0
	}
	
	// Recursively get a number digit by digit from the String.  I have trace code
	// in here to show the process better.
	// The int parameter is necessary because the digits build up to the final 
	// number.
	public int getNum(int val)
	{
		int ans = 0, dig;
		System.out.println("val = " + val);
		if (ind >= data.length())   // Base case end of String.  Return what we have
		{							// so far
			System.out.println("End of string, ans = " + val);
			return val;
		}
		else  // Not end, so look at the next character
		{
			char curr = data.charAt(ind);
			System.out.println("curr = " + curr);
			if (Character.isDigit(curr))    // If char is a digit
			{
				ind++;						// Incrememnt the index
				dig = curr - '0';			// Convert char to int
				val = val * 10 + dig;		// Mult. prev. by 10 and add new digit.
				ans = getNum(val);			// Recursively process rest of number
				System.out.println("Recursive ans = " + ans);
			}
			else	// Not a digit so must be a space.  Return previous answer.
			{
				ans = val;
				System.out.println("Base case ans = " + ans);
			}
			return ans;
		}
	}		
}
			
		
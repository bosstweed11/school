import java.util.*;

public class ex4
{
	public static void main (String [] args)
	{
		Scanner inScan = new Scanner(System.in);
		
		int iVal;
		float fVal;
		double dVal;
		String s1Val;
		String s2Val;
		
		System.out.print("Please enter an int: ");
		iVal = inScan.nextInt();
		
		System.out.print("Please enter a float: ");
		fVal = inScan.nextFloat();
		System.out.print("Please enter a double: ");
		dVal = inScan.nextDouble();
		
		System.out.print("You entered:\n" + iVal + "\n" + fVal + "\n");
		System.out.print(dVal + "\n");
		
		System.out.print("Please enter two strings: ");
		s1Val = inScan.next();
		s2Val = inScan.nextLine();
		System.out.print("You entered:" + s1Val + "\nand:" + s2Val + "\n");
		
		System.out.print("Please enter two more strings: ");
		s1Val = inScan.nextLine();
		s2Val = inScan.next();
		
		System.out.print("You entered:" + s1Val + "\nand:" + s2Val + "\n");
		System.out.print("Enter something else: ");
		
		if (inScan.hasNextInt())
		{
			iVal = inScan.nextInt();
			System.out.println("You entered the int " + iVal);
		}
		
		else if (inScan.hasNextFloat())
		{
			fVal = inScan.nextFloat();
			System.out.println("You entered the float " + fVal);
		}
		
		else if (inScan.hasNextDouble())
		{
			dVal = inScan.nextDouble();
			System.out.println("You entered the double " + dVal);
		}
		
		else	
		{
			s1Val = inScan.next();
			System.out.println("You entered the string " + s1Val);
		}
		
		
		}
}		
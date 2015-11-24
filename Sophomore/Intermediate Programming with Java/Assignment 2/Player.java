import java.util.Scanner;
import java.io.*;

public class Player 
{
	private int right,wrong,tright=0,twrong=0,r=0,w=0,i=0,j=0;
	private double questions, percent, q,p;
	private String pname;
	private String A;
	private String players="players.txt";
	private Scanner scanFile;
	private boolean taken=false,stats=false;
	
	
	public Player()
	{
		pname = null;
	
	}
	//Create new player
	public Player(String A)
	{
		pname = A;
	}
	//Check if the player has played before by checking text file
	public boolean checkName(String A) throws IOException
	{
		scanFile = new Scanner(new File(players));
		while(scanFile.hasNextLine())
		{
			
			pname = scanFile.nextLine();
			if (pname.equals(A))
			{
				taken = true;
			}
			
			if(taken == true)
			{
				return(false);
			}
			
			
		}
		return(true);
	}
	// Find previous players stats from the text file
	public void Stats(String A) throws IOException
	{
		
		scanFile = new Scanner(new File(players));
		while(stats == false)
		{
				
			pname = scanFile.nextLine();
			if (pname.equals(A))
			{
				stats = true;
			}
			
			if(stats == true)
			{
				right = scanFile.nextInt();
				wrong = scanFile.nextInt();
				percent = (right*100/(right+wrong));
				System.out.println("Name: " + pname + "\nRight: " + right + "\nWrong: " + wrong+ "\nPercent: " + percent);
				
			}
			
			
		}
		
		
	
	}
	
	//Get overall stats for the quiz
	public void overallStats() throws IOException
	{
		scanFile = new Scanner(new File(players));
		while(scanFile.hasNextLine())
		{
			pname = scanFile.nextLine();
			j++;
			if(scanFile.hasNextInt())
			{
			tright = scanFile.nextInt();
			}
			if(scanFile.hasNextInt())
			{
			twrong = scanFile.nextInt();
			}
			
			r = r + tright;
			w = w + twrong;
			if(scanFile.hasNextLine())
			scanFile.nextLine();
		}
		p = (r*100)/(r+w);
		System.out.println("Overall, " +j +" players have taken the quiz: \nTotal Right =" + r + " \nTotal Wrong =" + w +"\nPercent: " + p);
	}
	
	
	
	//Append current players stats to the text file.
	public void writeTo(String A,int right, int wrong) throws IOException
	{
		
		PrintWriter fileOut = new PrintWriter(new FileOutputStream(players,true));
		fileOut.println(A);
		fileOut.println(right);
		fileOut.println(wrong);
		fileOut.close();
	
		
	}
	
	
			
		
		

	
}
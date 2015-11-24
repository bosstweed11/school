// This program was written by Andrew Beers on 10/13/11
import java.util.*;
import java.io.*;
public class Assignment2
{

	public static void main (String [] args) throws IOException
	{
		int right=0,wrong = 0;
		double questions=0,percent;
		boolean namecheck=true;
		Quiz Q1=null;
		Player P1 = null;
		String name;
		String f="questions.txt";
		String A;
		String ans;
		Scanner inScan = new Scanner(System.in);
		
		
		System.out.println("Hello and welcome to the Quiz game, what is your name? ");
		name = inScan.nextLine();
		P1 = new Player(name);
		
		//See if this is a previous player or not, if not then go in first loop
		if (namecheck=P1.checkName(name))
		{
			System.out.println("Ready to take the quiz? \nPlease answer each question exactly \nYou will have two guesses for each question \n(If the answer is a number, please enter the digits, not words) ");
			Q1 = new Quiz(f);
			
			//Question/Answer structure
			do
			{
				if(Q1.hasAQuestion());
				{
					System.out.println(Q1.getQuestion());
					A = inScan.nextLine();
					//Assigning answer to variable allows user to answer twice at the same answer
					ans = Q1.getAnswer();
					
					if(A.equalsIgnoreCase(ans))
					{	
						System.out.println("Correct");
						right++;
					}
					else
					{
						System.out.println("Incorrect");
						System.out.println("Please try again: ");
						A = inScan.nextLine();
				
						if(A.equalsIgnoreCase(ans))
						{	
							System.out.println("Correct");
							right++;
						}
						else
						{
							System.out.println("Incorrect");
							System.out.println("Sorry you missed this one");
							System.out.println("The answer was " + ans);
						}
					}
						
						
					
				}
				questions++;
			}while(Q1.hasAQuestion());
		//Calculate players stats
		
		wrong = (int)questions-right;
		percent = (right/questions)*100;
		System.out.println("Here are your results: \nCorrect: " + right + "\nIncorrect: " + wrong + "\nPercentage: " + percent);
		
		//Append players stats to text file
		P1.writeTo(name,right,wrong);
		
		//Show overall stats of the quiz
		P1.overallStats();
		
		}
		else
			{
				//If the player has taken the quiz, show individual and overall stats
				System.out.println("You have already taken the quiz. Here are your results: ");
				P1.Stats(name);
				P1.overallStats();
			}

	
	}
}
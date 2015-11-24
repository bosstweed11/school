// This program was written by Andrew Beers on 11/1/11

import java.util.*;
import java.io.*;
public class Assign3
{
	private static int c=0,w=0,l=0,z=0,playernum;
	private static String currName, player = "players.txt";
	private static boolean found =false;
	public static void main(String [] args) throws IOException
	{
		ArrayList<HangPlayer> A = new ArrayList<HangPlayer>();
		Scanner scan = new Scanner(System.in);
		Scanner pScan = new Scanner(new File(player));
		
		String words,newword,g="-",subguess=null,oldname=null;
		String name=null,wins,losses;
		boolean redo,restart,q=false;;
		int index,miss=7,again=1,oldw,oldl,game=1;
		int d=0;
		char guess;
		
		
		//get file to load the words
		System.out.println("Please enter the name of your word file: ");
		words = scan.nextLine();
		WordServer W = new WordServer(words);
		Scanner inScan = new Scanner(new File(words));
		
		
	
		// Load in old players of hangman, to an arraylist
		while(pScan.hasNextLine())
		{
			name = pScan.nextLine();
			wins = pScan.nextLine();				
			losses = pScan.nextLine();
			oldw =Integer.parseInt(wins);
			oldl =Integer.parseInt(losses);
			HangPlayer B= new HangPlayer(name,oldw,oldl);
			A.add(B);
			z= z +3;
			
		}
		
		//Greets user asks for name, q to quit
		
		System.out.println("Please enter your name(or q to quit): ");
		currName = scan.nextLine();
		
		W.loadWords(inScan);
		while( !currName.equals("q"))
		{
			
				
				//If the old player is found, load in their old stats
				for(int i = 0; i < (z/3); i++)
				{
					if(currName.equals(A.get(i).getName()))
					{
						found = true;
						w= A.get(i).getWins();
						l =A.get(i).getLosses();
						playernum = i;
					}
				}
				again = 1;
				HangPlayer C;
				
				//if old player, welcome them back and load in old stats, else greet a new player with the rules
				if(found == true)
				{
					w=0;
					l=0;
					System.out.println("Welcome back " + currName + "!");
					System.out.println("The rules are as follows:\n You guess letters for each hidden word.\n If you are right the letters will show up in their correct locations\n If not you lose a turn. \nYou have 7 turns.\nYou get one free letter, this letter is determined before you start your word. \nIt is wise to use this letter early on in a word. Good Luck and have fun!" );
					C = A.get(playernum);
					System.out.println("These are your past stats: ");
					
					C.stats();
				}
				else
				{
					w=0;
					l=0;
					C = new HangPlayer(currName,w,l);
					System.out.println("Welcome " + currName +  ", to Hangman!");
					System.out.println("The rules are as follows:\n You guess letters for each hidden word.\n If you are right the letters will show up in their correct locations\n If not you lose a turn. \nYou have 7 turns.\nYou get one free letter, this letter is determined before you start your word, so it is wise to use this letter early on in a word. Good Luck and have fun!" );
				}
				//while they want to keep playing, run the main game loop
				while(again ==1)
				{
					miss =7;
					//get next word
					newword = W.getNextWord();
					//if returns end, the file is out of words
					if (newword.equals("end"))
					{
						System.out.println("Sorry no more words. Ending program..");
						miss = 0;
					}
					StringBuilder K = new StringBuilder();
					StringBuilder L = new StringBuilder();
					
					//Blank word to guess at
					for(int i =0; i<newword.length(); i++)
					{
						K.append(g);
					}
					System.out.println(K);
				
					//Players requirements for staying in the game, guessing loop
					while((!newword.equals(K.toString())) && (miss != 0) )
					{
						
						int turn = 0;
						System.out.println("Please make your guess(type ! for a free letter(can only use once)): ");
						System.out.println("You have " + miss +" misses left.");
						System.out.println("Enter 0 to quit the word.");
						subguess = scan.next();
						for (int i = 0; i < 18; i ++)
						System.out.println();
								
						if(subguess.equals("!")&&!q)
						{
							
							
							
							guess = W.free(newword);
							System.out.println(guess);
							subguess = Character.toString(guess);
							q = true;
						}
						else if(subguess.equals("!")&&q)
						{
							System.out.println("You already used your free guess, please guess again. ");
							subguess = scan.next();
						}
						//if guess is 0, quit word
						if(subguess.equals(Integer.toString(0)))
							miss =1;
							
						//Check if guess is letter or 0,1
						if(subguess != Integer.toString(0) && subguess!=Integer.toString(1))
						{
							
							redo =true;
							guess=subguess.charAt(0);
							boolean letter = Character.isLetter(guess);
							boolean upper = Character.isUpperCase(guess);
							if (upper)
							{	
								guess = Character.toLowerCase(guess);
							}
						
							//keep in here while not a letter or 0
							while (!letter)
							{
							
								if(subguess.equals(Integer.toString(0)))
								{
									
									guess = '0';
									letter = true;
								}
								if(!letter)
								{
									System.out.println("Sorry your guess was not a letter, please enter a letter: ");
									subguess = scan.next();
									guess = subguess.charAt(0);
									letter = Character.isLetter(guess);
								}
							}
							//Structure for making sure you do not repeat guesses
								if(turn!=0);
								{	
									while(redo)
									{
										redo =false;
										for(int i =0;i<L.length();i++)
										{
											if(guess == L.charAt(i))
											{
												System.out.println("You've already guessed that, guess again: ");
												System.out.println("Please make your guess: ");
												subguess = scan.next();
												guess=subguess.charAt(0);
												redo=true;
											}	
										}
									}
								}
								c=0;
								d=0;
								
								//Missing structure
								while(c<newword.length())
								{
									if (guess == newword.charAt(c))
									{
										K.deleteCharAt(c);
										K.insert(c,guess);
										--d;
									}
									c++;
									d++;
								}
					
								System.out.println(K);
								L.append(guess);
								System.out.println("You have guessed: "+ L);
								turn++;
								
								if(d==newword.length())
								{
									miss=miss-1;
								}
							
						}
					}
					
					//if you got the word, you win, add wins, update stats
					if(newword.equals(K.toString()))
					{
						System.out.println("You win!");
						w++;
						C.update(w,l);
					}
					// if not then you lose, add losses, update stats
					else if(miss==0)
					{
						if(newword!="end")
						{
							System.out.println("You lose.");
							l++;
							C.update(w,l);
							System.out.println("The correct word was " + newword);
						}
						else
						{
							again=0;
						}
					}
					
					
					//ask to play again
					if(again!=0)
					{
						System.out.println("Play again?(1=yes, 0=no) ");
						again = scan.nextInt();
						
					}
					
					//if not, then enter stats into the array list and print out final stats
					if(again==0)
					{
					
						HangPlayer P = new HangPlayer(currName,w,l);
						if(!found)
						{
							
							z = z+3;
						}
						A.add(P);
						C.stats();
						String Y;
						wins = Integer.toString(w);
						losses = Integer.toString(l);
						
						
						
						
						currName = "q";
					}
				}	
				
			// See if there is another player ready to play
			
			//as long as it is a different name then last time, they can play
			
			
			
		}
	System.out.println("Goodbye,Thanks for playing!");
	
	//Write the final arraylist over the last file
	toFile(A);
	
	}
	
	
	public static void toFile(ArrayList<HangPlayer> P) throws IOException
	{
		//Writes over previous file with current array list of hang players.
		PrintWriter fileOut = new PrintWriter(player);
		for(int i = 0; i<(z/3); i++)
		{
			fileOut.println(P.get(i).toString2());
		}
		
	
	fileOut.close();
	}
}


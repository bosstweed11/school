// CS 1501 Summer 2013
// Test program for DictInterface.  Use this program to see how the DictInterface
// works and also to test your DLB implementation of the DictInterface (for Part B
// of Assignment 1).  See the posted output file for the correct output.
// As written the program uses MyDictionary, so it will work and show you the
// output using the MyDictionary class.  To test your DBL class substitute a 
// DLB object as indicated below.  Note that since MyDictionary and DLB should
// both implement DictInterface no other changes are necessary for the program
// to work.

import java.io.*;
import java.util.*;
import java.util.Collections; 
public class Assignment1-2
{

	static DictInterface D;
	//static ArrayList<ArrayList<String>> Solutions = new ArrayList<ArrayList<String>>();
	static HashMap<Integer, ArrayList<String>> Solutions = new HashMap<Integer, ArrayList<String>>();
	//static ArrayList<String> Solutions = new ArrayList<String>();
				// Change MyDictionary() above to DLB() to test your DLB

	public static void main(String [] args) throws IOException
	{
	
		if ( args.length != 3 )
		{
			System.out.println( "USAGE: java Assignment1 <dictionaryFileName> <inputFileName> <dlb/orig>\n" );
		
		}
		else
		{
			Scanner fileScan = new Scanner(new FileInputStream(args[0]));	//read in dictionary file
			Scanner inputScan = new Scanner( new FileInputStream(args[1]));	//read in input file
			
			ArrayList<String> inputs = new ArrayList<String>();
			String st;
			
			//determine whether or not we are using a DLB
			if ( args[2].equals("orig") )
			{
				D = new MyDictionary();
			}
			else
			{
				D = new DLB();
			
			}
			
			//Timing for the dictionary load time
			long dictionaryStartTime = System.nanoTime();
			
			while (fileScan.hasNext())		//load the dictionary
			{
				st = fileScan.nextLine();
				st = st.replaceAll("\\s","");
				D.add(st);
			}
			
			long dictionaryEndTime = System.nanoTime();
			long dictionaryDuration = dictionaryEndTime - dictionaryStartTime;
			System.out.println("Time to load dictionary in seconds: " + dictionaryDuration/1000000000.0 );
			
			
			//read in input files trimming off spaces
			while ( inputScan.hasNext() )
			{
				st = inputScan.nextLine();
				st = st.replaceAll("\\s","");
				inputs.add(st);
			}
			
			//timing for all words
			long startTime = System.nanoTime();
			
			
			//for all of the input words to test, find the anagrams
			for ( int i = 0; i < inputs.size() ; i++ )
			{
				StringBuilder sb = new StringBuilder ( inputs.get(i) );
			
				System.out.println( "Results for " + inputs.get(i) + ":" );
			
			
				//this structure is for testing for anagrams, it is run for the amount of letters in
				//a given input string, there is a separate run to look for 1 word solutions, 2 word solutions...N word solutions
				
				//System.out.println( "Testing: " + inputs.get(i).toString() );
			
				//first find anagrams call
				findAnagrams( new StringBuilder(), new StringBuilder ( inputs.get(i) ), inputs.get(i).length(), 1 );
			
				for ( int j = 0; j < sb.length() ; j++ )
				{
					ArrayList<String> sol = Solutions.get( j );
					
					if ( sol != null )
					{
						Collections.sort( sol );	//sorting on solutions of size 1
					}
					
					for ( int k = 0; k < sol.size(); k++ )
					{
					
						System.out.println( sol.get( k ) );
					
					}
			
					Solutions.clear();	//clear solutions for the next run
				}
				
				
			}
			long endTime = System.nanoTime();
		
			long duration = endTime - startTime;
		
			System.out.println("Total Run Time in seconds: " + duration/1000000000.0 );
			
			
			
		}

		
	}
	
	public static void findAnagrams( StringBuilder currentString, StringBuilder lettersLeft, int length, int numWords )
	{
		boolean debug = false;
		
		if ( debug )
		{
			System.out.println( "\nNew call with: " + currentString.toString() );
			System.out.print("Beginning Current Call " + numWords + "\n");
		}
		
		//go through all the letters left
		for( int i = 0; i < lettersLeft.length() ; i++ )
		{
			
			if ( debug )
			{
				System.out.print(" Current Loop " + i + "\n");
				System.out.println( "Before append: " + "\n Current String: " + currentString.toString() + "\n Letters Left: " + lettersLeft.toString() );
			}
			
			//append the next letter to the solution
			currentString.append( lettersLeft.charAt(i) );
		
			if ( debug )
			{
				System.out.println( "After append: " + currentString.toString() );
			}
			
			int a;
			//searchPrefix to see if our current solution is a prefix, word, neither or both
			if ( numWords > 1 )
			{
				int index = currentString.lastIndexOf(" ");
				
				a = D.searchPrefix( currentString.substring(index + 1, currentString.length() - 1 ) );
			}
			else
			{
				a = D.searchPrefix( currentString );
			}
			
			/*if ( currentString.toString().equals("eels") ||  currentString.toString().equals("cease") ||  currentString.toString().equals("col") ||  currentString.toString().equals("lees") )
			{
				//System.out.println("\nReturn value for " + currentString.toString() + ": " + a );
			}
			*/
			if ( a == 1 || a == 3 )//remove this ?
			{
				
				if ( debug )
				{
					System.out.println( "--Before call: \n Current String: " + currentString.toString() + "\n Letters Left: " + lettersLeft.toString() + "\n Leaving Current Call: " + numWords );
				}
				
				if ( a == 3)	//if the solution is a word and a prefix
				{
					if ( currentString.length() == length )	//if we are using all the letters
					{
						if ( numWords == 1 )
						{
							if ( debug )
							{
								System.out.println( "The word is full length" + currentString.toString() ) ;
							}
							addWord( currentString, numWords );	//add the word
						}
					}
					else
					{
						if ( numWords != 1 ) //if not add the word but search again...try this another way?
						{
							StringBuilder save = new StringBuilder ( lettersLeft );
							addWord( currentString , numWords );
							lettersLeft = save;
						}
					}
				}
				else if ( a == 1 )	// solution is a prefix only
				{
					if ( debug )
					{
						System.out.println( "The prefix is: " + currentString.toString() );
					}
				}
				
				// save the lettersLeft before delete
				StringBuilder next = new StringBuilder ( lettersLeft );
				
				if ( debug )
				{
					System.out.println("Letters Left: " + lettersLeft.toString() );
				}
				
				//recursive call
				findAnagrams( currentString, lettersLeft.deleteCharAt( i ), length, numWords );
				
				//restore lettersLeft
				lettersLeft = next;
				
				if ( debug )
				{
					System.out.println( "--After call: \n Current String: " + currentString.toString() + "\n Letters Left: " + lettersLeft.toString() + "\n Returning Current Call: " + numWords  );
					System.out.println("\t Returning to Current Loop " + i + "\n");
				}
				
			}
			else if ( a == 2 )	//if the solution is only a word
			{
				
				if ( currentString.length() == length )	//if we are using all the letters
				{
					if ( numWords == 1 )	//if there is one word left...try this another way?
					{
						if ( debug )
						{
							System.out.println( "The word is full length" + currentString.toString() ) ;
						}
						addWord( currentString, numWords );	//add this solution
					}
				}
				else
				{
					StringBuilder save = new StringBuilder ( lettersLeft );
					currentString.append(' ');
					findAnagrams( currentString , lettersLeft.deleteCharAt( i ), save.length() - 1, numWords++ );
					
					lettersLeft = save;
					
					
				}
				
			}
			
				if ( debug )
				{
					System.out.println( "Before remove:  \n Current String: " + currentString.toString() + "\n Letters Left: " + lettersLeft.toString() );
				}
				currentString.deleteCharAt( currentString.length() - 1);
			
				if ( debug )
				{
					System.out.println( "After remove:  \n Current String: " + currentString.toString() + "\n Letters Left: " + lettersLeft.toString() );
				}
		}
	
		if ( debug )
		{
			System.out.println( "Ending Call: " + numWords + " String returned: " + currentString.toString() );
		}
		
		return;	//return something useful here?
	
	}
	
	public static boolean addWord( StringBuilder word , int numWords )
	{
		boolean debug = false;
		ArrayList<String> sol = Solutions.get( numWords );
		
		if ( sol == null )
		{
			sol = new ArrayList<String>();
		}

		
		if ( debug )
		{
			System.out.println("Found word of " + word + "numWords: " + numWords );
			System.out.println("Going for " + numWords + " word phrases. ");
		}
	
		if ( !sol.contains( word.toString() ) )
		{
			 
			sol.add ( word.toString() );
		}
		
		Solutions.put( numWords, sol );
					
		return true; //return something more useful here?
	}
}



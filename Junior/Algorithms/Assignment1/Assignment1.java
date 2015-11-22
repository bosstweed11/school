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
public class Assignment1

{

	static DictInterface D;
	//static ArrayList<ArrayList<String>> Solutions = new ArrayList<ArrayList<String>>();
	static ArrayList<String> Solutions = new ArrayList<String>();
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
				for ( int j = 1; j < sb.length(); j++ )
				{
					//System.out.println( "Testing: " + inputs.get(i).toString() );
				
					//first find anagrams call
					findAnagrams( new StringBuilder(), new StringBuilder ( inputs.get(i) ), inputs.get(i).length(), j );
			
					if ( Solutions.size() >= j )
					{
						if ( j == 1 )
						{
							Collections.sort( Solutions );	//sorting on solutions of size 1
						}
						System.out.println(j + " word solutions: " );	//print out solutions
					
						for ( int k = 0; k < Solutions.size() ; k++ )
						{
				
							System.out.println ( Solutions.get( k ) );
			
						}
					
					}
					else
					{
						//System.out.println("\n\n\nThere are no " + j + "word solutions: " );
					}
				
					Solutions.clear();	//clear solutions for the next run
				}
				System.out.println("");
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
			
			//searchPrefix to see if our current solution is a prefix, word, neither or both
			int a = D.searchPrefix( currentString );
			
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
							addWord( currentString, numWords, null );	//add the word
						}
					}
					else
					{
						if ( numWords != 1 ) //if not add the word but search again...try this another way?
						{
							StringBuilder save = new StringBuilder ( lettersLeft );
							addWord( currentString , numWords, lettersLeft.deleteCharAt(i) );
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
						addWord( currentString, numWords, null );	//add this solution
					}
				}
				else
				{
					if ( numWords != 1 )	//if we arent using all the letters, but we arent looking for 1 word solutions, add
					{
						StringBuilder save = new StringBuilder ( lettersLeft );
						addWord( currentString , numWords, lettersLeft.deleteCharAt(i) );
						lettersLeft = save;
					}
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
	
	public static boolean addWord( StringBuilder word, int numWords, StringBuilder lettersLeft )
	{
		boolean debug = false;
		
		if ( debug )
		{
			System.out.println("Found word of " + word + "numWords: " + numWords );
			System.out.println("Going for " + numWords + " word phrases. ");
		}
	
		if ( !Solutions.contains( word.toString() ) )
		{
			 
			if ( numWords > 1 && lettersLeft != null )
			{
				if ( debug )
				{
					System.out.println("\n\nNEW CALL: \nFound: " + word.toString() + "\nTrying new with: " + lettersLeft.toString() );
				}
				findAnagrams( new StringBuilder(), lettersLeft, lettersLeft.length(), numWords - 1);
				
				if ( debug )
				{
					System.out.println("\n\nFINISHED CALL:Ending Find Anagrams call. ");
				}
				
				
				if( ( Solutions.size() % numWords ) + 1 == numWords )	//if the previous words were added
				{
					Solutions.add ( word.toString() );
					if ( debug )
					{
						System.out.println("\nAlgo Adding: " + word.toString() );
					}
					
				}
				else	//this isnt a good idea
				{
					/*if ( !Solutions.isEmpty() )
					{
						do
						{
							System.out.println("\nRemoving: " + Solutions.get(Solutions.size() -1 ).toString() );
							Solutions.remove( Solutions.size() - 1);
							
						}while( (Solutions.size() % numWords ) + 1 == numWords );
						
						result = false;
					}
					*/
				}
			}
			else if ( numWords == 1 && lettersLeft == null )	
			{
				Solutions.add ( word.toString() );
				if ( debug )
				{
					System.out.println("\n1 word Adding: " + word.toString() );
				}
		
			}			
			
		}
		
		return true; //return something more useful here?
	}
}



// Andrew Beers CS 1501 Assignment 1 6/5/12

import java.io.*;
import java.util.*;
import java.util.Collections; 
public class Assignment1
{

	static DictInterface D;

	static ArrayList<ArrayList<String>> Solutions = new ArrayList<ArrayList<String>>();


	public static void main(String [] args) throws IOException
	{
		if ( args.length != 3 )
		{
			System.out.println( "USAGE: java Assignment1 <dictionaryFileName> <inputFileName> <dlb/orig>\n" );
		
		}
		else
		{
		
			//must initialize the solutions key to an arbitrary amount to make sure we dont try to add to a location in the arraylist that has not 
			//been allocated for, this number should represent the size of the largest input string
			//however cycling through finding the max length of all the input strings didnt seem relavent to 
			//this project
			for (int i = 0; i < 20; i++) 
    		{
    			ArrayList<String> sol = new ArrayList<String>();
    			
        		Solutions.add(i,  sol );
    		}
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
			
			//for running multiple times on timing analysis
			//for ( int g = 0; g < 5 ; g++ )
			//{
				//timing for all words
				long startTime = System.nanoTime();
				//for all of the input words to test, find the anagrams
				for ( int i = 0; i < inputs.size() ; i++ )
				{
					StringBuilder sb = new StringBuilder ( inputs.get(i) );
			
					System.out.println( "Results for " + inputs.get(i) + ":" );
			
					//first find anagrams call
					findAnagrams( new StringBuilder(), new StringBuilder ( inputs.get(i) ), inputs.get(i).length(), 1 );
				
					//looping on the size of the current string (max number of word solutions)
					for ( int j = 0; j < sb.length() ; j++ )
					{
						ArrayList<String> sol = Solutions.get( j );
					
						//if a solution exists of j + 1 words, sort it and print out the results
						if ( sol != null )
						{
							Collections.sort( sol );	//sorting on solutions of size 1
						
							if ( !sol.isEmpty() )
							{
								System.out.println("\n" + (j + 1) + " word solutions:" );
								for ( int k = 0; k < sol.size(); k++ )
								{
							
									System.out.println( sol.get( k ) );
						
								}
							}
						}
					
					}
					Solutions.clear();	//clear solutions for the next run
					
					//must initialize the solutions key to an arbitrary amount to make sure we dont try to add to a location in the arraylist that has not 
					//been allocated for, this number should represent the size of the largest input string
					//however cycling through finding the max length of all the input strings didnt seem relavent to 
					//this project
					for (int q = 0; q < 20; q++) 
					{
						ArrayList<String> sol = new ArrayList<String>();
						Solutions.add(q,  sol );
					}
					System.out.println("");
				
				}
				long endTime = System.nanoTime();
		
				long duration = endTime - startTime;
		
				System.out.println("Total Run Time in seconds: " + duration/1000000000.0 );
			//}	
		}	
	}
	
	public static void findAnagrams( StringBuilder currentString, StringBuilder lettersLeft, int length, int numWords )
	{

		//go through all the letters left
		for( int i = 0; i < lettersLeft.length() ; i++ )
		{
			
			//append the next letter to the solution
			currentString.append( lettersLeft.charAt(i) );

			int a;
			//searchPrefix to see if our current solution is a prefix, word, neither or both
			if ( numWords > 1 )
			{
				int index = currentString.lastIndexOf(" ");
				StringBuilder currString = new StringBuilder( currentString.substring( index + 1, currentString.length() ) );
				a = D.searchPrefix( currString );
			}
			else
			{
				a = D.searchPrefix( currentString );
			}
			
			if ( a == 1 || a == 3 )
			{
				
				if ( a == 3)	//if the solution is a word and a prefix
				{
				
					if ( currentString.length() == length )	//if we are using all the letters
					{
						addWord( currentString, numWords );	//add the word
					}
					else
					{
						
						StringBuilder save = new StringBuilder ( lettersLeft );
 
						currentString.append(' ');
						findAnagrams( currentString , lettersLeft.deleteCharAt( i ), length + 1, numWords + 1 );
						
						currentString.deleteCharAt( currentString.length() - 1 );
						lettersLeft = save;
					
					}
				}
				
				// save the lettersLeft before delete
				StringBuilder next = new StringBuilder ( lettersLeft );
				
				//recursive call
				findAnagrams( currentString, lettersLeft.deleteCharAt( i ), length, numWords );
				
				//restore lettersLeft
				lettersLeft = next;
				
			}
			else if ( a == 2 )	//if the solution is only a word
			{
				
				if ( currentString.length() == length )	//if we are using all the letters
				{
					addWord( currentString, numWords );	//add this solution
				}
				else
				{
					StringBuilder save = new StringBuilder ( lettersLeft );
					currentString.append(' ');
					findAnagrams( currentString , lettersLeft.deleteCharAt( i ), length + 1, numWords + 1 );
					currentString.deleteCharAt( currentString.length() - 1 );
					
					lettersLeft = save;

				}
				
			}
				currentString.deleteCharAt( currentString.length() - 1);

		}
		
		return;
	
	}
	
	public static boolean addWord( StringBuilder word , int numWords )
	{
		boolean debug = false, existed = true;
		ArrayList<String> sol = null;
		
		if ( numWords > Solutions.size() )
		{
			sol = new ArrayList<String>();
			existed = false;
		}
		else
		{
			sol = Solutions.get( numWords - 1 );
			if ( sol == null )
			{
				sol = new ArrayList<String>();
				existed = false;
			}
			
		}
	
		if ( !sol.contains( word.toString() ) )
		{
			 
			sol.add ( word.toString() );

		}

		if ( existed )
		{
			Solutions.set( numWords - 1 , sol );
		}
		else
		{
			Solutions.add( numWords - 1 , sol );
		}
					
		return true;
	}
	
}



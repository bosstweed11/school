

import java.io.*;
import java.util.*;
import java.util.Collections; 
public class DLB implements DictInterface
{

	private DLBnode root;
	
	public DLB( )
	{
	
		root = new DLBnode();
		
	}
	
	public boolean add( String word )
	{
	
	
		boolean again;
		int counter = 0;
		char current = word.charAt( counter );
		DLBnode currNode = root;
		DLBnode newNode;
		
		//System.out.println( "Adding: " + word ); 
		
		if ( currNode.getValue() == '0' )			//root node(this will only happen once to populate the root
		{
			
			currNode.setValue( current );
			//System.out.println("Changed value of root to: " + current );
				
		}
		
	
		
		while ( counter < word.length() )
		{

			current = word.charAt(counter);
			
			
			if ( current == currNode.getValue() ) 	//found value
			{
				//System.out.println("Found: " + current );
				
				if ( currNode.getChild() == null )	// do not have a child
				{
					//System.out.println("Finishing: " + word );
					counter++;
					while( counter < word.length() )		//<--- create rest of the word END
					{
						
						current = word.charAt( counter );
						
						newNode = new DLBnode( current );
						
						currNode.setChild( newNode );
						//System.out.println("Added as child: " + current);
						currNode = currNode.getChild();
						counter++;
					
					
					}
				
				}
				else								// have a child
				{
					
					currNode = currNode.getChild();
					//System.out.println("Found child. Proceeding..");
					counter++;
				}
				
			
			}
			else									//didn't find value
			{
				
				if ( currNode.getSibling() == null )	//letter does not exist at this level -> word doesnt exist
				{
					//System.out.println("Not found, no sibling. Adding as sibling: " + current);
					newNode = new DLBnode( current );
					counter++;
					currNode.setSibling( newNode );
					
					currNode = currNode.getSibling();
					//System.out.println("Finishing: " + word );
					while( counter < word.length() )			//<--- create rest of the word END
					{
						
						current = word.charAt( counter );
						
						newNode = new DLBnode( current );
						
						currNode.setChild( newNode );
						//System.out.println("Added as child: " + current);
						currNode = currNode.getChild();
						counter++;
					
					}
					
				
				
				}
				else
				{
					
					currNode = currNode.getSibling();
					//System.out.println("Getting next sibling. Proceeding...");
				}
			
			}
			
		}
		
		//System.out.println("Finished adding: " + word );
		//System.out.println(currNode.getValue() + " is the current value");
		// after creating the word we must add the !
	
		if ( currNode.getChild() == null )			//if theres no child, add the ! as a child
		{
			newNode = new DLBnode( '!' );
			//System.out.println("No child. Adding ! as child");
			currNode.setChild( newNode );
	
		}
		else
		{
			if ( currNode.getValue() == word.charAt(word.length() - 1 ) )
			{
				currNode = currNode.getChild();			//if there is a child, proceed to it and check the value
			}
			else
			{
				//System.out.println("Child not retrieved");
			}
		
			if ( currNode.getValue() != '!' )
			{
		
				if ( currNode.getSibling() == null )
				{
					//System.out.println("No sibling. Adding ! as sibling");
					newNode = new DLBnode( '!' );
					currNode.setSibling( newNode );
		
				}
				else
				{
		
					while( currNode.getSibling() != null )
					{
						
						if ( currNode.getValue() == '!' )
						{
							//System.out.println("Found !");
							break;
						}
						else
						{
							currNode = currNode.getSibling();
							//System.out.println("next sibling");
						}
		
					}
		
					if ( currNode.getValue() != '!' )
					{
		
						newNode = new DLBnode( '!' );
						currNode.setSibling( newNode );
						//System.out.println("Adding ! as sibling");
		
					}
				}
			}
			else
			{
				//System.out.println("! exists as child");
			}
		}
			
		return true;
	}
	
	public int searchPrefix(StringBuilder s)
	{
		int counter = 0;
		char current = 'a';
		boolean none = false, prefix = false, word = false, debug = false;
		
		DLBnode currNode = root;
		
		/*if ( s.toString().equals("wac") )
		{
			debug = true;
		}
		*/
		while ( counter < s.length() )
		{
			current = s.charAt(counter);
			
			if ( current == currNode.getValue() ) 	//found value
			{
				if ( debug )
					System.out.println("Found: " + current );
				
				if ( currNode.getChild() == null )	// do not have a child
				{
					counter = s.length() + 1;
					none = true;
					if ( debug )
						System.out.println("Null child. Ending..");
				
				}
				else								// have a child
				{
					counter++;
					if ( counter < s.length() )
					{
						currNode = currNode.getChild();
					}
					
					if ( debug )
					{
						System.out.println("Found child. " + current + " Proceeding..");
					}
				
				}
				
			
			}
			else									//didn't find value
			{
				
				if ( currNode.getSibling() == null )	//letter does not exist at this level -> word doesnt exist
				{
					none = true;
					counter = s.length() + 1;
					if ( debug )
					{
						System.out.println("Null sibling. Ending..");
					}
				}
				else
				{
					if ( debug )
					{
						System.out.println("Looking for: " + current );
						System.out.println("Getting next sibling. Proceeding...");
					}
					currNode = currNode.getSibling();
					
				}
			
			}
		}
		if ( debug )
		{
			System.out.println("Finished searching at : " + currNode.getValue() + " current letter: " + current);
		}
		
		if ( counter == s.length() )			// we made it through the entire word
		{
			if ( debug )
			{
				System.out.println("Checking end of word");
			}
			if ( currNode.getChild() == null )
			{
				none = true;
				if ( debug )
				{
					System.out.println( currNode.getValue() + " has no child. ");
				}
			
			}
			else
			{
				if ( debug )
				{
					System.out.println( currNode.getValue() + " has the child of ");
				}
				
				currNode = currNode.getChild();
				if ( debug )
				{
					System.out.println( currNode.getValue() );
				}
				if ( currNode.getValue() == '!' )
				{
					word = true;
					
					if ( currNode.getSibling() != null )
					{
						prefix = true;
					}
				
				}
				else
				{
					prefix = true;
					
					while ( currNode.getSibling() != null )
					{
						currNode = currNode.getSibling();
					
						if ( currNode.getValue() == '!' )
						{
							word = true;
						}
					}
				
				}
			
			}
		
		
		}
			
		
		if ( word && prefix )
		{
			return 3;
		}
		else if ( word )
		{
			return 2;
		}
		else if ( prefix )
		{
			return 1;
		}
		else
		{
		
			return 0;
		}	

	}
	
	public int searchPrefix(StringBuilder s, int start, int end)
	{
		//System.out.println("The string received is " + s.toString() );
		StringBuilder testString = new StringBuilder( s.substring( start,end ) );
		
		
		//System.out.println("The string being checked is " + testString.toString() );
		return searchPrefix(testString);
	}
	
	private class DLBnode
	{
		private char val;
		private DLBnode child;
		private DLBnode sibling;
		
		private DLBnode ()
		{
		
			val = '0';
			child = null;
			sibling = null;
		
		}
	
		private DLBnode( char value )
		{
		
			val = value;
			child = null;
			sibling = null;
		
		}
	
		private void setChild ( DLBnode theChild )
		{
		
			child = theChild;
			
			return;
		
		}
		
		private void setSibling ( DLBnode theSibling )
		{
		
			sibling = theSibling;
			
			return;
		
		}
		
		private DLBnode getChild ()
		{
		
			return child;
		
		}
		
		private DLBnode getSibling ()
		{
		
			return sibling;
		}
		
		private void setValue( char a )
		{
		
			val = a;
			return;
		}
		
		private char getValue ()
		{
		
			return val;
			
		}

	
	}

}



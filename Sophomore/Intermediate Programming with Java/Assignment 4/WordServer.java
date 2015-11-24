// This program was written by Andrew Beers on 11/1/11

import java.util.*;
import java.io.*;


public class WordServer
{
	//had to hard code index and initialize string to index here or else would get exception
	private int index,t=0;
	private String file,k,freeword;
	private String[] s;
	private int[] d;
	private boolean redo;
	private static char b;
	private int c;
	public WordServer(String f)
	{
		file = f;
		//index=0;
	}
	
	
	
	public void loadWords(Scanner S) throws IOException
	{
	
	//Loads words into an array
		index = Integer.parseInt(S.nextLine());
		s = new String[index];
		d = new int[index];
		for(int i = 0; i<index; i++)
		{
			s[i] = S.nextLine();
		}
		
	}
	
	
	
	public String getNextWord()
	{
		
		
	//Structure gets next word, making sure it does not repeat a word
		
		Random R = new Random();
		c = R.nextInt(index);
		
		if(t==index)
		{
			return(null);
		}
		
			if(t!=0)
			{	
				redo = true;
				while(redo)
				{
					redo =false;
					for(int i =0;i<t;i++)
					{
					
						if(c == d[i])
						{
							c =R.nextInt(index);
							
							redo = true;
						}
						
					}	
				}
				
			}	
		
	
			
			d[t]=c;
		
		
			t++;
			
		
			
			return(s[c]);
		
	}
	public char free(String word)
	{
		
		freeword = word;
		t=0;
		Random K = new Random();
		c = K.nextInt(freeword.length());
		b = word.charAt(c);
		
		
		return(b);
			
			
		
		
		
	}
	
	
	

}
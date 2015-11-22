// This program was written by Andrew Beers on 11/1/11

import java.util.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
public class HangPlayer
{
	private ArrayList<HangPlayer> P = new ArrayList<HangPlayer>();
	private String pname;
	private int w,l;
	
	
	
	public HangPlayer()
	{
		P = null;
	}
	//Constructor
	public HangPlayer(String name, int wins, int losses)
	{
		pname = name;
		w = wins;
		l = losses;
		
	}
	// Show Stats
	public void stats() throws IOException
	{
		
		//System.out.println("Thanks for playing " + pname + "\nYou have " + w + " wins.\nYou have " + l + " losses.");
		JOptionPane.showMessageDialog(null,pname + "\nYou have " + w + " wins.\nYou have " + l + " losses.");
	}
	//Update wins and losses 
	public void update(int wins, int losses)
	{
		w = wins;
		l = losses;
	}
	
	public void file(ArrayList<HangPlayer> A)
	{
		P = A;
		
		System.out.println(P.get(0));
	
	}
	
	// to String method
	public String toString()
	{
		StringBuilder S = new StringBuilder();
		S.append(pname);
		S.append(" ");
		S.append(Integer.toString(w));
		S.append(" ");
		S.append(Integer.toString(l));
		
		return(S.toString());
	}
	//get name, wins , losses methods
	public String getName()
	{
		return(pname);
	}
	public int getWins()
	{
		return(w);
	}
	public int getLosses()
	{
		return(l);
	}
	
	public String toString2()
	{
		String N= pname+ "\n" + Integer.toString(w) + "\n" +Integer.toString(l);
		return(N);

	}

}



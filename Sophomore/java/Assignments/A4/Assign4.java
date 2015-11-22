//This program was written by Andrew Beers on 11/18/2011


//imports
import java.util.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;


public class Assign4 
{
	//Variables
	private JFrame theWindow;	// Window
	private JPanel hangMan, status, gameButtons;
	private JLabel l1,l2,l3,l4,l5,l6,l7,l8,l9;
					// control buttons and the information
	private JButton b1,b2,b3;
					// JButtons for various actions
	private JTextField info;	
	private Container thePane;
	private gameListener Listener;
	private String k,p,newword,subguess,g="-",words,currName,gstatus,ginfo,cword,pguess,b1Text="Start Game",b2Text="End Game",b3Text="New Word",name,wins,losses;
	private int x,y,sz,turn=0,miss=7,d,c,gamestatus=0,oldw,oldl,w=0,l=0,playernum;
	private ArrayList<HangPlayer> A = new ArrayList<HangPlayer>();
	private Scanner scan = new Scanner(System.in);
	private Scanner pScan = new Scanner(new File(player));
	private boolean redo,found = false,first=true;
	private WordServer W;
	private char guess;
	private StringBuilder K;
	private	StringBuilder L = new StringBuilder();
	private MyPanel drawPanel;
	private HangFigure f;
	private HangPlayer C;
	private static String player ="players.txt";
	private static int z;
	
	
	public Assign4() throws IOException
	{
	
		//Constructor of Game Interface
		gstatus = "Game Not Started";
		ginfo = "Welcome to the Hangman Game";
		cword ="";
		pguess ="";
		hangMan = new JPanel();
		hangMan.setLayout(new GridLayout(1,1));
		drawPanel = new MyPanel(100, 100);
		hangMan.add(drawPanel);
		Listener = new gameListener();
		status = new JPanel();
		status.setLayout(new GridLayout(5,2));
		l1 = new JLabel("Game Status:");
		l2 = new JLabel(gstatus);
		l3 = new JLabel("Game Info:");
		l4 = new JLabel(ginfo);
		l5 = new JLabel("Current Word:");
		l6 = new JLabel(cword);
		l7 = new JLabel("Your Guesses:");
		l8 = new JLabel(pguess);
		l9 = new JLabel("Current Guess:");
		
		
		
		
		info = new JTextField();
		info.setEnabled(false);
		
		status.add(l1);
		status.add(l2);
		status.add(l3);
		status.add(l4);
		status.add(l5);
		status.add(l6);
		status.add(l7);
		status.add(l8);
		status.add(l9);
		status.add(info);
		info.addActionListener(Listener);
		gameButtons = new JPanel();
		gameButtons.setLayout(new GridLayout(3,1));
		
		b1 = new JButton(b1Text);
		b2 = new JButton(b2Text);
		b3 = new JButton(b3Text);
		
		
		b1.addActionListener(Listener);
		b2.addActionListener(Listener);
		b3.addActionListener(Listener);
		
		gameButtons.add(b1);
		gameButtons.add(b2);
		gameButtons.add(b3);
		b2.setEnabled(false);
		b3.setEnabled(false);
		
		
		theWindow = new JFrame("Assignment 4");
		theWindow.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
		thePane = theWindow.getContentPane();
		thePane.add(hangMan,BorderLayout.WEST);
		thePane.add(status,BorderLayout.CENTER);
		thePane.add(gameButtons,BorderLayout.EAST);
		theWindow.pack();
		theWindow.setVisible(true);
		
	}	
	
	//Action listener, main portion of program work
	private class gameListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e) 
		{
			Component theEvent = (Component) e.getSource();
			
			//Start game button, first initializes the game, then gets the next player
			if(theEvent == b1)
			{
				if(gamestatus == 0)
				{
					//initialize game by loading hangplayer objects into arraylist
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
		
					//Set buttons enabled or disabled, change interface
					gamestatus++;
					l2.setText("Ready for a Player");
					b1.setText("Next Player");
					JOptionPane.showMessageDialog(null, "Loading Players...OK");
					b2.setEnabled(true);
					info.setEnabled(false);
				}
				else
				{
					//Adding a player, get name and word file
					found =false;
					 currName = JOptionPane.showInputDialog("Enter Your Name");
					 words = JOptionPane.showInputDialog("Enter Your File of Words");
					 
					 
					 try
					 {	
					 	//load words into a wordserver
					 	W = new WordServer(words);
						Scanner inScan = new Scanner(new File(words));
					 	W.loadWords(inScan);
					 }
					 catch(IOException d)
					 {
					 	JOptionPane.showMessageDialog(null,"File does not exist, sorry. Ending program...");
					 	
					 	System.exit(0);
					 }
					 
					//Check if the player has played before from the players file
					for(int i = 0; i < (z/3); i++)
					{
						if(currName.equals(A.get(i).getName()))
						{
							found = true;
							w= A.get(i).getWins();
							l =A.get(i).getLosses();
							playernum = i;
							C = A.get(playernum);
						}
					}
					
					//Welcome returning player, show them their stats
					if(found)
					{
						JOptionPane.showMessageDialog(null, "Welcome back "+currName);
						JOptionPane.showMessageDialog(null,"The rules are as follows:\nYou guess letters for each hidden word.\nIf you are right the letters will show up in their correct locations.\nIf not you lose a turn.\nYou have 7 turns.\nGood Luck and have fun!" );
						
						JOptionPane.showMessageDialog(null,"These are your past stats: ");
					 	try
					 	{	
					 		C.stats();
						}
						 catch(IOException d)
					 	{
					 		JOptionPane.showMessageDialog(null,"Person does not exist, sorry. Ending program...");
					 		
					 		System.exit(0);
					 	}
						
					}
					
					//Welcome first time player, tell them the rules
					else
					{
						w=0;
						l=0;
						JOptionPane.showMessageDialog(null, "Welcome " + currName + " to Hangman!");
						C = new HangPlayer(currName,w,l);
						
						JOptionPane.showMessageDialog(null,"The rules are as follows:\nYou guess letters for each hidden word.\nIf you are right the letters will show up in their correct locations.\nIf not you lose a turn.\nYou have 7 turns.\nGood Luck and have fun!" );
					}
					
					//set buttons, change interface
					l4.setText("Welcome to Hangman, " + currName);
					b1.setEnabled(false);
					b3.setEnabled(true);
					
					if(currName!=null)
					{
						l2.setText("Game in Progress");
						b2.setText("End Player");
					}
				}
					
					
			}
			
			//Second button, end game, end player button
			else if(theEvent == b2)
			{
			
				drawPanel.setVisible(false);
				//End game condition
				if(gamestatus ==1 && currName == null)
				{
					
					
					JOptionPane.showMessageDialog(null,"Thanks for playing!");
					System.exit(0);
				}
				//End player condition
				if(currName!=null)
				{
					int newoption;
					//Confirmation required and handled here
					newoption = JOptionPane.showConfirmDialog (null, "Are you sure?");
					if(newoption == JOptionPane.NO_OPTION || newoption == JOptionPane.CANCEL_OPTION )
					{
						JOptionPane.showMessageDialog(null,"Continue.");
					}
					else
					{
						//if there was a pending word, give them a loss, then quit for them
						if(K!=null)
						{
							K=null;
							JOptionPane.showMessageDialog(null,"The word was " + newword);
							
							l8.setText("");
							l6.setText("");
							L = new StringBuilder();
							f = new HangFigure(x, y, sz);
							info.setEnabled(false);
							l2.setText("Player lost");
							l++;
						}
						C.update(w,l);
						
						//Show stats
						JOptionPane.showMessageDialog(null,"These are your current stats: ");
						try
						{	
						 	C.stats();
						}
						catch(IOException d)
						{
						 	JOptionPane.showMessageDialog(null,"Person does not exist, sorry. Ending program...");
					 		
						 	System.exit(0);
						}
						
						if(!found)
						{
								
								z = z+3;
								A.add(C);
						}
						//Load stats to the file
						try
						{	
						 	toFile(A);
						 	JOptionPane.showMessageDialog(null,"Stats successfully saved.");
						}
						catch(IOException d)
						{
					 		JOptionPane.showMessageDialog(null,"File does not exist, sorry. Ending program...");
					 			
					 		System.exit(0);
						}
						
						//update interface
						currName = null;
						gamestatus = 1;
						b2.setText("End Game");
						b1.setEnabled(true);
						b3.setEnabled(false);
						l2.setText("Ready for Next Player");
						info.setEnabled(false);
						l6.setText("");
						l8.setText("");
						l4.setText("Welcome to Hangman");
					}
				}
			}
			
			
			//last button, new word
			else if(theEvent == b3)
			{
			
				int option=0;
				
				//Confirmation needed, if previous word, it will give a loss, if no, then it will continue
				if(K!=null)
				{
					option = JOptionPane.showConfirmDialog (null, "Are you sure?\n(This will result in a loss of the current word)");
					if(option == JOptionPane.NO_OPTION || option == JOptionPane.CANCEL_OPTION)
					{
						JOptionPane.showMessageDialog(null,"Continue guessing at the word.");
					}
					else
					{	
						K=null;
						JOptionPane.showMessageDialog(null,"The word was " + newword);
						
						l8.setText("");
						l6.setText("");
						L = new StringBuilder();
						f = new HangFigure(x, y, sz);
						info.setEnabled(false);
						l2.setText("Player lost");
						l++;
						C.update(w,l);
						miss =7;
					}
				}
				
				if(option != JOptionPane.NO_OPTION && option != JOptionPane.CANCEL_OPTION)
				{
					l2.setText("Game in Progress");
					drawPanel.setVisible(false);
				
					K = new StringBuilder();
				
					
					l6.setText("");
				
					//get next word
				
					newword = W.getNextWord();
				
					//if returns null, the file is out of words, end player automatically
					if (newword == null)
					{
						JOptionPane.showMessageDialog(null,"Sorry no more words. Ending player..");
						JOptionPane.showMessageDialog(null,"These are your current stats: ");
						try
						{	
						 	C.stats();
						}
						catch(IOException d)
						{
						 	JOptionPane.showMessageDialog(null,"Person does not exist, sorry. Ending program...");
					 		
						 	System.exit(0);
						}
						
						HangPlayer P = new HangPlayer(currName,w,l);
						if(!found)
						{
							
							z = z+3;
						}
						A.add(P);
						try
						{	
						 	toFile(A);
					 	JOptionPane.showMessageDialog(null,"Stats successfully saved.");
						}
						
						catch(IOException d)
						{
						 	JOptionPane.showMessageDialog(null,"File does not exist, sorry. Ending program...");
						 		
					 		System.exit(0);
						}
						
						//change interface
						currName = null;
						gamestatus = 1;
						b2.setText("End Game");
						b1.setEnabled(true);
						b3.setEnabled(false);
						l2.setText("Ready for Next Player");
						info.setEnabled(false);
						l6.setText("");
						l8.setText("");
						l4.setText("Welcome to Hangman");
				}
				
					
				//Blank word to guess at
				for(int i =0; i<newword.length(); i++)
				{
					K.append(g);
				}
				k = K.toString();
				l6.setText(k);
				info.setEnabled(true);
				
				}
			}
			
			//Text box, guessing action
			else if(theEvent == info)
			{
				//Take the first character of the string of the textfield
				subguess = info.getText();
				guess=subguess.charAt(0);
				
				//Handle all cases
				
				boolean upper = Character.isUpperCase(guess);
				if (upper)
				{	
					guess = Character.toLowerCase(guess);
				}
				
				//handle non letters
				
				boolean letter = Character.isLetter(guess);
				while (!letter)
				{
					info.setText("");
					JOptionPane.showMessageDialog(null, "Not a letter, please enter a letter ");
					
					subguess = info.getText();
					guess=subguess.charAt(0);
					letter = Character.isLetter(guess);
				}
				//Handle previously guessed letters
				if(turn!=0)
				{	
					
					redo = true;
					while(redo)
					{
						redo =false;
						for(int i =0;i<L.length();i++)
						{	
							
							if(guess == L.charAt(i))
							{	
								info.setText("");
								JOptionPane.showMessageDialog(null, "Already used that Letter, please re-enter a letter ");
								subguess = info.getText();
								guess=subguess.charAt(0);
								redo=true;
							}	
						}
					}
				}
				
				L.append(guess);
				p = L.toString();
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
				k = K.toString();
				l6.setText(k);
				
				l8.setText(p);
				turn++;
				info.setText("");
				drawPanel.setVisible(true);
				if(d==newword.length())
				{
					miss=miss-1;
					//Draw chunk condition
					if(first)
					{
						drawPanel.setVisible(true);
						sz = 40;
						x = 25;
						y = 20;
						f = new HangFigure(x, y, sz);
						f.addChunk();
						drawPanel.repaint();
					}
					else
					{
						f.addChunk();
						drawPanel.repaint();
					}	
					first = false;
				}
				
				//Handle ending, win or loss, update stats
				if(k.equals(newword))
				{
					JOptionPane.showMessageDialog(null,"Congratulations, you got the word " + newword );
					l8.setText("");
					l6.setText("");
					L = new StringBuilder();
					f = new HangFigure(x, y, sz);
					info.setEnabled(false);
					l2.setText("Player won");
					w++;
					C.update(w,l);
					K=null;
					miss =7;
				}
				if(miss==0)
				{
					JOptionPane.showMessageDialog(null,"Sorry you lost, the word was " + newword );
					l8.setText("");
					l6.setText("");
					L = new StringBuilder();
					f = new HangFigure(x, y, sz);
					info.setEnabled(false);
					l2.setText("Player lost");
					l++;
					C.update(w,l);
					K=null;
					miss =7;
				}
						
			}
				
		}
				
			
			
			
		
		
	}
	
	//MyPanel class for the graphics
	class MyPanel extends JPanel
	{
		private int width, height;
		public MyPanel(int w, int h)
		{
			width = w;
			height = h;
		}

		// This method is implicitly called through the JFrame to see how much
		// space the JPanel needs.  Otherwise, the JPanel will be given a default,
		// very small size.
		public Dimension getPreferredSize()
		{
			return new Dimension(width, height);
		}

		// This method is implictly called every time the panel must be refreshed.
		// It will happen automatically if the window is minimized and then opened
		// again, or if it is covered and then exposed.  We can also request that it
		// be done at other times by calling the repaint() method, as shown above.
		public void paintComponent (Graphics g) // Pass the graphics object
												// to the Panel so it can
												// draw its shapes
		{
			super.paintComponent(g);         // don't forget this line!
			Graphics2D g2d = (Graphics2D) g;
			if (f != null)		// Note that we are calling the draw() method in
				f.draw(g2d);	// in the HangFigure class and passing the
								// Graphics2D object into it.  This way, our JPanel
						// does not have to know the specifics of how to draw the
						// figure -- it only has to know that the figure has a
						// draw() method.  See the draw() method in HangFigure for
						// more details.
		}
		
	}
	
	//toFile, to write to the file
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
	
	
	//main, constructor
	public static void main (String [] args) throws IOException
	{
		new Assign4();
	}
	


}

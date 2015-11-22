// CS 401 Fall 2011
// Assignment 5 Help Program
// Read this program carefully and run it as well.  This shows you how to
// do a number of things that are required for Assignment 5.  You may even
// use this as a starting point to build your completed assignment.  Note
// however that the credit will be given for what you ADD to this program,
// not for what has already been implemented by me!  Also note that you are
// not required to use this program at all, as long as you implement the
// assignment correctly! 

// For additional help, see Sections 14.5-14.6 in the text

import javax.swing.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.event.*;
import java.util.*;
import java.io.*;
import java.awt.print.*;

// Create enum types that will be useful in the program
enum Figures {TREE,SNOWFLAKE,GREETING,STAR,HOUSE,SNOWMAN};
enum Mode {NONE,DRAW,SELECTED,MOVING};

// Code extracted from Oracle Java Example programs.  See link below for full code:
// http://docs.oracle.com/javase/tutorial/2d/printing/examples/PrintUIWindow.java
class thePrintPanel implements Printable
{
	JPanel panelToPrint;
	
	public int print(Graphics g, PageFormat pf, int page) throws
                                                        PrinterException
    {
        if (page > 0) { /* We have only one page, and 'page' is zero-based */
            return NO_SUCH_PAGE;
        }

        /* User (0,0) is typically outside the imageable area, so we must
         * translate by the X and Y values in the PageFormat to avoid clipping
         */
        Graphics2D g2d = (Graphics2D)g;
        g2d.translate(pf.getImageableX(), pf.getImageableY());

        /* Now print the window and its visible contents */
        panelToPrint.printAll(g);

        /* tell the caller that this page is part of the printed document */
        return PAGE_EXISTS;
    }
    
    public thePrintPanel(JPanel p)
    {
    	panelToPrint = p;
    }
}

public class Assign5
{	
	private ShapePanel drawPanel;
	private JPanel buttonPanel;
	private JButton makeShape;
	private JRadioButton makeTree, makeFlake, makeGreet,makeStar,makeHouse,makeSnowman;
	private ButtonGroup shapeGroup;
	private Figures currShape;
	private JLabel msg;
	private JMenuBar theBar;
	private JMenu fileMenu,editMenu;
	private JMenuItem endProgram, printScene,save,open,saveas,newpage,cut,copy,paste;
	private JPopupMenu popper;
	private JMenuItem delete,resize;
	private JFrame theWindow;
	private int inputnewSize,numshape,option,index,shapeX,shapeY,shapeSize,saveshape,selindex,x1,y1,j=-1,k=-1;
	private String newSize,file,f,fileLine,shapename,shapeText,openFile,cutString;
	private PrintWriter fileOut;
	private boolean ans,caught=false;
	private String [] imageInfo;
	private int[] a;
	
	// This ArrayList is used to store the shapes in the program.
	// It is specified to be of type MyShape, so objects of any class
	// that implements the MyShape interface can be stored in here.
	// See Section 8.13 in your text for more info on ArrayList.
	private ArrayList<MyShape> shapeList;	
	private MyShape newShape;

	public Assign5()
	{
		drawPanel = new ShapePanel(500, 300);
		buttonPanel = new JPanel();
		buttonPanel.setLayout(new GridLayout(4, 3));

		makeShape = new JButton("Draw");

		ButtonHandler bhandler = new ButtonHandler();
		makeShape.addActionListener(bhandler);

		buttonPanel.add(makeShape);
		msg = new JLabel("");
		buttonPanel.add(msg);

		//new shapes added

		makeTree = new JRadioButton("Tree", false);
		makeFlake = new JRadioButton("Snowflake", true);
		makeGreet = new JRadioButton("Greeting", false);
		makeStar = new JRadioButton("Star",false);
		makeHouse = new JRadioButton("House",false);
		makeSnowman = new JRadioButton("Snowman",false);

		RadioHandler rhandler = new RadioHandler();
		makeTree.addItemListener(rhandler);
		makeFlake.addItemListener(rhandler);
		makeGreet.addItemListener(rhandler);
		makeStar.addItemListener(rhandler);
		makeHouse.addItemListener(rhandler);
		makeSnowman.addItemListener(rhandler);

		buttonPanel.add(makeFlake);
		buttonPanel.add(makeTree);
		buttonPanel.add(makeGreet);
		buttonPanel.add(makeStar);
		buttonPanel.add(makeHouse);
		buttonPanel.add(makeSnowman);
	
		// A ButtonGroup allows a set of JRadioButtons to be associated
		// together such that only one can be selected at a time
		shapeGroup = new ButtonGroup();
		shapeGroup.add(makeFlake);
		shapeGroup.add(makeTree);
		shapeGroup.add(makeGreet);
		shapeGroup.add(makeStar);
		shapeGroup.add(makeHouse);
		shapeGroup.add(makeSnowman);
		
		currShape = Figures.SNOWFLAKE;
		drawPanel.setMode(Mode.NONE);

		theWindow = new JFrame("Winter Paint Program");
		Container c = theWindow.getContentPane();
		drawPanel.setBackground(Color.lightGray);
		c.add(drawPanel, BorderLayout.NORTH);
		c.add(buttonPanel, BorderLayout.SOUTH);

		// Note how the menu is created.  First we make a JMenuBar, then
		// we put a JMenu in it, then we put JMenuItems in the JMenu.  We
		// can have multiple JMenus if we like.  JMenuItems generate
		// ActionEvents, just like JButtons, so we just have to link an
		// ActionListener to them.
		theBar = new JMenuBar();
		theWindow.setJMenuBar(theBar);
		fileMenu = new JMenu("File");
		
		//added edit menu
		editMenu = new JMenu("Edit");
		theBar.add(fileMenu);
		theBar.add(editMenu);
		
		//added new, open, save, save as
		printScene = new JMenuItem("Print");
		newpage = new JMenuItem("New");
		open = new JMenuItem("Open");
		save = new JMenuItem("Save");
		saveas = new JMenuItem("Save As");
		endProgram = new JMenuItem("Exit");
		fileMenu.add(printScene);
		fileMenu.add(newpage);
		fileMenu.add(open);
		fileMenu.add(save);
		fileMenu.add(saveas);
		fileMenu.add(endProgram);
		
		
		//add cut copy paste
		cut = new JMenuItem("Cut");
		copy = new JMenuItem("Copy");
		paste = new JMenuItem("Paste");
		editMenu.add(cut);
		editMenu.add(copy);
		editMenu.add(paste);
		
		paste.setEnabled(false);
		cut.setEnabled(false);
		copy.setEnabled(false);
		printScene.addActionListener(bhandler);
		newpage.addActionListener(bhandler);
		open.addActionListener(bhandler);
		save.addActionListener(bhandler);
		cut.addActionListener(bhandler);
		copy.addActionListener(bhandler);
		paste.addActionListener(bhandler);
		saveas.addActionListener(bhandler);
		endProgram.addActionListener(bhandler);


		// JPopupMenu() also holds JMenuItems.  To see how it is actually
		// brought out, see the mouseReleased() method in the ShapePanel class
		// below.
		popper = new JPopupMenu();
		delete = new JMenuItem("Delete");
		resize = new JMenuItem("Resize");
		delete.addActionListener(bhandler);
		resize.addActionListener(bhandler);
		popper.add(delete);
		popper.add(resize);
		theWindow.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		theWindow.pack();
		theWindow.setVisible(true);
	}

	public static void main(String [] args)
	{
		new Assign5();
	}

	// See Section 7.5 for information on JRadioButtons.  Note that the
	// text uses ActionListeners to handle JRadioButtons.  Clicking on
	// a JRadioButton actually generates both an ActionEvent and an
	// ItemEvent.  I am using the ItemEvent here.  To handle the event,
	// all I am doing is changing a state variable that will affect the
	// MouseListener in the ShapePanel.
	private class RadioHandler implements ItemListener
	{
		public void itemStateChanged(ItemEvent e)
		{
			if (e.getSource() == makeTree)
				currShape = Figures.TREE;
			else if (e.getSource() == makeFlake)
				currShape = Figures.SNOWFLAKE;
			else if (e.getSource() == makeGreet)
				currShape = Figures.GREETING;
			else if (e.getSource() == makeStar)
				currShape = Figures.STAR;
			else if (e.getSource() == makeHouse)
				currShape = Figures.HOUSE;
			else if (e.getSource() == makeSnowman)
				currShape = Figures.SNOWMAN;
		}
	}

	// Note how the makeShape button and moveIt menu item are handled 
	// -- we again simply set the state in the panel so that the mouse will 
	// actually do the work.  The state needs to be set back in the mouse
	// listener.
	private class ButtonHandler implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
		
			//makeshape given
			if (e.getSource() == makeShape)
			{
				drawPanel.setMode(Mode.DRAW);
				msg.setText("Position new shape with mouse");
				makeShape.setEnabled(false);
			}
			//delete given
			else if (e.getSource() == delete)
			{
				ans = drawPanel.deleteSelected();
				if (ans)
				{
					msg.setText("Shape deleted");
					drawPanel.repaint();
				}
			}
			
			// resize, calls resize for selected shape
			else if (e.getSource() == resize)
			{
				newSize = JOptionPane.showInputDialog("Enter the new size:");
				inputnewSize = Integer.parseInt(newSize);
				boolean ANS = drawPanel.resizeSelected();
				if (ANS)
				{
					msg.setText("Shape resized");
					drawPanel.repaint();
				}
				
			}
			//print given
			else if (e.getSource() == printScene)
			{
				Printable thePPanel = new thePrintPanel(drawPanel); 
			    PrinterJob job = PrinterJob.getPrinterJob();
         		job.setPrintable(thePPanel);
         		boolean ok = job.printDialog();
         		if (ok) 
         		{
             	 	try 
             	 	{
                  		job.print();
             		} 
             		catch (PrinterException ex) {
              		/* The job did not successfully complete */
             		}
             	}
            }
            
            // new page, only calls if there are shapes, option to save if changes were made
            else if (e.getSource() == newpage)
            {
            	if(numshape!=0)
				{
					int optionSave = JOptionPane.showConfirmDialog (null, "Would you like to save before clearing?");
					if(optionSave == JOptionPane.NO_OPTION || optionSave == JOptionPane.CANCEL_OPTION )
					{
						drawPanel.newPage();
            			drawPanel.repaint();
            		}
            		else
            		{
            			file = JOptionPane.showInputDialog("Enter the name of the file you would like to save to:");
					
						drawPanel.savePage(file);
					}
				}
				else
				{
					drawPanel.newPage();
            		drawPanel.repaint();
            	}
            
            }
            //calls save, only prompts if user has not saved before
            else if (e.getSource() == save)
            {
        		if(file == null)
				{
						file = JOptionPane.showInputDialog("Enter the name of the file you would like to save to:");
				}
            	drawPanel.savePage(file);
            	
            }
            
            //open, calls open, prompts user to save current if not saved yet
			else if(e.getSource() == open)
			{
				if(saveshape!=numshape)
				{
					int change = JOptionPane.showConfirmDialog (null, "Changes have been made since your last save, would you like to save?(After this save, you will immediately be prompted to open a file)");
					if(change==JOptionPane.YES_OPTION)
					{
						file = JOptionPane.showInputDialog("Enter the name of the file you would like to save to:");
					
						drawPanel.savePage(file);
					}
					drawPanel.newPage();
            		drawPanel.openPage();
					drawPanel.repaint();
				}
				else
				{
					drawPanel.newPage();
            		drawPanel.openPage();
					drawPanel.repaint();
				}
					
					
			}
			
			//save as listener, always prompts for file name, saves
            else if (e.getSource() == saveas)
            {
            		
					//Writes over previous file with current array list of hang players.
					
					file = JOptionPane.showInputDialog("Enter the name of the file you would like to save to:");
					
					drawPanel.savePage(file);
            }
            
            //cut listener, calls copy and deletes shape, sets correct buttons enabled
            else if (e.getSource() == cut)
            {
            	
            	drawPanel.copyPage();
				ans = drawPanel.deleteSelected();
				if (ans)
				{
					
					drawPanel.repaint();
					
				}
				msg.setText("Shape cut");
            	paste.setEnabled(true);
            	cut.setEnabled(false);
            	copy.setEnabled(false);
            }
            
            //copy listener, calls copy, sets correct buttons enabled
            else if (e.getSource() == copy)
            {
            	drawPanel.copyPage();
				msg.setText("Shape copied");
				paste.setEnabled(true);
				cut.setEnabled(false);
            	copy.setEnabled(false);
            }
            
            //paste listener, calls paste, sets correct buttons enabled
            else if (e.getSource() == paste)
            {
            			drawPanel.pastePage();
            			msg.setText("Shape pasted");
            			cut.setEnabled(false);
            			copy.setEnabled(false);
            			paste.setEnabled(false);
            			
            }
            //End program listener
			else if (e.getSource() == endProgram)
			{
				//check if the number of shapes has been changed to prompt to see if the user wants to save
				if(file == null&&numshape>0 || saveshape!=numshape)
				{
					option = JOptionPane.showConfirmDialog (null, "Changes have been made. Would you like to save?");
				
				
					//if so save
					if(option==JOptionPane.YES_OPTION)
					{
						
						
						file = JOptionPane.showInputDialog("Enter the name of the file you would like to save to:");
						
						drawPanel.savePage(file);
				
            		JOptionPane.showMessageDialog(null,"File successfully saved");
            		}
            		
            	//else end, end after saving
            	}
            	System.exit(0);
				
			}
		}
	}

	// Here we are extending JPanel.  This way we can use all of the
	// properties of JPanel (including generating MouseEvents) and also
	// add new instance data and methods, as shown below.  Since this is
	// an inner class, it can access instance variables from the A5Help
	// class if necessary.
	private class ShapePanel extends JPanel
	{
		
		// These instance variables are used to store the desired size
		// of the panel
		private int prefwid, prefht;

		// Store index of the selected MyShape.  This allows the Shape
		// to be moved and updated.
		

		// Keep track of positions where mouse is moved on the display.
		// This is used by mouse event handlers when moving the shapes.
		private int  x2, y2; 
		
		private boolean popped; // has popup menu been activated?

		private Mode mode;   // Keep track of the current Mode

		public ShapePanel (int pwid, int pht)
		{
			shapeList = new ArrayList<MyShape>(); // create empty ArrayList
			selindex = -1;
		
			prefwid = pwid;	// values used by getPreferredSize method below
			prefht = pht;   // (which is called implicitly).  This enables
			// the JPanel to request the room that it needs.
			// However, the JFrame is not required to honor
			// that request.

			setOpaque(true);// Paint all pixels here (See API)

			setBackground(Color.lightGray);

			addMouseListener(new MyMouseListener());
			addMouseMotionListener(new MyMover());
			popped = false;
		}  // end of constructor

		// This class is extending MouseAdapter.  MouseAdapter is a predefined
		// class that implements MouseListener in a trivial way (i.e. none of
		// the methods actually do anything).  Extending MouseAdapter allows
		// a programmer to implement only the MouseListener methods that
		// he/she needs but still satisfy the interface (recall that to
		// implement an interface one must implement ALL of the methods in the
		// interface -- in this case I do not need 3 of the 5 MouseListener
		// methods)
		private class MyMouseListener extends MouseAdapter
		{
			public void mousePressed(MouseEvent e)
			{
				x1 = e.getX();  // store where mouse is when clicked
				y1 = e.getY();

				if (!e.isPopupTrigger() && (mode == Mode.NONE ||
										    mode == Mode.SELECTED)) // left click and
				{												    // either NONE or
					if (selindex >= 0)								// SELECTED mode
					{
						unSelect();			// unselect previous shape
						mode = Mode.NONE;
						cut.setEnabled(false);
						copy.setEnabled(false);
					}
					selindex = getSelected(x1, y1);  // find shape mouse is
													 // clicked on
					if (selindex >= 0)
					{
						mode = Mode.SELECTED;  	// Now in SELECTED mode for shape
						
						// Check for double-click.  If so, show dialog to update text of
						// the current text shape (will do nothing if shape is not a MyText)
						MyShape curr = shapeList.get(selindex);
						if (curr instanceof MyText && e.getClickCount() == 2)
						{
							String newText = JOptionPane.showInputDialog(theWindow, "Enter new text [Cancel for no change]");
							if (newText != null)
								((MyText) curr).setText(newText);
						}
						cut.setEnabled(true);
						copy.setEnabled(true);
					}
					repaint();	
				}
				else if (e.isPopupTrigger() && selindex >= 0)  // if button is
				{								               // the popup menu
					popper.show(ShapePanel.this, x1, y1);      // trigger, show
					popped = true;							   // popup menu
				}											  
			}
			public void mouseReleased(MouseEvent e)
			{
				if (mode == Mode.DRAW) // in DRAW mode, create the new Shape
				{					   // and add it to the list of Shapes
					if (currShape == Figures.TREE)
					{
						newShape = new Tree(x1,y1,50);
					}
					else if (currShape == Figures.SNOWFLAKE)
					{
						newShape = new Snowflake(x1,y1,10);
					}
					else if (currShape == Figures.GREETING)
					{
						newShape = new Greeting(x1,y1,30);
					}
					else if (currShape == Figures.STAR)
					{
						newShape = new Star(x1,y1,30);
					}
					else if (currShape == Figures.HOUSE)
					{
						newShape = new House(x1,y1,30);
					}
					else if (currShape == Figures.SNOWMAN)
					{
						newShape = new Snowman(x1,y1,30);
					}
					addShape(newShape);
					makeShape.setEnabled(true);  // Set interface back to
					mode = Mode.NONE;			 // "base" state
					msg.setText("");
					numshape++;
					System.out.println(numshape);
				}
				// In MOVING mode, set mode back to NONE and unselect shape (since 
				// the move is now finished).
				else if (mode == Mode.MOVING) 
				{
					mode = Mode.NONE;
					unSelect();  
					makeShape.setEnabled(true);
					msg.setText("");
					repaint();
				}
				else if (e.isPopupTrigger() && selindex >= 0) // if button is
				{							// the popup menu trigger, show the
					popper.show(ShapePanel.this, x1, y1); // popup menu
				}
				popped = false;  // unset popped since mouse is being released
			}
		}

		// the MouseMotionAdapter has the same idea as the MouseAdapter
		// above, but with only 2 methods.  The method not implemented
		// here is mouseMoved
		private class MyMover extends MouseMotionAdapter
		{
			public void mouseDragged(MouseEvent e)
			{
				x2 = e.getX();   // store where mouse is now
				y2 = e.getY();

				// Note how easy moving the shapes is, since the "work"
				// is done within the various shape classes.  All we do
				// here is call the appropriate method.  However, we don't 
				// want to accidentally move the selected shape with a right click
				// so we make sure the popup menu has not been activated.
				if ((mode == Mode.SELECTED || mode == Mode.MOVING) && !popped)
				{
					MyShape s = shapeList.get(selindex);
					mode = Mode.MOVING;
					s.move(x2, y2);
				}
				repaint();	// Repaint screen to show updates
			}
		}

		// Check to see if point (x,y) is within any of the shapes.  If
		// so, select that shape and highlight it so user can see.
		// This version of getSelected() always considers the shapes from
		// beginning to end of the ArrayList.  Thus, if a shape is "under"
		// or "within" a shape that was previously created, it will not
		// be possible to select the "inner" shape.  In your assignment you
		// must redo this method so that it allows all shapes to be selected.
		// Think about how you would do this.
		private int getSelected(double x, double y)
		{      
			for (int i = 0; i < shapeList.size(); i++)
			{
				if (shapeList.get(i).contains(x, y))
				{
					boolean caught =true;
					shapeList.get(i).highlight(true); 
					
					return i;
				}			
			}
			return -1;	
		}
		
		// This was my attempt at cycling through shapes and logically makes sense to me but does not work
		// The idea is to have each shape index put into an array, increment the array index each time something is added, then return the array from the 0th index
		// I get an array out of bounds exception and i dont know why, please check my logic and see that i spent a lot of time on this.
		/*
		private int getSelected(double x, double y)
		{
			if(k<numshape)
				k++;
			else
				k=0;
			if(j==-1)
			{
				a = new int[numshape];
				j++;
			}
			for (int i = 0; i < shapeList.size(); i++)
			{
				if (shapeList.get(i).contains(x, y))
				{
					boolean caught =true;
					shapeList.get(i).highlight(true); 
					a[j]=i;
					if(j<numshape)
						j++;
					else 
						j=0;
				}			
			}
			if(!caught)
				return -1;
			else
			{
				return a[k];
			}
		}
		*/
		
		
		// Unselect method given
		public void unSelect()
		{
			if (selindex >= 0)
			{
				shapeList.get(selindex).highlight(false);
				selindex = -1;
			}
		}
		
		// deleteselected given, used for newpage
		public boolean deleteSelected()
		{
			if (selindex >= 0)
			{
				shapeList.remove(selindex);
				selindex = -1;
				numshape--;
				return true;
			}
			else return false;
		}
		
		//resize selected given
		public boolean resizeSelected()
		{
			if (selindex >= 0)
			{
				shapeList.get(selindex).resize(inputnewSize);
				selindex = -1;
				return true;
			}
			else return false;
		}
		
		//set mode, given
		public void setMode(Mode newMode)            // set Mode
		{
			mode = newMode;
		}

		//add shape, given
		private void addShape(MyShape newshape)      // Add shape
		{
			shapeList.add(newshape);
			repaint();	// repaint so we can see new shape
		}
		
		
		//save page method used for saving, save as, and prompted for if the user changed his screen and tried to clear before saving
		public void savePage(String f)
		{
				//try file	
				try
				{
				fileOut = new PrintWriter(f);
				}
				
				//catch any exception and shut down program to avoid error
				catch(Exception a)
				{
					JOptionPane.showMessageDialog(null,"There has been a problem. Program ending...");
					System.exit(0);
				}
				fileOut.println(numshape);
				
				//for loop for printing info to file
				for(int i = 0; i<numshape; i++)
				{
					fileOut.println(shapeList.get(i).saveData());
				}
				fileOut.close();
				
				// message to show the file was saved.
				JOptionPane.showMessageDialog(null,"File successfully saved");
				saveshape = numshape;
		}
		
		//new page, clears the current page
		public void newPage()
		{
			
			for(int i=0;i<numshape;i++)
            {
            		
            		shapeList.remove(0);
            		
            }
            	
            numshape=0;
        }
        
        // copypage, used for copy, cut, very similar to open
        public void copyPage()
        {
        	cutString= shapeList.get(selindex).saveData();
            String [] cutInfo = cutString.split(":");
			shapename = cutInfo[0];
			shapeX = Integer.parseInt(cutInfo[1]);
			shapeY = Integer.parseInt(cutInfo[2]);
			shapeSize = Integer.parseInt(cutInfo[3]);
			if(cutInfo[0].equals("Greeting"))
				shapeText = cutInfo[4];
		}
		
		//pastepage, similar to open
		public void pastePage()
		{
						if (shapename.equals("Tree"))
						{
							
							newShape = new Tree(x1,y1,shapeSize);
						}
						else if (shapename.equals("Snowflake"))
						{
							newShape = new Snowflake(x1,y1,shapeSize);
						}
						else if (shapename.equals("Greeting"))
						{
							
							newShape = new Greeting(x1,y1,shapeSize,shapeText);
						}
						else if (shapename.equals("Star"))
						{
							newShape = new Star(x1,y1,shapeSize);
						}
						else if (shapename.equals("House"))
						{
							newShape = new House(x1,y1,shapeSize);
						}
						else if (shapename.equals("Snowman"))
						{
							newShape = new Snowman(x1,y1,shapeSize);
						}
						drawPanel.addShape(newShape);
						makeShape.setEnabled(true);  // Set interface back to
									 // "base" state
						msg.setText("");
						numshape++;
		}
		
		//open page, had to check if file exists, if not clear page
        public void openPage()
        {
        		openFile = JOptionPane.showInputDialog("Enter the name of the file:");
				
				// Try file name
				try
				{
					
					Scanner S = new Scanner(new File(openFile));
					index = Integer.parseInt(S.nextLine());
					
					
					//parse savedata method for syntax and rename all variables
					for(int i = 0; i<index;i++)
					{
						
						fileLine = S.nextLine();
						String [] imageInfo = fileLine.split(":");
						shapename = imageInfo[0];
						shapeX = Integer.parseInt(imageInfo[1]);
						shapeY = Integer.parseInt(imageInfo[2]);
						shapeSize = Integer.parseInt(imageInfo[3]);
						
						
						//make shape from file with desired attributes
						if (shapename.equals("Tree"))
						{
							
							newShape = new Tree(shapeX,shapeY,shapeSize);
						}
						else if (shapename.equals("Snowflake"))
						{
							newShape = new Snowflake(shapeX,shapeY,shapeSize);
						}
						else if (shapename.equals("Greeting"))
						{
							shapeText = imageInfo[4];
							newShape = new Greeting(shapeX,shapeY,shapeSize,shapeText);
						}
						else if (shapename.equals("Star"))
						{
							newShape = new Star(shapeX,shapeY,shapeSize);
						}
						else if (shapename.equals("House"))
						{
							newShape = new House(shapeX,shapeY,shapeSize);
						}
						else if (shapename.equals("Snowman"))
						{
							newShape = new Snowman(shapeX,shapeY,shapeSize);
						}
						drawPanel.addShape(newShape);
						makeShape.setEnabled(true);  // Set interface back to
									 // "base" state
						msg.setText("");
						numshape++;
					}
					
				}
				
				//if no file, clear screen
				catch(Exception d)
				{
					JOptionPane.showMessageDialog(null,"No such file exists. Clearing page..");
					
				
				}
		}	
		// Method called implicitly by the JFrame to determine how much
		// space this JPanel wants
		public Dimension getPreferredSize()
		{
			return new Dimension(prefwid, prefht);
		}

		// This method enables the shapes to be seen.  Note the parameter,
		// which is implicitly passed.  To draw the shapes, we in turn
		// call the draw() method for each shape.  The real work is in the draw()
		// method for each MyShape
		public void paintComponent (Graphics g)    
		{
			super.paintComponent(g);         // don't forget this line!
			Graphics2D g2d = (Graphics2D) g;
			for (int i = 0; i < shapeList.size(); i++)
			{
				shapeList.get(i).draw(g2d);
			}
		}
	} // end of ShapePanel
}
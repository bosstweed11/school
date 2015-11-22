// CS 0401 Fall 2011
// This handout demonstrates extending the JPanel class to store and
// and manipulate its own graphical components.  Compare this to Counters.java.
// Note that the execution appears identical to that of Counters.java, but the
// code has some interesting differences.  

// See Chapter 7 of the Gaddis text for another example of extending JPanel
// See more comments in the code below.
// We will discuss inheritance in detail soon.

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class Counters2
{
	private JFrame theWindow;
	
	// Note now that there are very few instance variables in the Counters2
	// class -- most of the functionality has been pushed down into the
	// JPanel subclasses.
	private TopPanelType topPanel;
	private BottomPanelType bottomPanel;
	private Container thePane;
	
	public Counters2()
	{
		// All we need to set up now is the two panels within the JFrame.  The
		// setup of the components within the panels is done within those
		// classes.  Note how this approach is more modular than the one in
		// Counters.java
		topPanel = new TopPanelType();
		bottomPanel = new BottomPanelType();
		
		theWindow = new JFrame("Counting with Buttons");
		theWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		thePane = theWindow.getContentPane();

		thePane.add(topPanel, BorderLayout.NORTH);
		thePane.add(bottomPanel, BorderLayout.SOUTH);
		theWindow.pack();
		theWindow.setVisible(true);
	}

	// This class has all of the properties and functionality of a JPanel.
	// However, we are now adding more to it -- extra instance variables that
	// enhance its functionality.  Whereas before we were adding the
	// components to the JPanel for display purposes only via the
	// LayoutManager, in this version the JButtons and JLabels are actually
	// PART of the JPanel.
	private class TopPanelType extends JPanel
	{
		private JButton b1, b2, b3,c1,c2,c3,d1,d2,d3,e1,e2,e3;
	
		
		
		public TopPanelType()
		{
			setLayout(new GridLayout(4,3));
			b1 = new JButton("1");
			b2 = new JButton("2");
			b3 = new JButton("3");
			CountListener theListener = new CountListener();
			
			b1.addActionListener(theListener);
			b2.addActionListener(theListener);
			b3.addActionListener(theListener);
		
			// When adding to a JPanel, you do not need to first
			// access the ContentPane -- you add directly to the JPanel.
			// The code below is more or less the same as the code to set
			// up the JButtons and JLabels in Counters.java.  What differs
			// is the location of the code.
			add(b1);
			add(b2);
			add(b3);
	
			c1 = new JButton("4");
			c2 = new JButton("5");
			c3 = new JButton("6");
			
			c1.addActionListener(theListener);
			c2.addActionListener(theListener);
			c3.addActionListener(theListener);
			
			add(c1);
			add(c2);
			add(c3);
			
			d1 = new JButton("7");
			d2 = new JButton("8");
			d3 = new JButton("9");
			
			d1.addActionListener(theListener);
			d2.addActionListener(theListener);
			d3.addActionListener(theListener);
			
			add(d1);
			add(d2);
			add(d3);
			
			e1 = new JButton("+");
			e2 = new JButton("0");
			e3 = new JButton("-");
			
			e1.addActionListener(theListener);
			e2.addActionListener(theListener);
			e3.addActionListener(theListener);
			
			add(e1);
			add(e2);
			add(e3);

			 
		}
		
		// Note that CountListener is now within the TopPanelType
		// class, so it is two levels down from the Counters2 class.
		// Look at the .class files to see how they are named in
		// this case.
		private class CountListener implements ActionListener
		{	
			int[] a = new int[2];
			int i=0,b;
			StringBuilder N = new StringBuilder();
			String B;
			boolean done = false,result =false;
			public void actionPerformed(ActionEvent e)
			{
				if (e.getSource() == b1)
				{
					N.append("1");
					
				}
				else if (e.getSource() == b2)
				{
					N.append("2");
					
				}
				else if (e.getSource() == b3)
				{
					N.append("3");
				}
				else if (e.getSource() == c1)
				{
					N.append("4");
				}
				else if (e.getSource() == c2)
				{
					N.append("5");
				}
				else if (e.getSource() == c3)
				{
					N.append("6");
				}
				else if (e.getSource() == d1)
				{
					N.append("7");
				}
				else if (e.getSource() == d2)
				{
					N.append("8");
				}
				else if (e.getSource() == d3)
				{
					N.append("9");
				}
				else if (e.getSource() == e1)
				{
				System.out.println("hi");
					B = N.toString();
					a[i] = Integer.parseInt(B);
					System.out.println(a[i]);
					done = true;
					i++;
					StringBuilder N = new StringBuilder();
					
				}
				else if (e.getSource() == e3)
				{	
					i++;
					if(i==2)
					{
						b = a[0] +a[1];
						System.out.println(b);
					}
				}
				else
				{
					N.append("0");
				}
				if(done)
				{
					
					System.out.println(a[i]);
					done = false;
				}
				
				
				// Below is one important difference between this
				// version and Counters.java.  In Counters.java, the sum
				// variable was shared and thus could be accessed via the
				// top panel and the bottom panel.  However, in this version
				// we need to put it into one panel or the other.  Due to
				// the formatting of the components (the label showing the sum
				// is not the same layout as the other components in the upper
				// panel) I have put it into the bottom panel.  Now, in order
				// to modify it I need to call a method the I have written in
				// the BottomPanelType class.
				B = N.toString();
				bottomPanel.updateCount(B);
				
			}
			
		} // CountListener
	} // TopPanelType
	
	private class BottomPanelType extends JPanel
	{
		private JButton d5;
		private JLabel total;
		private StringBuilder N;
		
		private BottomPanelType()
		{
			QuitListener Listener = new QuitListener();
			setLayout(new GridLayout(2, 1));
			
			StringBuilder N = null;
			
			
			total = new JLabel("0");
			total.setHorizontalAlignment(SwingConstants.CENTER);
	
			add(total);
			d5 = new JButton("Exit");
			d5.addActionListener(Listener);
			add(d5);
		}
		
		// Update sum and its JLabel
		public void updateCount(String newVal)
		{
			String N = null;
			N = newVal.toString();
			
			total.setText(N);
		}
		
		// Another double-nested inner class
		private class QuitListener implements ActionListener
		{
			public void actionPerformed(ActionEvent e)
			{
					System.exit(0);
			}
		}
	}

	public static void main(String [] args)
	{
		new Counters2();
		// Since the functionality is encapsulated into the Counter2 class,
		// we can actually create multiple windows, identical in function
		// but with separate data, by creating multiple objects. Uncomment
		// the code below to see this.
		//new Counters2();
	}
}

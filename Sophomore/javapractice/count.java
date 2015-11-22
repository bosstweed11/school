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
		private JButton b1, b2, b3;
		private JLabel c1, c2, c3;
		private int count1, count2, count3;
		
		public TopPanelType()
		{
			setLayout(new GridLayout(2,3));
			b1 = new JButton("PUSH 1");
			b2 = new JButton("PUSH 2");
			b3 = new JButton("PUSH 3");
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
	
			c1 = new JLabel("0");
			c1.setHorizontalAlignment(SwingConstants.CENTER);
			c2 = new JLabel("0");
			c2.setHorizontalAlignment(SwingConstants.CENTER);
			c3 = new JLabel("0");
			c3.setHorizontalAlignment(SwingConstants.CENTER);
			
			add(c1);
			add(c2);
			add(c3);

			count1 = 0; count2 = 0; count3 = 0; 
		}
		
		// Note that CountListener is now within the TopPanelType
		// class, so it is two levels down from the Counters2 class.
		// Look at the .class files to see how they are named in
		// this case.
		private class CountListener implements ActionListener
		{
			public void actionPerformed(ActionEvent e)
			{
				if (e.getSource() == b1)
				{
					count1++;
					c1.setText(String.valueOf(count1));
				}
				else if (e.getSource() == b2)
				{
					count2++;
					c2.setText("" + count2);
				}
				else if (e.getSource() == b3)
				{
					count3++;
					c3.setText(String.valueOf(count3));
				}
				int sum = count1 + count2 + count3;
				
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
				bottomPanel.updateCount(sum);
			}
		} // CountListener
	} // TopPanelType
	
	private class BottomPanelType extends JPanel
	{
		private JButton b1;
		private JLabel total;
		private int sum;
		
		private BottomPanelType()
		{
			setLayout(new GridLayout(2, 1));
			total = new JLabel("0");
			total.setHorizontalAlignment(SwingConstants.CENTER);

			b1 = new JButton("Click to Quit");
			sum = 0;
		
			b1.addActionListener(new QuitListener());
			add(total);
			add(b1);
		}
		
		// Update sum and its JLabel
		public void updateCount(int newVal)
		{
			sum = newVal;
			total.setText("" + sum);
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

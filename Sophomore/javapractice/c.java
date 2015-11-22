

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;


public class c
{
	private JFrame theWindow;
	
	// Note now that there are very few instance variables in the Counters2
	// class -- most of the functionality has been pushed down into the
	// JPanel subclasses.
	private TopPanelType topPanel;
	private BottomPanelType bottomPanel;
	private Container thePane;
	private double hand;
	private JButton b17;
	private JLabel hands;
	
	
	public c()
	{
		// All we need to set up now is the two panels within the JFrame.  The
		// setup of the components within the panels is done within those
		// classes.  Note how this approach is more modular than the one in
		// Counters.java
		topPanel = new TopPanelType();
		bottomPanel = new BottomPanelType();
		
		theWindow = new JFrame("BLACKJACK");
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
		private JButton b1, b2, b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15;
		private JLabel c1, c2, c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15;
		private int count1, count2, count3,count4,count5,count6,count7,count8,count9,count10;
		private double count11,count12,count13,count14,count15;
		private CountListener theListener;
		public TopPanelType()
		{
			setLayout(new GridLayout(11,2));
			b1 = new JButton("WIN 5");
			b2 = new JButton("WIN 10");
			b3 = new JButton("WIN 20");
            b4 = new JButton("WIN 25");
            b5 = new JButton("WIN 30");
            b6 = new JButton("LOSE 5");
			b7 = new JButton("LOSE 10");
			b8 = new JButton("LOSE 20");
            b9 = new JButton("LOSE 25");
            b10 = new JButton("LOSE 30");
            b11 = new JButton("Blackjack on 5");
            b12 = new JButton("Blackjack on 10");
            b13 = new JButton("Blackjack on 20");
            b14 = new JButton("Blackjack on 25");
            b15 = new JButton("Blackjack on 30");
			theListener = new CountListener();
			
			b1.addActionListener(theListener);
			b2.addActionListener(theListener);
			b3.addActionListener(theListener);
			b4.addActionListener(theListener);
			b5.addActionListener(theListener);
			b6.addActionListener(theListener);
			b7.addActionListener(theListener);
			b8.addActionListener(theListener);
			b9.addActionListener(theListener);
			b10.addActionListener(theListener);
			b11.addActionListener(theListener);
			b12.addActionListener(theListener);
			b13.addActionListener(theListener);
			b14.addActionListener(theListener);
			b15.addActionListener(theListener);
			// When adding to a JPanel, you do not need to first
			// access the ContentPane -- you add directly to the JPanel.
			// The code below is more or less the same as the code to set
			// up the JButtons and JLabels in Counters.java.  What differs
			// is the location of the code.
			add(b1);
			add(b2);
			add(b3);
			add(b4);
			add(b5);
			add(b6);
			add(b7);
			add(b8);
			add(b9);
			add(b10);
			add(b11);
			add(b12);
			add(b13);
			add(b14);
			add(b15);
			c1 = new JLabel("0");
			c1.setHorizontalAlignment(SwingConstants.CENTER);
			c2 = new JLabel("0");
			c2.setHorizontalAlignment(SwingConstants.CENTER);
			c3 = new JLabel("0");
			c3.setHorizontalAlignment(SwingConstants.CENTER);
			c4 = new JLabel("0");
			c4.setHorizontalAlignment(SwingConstants.CENTER);
			c5 = new JLabel("0");
			c5.setHorizontalAlignment(SwingConstants.CENTER);
			c6 = new JLabel("0");
			c6.setHorizontalAlignment(SwingConstants.CENTER);
			c7 = new JLabel("0");
			c7.setHorizontalAlignment(SwingConstants.CENTER);
			c8 = new JLabel("0");
			c8.setHorizontalAlignment(SwingConstants.CENTER);
			c9 = new JLabel("0");
			c9.setHorizontalAlignment(SwingConstants.CENTER);
			c10 = new JLabel("0");
			c10.setHorizontalAlignment(SwingConstants.CENTER);
			c11 = new JLabel("0");
			c11.setHorizontalAlignment(SwingConstants.CENTER);
			c12 = new JLabel("0");
			c12.setHorizontalAlignment(SwingConstants.CENTER);
			c13 = new JLabel("0");
			c13.setHorizontalAlignment(SwingConstants.CENTER);
			c14 = new JLabel("0");
			c14.setHorizontalAlignment(SwingConstants.CENTER);
			c15 = new JLabel("0");
			c15.setHorizontalAlignment(SwingConstants.CENTER);
			add(c1);
			add(c2);
			add(c3);
			add(c4);
			add(c5);
			add(c6);
			add(c7);
			add(c8);
			add(c9);
			add(c10);
			add(c11);
			add(c12);
			add(c13);
			add(c14);
			add(c15);


			count1 = 0; count2 = 0; count3 = 0; count4=0;count5=0;count5=0;count6=0;count7=0;count8=0;count9=0;count10=0;count11=0;count12=0;count13=0;count14=0;count15=0;
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
				else if (e.getSource() == b4)
				{
					count4++;
					c4.setText("" + count4);
				}
				else if (e.getSource() == b5)
				{
					count5++;
					c5.setText(String.valueOf(count5));
				}
				else if (e.getSource() == b6)
				{
					count6++;
					c6.setText("" + count6);
				}
				else if (e.getSource() == b7)
				{
					count7++;
					c7.setText(String.valueOf(count7));
				}
				else if (e.getSource() == b8)
				{
					count8++;
					c8.setText("" + count8);
				}
				else if (e.getSource() == b9)
				{
					count9++;
					c9.setText(String.valueOf(count9));
				}
				else if (e.getSource() == b10)
				{
					count10++;
					c10.setText("" + count10);
				}
				else if (e.getSource() == b11)
				{
					count11++;
					c11.setText("" + count11);
				}
				else if (e.getSource() == b12)
				{
					count12++;
					c12.setText("" + count12);
				}
				else if (e.getSource() == b13)
				{
					count13++;
					c13.setText(String.valueOf(count13));
				}
				else if (e.getSource() == b14)
				{
					count14++;
					c14.setText("" + count14);
				}
				else if (e.getSource() == b15)
				{
					count15++;
					c15.setText("" + count15);
				}
				else if(e.getSource() == b17)
				{
					hand--;
					hands.setText("" + hand);
				}
				
				
				double sum = 5*count1 + 10*count2 + 20*count3 + 25*count4 + 30*count5 - 5*count6-10*count7-20*count8-25*count9-30*count10+7.5*count11+15*count12+30*count13+ 37.5*count14+ 45*count15;
				hand = count1 + count2 + count3 + count4 + count5 + count6+count7+count8+count9+count10+count11+count12+count13+ count14+ count15;
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
				bottomPanel.updateHands(hand);
			}
		} // CountListener
	} // TopPanelType
	
	private class BottomPanelType extends JPanel
	{
		private JButton b2;
		private JLabel total;
		private double sum;
		private double hand;
		
		private BottomPanelType()
		{
			setLayout(new GridLayout(2, 1));
			total = new JLabel("0");
			total.setHorizontalAlignment(SwingConstants.CENTER);
			hands = new JLabel("0");
			hands.setHorizontalAlignment(SwingConstants.CENTER);
			

			b2 = new JButton("Click to Quit");
			sum = 0;
			hand = 0;
			//b17.addActionListener();
			b2.addActionListener(new QuitListener());
			add(total);
			add(b2);
			add(hands);
			
			
		}
		
		// Update sum and its JLabel
		public void updateCount(double newVal)
		{
			sum = newVal;
			total.setText("" + sum);
		}
		public void updateHands(double newVal)
		{
			hand = newVal;
			
			hands.setText("" + hand);
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
		new c();
		// Since the functionality is encapsulated into the Counter2 class,
		// we can actually create multiple windows, identical in function
		// but with separate data, by creating multiple objects. Uncomment
		// the code below to see this.
		//new Counters2();
	}
}

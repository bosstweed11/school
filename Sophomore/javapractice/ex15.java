import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

public class ex15
{

	public static void main(String [] args)
	{
		JFrame theWindow = new JFrame("Example 15");
		theWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		JButton theButton = new JButton("Count to 6!");
		theButton.setFont(new Font("Serif",Font.ITALIC + Font.BOLD,72));
		
		theButton.setForeground(Color.RED);
		theButton.setOpaque(true);
		theButton.setBackground(Color.WHITE);
		
		theButton.addActionListener(new MyListener());
		
		Container thePane = theWindow.getContentPane();
		thePane.add(theButton);
		
		theWindow.pack();
		
		theWindow.setVisible(true);
	}
}
	
	class MyListener implements ActionListener
	{
		static int toggle =0;
		public void actionPerformed(ActionEvent e)
		{
		 
			Component theEventer = (Component) e.getSource();
			if (toggle==0)
			{
			
				((JButton)theEventer).setForeground(Color.BLACK);
				((JButton)theEventer).setText("1");
				toggle = 1;
			}
			else if (toggle==1)
			{
			
				((JButton)theEventer).setForeground(Color.BLUE);
				((JButton)theEventer).setText("2");
				toggle = 2;
			}
			else if (toggle==2)
			{
			
				((JButton)theEventer).setForeground(Color.YELLOW);
				((JButton)theEventer).setText("3");
				toggle = 3;
			}
			else if (toggle==3)
			{
			
				((JButton)theEventer).setForeground(Color.CYAN);
				((JButton)theEventer).setText("4");
				toggle = 4;
			}
			else if (toggle==4)
			{
			
				((JButton)theEventer).setForeground(Color.MAGENTA);
				((JButton)theEventer).setText("5");
				toggle = 5;
			}
			else if (toggle==5)
			{
			
				((JButton)theEventer).setForeground(Color.ORANGE);
				((JButton)theEventer).setText("6");
				toggle = 6;
			}
			else
			{
				
				toggle = 0;
				System.exit(0);
			}
		}
	}

		
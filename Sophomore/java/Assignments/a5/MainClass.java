import java.awt.Color;
import java.awt.Graphics;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class MainClass extends JPanel {

  public static void main(String[] a) {
    JFrame f = new JFrame();
    f.setSize(400, 400);
    f.add(new MainClass());
    f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    f.setVisible(true);
  }

  public void paint(Graphics g) {
    g.setColor (Color.yellow);  
    g.fillOval (5, 15, 50, 75);    
    g.setColor(Color.red);
    g.fillOval(40,30,50,50);
  }
}
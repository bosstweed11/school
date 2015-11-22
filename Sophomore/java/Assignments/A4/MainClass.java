import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class MainClass extends JPanel  implements ActionListener {
  JTextField jtf = new JTextField(15);
  public MainClass() {
 
      add(jtf); 
      jtf.addActionListener(this); 
    } 
   
    // Show text when user presses ENTER. 
    public void actionPerformed(ActionEvent ae) { 
      System.out.println(jtf.getText()); 
    } 

  public static void main(String[] args) {
    JFrame frame = new JFrame();
    frame.getContentPane().add(new MainClass());

    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    frame.setSize(200, 200);
    frame.setVisible(true);
  }

}


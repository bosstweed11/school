//package trainControl.TrackController;

import javax.swing.JFrame;

public class trackControllerTest {

	
	public static void main(String[] args) {
		
		JFrame frame = new JFrame("Track Controller");
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(600, 150);
		
		frame.add(new display());
		frame.setVisible(true);
		

	}

}

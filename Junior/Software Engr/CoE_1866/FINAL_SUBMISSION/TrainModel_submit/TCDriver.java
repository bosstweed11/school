//This is a driver program
import java.util.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class TCDriver{
	public TrainModel train;
	
	public static void main(String [] args){
		JFrame theWindow;
		Container thePane;
		//TrainPanel b;
		//ArrayList<HashMap<String,Object>> theRoute = new ArrayList<HashMap<String,Object>>();
		//HashMap<String,Object> a = new HashMap<String,Object>();
		//theRoute.add(a);
		//a = new TrainController(1);
		//b = new TrainPanel();
		train = new TrainModel;
		
		theWindow = new JFrame("Train Model");	
		thePane = theWindow.getContentPane();					
		thePane.setLayout(new GridLayout(1, 1));
	
	
		thePane.add(b.getTrainPanel());
		theWindow.pack();
		theWindow.setVisible(true);
		
		
		b.createController(1,5,"Red",theRoute);
		
		while(true){
		
			
			try{
				Thread.sleep(1500);
			}
			catch(Exception e){
				System.exit(1);
			}
			b.updateTrains();
		
		
		}
	}
	
	
}
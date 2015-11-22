//Andrew Beers MyPanel class 11/9
import java.util.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;


public class TrainPanel{

	
	
	//variables
	private String errorMsg;
	private TrainModel train;
	//public  ArrayList<TrainController> controllerList = new ArrayList<TrainController>(1);
	private Station station= new Station();
	private int numControllers=0;
	private String setLimit;
	private double trainSetSpeed;
	private JFrame theWindow;
	private Container thePane;
	private Dimension dim = new Dimension(880,400);
	private JPanel controllerPanel;
	private JPanel array [];				
	private JPanel columnFour,topPanel,columnTwo,columnThree,columnOne, leftPanel,mainPanel,dropDownPanel,messagePanel,linesPanel,totalPanel,nextLinePanel,levelPanel;
	private JButton eBrake,setSpeedLimit,passEBrake,engineFail,brakeFail,signalFail,lightSwitch,doorSwitch,incSpeed,decSpeed;
	private MyListener theListener;
	private JLabel messageText, IDText, passCountText,currSpeedText,currSpeedLimitText,currSpeedLimitTextValue,currSetSpeedText,currLineText,currAuthorityText, IDTextValue,currLineTextValue,currAuthorityTextValue, passCountTextValue, currSetSpeedTextValue,currSpeedTextValue,lightTextValue,doorTextValue,nextText;
	//public JComboBox trainList= new JComboBox();
	private HashMap<String,TrainController> controllerList = new HashMap<String,TrainController>();
	

	
	//Initializer for the Panel
	
	public TrainPanel() {
		//action listener for buttons
		theListener = new MyListener();
		
		//initialize dropbox
	
		
		
		
		//Action listener for drop down menu
		/*trainList.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent event) {
                //
                // Get the source of the component, which is our combo
                // box.
                //
                JComboBox comboBox = (JComboBox) event.getSource();
 
                //
                // Print the selected items and the action command.
                //
                String selected = (String)comboBox.getSelectedItem();
                
                String command = event.getActionCommand();
                
 				train = controllerList.get(selected);
                update();
                System.out.println("heheehrherherh\n"); 
                
            }
        });*/
	
		
		// Window holder of the panel
		theWindow = new JFrame("Train Controller");	
		thePane = theWindow.getContentPane();					
		thePane.setLayout(new GridLayout(1, 1));
		
		//Set dimensions of the Panel
		controllerPanel = new JPanel(new GridLayout(1, 2));		
		
		
		//Break up panel into 2 pieces
		mainPanel = new MyPanel(400,500);
		mainPanel.setLayout(new GridLayout(2,1));
		
		//column 1 of the top half of panel
		columnOne = new MyPanel(100,100);
		columnOne.setLayout(new GridLayout(4,1));
		
		
		//column 2 of the top half of panel
		columnTwo = new MyPanel(100,100);
		columnTwo.setLayout(new GridLayout(9,1));
		
		//column 3 of the top half of panel
		columnThree = new MyPanel(100,100);
		columnThree.setLayout(new GridLayout(9,1));
		
		//column 4 of the top half of panel
		columnFour = new MyPanel(100,100);
		columnFour.setLayout(new GridLayout(5,1));
		
		//Top half of the Panel
		topPanel = new MyPanel(100,100);
		topPanel.setLayout(new GridLayout(1,3));
		
		
		//Bottom half of the Panel
		messagePanel = new MyPanel(100,100);
		messagePanel.setLayout(new GridLayout(1,1));
		
		
		
		
		
		/*//Button for emergency brake
		eBrake = new JButton("Emergency Brake");
		eBrake.setFont(new Font("Serif", Font.BOLD, 16));
		eBrake.addActionListener(theListener);
		*/
		
		//Button for passenger emergency brake
		passEBrake = new JButton("Passenger Emergency Brake");
		passEBrake.setFont(new Font("Serif", Font.BOLD, 16));
		passEBrake.addActionListener(theListener);
		
		
		//Button for engine failure
		engineFail = new JButton("Engine Failure");
		engineFail.setFont(new Font("Serif", Font.BOLD, 16));
		engineFail.addActionListener(theListener);
		
		
		//Button for brake failure
		brakeFail = new JButton("Brake Failure");
		brakeFail.setFont(new Font("Serif", Font.BOLD, 16));
		brakeFail.addActionListener(theListener);
		
		
		//Button for signal failure
		signalFail = new JButton("Signal Failure");
		signalFail.setFont(new Font("Serif", Font.BOLD, 16));
		signalFail.addActionListener(theListener);


		//Label for bottom message value
		messageText = new JLabel("N/A",SwingConstants.CENTER);
		messageText.setFont(new Font("Serif", Font.ITALIC, 16));
		messageText.setBorder(BorderFactory.createLineBorder(Color.gray));
		
		
		
		//Label for TrainID field
		IDText = new JLabel("Train ID",SwingConstants.CENTER);
		IDText.setFont(new Font("Serif", Font.ITALIC, 16));
		IDText.setBorder(BorderFactory.createLineBorder(Color.gray));
		
		
		//Label for Passenger Count field
		passCountText = new JLabel("Passenger Count",SwingConstants.CENTER);
		passCountText.setFont(new Font("Serif", Font.ITALIC, 16));
		passCountText.setBorder(BorderFactory.createLineBorder(Color.gray));
		
		//Label for speed limit field
		currSpeedLimitText = new JLabel("Speed Limit",SwingConstants.CENTER);
		currSpeedLimitText.setFont(new Font("Serif", Font.ITALIC, 16));
		currSpeedLimitText.setBorder(BorderFactory.createLineBorder(Color.gray)); 
		
		
		//Label for Current Speed field
		currSetSpeedText = new JLabel("Set Speed",SwingConstants.CENTER);
		currSetSpeedText.setFont(new Font("Serif", Font.ITALIC, 16));
		currSetSpeedText.setBorder(BorderFactory.createLineBorder(Color.gray));
		
		//Label for Current Speed field
		currSpeedText = new JLabel("Current Speed",SwingConstants.CENTER);
		currSpeedText.setFont(new Font("Serif", Font.ITALIC, 16));
		currSpeedText.setBorder(BorderFactory.createLineBorder(Color.gray));
		
		currLineText = new JLabel("Line",SwingConstants.CENTER);
		currLineText.setFont(new Font("Serif", Font.ITALIC, 16));
		currLineText.setBorder(BorderFactory.createLineBorder(Color.gray));
		
		currAuthorityText = new JLabel("Authority",SwingConstants.CENTER);
		currAuthorityText.setFont(new Font("Serif", Font.ITALIC, 16));
		currAuthorityText.setBorder(BorderFactory.createLineBorder(Color.gray));
		
		
		//Label for light switch field
		lightSwitch = new JButton("Light Switch");
		lightSwitch.setFont(new Font("Serif", Font.BOLD, 16));
		lightSwitch.addActionListener(theListener);
		
		
		//Label for door switch field
		doorSwitch = new JButton("Door Switch");
		doorSwitch.setFont(new Font("Serif", Font.BOLD, 16));
		doorSwitch.addActionListener(theListener);
		
		
		//Label for Speed Control field
		JLabel setSpeed = new JLabel("<html> <u>Speed Control</u></html>",SwingConstants.CENTER);
		setSpeed.setFont(new Font("Serif", Font.ITALIC, 16));
		setSpeed.setBorder(BorderFactory.createLineBorder(Color.gray));
		
		
		//Label for increase speed field
		setSpeedLimit = new JButton("Set Speed Limit");
		setSpeedLimit.setFont(new Font("Serif", Font.BOLD, 16));
		setSpeedLimit.addActionListener(theListener);
		
		
		
		
		
		//Label for TrainID value
		IDTextValue = new JLabel("",SwingConstants.CENTER);
		IDTextValue.setFont(new Font("Serif", Font.ITALIC, 16));
		IDTextValue.setBorder(BorderFactory.createLineBorder(Color.gray));
		
		
		//Label for Passenger count value
		passCountTextValue = new JLabel("",SwingConstants.CENTER);
		passCountTextValue.setFont(new Font("Serif", Font.ITALIC, 16));
		passCountTextValue.setBorder(BorderFactory.createLineBorder(Color.gray));
		
		//Label for speed limit field
		currSpeedLimitTextValue = new JLabel("Speed Limit",SwingConstants.CENTER);
		currSpeedLimitTextValue.setFont(new Font("Serif", Font.ITALIC, 16));
		currSpeedLimitTextValue.setBorder(BorderFactory.createLineBorder(Color.gray)); 
	
		//Label for current Speed value
		currSetSpeedTextValue = new JLabel("70 km/h",SwingConstants.CENTER);
		currSetSpeedTextValue.setFont(new Font("Serif", Font.ITALIC, 16));
		currSetSpeedTextValue.setBorder(BorderFactory.createLineBorder(Color.gray));
	
		//Label for current Speed value
		currSpeedTextValue = new JLabel("0 km/h",SwingConstants.CENTER);
		currSpeedTextValue.setFont(new Font("Serif", Font.ITALIC, 16));
		currSpeedTextValue.setBorder(BorderFactory.createLineBorder(Color.gray));
		
		//Label for current Speed value
		currLineTextValue = new JLabel("",SwingConstants.CENTER);
		currLineTextValue.setFont(new Font("Serif", Font.ITALIC, 16));
		currLineTextValue.setBorder(BorderFactory.createLineBorder(Color.gray));
		/*
		//Label for current authority value
		currAuthorityTextValue = new JLabel("",SwingConstants.CENTER);
		currAuthorityTextValue.setFont(new Font("Serif", Font.ITALIC, 16));
		currAuthorityTextValue.setBorder(BorderFactory.createLineBorder(Color.gray));
		*/
	
		//Label for light switch value
		lightTextValue = new JLabel("NO CURRENT TRAIN",SwingConstants.CENTER);
		doorTextValue = new JLabel("NO CURRENT TRAIN",SwingConstants.CENTER);
		
		lightTextValue.setFont(new Font("Serif", Font.ITALIC, 16));
		lightTextValue.setBorder(BorderFactory.createLineBorder(Color.gray));
		
		doorTextValue.setFont(new Font("Serif", Font.ITALIC, 16));
		doorTextValue.setBorder(BorderFactory.createLineBorder(Color.gray));
		
	
	
	
		//Label for train select value
		nextText = new JLabel("Train Select",SwingConstants.CENTER);
	    nextText.setFont(new Font("Serif", Font.ITALIC, 16));
	    nextText.setBorder(BorderFactory.createLineBorder(Color.gray));

	    //add fields to column 1
	    columnOne.add(nextText);
		columnOne.add(trainList);
	    columnOne.add(setSpeed);
	    columnOne.add(setSpeedLimit);
	    
	    
	    //add fields to column 2
	    columnTwo.add(IDText);
	    columnTwo.add(passCountText);
	    columnTwo.add(currSpeedLimitText);
	    columnTwo.add(currSetSpeedText);
	    columnTwo.add(currSpeedText);
	    columnTwo.add(currLineText);
	    columnTwo.add(currAuthorityText);
	    columnTwo.add(lightSwitch);
	    columnTwo.add(doorSwitch);
	    
	    //add fields to column 3
	    columnThree.add(IDTextValue);
	    columnThree.add(passCountTextValue);
	    columnThree.add(currSpeedLimitTextValue); 
	    columnThree.add(currSetSpeedTextValue);
	    columnThree.add(currSpeedTextValue);
	    columnThree.add(currLineTextValue);
	    columnThree.add(currAuthorityTextValue);
	    columnThree.add(lightTextValue);
	    columnThree.add(doorTextValue);
	    
	     //add fields to column 3
	    columnFour.add(eBrake);
	    columnFour.add(passEBrake);
	    columnFour.add(engineFail);
	    columnFour.add(brakeFail);
	    columnFour.add(signalFail);
	    
	
		//add columns to the top panel
		topPanel.add(columnOne);
		topPanel.add(columnTwo);
		topPanel.add(columnThree);
		topPanel.add(columnFour);
		
		//add message text to the bottom message panel
		messagePanel.add(messageText);
		
		//add both main panels to the holder main panel
		mainPanel.add(topPanel);
		mainPanel.add(messagePanel);
		
		//add the main panel to the window panel
		controllerPanel.add(mainPanel);
		
		//add the window panel to the window pane
		thePane.add(controllerPanel);
		theWindow.pack();
		theWindow.setVisible(false);
		
		//for loop to initialized drop down menu, and populate with 10 controllers
		//createController(1,5,"red");
		
		//train = controllerList.get("1");
			
		
	
	
	}
	
	
	class MyListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			double theSpeed;
			String message;
			
			if(numControllers==0||e.getSource() == doorSwitch){
				if(numControllers==0)
					messageText.setText("No Trains");
				
				if(e.getSource() == doorSwitch){
				
					//createController(numControllers+1,6,"Red");
					train.setDoors(!train.getDoors());
					
					if(train.getDoorStatus()){
						doorTextValue.setText("Open");
					}
					else{
						doorTextValue.setText("Closed");
					}
					
				
				}
			}
			else{
			
				if(e.getSource() == setSpeedLimit)	//decrease speed button
				{	
					setLimit = JOptionPane.showInputDialog("Enter the Set Speed You Wish To Set For This Train");
					if(setLimit == null){
						messageText.setText("Action Cancelled");
					}
					else if(isInteger(setLimit)){
					
						
						trainSetSpeed = Double.parseDouble(setLimit);
						if(trainSetSpeed<0){
							messageText.setText("Set Speed is not allowed to be negative.");
						}
						else if(trainSetSpeed>train.getSpeedLimit()){
							messageText.setText("Set Speed is higher than the Speed Limit.");
						}
						else{
							
							train.setSpeed(trainSetSpeed);
							currSetSpeedTextValue.setText(train.getSetSpeed()+ "km/h");
						}
						
					}
					else{
					
						messageText.setText("Invalid input");
					}
				
				}/*
				else if(e.getSource() == engineFail)	//engine failure button
				{
					
					
					if(train.engineFailure){
						engineFail.setText("Fix Failure");
						message =train.fixFailure(1);
						
					}
					else{
						engineFail.setText("Engine Failure");
						message = train.setFailure(1);
					
					}
					messageText.setText(message);
				}
				else if(e.getSource() == brakeFail)		//brake failure button
				{
					
					
					if(train.brakeFailure){
						brakeFail.setText("Fix Brake");
						message =train.fixFailure(2);
						
					}
					else{
						brakeFail.setText("Brake Failure");
						message = train.setFailure(2);
					}
					
					messageText.setText(message);
				}
				else if(e.getSource() == signalFail)	//signal failure button
				{
					
					if(train.signalFailure){
						signalFail.setText("Signal Failure");
						message=train.fixFailure(3);
						
					}
					else{
						signalFail.setText("Fix Failure");
						message = train.setFailure(3);
					}
					
					messageText.setText(message);
				}*/
				else if(e.getSource() == lightSwitch){
					train.setLights(!train.getLights());
					
					if(train.getLights()){
						lightTextValue.setText("On");
					}
					else{
						lightTextValue.setText("Off");
					}
					
				
				}
				else if(e.getSource() == eBrake){
					
					//stationVisit();
					
				
				}
			}
			
		}
	}
	
	//updates new train controller info
	public void update(){
		currSpeedTextValue.setText(Math.floor(train.getSpeed())+ "km/h");
		IDTextValue.setText(""+train.trainID);
		passCountTextValue.setText(""+train.getPassengers());
		
		//currAuthorityTextValue.setText(train.getAuthority() +"");
		currLineTextValue.setText(train.getLine());
		currSetSpeedTextValue.setText(train.getSetSpeed()+"km/h");
		currSpeedLimitTextValue.setText(train.getSpeedLimit()+"km/h");
		
		if(train.getDoors()){
			doorTextValue.setText("Open");
		}
		else{
			doorTextValue.setText("Closed");
		}
		
		if(train.getLights()){
			lightTextValue.setText("On");
		}
		else{
			lightTextValue.setText("Off");
		}
		
		
		
		
	}
	/*
	public void createController(Integer ID,int authority, String theLine,int firstBlock, TrackModel theRoute){
		TrainController a = new TrainController(ID,authority,theLine,firstBlock, theRoute);
		controllerList.put(ID.toString(),a);
		trainList.addItem(ID.toString());
		train = a;
		numControllers++;
		trainList.setSelectedItem(a.getTrainID());
		messageText.setText("New Train Added");
		currSetSpeedTextValue.setText(train.getSetSpeed() +"km/h");
		update();
		
		
	}*/
	/*
	public void removeController(Integer ID){
		System.out.println("tried to remove index " + (ID-1) +"\n");
		if(numControllers<=1){
			messageText.setText("Cannot Remove Train");
		}
		else{
			controllerList.remove(ID.toString());
			trainList.removeItem(ID.toString());
			numControllers--;
		}
		
	
	}*/
	/*
	public void stationVisit(){
	
		int currPassengers;
		int passengersOff;
		int passengersOn;
		currPassengers= train.getPassengerCount();
			
		train.setDoorStatus(true);
		doorTextValue.setText("Open");
		try{
				Thread.sleep(60000);
		}
		catch(Exception e){
			System.exit(1);
		}
		passengersOff=station.getPassengersOff(currPassengers);
		
		
		passengersOn=station.getPassengersOn((currPassengers-passengersOff));
		
		train.setPassengerCount(currPassengers-passengersOff+passengersOn);
		
		train.setDoorStatus(false);
		doorTextValue.setText("Closed");
		passCountTextValue.setText(""+train.getPassengerCount());
		
	}
	*/
	public JPanel getTrainPanel(){
	
	
		return controllerPanel;
	
	}
	
	public void emergencyStop(){
	
		messageText.setText("EMERGENCY BRAKE IMMEDIATELY, INCOMING TRAIN");
	
	}
	/*
	public int getTrainAuthority(Integer ID){
	
		return(controllerList.get(ID.toString()).getAuthority());
	
	}
	*/
	/*
	public void setTrainAuthority(Integer ID, Integer authority){
	
		controllerList.get(ID.toString()).setAuthority(authority);
		
		currAuthorityTextValue.setText(authority +"");
	
	}*/
	
	public double getTrainSpeed(Integer ID){
	
		return(controllerList.get(ID.toString()).getCurrSpeed());
	}
	
	public String getTrainRoute(Integer ID){
	
		return(controllerList.get(ID.toString()).getLine());
	}
	
	public int getTrainPassengers(Integer ID){
	
		return(controllerList.get(ID.toString()).getPassengerCount());
	
	}
	
	private boolean isInteger(String input)  
	{  
   		try  
   		{  
     	 	Double.parseDouble(input);  
      		return true;  
   		}  
   		catch(Exception e)  
   		{  
      		return false;  
   		}  
}  

	public void requestSpeedChange(double trainSetSpeed, Integer a){
	
		String current = a.toString();
		train = controllerList.get(current);
		
		if(trainSetSpeed<0){
				messageText.setText("Set Speed is not allowed to be negative.");
		}
		else if(trainSetSpeed>train.speedLimit){
				messageText.setText("Set Speed is set too high.");
		}
		else{
		
		System.out.println("Train set speed(TP): " + trainSetSpeed + "\n");
		train.setSetSpeed(trainSetSpeed);
		}
	
	
	}
	
	public ArrayList<HashMap<String,Object>> updateTrains(){
		//make arraylist of hashmaps each hashmap is each train arraylist is all trains
		//keys are "line","currentBlock","authority","trainID" << all strings -> objects
		Set<String> theSet = controllerList.keySet();
		ArrayList<HashMap<String,Object>> trains = new ArrayList<HashMap<String,Object>>();
		HashMap<String,Object> location = new HashMap<String,Object>();
		TrainController curr;
		update();
		for(String k: theSet){
			
			curr =controllerList.get(k);
	
			curr.updateTrain();
			
			location.put("currentBlock",curr.getBlockID());
			location.put("trainID",k);
			location.put("line", curr.getLine());
			location.put("authority",curr.getAuthority());
			location.put("previousBlock",curr.getPrevBlock());
			trains.add(location);
			
			
			
		
		}
		
		return trains;
	
	}
	
	public void lightsOn(Integer ID){
		
		controllerList.get(ID.toString()).setLightStatus(true);
		update();
	
	}
	
	public void lightsOff(Integer ID){
		controllerList.get(ID.toString()).setLightStatus(false);
		update();
	}
	
	

	

}
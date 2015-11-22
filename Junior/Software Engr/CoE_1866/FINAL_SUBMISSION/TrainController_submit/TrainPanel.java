/*
Train Panel Class Andrew Beers Started on 11/9/12 Finished on 12/13/12
*/

import java.util.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

/*
TrainPanel main class: holder for all Train Controllers and UI
*/

public class TrainPanel{

	//public variables
	public JComboBox trainList= new JComboBox();
	
	
	//private variables
	private String errorMsg;
	private TrainController currController;
	private Station station= new Station();
	private int numControllers=0;
	private String setLimit,tempLimit;
	private double trainSetSpeed;
	private JFrame theWindow;
	private Container thePane;
	private Dimension dim = new Dimension(880,400);
	private JPanel controllerPanel;
	private JPanel array [];				
	private JPanel columnFour,topPanel,columnTwo,columnThree,columnOne, leftPanel,mainPanel,dropDownPanel,messagePanel,linesPanel,totalPanel,nextLinePanel,levelPanel;
	private JButton eBrake,setSpeedLimit,setTemp,passEBrake,engineFail,brakeFail,signalFail,lightSwitch,doorSwitch,incSpeed,decSpeed;
	private MyListener theListener;
	private JLabel messageText, IDText, temperatureText,temperatureTextValue, passCountText,currSpeedText,currSpeedLimitText,currSpeedLimitTextValue,currSetSpeedText,currLineText;
	private JLabel currAuthorityText, IDTextValue,currLineTextValue,currAuthorityTextValue;
	private JLabel passCountTextValue, currSetSpeedTextValue,currSpeedTextValue,lightTextValue,doorTextValue,nextText;
	private HashMap<String,TrainController> controllerList = new HashMap<String,TrainController>();
	private Integer tempVal;
	private int authorityLimit=5;

	
	/*
	Initializer for the Panel
	*/
	
	public TrainPanel() {
		//action listener for buttons
		theListener = new MyListener();
	
		//Action listener for drop down menu
		trainList.addActionListener(new ActionListener(){
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
                
 				currController = controllerList.get(selected);
                update();
                
                
            }
        });
	
		
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
		columnOne.setLayout(new GridLayout(7,1));
		
		
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
		
		//Button for emergency brake
		eBrake = new JButton("Emergency Brake");
		eBrake.setFont(new Font("Serif", Font.BOLD, 16));
		eBrake.addActionListener(theListener);
		
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
		
		//Label for current authority value
		currAuthorityTextValue = new JLabel("",SwingConstants.CENTER);
		currAuthorityTextValue.setFont(new Font("Serif", Font.ITALIC, 16));
		currAuthorityTextValue.setBorder(BorderFactory.createLineBorder(Color.gray));
		
	
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
	    
	    //label for temperature text
	    temperatureText = new JLabel("<html><u><i>Temperature</i></u></html>",SwingConstants.CENTER);
	    temperatureText.setFont(new Font("Serif", Font.ITALIC, 16));
	    temperatureText.setBorder(BorderFactory.createLineBorder(Color.gray));
	    
	    //button for temperature 
	    setTemp = new JButton("Set Temperature");
		setTemp.setFont(new Font("Serif", Font.BOLD, 16));
		setTemp.addActionListener(theListener);
	    
	    //label for temperature value
	    temperatureTextValue = new JLabel("NO TRAIN F",SwingConstants.CENTER);
	    temperatureTextValue.setFont(new Font("Serif", Font.ITALIC, 16));
	    temperatureTextValue.setBorder(BorderFactory.createLineBorder(Color.gray));

	    //add fields to column 1
	    columnOne.add(nextText);
		columnOne.add(trainList);
		columnOne.add(temperatureText);
		columnOne.add(setTemp);
		columnOne.add(temperatureTextValue);
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
		
	}
	
	/*
	ActionListener for all Train Controller UI
	*/
	
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
				
					currController.setDoorStatus(!currController.getDoorStatus());
					
					if(currController.getDoorStatus()){
						doorTextValue.setText("Open");
					}
					else{
						doorTextValue.setText("Closed");
					}
					
				
				}
			}
			else{
			
				if(e.getSource() == setSpeedLimit)
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
						else if(trainSetSpeed>currController.getSpeedLimit()){
							messageText.setText("Set Speed is higher than the Speed Limit.");
						}
						else{
							
							currController.setSetSpeed(trainSetSpeed);
							currSetSpeedTextValue.setText(currController.getSetSpeed()+ "km/h");
						}
						
					}
					else{
					
						messageText.setText("Invalid input");
					}
				
				}
				else if(e.getSource() == setTemp){
					tempLimit = JOptionPane.showInputDialog("Enter the Set Speed You Wish To Set For This Train");
					if(tempLimit == null){
						messageText.setText("Action Cancelled");
					}
					else if(isInteger(tempLimit)){
					
						
						tempVal = Integer.parseInt(tempLimit);
						if(tempVal<50){
							messageText.setText("Temperature too cold.");
						}
						else if(tempVal>70){
							messageText.setText("Temperature too hot.");
						}
						else{
							
							currController.setTemp(tempVal);
							temperatureTextValue.setText(currController.getTemp()+ " F");
							
						}
						
					}
					else{
					
						messageText.setText("Invalid input");
					}
				
				
				}
				else if(e.getSource() == engineFail)	//engine failure button
				{
					
					
					if(currController.engineFailure){
						engineFail.setText("Fix Failure");
						message =currController.fixFailure(1);
						
					}
					else{
						engineFail.setText("Engine Failure");
						message = currController.setFailure(1);
					
					}
					messageText.setText(message);
				}
				else if(e.getSource() == brakeFail)		//brake failure button
				{
					
					
					if(currController.brakeFailure){
						brakeFail.setText("Fix Brake");
						message =currController.fixFailure(2);
						
					}
					else{
						brakeFail.setText("Brake Failure");
						message = currController.setFailure(2);
					}
					
					messageText.setText(message);
				}
				else if(e.getSource() == signalFail)	//signal failure button
				{
					
					if(currController.signalFailure){
						signalFail.setText("Signal Failure");
						message=currController.fixFailure(3);
						
					}
					else{
						signalFail.setText("Fix Failure");
						message = currController.setFailure(3);
					}
					
					messageText.setText(message);
				}
				else if(e.getSource() == lightSwitch){
					currController.setLightStatus(!currController.getLightStatus());
					
					if(currController.getLightStatus()){
						lightTextValue.setText("On");
					}
					else{
						lightTextValue.setText("Off");
					}
					
				
				}
				else if(e.getSource() == eBrake){
					
					stationVisit(currController.getTrainID());
					
				
				}
			}
			
		}
	}
	
	/*
	Updates new train controller info
	*/
	
	public void update(){
		
		if(!controllerList.isEmpty()){
			currSpeedTextValue.setText(Math.floor(currController.getCurrSpeed())+ "km/h");
			IDTextValue.setText(""+currController.getTrainID());
			passCountTextValue.setText(""+currController.getPassengerCount());
			
			currAuthorityTextValue.setText(currController.getAuthority() +"");
			currLineTextValue.setText(currController.getLine());
			currSetSpeedTextValue.setText(currController.getSetSpeed()+"km/h");
			currSpeedLimitTextValue.setText(currController.getSpeedLimit()+"km/h");
			temperatureTextValue.setText(currController.getTemp()+ " F");
			
			if(currController.getDoorStatus()){
				doorTextValue.setText("Open");
			}
			else{
				doorTextValue.setText("Closed");
			}
			
			if(currController.getLightStatus()){
				lightTextValue.setText("On");
			}
			else{
				lightTextValue.setText("Off");
			}
		}

	}
	
	/*
	createController: creates a new Train Controller and adds it to the combo box
	*/
	
	public void createController(Integer ID,int authority, String theLine,int firstBlock, TrackModel theRoute){
		TrainController a = new TrainController(ID,authority,theLine,firstBlock, theRoute);
		controllerList.put(ID.toString(),a);
		trainList.addItem(ID.toString());
		currController = a;
		numControllers++;
		trainList.setSelectedItem(a.getTrainID());
		messageText.setText("New Train Added");
		currSetSpeedTextValue.setText(currController.getSetSpeed() +"km/h");
		temperatureTextValue.setText(currController.getTemp() +" F");
		update();
		
		
	}
	
	/*
	removeController: removes train controller of the ID parameter
	*/
	
	public void removeController(Integer ID){
		System.out.println("tried to remove index " + (ID-1) +"\n");
		/*
		if(numControllers<=1){
			messageText.setText("Cannot Remove Train");
		}
		else{
			controllerList.remove(ID.toString());
			trainList.removeItem(ID.toString());
			numControllers--;
			
		}
		*/
		
		controllerList.remove(ID.toString());
		trainList.removeItem(ID.toString());
		numControllers--;
		if(numControllers==0){
			IDTextValue.setText("");
			passCountTextValue.setText("");
			lightTextValue.setText("");
			doorTextValue.setText("");
			temperatureTextValue.setText("");
			currSpeedLimitTextValue.setText("");
		
		
			currSetSpeedTextValue.setText(""); 
		
	
		
			currSpeedTextValue.setText(""); 
			currLineTextValue.setText("NO TRAIN");
	
			currAuthorityTextValue.setText("");
		
		}
	
	}
	
	/*
	stationVisit: simulates train visiting a station based off of ID parameter
	*/
	
	public void stationVisit(Integer ID){
	
		int currPassengers;
		int passengersOff;
		int passengersOn;
		currController = controllerList.get(ID.toString());
		currPassengers= currController.getPassengerCount();
		
		
		passengersOff=station.getPassengersOff(currPassengers);
		
		
		passengersOn=station.getPassengersOn((currPassengers-passengersOff));
		
		currController.setPassengerCount(currPassengers-passengersOff+passengersOn);
		
		passCountTextValue.setText(""+currController.getPassengerCount());
		
	}
	
	/*
	getTrainPanel: returns UI panel 
	*/
	
	public JPanel getTrainPanel(){
	
	
		return controllerPanel;
	
	}
	
	/*
	getTrainAuthority: returns authority of train of ID parameter
	*/
	
	public int getTrainAuthority(Integer ID){
	
		return(controllerList.get(ID.toString()).getAuthority());
	
	}
	
	/*
	setTrainAuthority: sets train authority to train ID of parameter
	*/
	
	public void setTrainAuthority(Integer ID, Integer authority){
		if(authority>5 || authority<0){
			authority =5;
		}
		controllerList.get(ID.toString()).setAuthority(authority);
		
		currAuthorityTextValue.setText(authority +"");
	
	}
	
	/*
	getTrainSpeed: returns the current velocity of the Train ID of parameter
	*/
	
	public double getTrainSpeed(Integer ID){
	
		return(controllerList.get(ID.toString()).getCurrSpeed());
	}
	
	/*
	getTrainRoute: returns a string of "Red" or "Green" of the asked for Train route
	*/
	
	public String getTrainRoute(Integer ID){
	
		return(controllerList.get(ID.toString()).getLine());
	}
	
	/*
	getTrainPassengers: returns current number of passengers
	*/
	
	public int getTrainPassengers(Integer ID){
	
		return(controllerList.get(ID.toString()).getPassengerCount());
	
	}
	
	/*
	isInteger: returns if the input is an integer
	*/
	
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

	/*
	requestSpeedChange: changes speed limit for the train and checks if it is a legal speed change
	*/

	public void requestSpeedChange(double trainSetSpeed, Integer a){
	
		String current = a.toString();
		currController = controllerList.get(current);
		
		if(trainSetSpeed<0){
				messageText.setText("Set Speed is not allowed to be negative.");
		}
		else if(trainSetSpeed>currController.speedLimit){
				messageText.setText("Set Speed is set too high.");
		}
		else{
		
		System.out.println("Train set speed(TP): " + trainSetSpeed + "\n");
		currController.setSetSpeed(trainSetSpeed);
		}
	
	
	}
	
	/*
	updateTrains: update train loop, part of the main system update; returns to TrackController
	*/
	
	public ArrayList<HashMap<String,Object>> updateTrains(){
		if(!controllerList.isEmpty()){
			//make arraylist of hashmaps each hashmap is each train arraylist is all trains
			//keys are "line","currentBlock","authority","trainID" << all strings -> objects
			Set<String> theSet = controllerList.keySet();
			ArrayList<HashMap<String,Object>> trains = new ArrayList<HashMap<String,Object>>();
			HashMap<String,Object> location;
			TrainController curr;
			update();
			for(String k: theSet){
				location = new HashMap<String,Object>();
				curr =controllerList.get(k);
		
				curr.updateTrain();
				
				location.put("currentBlock",curr.getBlockID());
				location.put("trainID",k);
				System.out.println("TRAIN ID: " + k);
				location.put("line", curr.getLine());
				location.put("authority",curr.getAuthority());
				location.put("previousBlock",curr.getPrevBlock());
				trains.add(location);
				
			}
		
			return trains;
	
		}
		else{
		
			return null;
		}
	
	}
	
	/*
	lightsOn: function will turn the Trains lights on for the ID of the parameter
	*/
	
	public void lightsOn(Integer ID){
		
		controllerList.get(ID.toString()).setLightStatus(true);
		update();
	
	}
	
	/*
	lightsOff: function will turn the Trains lights off for the ID of the parameter
	*/
	
	public void lightsOff(Integer ID){
		controllerList.get(ID.toString()).setLightStatus(false);
		update();
	}
	
	
	/*
	doors Open: function will change the door status in the event of a station visit
	*/
	
	public void doorsOpen(Integer ID,boolean status){
		
		controllerList.get(ID.toString()).setDoorStatus(status);
		if(controllerList.get(ID.toString()).getDoorStatus()){
			doorTextValue.setText("Open");
		}
		else{
			doorTextValue.setText("Closed");
		}
		update();
	
	}
	

	
	/*
	isStopped: will return if the train with the ID passed as a parameter is stopped
	*/
	public String getLine(Integer ID){
		return controllerList.get(ID.toString()).getLine();
	}
	
	/*
	isStopped: will return if the train with the ID passed as a parameter is stopped
	*/
	public int getCurrentBlock(Integer ID){
		return controllerList.get(ID.toString()).getBlockID();
	}
	
	/*
	isStopped: will return if the train with the ID passed as a parameter is stopped
	*/
	public int getPreviousBlock(Integer ID){
		return controllerList.get(ID.toString()).getPrevBlock();
	}
	
	/*
	isStopped: will return if the train with the ID passed as a parameter is stopped
	*/
	
	public boolean isStopped(Integer ID){
	
		boolean stopped = false;
		
		if(controllerList.get(ID.toString()).getCurrSpeed()==0){
			stopped = true;
		}
		
		return stopped;

	}
	
	/*
	setStationSpeed: will set the speed of a train after a station visit has occured
	*/
	
	public void setStationSpeed(Integer ID, Integer newSpeed){
		controllerList.get(ID.toString()).setSpeedLimit(newSpeed);
		controllerList.get(ID.toString()).setSetSpeed(newSpeed);
		
	}
	
	/*
	simulationMode: method to start or stop 10x speed mode of the program
	*/

	public void simulationMode(int multiplier) {
		Set<String> theSet = controllerList.keySet();
		TrainController curr;
		
		for(String k: theSet){
			
			curr =controllerList.get(k);
			
			curr.simulationMode(multiplier);
		}
		
	}

	

}
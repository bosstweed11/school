/*
Train Controller Class Andrew Beers Started on 11/9/12 Finished on 12/13/12
*/
import java.util.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

/*
Train Controller class: Deals directly with its assigned Train Model handling power,
lights, doors, failure modes, and any other pertinent information about the Train Model

*/

public class TrainController {

	//public variables
	public double speedLimit= 70;
	public boolean engineFailure=false;
	public boolean brakeFailure =false;
	public boolean signalFailure =false;
	
	//private variables
	private double currSpeed=0;
	private TrainModel myTrain;
	private String nextStation;
	private int trainID;
	private int blockID;
	private int authority;
	private int currPassengers=0;
	private double power;
	private double setSpeed;
	private TrainPanel panel;
	private String errorMsg;
	private boolean doorOpen=false;
	private boolean lightOn=false;
	private double uk=.5;
	private double kp =20932;
	private double ki=20932;
	private double ekPrev;
	private double ek;
	private double T=.5;
	private double pCmd;
	private long lastTimeV=-1;
	private double pMax=128000;
	private int temp;
	
	
	/*
	Train Controller Constructor: Creates and initializes its associated Train Model
	*/
	
	public TrainController(int ID,int theAuthority,String theLine, int block, TrackModel model){
		trainID = ID;
		createTrain(theLine,block, model);
		setAuthority(theAuthority);
		ekPrev =0;
		pCmd =0;
		
		
	}
	
	/*
	createTrain method: Calls Train Model constructor with parameters to create the Train
	*/
	
	private void createTrain(String theLine,int block, TrackModel model){
	
		myTrain = new TrainModel(1,trainID,theLine,block,model);
		
	}
	
	/*
	getPrevBlock method: Gets the block the Train was previously on for the
	Track Controller to give direction of the train
	*/
	
	public Integer getPrevBlock(){
		return myTrain.getPrevBlock();
	}
	
	/*
	setFailure method: Called to alert Train Controller to set a failure
	based off of the parameter given to it, it will return the error message
	associated with that failure, along with handling the failure
	*/
	
	public String setFailure(int failNum){
		String errorMsg= null;
	
		if(failNum==1){
			errorMsg = "Engine Failure: Braking";
			myTrain.setSpeedLimit(0);
			myTrain.setSetSpeed(0);
			myTrain.eBrake();
			engineFailure = true;
			
		}
		else if(failNum==2){
			errorMsg = "Brake Failure: Hold on Tight";
			brakeFailure = true;
		}
		else if(failNum==3){
			errorMsg = "Signal Failure: Braking";
			myTrain.setSpeedLimit(0);
			myTrain.setSetSpeed(0);
			myTrain.eBrake();
			signalFailure = true;	
		}
		
		return errorMsg;
	
	}
	
	/*
	fixFailure method: called with a parameter of the failure type to 
	fix, the method will return the 
	*/
	
	public String fixFailure(int failNum){
		String errorMsg= null;
	
		if(failNum==1){
			errorMsg = "Engine Failure: Fixed";
			myTrain.setSpeedLimit(40);
			myTrain.setSetSpeed(40);
			myTrain.fixBrake();
			engineFailure = false;
			
		}
		else if(failNum==2){
			errorMsg = "Brake Failure: Fixed";
			brakeFailure = false;
		}
		else if(failNum==3){
			errorMsg = "Signal Failure: Fixed";
			myTrain.setSpeedLimit(40);
			myTrain.setSetSpeed(40);
			myTrain.fixBrake();
			signalFailure = false;
			
		}
		
		return errorMsg;
	
	}
	
	/*
	getPassengerCount: returns the current passenger count on the train
	*/
	
	public int getPassengerCount(){
		return myTrain.getPassengers();
	
	}
	
	/*
	setPassengerCount: assigns passenger count on the train to the parameter given
	*/
	
	public void setPassengerCount(int passengers){
		 myTrain.setPassengers(passengers);
		 
		 return;
	
	}
	
	/*
	getDoorStatus: returns the status of the doors (true-open false-closed)
	*/
	
	public boolean getDoorStatus(){
		return myTrain.getDoors();
	
	}
	
	/*
	setDoorStatus: assigns door status passed in to the train doors(true-open false-closed)
	*/
	
	public void setDoorStatus(boolean status){
		myTrain.setDoors(status);
	
	}
	
	/*
	getLightStatus: returns status of the lights (true -on false-off)
	*/
	
	public boolean getLightStatus(){
		return myTrain.getLights();
	
	}
	
	/*
	setLightStatus: assigns door status passed in to the train lights (true -on false-off)
	*/
	
	public void setLightStatus(boolean status){
		myTrain.setLights(status);
	
	}
	
	/*
	getCurrSpeed: returns the current velocity of the train
	*/
	
	public double getCurrSpeed(){
		return myTrain.getSpeed();
	
	}
	
	/*
	setPower: algorithm for applying power to the train
	*/
	
	public void setPower(){
		long curTime = System.currentTimeMillis();
		if(lastTimeV == -1){
			lastTimeV = curTime;
		}
		long timePassed = curTime-lastTimeV;
		lastTimeV = curTime;
		ek = myTrain.getSetSpeed()-myTrain.getSpeed();
		
		uk = uk +(timePassed/2000)*(ek+ekPrev);
		ekPrev = ek;
		
		pCmd = kp*ek +ki*uk;
		
		if(pCmd<pMax){
			
		}
		else{
			pCmd = pMax;
		}
		System.out.println("Power: " +pCmd + "\nEk: " + ek + "\nTrainSpeed: " + myTrain.getSpeed());
		myTrain.setPower(pCmd);
	}
	
	/*
	setSetSpeed: assigns a new set speed or a desired speed of the train based
	off of the parameter
	*/
	
	public void setSetSpeed(double lim){
	
		myTrain.setSetSpeed(lim);
	
	}
	
	/*
	getSetSpeed: gets the current set speed or desired speed from the train
	*/
	
	public double getSetSpeed(){
		
		return myTrain.getSetSpeed();
	
	}
	
	/*
	setSpeedLimit: sets the maximum allowable speed of the train
	*/
	
	public void setSpeedLimit(double lim){
	
		myTrain.setSpeedLimit(lim);
	
	}
	
	/*
	getSpeedLimit: gets the maximum allowable speed for the train
	*/
	
	public double getSpeedLimit(){
		
		return myTrain.getSpeedLimit();
	
	}
	
	/*
	getTrainID: returns the train ID
	*/
	
	public int getTrainID(){
		return myTrain.trainID;
	
	}
	
	/*
	getLine: returns "Red" or "Green" depending on the line the train is on
	*/
	
	public String getLine(){
		return myTrain.line;
	}
	
	/*
	getAuthority: returns the current authority of the train
	*/
	
	public int getAuthority(){
		return authority;
	
	}
	
	/*
	setAuthority: sets the current authority based off of the parameter, and if the
	train is close to a stopping point, speed limits and set speeds will automatically 
	be set to a lower value
	*/
	
	public void setAuthority(Integer auth){
		
		if ((auth> 5) || (auth<0))
			auth = 5;
		
		if(auth>=0){
			if(auth ==3){
				myTrain.setSetSpeed(35);
				myTrain.setSpeedLimit(35);
			}
			else if(auth ==2){
				myTrain.setSetSpeed(20);
				myTrain.setSpeedLimit(20);
			}
			else if(auth ==1){
				myTrain.setSetSpeed(10);
				myTrain.setSpeedLimit(10);
			}
			else if(auth ==0){
				myTrain.setSetSpeed(0);
				myTrain.setSpeedLimit(0);
			
			}
			authority=auth;
		}
	
	}
	
	/*
	updateTrain: main update loop, calculates a new power for the train
	*/
	
	public void updateTrain(){
		setPower();
	
	}
	
	/*
	setBlockID: sets the current block ID
	*/
	
	public void setBlockID(int a){
	
		myTrain.setBlockID(a);
	}
	
	/*
	getBlockID: gets the block ID of the current block the train is on
	*/
	
	public int getBlockID(){
		
		return myTrain.getBlockID();
	
	}
	
	/*
	setTemp: sets the temperature
	*/
	
	public void setTemp(int a){
	
		myTrain.setTemp(a);
	}
	
	/*
	getTemp: gets the temperature
	*/
	
	public int getTemp(){
		
		return myTrain.getTemp();
	
	}

	public void simulationMode(int multiplier) {
		myTrain.simulationMode(multiplier);
		
	}
	
	
	

	
	

}


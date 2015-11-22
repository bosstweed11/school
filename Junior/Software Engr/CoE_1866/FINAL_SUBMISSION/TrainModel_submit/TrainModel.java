//
//  Caleb Dusenbery
//  Mysterious Steam
//  CoE 1186
//	Created 11/9/2012
//
import java.util.*;

public class TrainModel {
	public int trainID;
	public String line;
	private int passengerCount;
	private int crewCount=1;
	private int blockID;
	private double length;
	private double width;
	private double heigth;
	private double mass;
	private double speed;
	private double SetSpeed;
	private double speedLimit;
	private boolean light;
	private double curspeed;
	private double currentSpeed;
	private boolean door;
	private int temperature;
	private Movement tMove;
	private ArrayList<HashMap<String,Object>> myRoute;
	private TrackModel theTrack;
		
	
	public TrainModel(int crew, int newID,String theLine, int block, TrackModel model){
		/*
			Initial setup of the train 
			sets up intial variables
		*/
		System.out.println("birth\n");
		passengerCount = 20;//initial passengers
		length = 32.2;//m
		width = 3.42;//m
		heigth = 2.65;//m
		light = false;//off
		door = false;//closed
		trainID = newID;
		line = theLine;
		blockID = block;
		theTrack = model;
		temperature = 70;
		
		tMove = new Movement(theTrack, line, block);
		if(theLine.equals("Red")){
			setSpeedLimit(40);
			tMove.setSpeed =40;
		}
		else{
			setSpeedLimit(70);
			tMove.setSpeed =70;
		}
		determineWeight();
		System.out.println("CURRENT SPEED LIMIT:"+ speedLimit);
		
		//FailureModes tFail = new FailureModes();//not used anymore
	}
	public void determineWeight() {
		/*
			Calculates the weight of the train
			Usually called after a station visit
		*/
		double passWeight = 200;
		mass = passWeight * (passengerCount + crewCount);
		tMove.weight = mass;
	}
	
	public double getSpeed() {
		/*
			Returns the current speed of the train
		*/
		curspeed = tMove.kilozPerHour;
		System.out.println("CURRENT SPEED :"+ curspeed);
		return curspeed;
	}
	
	public void setSpeed(double setSpeed) {
		/*
			Sets the set speed of the train
		*/
		tMove.setSpeed=setSpeed;
	}
	
	public void setPower(double power) {
		/*
			Sets the power of the train
			calls update speed to add new acceleration
		*/
		//System.out.println("power in model\n");
		tMove.updatePower(power);
		tMove.updateSpeed();
		
	}
	
	public int getPassengers() {
		/*
			get current passengers
		*/
		System.out.println("got passengers in model\n");
		return passengerCount;
	}
	
	public void setPassengers(int passint) {
		/*
			set current passengers
		*/
		System.out.println("set passengers in model\n");
		passengerCount = passint;
		determineWeight();
	}
	
	public void setLights(boolean setlight) {
		/*
			set lights
		*/
		System.out.println("lights in model\n");
		light = setlight;
	}
	
	public boolean getLights() {
		/*
			get lights
		*/
		return light;
	}
	
	public void setDoors(boolean setDoors) {
		/*
			set doors
		*/
		System.out.println("doors in model\n");
		door = setDoors;
	}
	
	public boolean getDoors() {
		/*
			get door state
		*/
		return door;
	}
	
	public double getSetSpeed() {		
		/*
			get setSpeed
		*/
		return tMove.setSpeed;
	}
	
	public void setSetSpeed(double limit){
		/*
			set Set speed
		*/
		System.out.println("SET SPEED: "+ limit);
		tMove.setSpeed = limit;
	}
	
	public double getSpeedLimit() {		
		/*
			get speed limit from track
		*/
		if(speedLimit <= tMove.tCom.speedLimit){
			return speedLimit;
		}else{
			return tMove.tCom.speedLimit;
		}
	}
	
	public void setSpeedLimit(double limit){
		/*
			set speed limit
		*/
		System.out.println("SPEED LIMIT: "+ limit);
		speedLimit = limit;
	}
	
	public void eBrake(){
		/*
			Emergency Brake
		*/
		tMove.eBrake();
	}
	
	public void fixBrake(){
		/*
			reset brake ater failure
		*/
		tMove.fixBrake();
	}
	
	public void setRoute (ArrayList<HashMap<String,Object>> theRoute){
		/*
			set Route
		*/
		myRoute= theRoute;
	}
	
	public void setBlockID(int a){
		/*
			set initial block
		*/
		blockID =a;
	}
	
	public int getBlockID(){
		/*
			get current block
		*/
		return blockID = tMove.tCom.block;
	}
	
	public Integer getPrevBlock(){
		/*
			get previous block
		*/
		return tMove.tCom.lastBlock;
	}
	
	public void setTemp(int temp){
		/*
			set temperature
		*/
		temperature = temp;
	}
	
	public int getTemp(){
		/*
			get temperature
		*/
		return temperature;
	}
	/*
	public boolean brakeFail(){
		tFail.simBrakeFail();
		return tFail.brakeFailure;
	}
	
	public boolean sigFail(){
		tFail.simSignalFail();
		return tFail.signalFailure;
	}
	
	public boolean engFail(){
		tFail.simEngineFail();
		return tFail.EngineFailure;
	}
	*/
}



import java.io.File;
import java.util.*;

import javax.swing.JComboBox;
import javax.swing.JPanel;


public class Synchronize {
	
	private HashMap<String, TrackController> controllers;
	private TrackModel trackModel;
	private TrainPanel trainController;
	
	private String GRADE = "grade";
	private String GREEN = "Green";
	private String RED = "Red";
	
	private boolean redCrossingStatus;
	private boolean greenCrossingStatus;
		
	private Status myStatus = Status.OK;
	
	private ArrayList<HashMap<String, Object>> currTrains;
	private ArrayList<Integer> removedTrains;
	private HashMap<Integer, Long> trainsAtStation;
	
	private int numberOfLines;
	
	
	public Synchronize() {
		
		controllers = new HashMap<String, TrackController>();
		trackModel = new TrackModel();
		trainController = new TrainPanel();
		currTrains = new ArrayList<HashMap<String, Object>>();
		removedTrains = new ArrayList<Integer>();
		trainsAtStation = new HashMap<Integer, Long>();
				
	}
	
	public void loadTrack(File[] files) {
		
		String line;
		
		for(int i=0; i<files.length; i++){
			line = trackModel.loadTrack(files[i]);
			createController(line);			
			
			numberOfLines++;
			
		}
	}
	
	
	private void createController(String line) {
		
			controllers.put(line, new TrackController(line, trackModel));
	}


	public synchronized boolean addTrain(int trainID, int authority, String line) {
		
		int firstBlockID;
		HashMap<String, Object> tempTrain = new HashMap<String, Object>();
		
		TrackController controller = controllers.get(line);
		
		//Check if the block the train will first appear on is occupied
		if (controller.checkFirstBlock())	
			return false;
		
		//Check if the first switch is in the correct position
		if (controller.checkFirstSwitch()) {
			if (!controller.toggleFirstSwitch())
				return false;  //Returns false if unable to toggle the switch in the correct position
		}
		
		firstBlockID = controller.getFirstBlockID();

		trainController.createController(trainID, authority, line, firstBlockID, trackModel);
		controller.addTrain(trainID);
		
		//Temporarily add this train to the list of current trains upon creation.  Data will be overwritten on an update
		tempTrain.put("trainID", Integer.toString(trainID));
		tempTrain.put("line", line);
		tempTrain.put("currentBlock", firstBlockID);
		tempTrain.put("authority", authority);
		
		
		currTrains.add(tempTrain);

		return true;		
	}	
	
	public synchronized void update(){
		
		int updatedAuthority, prevBlock, trainID, currentBlock, authority;
		String line;
		HashMap<Integer, Integer> previousBlocks = new HashMap<Integer, Integer>();
		ArrayList<HashMap<String, Object>> currTrainsTemp;
		
		
		
		
		
		if (currTrains != null){
			
			for (HashMap<String, Object> train : currTrains) {
				trainID = Integer.parseInt((String)train.get("trainID"));
				currentBlock = (Integer)train.get("currentBlock");
				
				previousBlocks.put(trainID, currentBlock);
				
			}
			
			currTrainsTemp = trainController.updateTrains(); //trainID, line, currentBlock, authority 
			
			if (currTrainsTemp == null)
				return;
			else
				currTrains = currTrainsTemp;
			
	
			if (currTrains.size() == 1) {
				controllers.get(RED).setOneTrain(true);
				controllers.get(GREEN).setOneTrain(true);
			}
			else
			{
				if ((controllers.get(RED) != null) && (controllers.get(GREEN) != null)){
					controllers.get(RED).setOneTrain(false);
					controllers.get(GREEN).setOneTrain(false);
				}
			}
			
			for (HashMap<String, Object> train : currTrains) {

				trainID = Integer.parseInt((String)train.get("trainID"));
				prevBlock = previousBlocks.get(trainID);
				currentBlock = (Integer)train.get("currentBlock");
				authority = (Integer)train.get("authority");
				line = (String)train.get("line");
				
				
				TrackController controller = controllers.get(line);

				if ( currentBlock == 0) //Check if train has entered the yard, remove train if it has
				{
					trainController.removeController(trainID);
					removedTrains.add(trainID); //Allows CTC to know train is removed
					controller.removeTrain(trainID);
					if (line.equals(RED))
						trackModel.occupyBlock(line, 9, -1);
					else
						trackModel.occupyBlock(line, 58, -1);
					continue; //Train no longer exists, move on to the next one
				}
				
				if (((controller.atStation(currentBlock)) && (authority != 5))) {
					trainController.setTrainAuthority(trainID, 0);
					if (trainController.isStopped(trainID)) {
						stationVisit(trainID, (Integer)train.get("previousBlock"));
						System.out.println("StationVisit");
					}
				}
				if ((authority == 0) && (!controller.atStation(currentBlock)))
				{
					updatedAuthority = controller.updateAuthority(5, currentBlock, (Integer)train.get("previousBlock"));
					trainController.setTrainAuthority(trainID, updatedAuthority);
				}
				
				if (prevBlock != currentBlock) {
					if (!controller.atStation(currentBlock)){
						updatedAuthority = controller.updateAuthority(authority, currentBlock, prevBlock);
						System.out.println("Updated Authority " + updatedAuthority);
						trainController.setTrainAuthority(trainID, updatedAuthority);
					}
					
					//controller.checkSpecialBlocks();
					
					controller.updateSwitch(train, prevBlock);

					if (controller.isUnderground(currentBlock)){
						trainController.lightsOn(trainID); 
					}
					else{
						trainController.lightsOff(trainID); 
					}
					
					if (line.equals(RED)){
						redCrossingStatus = controller.activateCrossings(line, currentBlock, (Integer)train.get("previousBlock"));
					}
					else {
						greenCrossingStatus = controller.activateCrossings(line, currentBlock, (Integer)train.get("previousBlock"));
					}
						
					
				}
				
			}   	
		}
		
		
		System.out.println(currTrains);
		
		
			
	}
	
	public boolean getRedCrossingStatus(){
		return redCrossingStatus;
	}
	
	public boolean getGreenCrossingStatus(){
		return greenCrossingStatus;
	}
	
	public boolean containsTrain(String line, int blockNumber)
	{
		return trackModel.containsTrain(line, blockNumber);
	}
	
	
	private void stationVisit(int trainID, int prevBlock) {
		if (trainsAtStation.containsKey(trainID)) {
			long currentTime = System.currentTimeMillis();
			long timeEnteredStation = trainsAtStation.get(trainID);
			
			if (currentTime - timeEnteredStation >= 60000){
				finishedStationVisit(trainID, prevBlock);
			}
			
		}
		else {
			trainsAtStation.put(trainID, System.currentTimeMillis());
			trainController.doorsOpen(trainID, true);
			trainController.stationVisit(trainID);
		}
		
	}

	public void finishedStationVisit(int trainID, int prevBlock)
	{
		int thisTrainID, updatedAuthority, currentBlock;
		String line;
		
		for (HashMap<String, Object> train : currTrains) {
			
			thisTrainID = Integer.parseInt((String)train.get("trainID"));
			
			if (thisTrainID == trainID)
			{
				line = (String)train.get("line");
				currentBlock = (Integer)train.get("currentBlock");
				System.out.println("curr: "+ currentBlock +" prev: " + prevBlock);//
				//updatedAuthority = controllers.get(line).updateAuthority(5, currentBlock, prevBlock);
				trainController.setTrainAuthority(trainID, 5);
				trainController.setStationSpeed(trainID, trackModel.speedLimit(line, trackModel.next(line, currentBlock, prevBlock)));
				trainsAtStation.remove(trainID);
				trainController.doorsOpen(trainID, false);
				break;
			}
			
		}
	}
	
	public ArrayList<Integer> getRemovedTrains()
	{
		ArrayList<Integer> temp = removedTrains;
		removedTrains = new ArrayList<Integer>();
		return temp;
	}

	public int getBlock(int trainIDFromCTC)
	{
		int thisTrainID;
		
		//System.out.println("TrackController getBlock Called.");
		
		for (HashMap<String, Object> train : currTrains) {
			thisTrainID = Integer.parseInt((String)train.get("trainID"));
			System.out.println("Searching For: " + trainIDFromCTC + " Found: " + thisTrainID);
		    if (thisTrainID == trainIDFromCTC)
		    	return (Integer)train.get("currentBlock");
		}
		//System.exit(-1);
		return -10000000;
	}

	//TODO get a trains authority
	public int getTrainAuthority(int id) {
		return trainController.getTrainAuthority(id);
	}

	public double getTrainSpeed(int id) {
		// TODO get trains speed
		return trainController.getTrainSpeed(id);
	}

	public int getTrainPassengerCount(int id) {
		// TODO get number of passengers on train
		return trainController.getTrainPassengers(id);
	}

	public String getTrainRoute(int id) {
		return trainController.getTrainRoute(id);
	}
	
	public String getFailure(String line, int blockNumber)
	{
		return trackModel.getFailure(line, blockNumber);
	}
	
	public void requestSpeedChange(int ID, double speed){
		trainController.requestSpeedChange(speed, ID);
	}
	
	public void requestAuthorityChange(int ID, int authority){
		
		int currentBlock, prevBlock, newAuth;
		String line;
		
		line = trainController.getLine(ID);
		currentBlock = trainController.getCurrentBlock(ID);
		prevBlock = trainController.getPreviousBlock(ID);
		//newAuth = controllers.get(line).updateAuthority(authority, currentBlock, prevBlock);
		trainController.setTrainAuthority(ID, authority);
	}
	
	public void setStatus(Status newStatus){
		myStatus = newStatus;
	}
	
	public Status getStatus(){
		return myStatus;
	}
	
	public JPanel getTrainPanel(){
		return trainController.getTrainPanel();
	}
	
	public JPanel getTrackPanel(){
		return trackModel.getTrackModelPanel();
	}
	
	public double getBlockGrade(String line, int blockID){
		return (Double)trackModel.blockGrade(line, blockID);
	}

	public double getBlockElevation(String line, int blockID){
		return 0;// (Double)trackModel(line, blockID);
	}

	public double getBlockCumElevation(String line, int blockID){
		return 0;//(Double)trackModel.get(line, blockID, TrackModel.CUMULATIVE_ELEVATION);
	}
	public String getBlockInfrastructure(String line, int blockID){
		return (String)trackModel.infrastructure(line, blockID);
	}

	public boolean getBlockFull(String line, int blockID){
		return (Boolean)trackModel.containsTrain(line, blockID);
	}

	public Double getBlockLength(String line, int blockID){
		return (Double)trackModel.blockLength(line, blockID);
	}

	public String getBlockSection(String line, int blockID){
		return "A";// (String)trackModel.get(line, blockID);
	}

	public Integer getBlockSpeedLimit(String line, int blockID){
		return (Integer)trackModel.speedLimit(line, blockID);
	}
	public void simulationMode(int multiplier)
	{
		trainController.simulationMode(multiplier);
	}
	
	public static void main(String[] args)
	{
		File a = new File("C:/Users/Matt/Desktop/gLine.csv");
		File b = new File("C:/Users/Matt/Desktop/rLine.csv");
		
		File[] files = new File[2];
		files[0] = a;
		files[1] = b;
		
		Synchronize s = new Synchronize();
		s.loadTrack(files);
		
		
		System.out.println(s.trackModel.next("Green", 77, 78));
		System.out.println(s.trackModel.switchPosition("Green", 77));
		System.out.println(s.trackModel.toggleSwitch("Green", 77));
		System.out.println(s.trackModel.next("Green", 77, 78));
		//System.out.println(s.trackModel.toggleSwitch("Green", 77));
		//System.out.println(s.trackModel.next("Green", 77, 78));
	}
}


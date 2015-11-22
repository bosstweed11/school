import java.io.File;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.Set;

import javax.swing.JFrame;




public class CTCOfficeControl {
	private Set<Integer> trainIDs = null;
	private Synchronize s = null;
	private int trainIdCounter = 0;
	public CTCOfficeControl(){
		trainIDs = new LinkedHashSet<Integer>();
		s = new Synchronize();
	}
	
	public int getNewTrainID(){
		return trainIdCounter++;
	}
	
	public void initTrack(File[] track){
		s.loadTrack(track);
	}
	
	//TODO fix error when passing to TrainPanel
	public synchronized boolean dispatchTrain(int ID, int authority, String line){

		if((s.addTrain(ID, authority, line))){
			if(trainIDs.add(ID)){
				return true;
			}
		}

		return false;
	}
	
	public double getSpeed(int ID){
		if(trainIDs.contains(ID)){
			return s.getTrainSpeed(ID);
		}
		return -1;
	}
	
	public JFrame getTrainControllerPanel(){
		JFrame frame = new JFrame();
		frame.add(s.getTrainPanel());
		return frame;
	}
	
	public JFrame getTrackControllerPanel(){
		return null;//s.getTrackControllerPanel();
	}
	
	public JFrame getTrainModelPanel(){
		return null; //s.getTrainModelPanel();
	}
	
	public JFrame getTrackModelPanel(){
		JFrame frame = new JFrame();
		frame.add(s.getTrackPanel());
		return frame;
	}
	
	public int getAuthority(int ID){
		if(trainIDs.contains(ID)){
			return s.getTrainAuthority(ID);
		}
		return -1;
	}
	
	public int getPassengerCount(int ID){
		if(trainIDs.contains(ID)){
			return s.getTrainPassengerCount(ID);
		}
		return -1;
	}
	
	//TODO
	public void checkForRemovedTrains(){
		ArrayList<Integer> ids = null; // s.getRemovedTrains()
		for(Integer id : ids){
			if(trainIDs.contains(id))
				ids.remove(id);
		}
	}
	
	public Set<Integer> getTrainIDs(){
		return trainIDs;
	}

	public String getRoute(int id) {

		return s.getTrainRoute(id);
	}
	
	public synchronized void removeTrain(int id){
		if(trainIDs.contains(id)){
			trainIDs.remove(id);
		}
	}
	
	public synchronized double getBlockGrade(String line, int blockID){
		return s.getBlockGrade(line,  blockID);
	}
	
	public synchronized double getBlockElevation(String line, int blockID){
		return s.getBlockElevation(line, blockID);
	}
	
	public synchronized double getBlockCumElevation(String line, int blockID){
		return s.getBlockCumElevation(line, blockID);
	}
	public synchronized String getBlockInfrastructure(String line, int blockID){
		return s.getBlockInfrastructure(line, blockID);
	}
	
	public synchronized boolean getBlockFull(String line, int blockID){
		return s.containsTrain(line,  blockID);
	}
	
	public synchronized Double getBlockLength(String line, int blockID){
		return s.getBlockLength(line,  blockID);
	}
	
	public String getBlockSection(String line, int blockID){
		return s.getBlockSection(line, blockID);
	}
	
	public Integer getBlockSpeedLimit(String line, int blockID){
		return s.getBlockSpeedLimit(line, blockID);
	}
	
	public void sendSpeedRequest(int ID, int speed){
		s.requestSpeedChange(ID, speed);
	}
	
	public void sendAuthorityRequest(int ID, int speed){
		s.requestAuthorityChange(ID,  speed);
	}

	public void update() {
		// TODO Auto-generated method stub
		s.update();
		ArrayList<Integer> removedTrains = s.getRemovedTrains();
		for(Integer i : removedTrains){
			trainIDs.remove(i);
		}
	}

	public int getBlock(int ID) {
		// TODO Auto-generated method stub
		return s.getBlock(ID);
	}
	
	public synchronized ArrayList<Integer> getRemovedTrains(){
		return s.getRemovedTrains();
	}

	public void simulationMode(int i) {
		s.simulationMode(i);
		
	}
}

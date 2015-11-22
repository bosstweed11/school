//package trainControl.TrackController;

import java.util.ArrayList;
import java.util.HashMap;

import javax.swing.JComboBox;

//import trainControl.TrackModel.TrackModel;
//import trainControl.TrainController.TrainPanel;

public class TrackController {
	
	private String line;
	private TrackModel trackModel;
	
	private int firstBlockID;
	private boolean oneTrain;
	
	private HashMap<Integer, HashMap<String, Object>> trainsOnLine;
	
	public TrackController(String line, TrackModel model) {
		this.line = line;
		this.trackModel = model;
		this.trainsOnLine = new HashMap<Integer, HashMap<String, Object>>();
		
		if (line.equalsIgnoreCase("Red")) {
			this.firstBlockID = 9;
		}
		else {
			this.firstBlockID = 63;
		}
	}

	public int updateAuthority(int authority, int currentBlock, int prevBlock) {
		
		int newAuthority = authority;
		
		for(int i = 1; i <= authority; i++) {
			
			int nextBlock;
			//S/stem.out.println("Track Controller - current: "+ currentBlock);
			//System.out.println("Track Controller - prev: " + prevBlock);
			
			
			if(currentBlock == 0)
			{
				return authority;
			}
			nextBlock = trackModel.next(line, currentBlock, prevBlock);
			
			
			if (nextBlock < 0)
				nextBlock = -nextBlock;
			
//			if (i == 0)
//				nextBlock = currentBlock;
			
			if (checkBlockForTrain(nextBlock)) {
				newAuthority = i;
				//System.out.println(newAuthority);
				//System.exit(0);
				return (newAuthority-1);
				
			}
			if (checkBlockForStation(nextBlock)) {
				newAuthority = i;
				return newAuthority;
			}
			prevBlock = currentBlock;
			currentBlock = nextBlock;
		}
		return authority;
		
	}
	
	

	private boolean checkBlockForTrain(int block) {
		if (block != 0)
			return trackModel.containsTrain(line, block);
		else
			return false;
	}

	private boolean checkBlockForStation(int block) {
		return trackModel.infrastructure(line, block).contains("STATION");
		
	}
	
	public void updateCrossing(int BlockID, boolean isActive) {
		trackModel.crossingStatus(line, BlockID, isActive);
	}
	
	private void toggleSwitch(int BlockID) {
		trackModel.toggleSwitch(line, BlockID);
	}
			
	public boolean checkFirstBlock() {
		return trackModel.containsTrain(line, firstBlockID);
	}
	
	public int getFirstBlockID() {
		return this.firstBlockID;
	}

	public boolean checkFirstSwitch() {
		if (trackModel.switchPosition(line, firstBlockID) == 0) {
			return false;
		}
		return true;
	}

	public boolean toggleFirstSwitch() {
		trackModel.toggleSwitch(line,  firstBlockID);
		return true;
		//TODO: Implement a check to make sure switching will not cause an issue with any train on the track..
	}

	public void updateSwitch(HashMap<String, Object> train, int oldBlock) {
		int nextBlock;
		
		nextBlock = trackModel.next(line, (Integer)train.get("currentBlock"), oldBlock);
		
		if (nextBlock < 0) {
			trackModel.toggleSwitch(line, (nextBlock * -1));
		}
		
		if (line.equals("Green"))
			updateGreenSwitches(train, oldBlock);
		else if (line.equals("Red"))
			updateRedSwitches(train, oldBlock);
		
	}

	private void updateRedSwitches(HashMap<String, Object> train, int previousBlock) {
		
		int currentBlock;
		int switchBlock;
		
		currentBlock = (Integer)train.get("currentBlock");
		switchBlock = trackModel.next(line, currentBlock, previousBlock);
		
		if (switchBlock == 0)
			return;
		
		if ((trackModel.infrastructure(line, switchBlock).contains("SWITCH")))
			return; //No switch ahead, leave
		
		if ( (currentBlock == 8) && (previousBlock == 7) ) {
			
			if (trackModel.switchPosition(line, switchBlock) != 0) {
				if (!oneTrain)
					trackModel.toggleSwitch(line, switchBlock);
			}
		}
		
		
		//Top cut-away red line
		if ( (currentBlock == 26) && (previousBlock == 25) ) {
			if (trackModel.switchPosition(line, switchBlock) == 28){
				if (isSegmentClosed(28, 32))
					trackModel.toggleSwitch(line, switchBlock);
			}
			if (trackModel.switchPosition(line, switchBlock) == 76) {
				if (isSegmentClosed(72, 76))
					trackModel.toggleSwitch(line, switchBlock);
			}
		}
		
		if ( (currentBlock == 33) && (previousBlock == 34) ) {
			if (trackModel.switchPosition(line, switchBlock) == 31){
				if (isSegmentClosed(31, 27))
					trackModel.toggleSwitch(line, switchBlock);
			}
			if (trackModel.switchPosition(line, switchBlock) == 72) {
				if (isSegmentClosed(72, 76))
					trackModel.toggleSwitch(line, switchBlock);
			}
		}
		//End top cut-away red line section
		
		//Lower cut-away red line
		if ( (currentBlock == 37) && (previousBlock == 36) ) {
			if (trackModel.switchPosition(line, switchBlock) == 39){
				if (isSegmentClosed(39, 43))
					trackModel.toggleSwitch(line, switchBlock);
			}
			if (trackModel.switchPosition(line, switchBlock) == 71) {
				if (isSegmentClosed(67, 71))
					trackModel.toggleSwitch(line, switchBlock);
			}
		}
		if ( (currentBlock == 44) && (previousBlock == 45) ) {
			if (trackModel.switchPosition(line, switchBlock) == 42){
				if (isSegmentClosed(38, 42))
					trackModel.toggleSwitch(line, switchBlock);
			}
			if (trackModel.switchPosition(line, switchBlock) == 67) {
				if (isSegmentClosed(67, 71))
					trackModel.toggleSwitch(line, switchBlock);
			}
		}
		//End Lower cut-away red line section
		
	}

	private void updateGreenSwitches(HashMap<String, Object> train, int previousBlock) {
		
		int currentBlock;
		int switchBlock;
		//int switchBlock;
		
		currentBlock = (Integer)train.get("currentBlock");
		switchBlock = trackModel.next(line, currentBlock, previousBlock);
		//switchBlock = trackModel.next(line, nextBlock, currentBlock);
		
		if (switchBlock == 0)
			return;
		
		if (!(trackModel.infrastructure(line, switchBlock).contains("SWITCH"))){			
			return; //No switch ahead, leave
		}
		
		if ( (currentBlock == 78) && (previousBlock == 79) ) {
			//System.out.println("Train on block 78");
			if (trackModel.switchPosition(line, switchBlock) != 101){
				trackModel.toggleSwitch(line, switchBlock);
				//System.out.println("SwitchWasToggled");
			}
		}
		
		if ( (currentBlock == 14) && (previousBlock == 15) ) {
			if (trackModel.switchPosition(line, switchBlock) != 12)
				trackModel.toggleSwitch(line, switchBlock);
		}
		if ( (currentBlock == 57) && (previousBlock == 56) ) {
			if (trackModel.switchPosition(line, switchBlock) != 0)
				trackModel.toggleSwitch(line, switchBlock);
			if (trackModel.switchPosition(line, switchBlock) == 0)
			{
				if (oneTrain)
					trackModel.toggleSwitch(line, switchBlock);
			}
		}
		if ( (currentBlock == 27) && (previousBlock == 26) ) {
			if (trackModel.switchPosition(line, switchBlock) != 29)
				trackModel.toggleSwitch(line, switchBlock);
		}
		if ( (currentBlock == 84) && (previousBlock == 83) ) {
			if (trackModel.switchPosition(line, switchBlock) != 86)
				trackModel.toggleSwitch(line, switchBlock);
		}
		
		
	}

	private boolean isSegmentClosed(int start, int last) {
		
		for (int i = start; i <= last; i++) {
			if (trackModel.isClosed(line, i) || trackModel.getFailure(line, i).contains("Rail"))
				return true;
		}
		return false;
		
	}

	public boolean atStation(int currentBlock) {
		
		if (trackModel.infrastructure(line, currentBlock).contains("STATION"))
			return true;
		
		return false;
	}

	public boolean isUnderground(int currentBlock) {
				
		if (trackModel.infrastructure(line, currentBlock).contains("UNDERGROUND"))
			return true;
		
		return false;
	}

	public void addTrain(int trainID) {
		trainsOnLine.put(trainID, new HashMap<String, Object>());
		
	}

	public void removeTrain(int trainID) {
		trainsOnLine.remove(trainID);
		
	}

	public void checkSpecialBlocks() {
		
		
	}
	
	public void setOneTrain(boolean status)
	{
		oneTrain = status;
	}

	public boolean activateCrossings(String line, int currentBlock, int prevBlock) {
		
		int crossingBlock;
		int block;
		
		if (currentBlock==0)
			return false;
		
		crossingBlock = trackModel.next(line, currentBlock, prevBlock);
		
		if ((crossingBlock < 0) || (crossingBlock == 0)) 
			return false;
		
		if (trackModel.infrastructure(line, crossingBlock).contains("CROSSING")) {
			trackModel.crossingStatus(line, crossingBlock, true);
			return true;
		}
		else if (trackModel.infrastructure(line, currentBlock).contains("CROSSING")) {
			trackModel.crossingStatus(line, currentBlock, true);
			return true;
		}
		else{
			if (line.equals("Red"))
				block = 47;
			else 
				block = 19;
			
			trackModel.crossingStatus(line, block, false);
			return false;
		}
	}

	

}

//package latestTrackModelRELEASE;
/**************************************************
	Author:		Zach Olawski
	Course:		CoE_1186
	Detail:		Final Project
					Track Model Module
					User Interface Layout
**************************************************/

import java.util.ArrayList;
import java.io.File;
import java.io.BufferedReader;
import java.io.FileReader;
import java.util.StringTokenizer;
import java.util.HashMap;
import javax.swing.JPanel;

/**************************************************
*	This is my main class that handles reading in
*		the file and setting up the data structure
*	Values unobtainable from the file are also
*		hardcoded here
**************************************************/
public class TrackModel{
	
	private
		HashMap<String, ArrayList<HashMap<String, Object>>>	dataStruct;		//	main data structure
		ArrayList<String> lineList;											//	arraylist of all loaded lines
		TrackModelGUI myTrackModel;
	
	/**************************************************
	*	Constructor
	*		Creates empty data structure and arraylist
	**************************************************/
	public TrackModel(){
		dataStruct	= new HashMap<String, ArrayList<HashMap<String, Object>>>();
		lineList = new ArrayList<String>();
		myTrackModel = new TrackModelGUI(this);
	}
	
	/**************************************************
	*	Constructor
	*		Accepts an initial file to load (loadTrack)
	*		Creates data structure and arraylist
	**************************************************/
	public TrackModel(File inFile){
		dataStruct	= new HashMap<String, ArrayList<HashMap<String, Object>>>();
		lineList = new ArrayList<String>();
		loadTrack(inFile);
		myTrackModel = new TrackModelGUI(this);
	}
	
	/**************************************************
	*	Method
	*		Takes an initial file to load
	*		Populates 'dataStruct'
	*	Structure of 'dataStruct'
	*		[Track Line (ex: "Green", "Red")] => HashMap key
	*		[Block Number (ex: 1, 2)] => ArrayList index
	*		[Info Type (ex: nextBlock)] => HashMap key
	*	Return
	*		'dataStruct' returns different types
	*			all casted as an Object
	**************************************************/
	public String loadTrack(File inFile){
		try{
			int instructionCount = 0;
			BufferedReader reader  = new BufferedReader(new FileReader(inFile));
			ArrayList<HashMap<String, Object>> blocks = new ArrayList<HashMap<String, Object>>();
			String line = reader.readLine();	//Gets Rid of First Row with Column Descriptors
			String trackLine = "";
			HashMap<String, Object> blockStats = null;
			int failureArray[] = {0,0,0}; //Represents: Broken Rail, Track Circuit Failure, Power Failure
			while((line = reader.readLine()) != null){
				blockStats = new HashMap<String, Object>();
				StringTokenizer token = new StringTokenizer(line,",");
				if(token.countTokens() == 8){
					trackLine = token.nextToken();
					blockStats.put("section", token.nextToken());
					blockStats.put("currentBlock", token.nextToken());
					blockStats.put("blockLength", token.nextToken());
					blockStats.put("blockGrade", token.nextToken());
					blockStats.put("speedLimit", token.nextToken());
					blockStats.put("blockElevation", token.nextToken());
					blockStats.put("cumulativeElevation", token.nextToken());
				}
				else if(token.countTokens() == 9){
					trackLine = token.nextToken();
					blockStats.put("section", token.nextToken());
					blockStats.put("currentBlock", token.nextToken());
					blockStats.put("blockLength", token.nextToken());
					blockStats.put("blockGrade", token.nextToken());
					blockStats.put("speedLimit", token.nextToken());
					String infrastructure = token.nextToken();
					if(infrastructure.compareTo("RAILWAY CROSSING") == 0){
						blockStats.put("crossingStatus", false);
					}
					blockStats.put("infrastructure", infrastructure);
					blockStats.put("blockElevation", token.nextToken());
					blockStats.put("cumulativeElevation", token.nextToken());
				}
				else{
					//End of File Reached? Used "continue" Just In Case...
					continue;
				}
				blockStats.put("failureType", failureArray);
				blockStats.put("containsTrain", false);
				blockStats.put("isOpen", true);
				blockStats.put("heaterOn", false);
				blockStats.put("nextBlock", Integer.toString(Integer.parseInt((String)blockStats.get("currentBlock")) + 1));
				if(instructionCount == 0){
					blockStats.put("previousBlock", "-1");
					blockStats.put("startBlock", true);
				}
				else{
					blockStats.put("previousBlock", blocks.get(instructionCount - 1).get("currentBlock"));
				}
				
				blocks.add(blockStats);
				++instructionCount;
			}
			lineList.add(trackLine);
			if(blockStats != null){
				blockStats.put("nextBlock", "-1");
				blockStats.put("endBlock", true);
				blockStats.put("blockCount", instructionCount);
				blocks.set(blocks.size() - 1, blockStats);
			}
			dataStruct.put(trackLine, blocks);
			
			//This Section Manually Adds Information Used for Switching
			if(dataStruct.containsKey("Green") && !dataStruct.get("Green").get(13 - 1).containsKey("switchPosition")){
				int options[];
				options = new int[]{13,12,1};
				dataStruct.get("Green").get(13 - 1).put("switchOptions", options);
				dataStruct.get("Green").get(13 - 1).put("switchPosition", 12);
				options = new int[]{28,29,150};
				dataStruct.get("Green").get(28 - 1).put("switchOptions", options);
				dataStruct.get("Green").get(28 - 1).put("switchPosition", 29);
				options = new int[]{58,0,59};
				dataStruct.get("Green").get(58 - 1).put("switchOptions", options);
				dataStruct.get("Green").get(58 - 1).put("switchPosition", 0);
				options = new int[]{63,0,62};
				dataStruct.get("Green").get(63 - 1).put("switchOptions", options);
				dataStruct.get("Green").get(63 - 1).put("switchPosition", 0);
				options = new int[]{77,76,101};
				dataStruct.get("Green").get(77 - 1).put("switchOptions", options);
				dataStruct.get("Green").get(77 - 1).put("switchPosition", 76);
				options = new int[]{85,86,100};
				dataStruct.get("Green").get(85 - 1).put("switchOptions", options);
				dataStruct.get("Green").get(85 - 1).put("switchPosition", 86);
				
				//This information may not need to be contained in this class
				dataStruct.get("Green").get(1 - 1).put("previousBlock", "13");
				dataStruct.get("Green").get(100 - 1).put("nextBlock", "85");
				dataStruct.get("Green").get(101 - 1).put("previousBlock", "77");
				//dataStruct.get("Green").get(77 - 1).put("nextBlock", "85");	//	Not sure yet how to handle this
				//dataStruct.get("Green").get(85 - 1).put("previousBlock", "77");	//	Not sure yet how to handle this
				dataStruct.get("Green").get(150 - 1).put("nextBlock", "28");
				//dataStruct.get("Green").get(28 - 1).put("previousBlock", "150");	//	Not sure yet how to handle this
			}
			else if(dataStruct.containsKey("Red") && !dataStruct.get("Red").get(9 - 1).containsKey("switchPosition")){
				//"switchPosition" is put initially to 10 to test switching earliest in the track
				int options[];
				options = new int[]{9,0,10};
				dataStruct.get("Red").get(9 - 1).put("switchOptions", options);
				dataStruct.get("Red").get(9 - 1).put("switchPosition", 10);
				options = new int[]{16,1,15};
				dataStruct.get("Red").get(16 - 1).put("switchOptions", options);
				dataStruct.get("Red").get(16 - 1).put("switchPosition", 1);
				options = new int[]{27,28,76};
				dataStruct.get("Red").get(27 - 1).put("switchOptions", options);
				dataStruct.get("Red").get(27 - 1).put("switchPosition", 28);
				options = new int[]{32,31,76};
				dataStruct.get("Red").get(32 - 1).put("switchOptions", options);
				dataStruct.get("Red").get(32 - 1).put("switchPosition", 31);
				options = new int[]{38,39,71};
				dataStruct.get("Red").get(38 - 1).put("switchOptions", options);
				dataStruct.get("Red").get(38 - 1).put("switchPosition", 39);
				options = new int[]{43,42,67};
				dataStruct.get("Red").get(43 - 1).put("switchOptions", options);
				dataStruct.get("Red").get(43 - 1).put("switchPosition", 42);
				options = new int[]{52,53,66};
				dataStruct.get("Red").get(52 - 1).put("switchOptions", options);
				dataStruct.get("Red").get(52 - 1).put("switchPosition", 53);
				
				//This information may not need to be contained in this class
				dataStruct.get("Red").get(1 - 1).put("previousBlock", "16");
				//dataStruct.get("Red").get(27 - 1).put("nextBlock", "76");	//	Next or Previous Block???
				dataStruct.get("Red").get(76 - 1).put("nextBlock", "27");
				//dataStruct.get("Red").get(32 - 1).put("nextBlock", "72");	//	Next or Previous Block???
				dataStruct.get("Red").get(72 - 1).put("previousBlock", "32");
				//dataStruct.get("Red").get(38 - 1).put("nextBlock", "71");	//	Next or Previous Block???
				dataStruct.get("Red").get(71 - 1).put("nextBlock", "38");
				//dataStruct.get("Red").get(43 - 1).put("nextBlock", "67");	//	Next or Previous Block???
				dataStruct.get("Red").get(67 - 1).put("previousBlock", "43");
				//dataStruct.get("Red").get(52 - 1).put("nextBlock", "66");	//	Next or Previous Block???
				dataStruct.get("Red").get(66 - 1).put("nextBlock", "52");
			}
			else{
				System.out.println("----------------------NOTICE-----------------------");
				System.out.println("Unknown Track Line! -> Switch Information Not Added");
				System.out.println("---------------------------------------------------");
			}
			return trackLine;
		}
		catch(Exception e){
			e.printStackTrace();
			System.out.println("Exception in loadTrack Method: " + e);
			return "ERROR";
		}
	}
	
	public int next(String line, int blockNumber, int oldBlock){
		System.out.println("current " + blockNumber + "; previous " + oldBlock);
		int nextBlock = nextBlock(line, blockNumber);
		int previousBlock = previousBlock(line, blockNumber);
		int switchPosition = switchPosition(line, blockNumber);
		int switchPositionInverse = switchPositionInverse(line, blockNumber);
		System.out.println("next " + nextBlock + " previous " + previousBlock + " switch " + switchPosition + " inverse " + switchPositionInverse);
		if(dataStruct.get(line).get(blockNumber - 1).containsKey("switchPosition")){
			if(oldBlock == switchPosition){
				if( (nextBlock == switchPosition)||(nextBlock == switchPositionInverse) ){
					return checkFailure(line, previousBlock);
				}
				else if( (previousBlock == switchPosition)||(previousBlock == switchPositionInverse) ){
					return checkFailure(line, nextBlock);
				}
			}
			if(switchPosition == 0){
				return switchPosition;
			}
			return checkFailure(line, switchPosition);
		}
		if(line.compareTo("Green") == 0){
			String section = (String)dataStruct.get(line).get(blockNumber - 1).get("section");
			if( (section.compareTo("A") == 0)||(section.compareTo("B") == 0)||(section.compareTo("C") == 0) ){
				return switchCheck(line, previousBlock, blockNumber);
			}
			//return switchCheck(line, nextBlock, blockNumber);
		}
		if(previousBlock == oldBlock){
			return switchCheck(line, nextBlock, blockNumber);
		}
		else if(nextBlock == oldBlock){
			return switchCheck(line, previousBlock, blockNumber);
		}
		System.out.println("!!Track Assembly Error!!");
		return -666;
	}
	//Helper for public int next(String line, int blockNumber, int previousBlock){
	private int switchCheck(String line, int blockNumber, int oldBlock){
		if(dataStruct.get(line).get(blockNumber - 1).containsKey("switchPosition")){
			if(oldBlock == switchPosition(line, blockNumber)){
				return checkFailure(line, blockNumber);
			}
			else if(oldBlock == switchPositionInverse(line, blockNumber)){
				return -blockNumber;
				//System.out.println("!!TRAIN DERAILED!!\n\nSay Goodbye to Your B+");
				//return -666;
			}
			return blockNumber;
		}
		return checkFailure(line, blockNumber);
	}
	//Helper for public int next(String line, int blockNumber, int previousBlock){
	private int nextBlock(String line, int blockNumber){
		return Integer.parseInt((String)dataStruct.get(line).get(blockNumber - 1).get("nextBlock"));
	}
	//Helper for public int next(String line, int blockNumber, int previousBlock){
	private int previousBlock(String line, int blockNumber){
		return Integer.parseInt((String)dataStruct.get(line).get(blockNumber - 1).get("previousBlock"));
	}
	//Helper for public int next(String line, int blockNumber, int previousBlock){
	//Checks for failures thrown and acts appropriately
	private int checkFailure(String line, int blockNumber){
		int failureStatus[] = (int[])dataStruct.get(line).get(blockNumber - 1).get("failureType");
		if(failureStatus[0] == 1){
			System.out.println("!!BROKEN RAIL FAILURE!!\n!!!!TRAIN DERAILED!!!!!\n\nSay Goodbye to Your B+");
			return -10;
		}
		else if(failureStatus[1] == 1){
			System.out.println("!!TRACK CIRCUIT FAILURE!!\n!!!IDK WHAT THAT MEANS!!!\n\nSay Goodbye to Your B+");
			return -20;
		}
		else if(failureStatus[1] == 1){
			System.out.println("!!!POWER FAILURE!!!\nMAYBE TELL SOMEONE?\n\nSay Goodbye to Your B+");
			return -30;
		}
		return blockNumber;
	}
	
	public int throwFailure(String line, int blockNumber, int type){
		int failureStatus[] = (int[])dataStruct.get(line).get(blockNumber - 1).get("failureType");
		if( (type >= 0)&&(type <= 2) ){
			if(failureStatus[type] == 0){
				failureStatus[type] = 1;
				dataStruct.get(line).get(blockNumber - 1).put("failureType", failureStatus);
				return 1;
			}
			failureStatus[type] = 0;
			dataStruct.get(line).get(blockNumber - 1).put("failureType", failureStatus);
			return 0;
		}
		return -1;
	}
	
	public int crossingStatus(String line, int blockNumber, boolean active){
		if(dataStruct.get(line).get(blockNumber - 1).containsKey("crossingStatus")){
			dataStruct.get(line).get(blockNumber - 1).put("crossingStatus", active);
			return 1;
		}
		return -1;
	}
	
	public int toggleSwitch(String line, int blockNumber){
		if(dataStruct.get(line).get(blockNumber - 1).containsKey("switchOptions")){
			if(switchPosition(line, blockNumber) == ((int[])dataStruct.get(line).get(blockNumber - 1).get("switchOptions"))[1]){
				dataStruct.get(line).get(blockNumber - 1).put("switchPosition", ((int[])dataStruct.get(line).get(blockNumber - 1).get("switchOptions"))[2]);
				return switchPosition(line, blockNumber);
			}
			else{
				dataStruct.get(line).get(blockNumber - 1).put("switchPosition", ((int[])dataStruct.get(line).get(blockNumber - 1).get("switchOptions"))[1]);
				return switchPosition(line, blockNumber);
			}
		}
		return -1;
	}
	
	public int switchPosition(String line, int blockNumber){
		if(dataStruct.get(line).get(blockNumber - 1).containsKey("switchPosition")){
			return (Integer)dataStruct.get(line).get(blockNumber - 1).get("switchPosition");
		}
		return -1;
	}
	
	public int switchPositionInverse(String line, int blockNumber){
		if(dataStruct.get(line).get(blockNumber - 1).containsKey("switchOptions")){
			if(switchPosition(line, blockNumber) == ((int[])dataStruct.get(line).get(blockNumber - 1).get("switchOptions"))[1]){
				return ((int[])dataStruct.get(line).get(blockNumber - 1).get("switchOptions"))[2];
			}
			return ((int[])dataStruct.get(line).get(blockNumber - 1).get("switchOptions"))[1];
		}
		return -1; // changed this to return something, always need to return outside of if statements
	}
	
	public double blockLength(String line, int blockNumber){
		return Double.parseDouble((String)dataStruct.get(line).get(blockNumber - 1).get("blockLength"));
	}
	
	public double blockGrade(String line, int blockNumber){
		return Double.parseDouble((String)dataStruct.get(line).get(blockNumber - 1).get("blockGrade"));
	}
	
	public double blockElevation(String line, int blockNumber){
		return Double.parseDouble((String)dataStruct.get(line).get(blockNumber - 1).get("blockElevation"));
	}
	
	public int speedLimit(String line, int blockNumber){
		return Integer.parseInt((String)dataStruct.get(line).get(blockNumber - 1).get("speedLimit"));
	}
	
	public String infrastructure(String line, int blockNumber){
		if(blockNumber < 1){
			return "Yard";
		}
		if(dataStruct.get(line).get(blockNumber - 1).containsKey("infrastructure")){
			return (String)dataStruct.get(line).get(blockNumber - 1).get("infrastructure");
		}
		return "None";
	}
	
	public String section(String line, int blockNumber){
		return (String)dataStruct.get(line).get(blockNumber - 1).get("section");
	}
	
	public boolean containsTrain(String line, int blockNumber){
		return (Boolean)dataStruct.get(line).get(blockNumber - 1).get("containsTrain");
	}
	
	public void occupyBlock(String line, int blockNumber){
		dataStruct.get(line).get(blockNumber - 1).put("containsTrain", true);
	}
	
	public int occupyBlock(String line, int oldBlock, int newBlock){
		if(newBlock < 0){
			dataStruct.get(line).get(oldBlock - 1).put("containsTrain", false);
			return 0;
		}
		if((Boolean)dataStruct.get(line).get(oldBlock - 1).get("containsTrain")){
			dataStruct.get(line).get(oldBlock - 1).put("containsTrain", false);
			dataStruct.get(line).get(newBlock - 1).put("containsTrain", true);
			return 1;
		}
		return -1;
	}
	
	public ArrayList<String> getLines(){
		return lineList;
	}
	
	public int blockCount(String line){
		return (Integer)dataStruct.get(line).get(dataStruct.get(line).size() - 1).get("blockCount");
	}
	
	public void closeBlock(String line, int blockNumber){
		dataStruct.get(line).get(blockNumber - 1).put("isOpen", false);
	}
	
	public boolean isOpen(String line, int blockNumber){
		if((Boolean)dataStruct.get(line).get(blockNumber - 1).get("isOpen")){
			return true;
		}
		return false;
	}
	
	public boolean heaterOn(String line, int blockNumber){
		return (Boolean)dataStruct.get(line).get(blockNumber - 1).get("heaterOn");
	}
	
	public boolean toggleHeater(String line, int blockNumber){
		if((Boolean)dataStruct.get(line).get(blockNumber - 1).get("heaterOn")){
			dataStruct.get(line).get(blockNumber - 1).put("heaterOn", false);
			return false;
		}
		dataStruct.get(line).get(blockNumber - 1).put("heaterOn", true);
		return true;
	}
	
	public void setFailure(String line, int blockNumber, String type){
		int set[] = (int[])dataStruct.get(line).get(blockNumber - 1).get("failureType");
		if(type.compareTo("Broken Rail") == 0){
			set[0] = 1;
		}
		else if(type.compareTo("Track Circuit Failure") == 0){
			set[1] = 1;
		}
		else if(type.compareTo("Power Failure") == 0){
			set[2] = 1;
		}
		dataStruct.get(line).get(blockNumber - 1).put("failureType", set);
	}
	
	public void unsetFailure(String line, int blockNumber, String type){
		int set[] = (int[])dataStruct.get(line).get(blockNumber - 1).get("failureType");
		if(type.compareTo("Broken Rail") == 0){
			set[0] = 0;
		}
		else if(type.compareTo("Track Circuit Failure") == 0){
			set[1] = 0;
		}
		else if(type.compareTo("Power Failure") == 0){
			set[2] = 0;
		}
		dataStruct.get(line).get(blockNumber - 1).put("failureType", set);
	}
	
	public int[] getFailure(String line, int blockNumber){
		int ret[] = (int[])dataStruct.get(line).get(blockNumber - 1).get("failureType");
		return ret;
	}
	
	public JPanel getTrackModelPanel(){
		return myTrackModel.getTrackModelPanel();
	}
	///*	NOTE: This will not be on the final release. It is used for finding bugs w/in my functions/program
	public static void main(String[] args){
		try{
			TrackModel myTrack = new TrackModel();
			//C:\\Users\\trottj\\Dropbox\\CoE_1866\\Code\\TrackModel\\latestTrackModelRELEASE
			
			String greenLine = myTrack.loadTrack(new File("gLine.csv"));
			String redLine = myTrack.loadTrack(new File("rLine.csv"));
			/*
			System.out.println("---------------SCENARIO 1---------------");
			System.out.println("Train on Red Line Right Out of Yard...");
			Thread t = new Thread();
			int pre = 0;
			int cur = 9;
			int tmp;
			if(myTrack.switchPosition(redLine, cur) != 0){
				System.out.println("Waiting for Switch to Point to Yard");
				System.out.println("Block " + cur + " Switch Set to " + myTrack.toggleSwitch(redLine, cur));
			}
			myTrack.occupyBlock(redLine, cur);
			for(int cnt = 0; cnt < 200; ++cnt){
				System.out.println("Train Traveling on Block " + cur);
				tmp = pre;
				pre = cur;
				int tst = myTrack.next(redLine, cur, tmp);
				if(tst == 0){
					System.out.println("Train Pulled Into Yard Successfully");
					System.exit(0);
				}
				int err = 0;
				while((tst < 0)){
					tst = -tst;
					int retry = myTrack.toggleSwitch(redLine, tst);
					if(retry < 0){
						System.out.println("IDK What's Going On");
						System.exit(-1);
					}
					tst = myTrack.next(redLine, cur, tmp);
					t.sleep(300);
				}
				cur = tst;
				myTrack.occupyBlock(redLine, pre, cur);
				
			}
			*/
		}
		catch(Exception e){
			e.printStackTrace();
			System.out.println("Exception: " + e);
			System.exit(0);
		}
	}
	//*/
}
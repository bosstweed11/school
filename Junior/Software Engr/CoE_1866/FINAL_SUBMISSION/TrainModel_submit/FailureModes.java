//
//
//  Generated by StarUML(tm) Java Add-In
//
//  @ Project : Untitled
//  @ File Name : Failure Modes.java
//  @ Date : 11/9/2012
//  @ Author : 
//
//




public class FailureModes {
	public boolean brakeFailure;
	public boolean engineFailure;
	public boolean signalFailure;

	public void simBrakeFail() {
		brakeFailure = true;
		notifyTrainCon(brakeFailure, engineFailure, signalFailure);
	}
	
	public void simEngineFail() {
		engineFailure = true;
		notifyTrainCon(brakeFailure, engineFailure, signalFailure);	
	}
	
	public void simSignalFail() {
		signalFailure = true;
		notifyTrainCon(brakeFailure, engineFailure, signalFailure);
	}
	
	public void notifyTrainCon(boolean brakeF, boolean engF, boolean sigF) {
	
	}
}
//
//
//  Generated by StarUML(tm) Java Add-In
//
//  @ Project : Untitled
//  @ File Name : Movement.java
//  @ Date : 11/9/2012
//  @ Author : 
//
//
import java.lang.Math;



public class Movement {
	public double setSpeed;
	public double acceleration;
	public double gaccerleration;
	public double velocity;
	public double distance;
	public double power;
	public double accelerationLimit;
	public double deccelerationLimit;
	public double eBrakeDLimit;
	public double kilozPerHour;
	public boolean eBrake;
	public double weight;
	private long lastTimeV = -1;
	private long lastTimeD = -1;
	public trackCom tCom;
	
	
	public Movement(TrackModel model, String line, int block){
		tCom = new trackCom(model, line, block);
		accelerationLimit = 0.5;
		deccelerationLimit = -1.2;//m/s^2
		eBrakeDLimit = -2.73;
		velocity = 0.0000001;
		distance = 0;
		power = 0;
		
	}
	public void updateSpeed() {
		long curTime = System.currentTimeMillis();
		if(lastTimeV == -1){
			lastTimeV = curTime;
		}
		
		long time = curTime-lastTimeV;
		lastTimeV = curTime;
		
		//gravity
		
		
		if(velocity >= 0){
			if(setSpeed == 0 && velocity ==0){
			
			}else{
				velocity = ((acceleration) * time/1000) + velocity; 
			    kilozPerHour = (velocity*3.6);
			}
			
		}else{
			velocity = 0;
			kilozPerHour = (velocity*3.6);
		}
		
		if(velocity<0){
			velocity =0;
			kilozPerHour = (velocity*3.6);
		}
		System.out.println("\nvelocity after: " + velocity);
		updateDistance();
	}
	
	public void updateDistance() {
		long curTime = System.currentTimeMillis();
		if(lastTimeD == -1){
			lastTimeD = curTime;
		}
		long upTime = curTime - lastTimeD;
		lastTimeD = curTime;
		distance = (velocity * (upTime/1000)) + (0.5 * acceleration * (upTime/1000) * (upTime/1000));
		if(distance<0){
			distance =0;
		}
		System.out.println("\ndistance: " + distance);
		tCom.checkBlock(distance);
	}
	
	public void updatePower(double newPow) {
		power = newPow;
		double grav = ((9.8)*(Math.cos(Math.atan(tCom.blockLength/tCom.grade))));
		if(velocity<=0 && setSpeed !=0){
			velocity =.001;
		}
		acceleration = (newPow/(velocity*weight));
		if(acceleration==Double.POSITIVE_INFINITY || acceleration==Double.NEGATIVE_INFINITY){
			acceleration =0;
		}
		System.out.println("GRADE : " + tCom.grade + "\nACCEL :" + acceleration + "\nGRAV" + grav);
		
		acceleration -= grav;
		
		if(acceleration > accelerationLimit){
		
				acceleration = accelerationLimit;
		
		}else if(acceleration < deccelerationLimit){
			acceleration = deccelerationLimit;
		}
		
	}
	
	public void eBrake(){
		deccelerationLimit = eBrakeDLimit;
		
	}
	
	public void fixBrake(){
		deccelerationLimit = -1.2;
	}
}

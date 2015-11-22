



class PIDController:
    def __init__( self, _pc, _ic, _dc, _minOutput, _maxOutput ):
        self.pc = _pc
        self.ic = _ic
        self.dc = _dc

        self.minOutput = _minOutput
        self.maxOutput = _maxOutput
        
        self.bias = 0.001
        
        self.error = 0.0
        self.previousError = 0.0
        self.integral = 0.0
        
        self.target = 0.0
        self.current = 0.0
        self.lastInterval = 0.0
        self.derivative = 0.0
        self.output = 0.0
        self.outputCalculated = 0.0
        
        self.currentTime = 0.0
 
    def __str__( self ):
        return "Controller Info => pc=" + str(self.pc) + " ic=" + str(self.ic) + " dc=" + str(self.dc) \
                                        + " error=" + str(self.error) + " prevError=" + str(self.previousError) \
                                        + " integral=" + str(self.integral) + " target=" + str(self.target) \
                                        + " current=" + str(self.current) + " lastInterval=" + str(self.lastInterval) \
                                        + " derivative=" + str(self.derivative) + " output=" + str(self.output) \
                                        + " minOutput=" + str(self.minOutput) + " maxOutput=" + str(self.maxOutput) \
                                        + " outputCalculated=" + str(self.outputCalculated) + " currentTime=" + str(self.currentTime)
                                        
    def updateError( self, currentError, currentIntegral, printState ):
        self.error = currentError
        self.integral = currentIntegral
        
        # Print out the object prior to updating the previous_error member, otherwise previousError == error
        if ( printState ):
            print self
            
        self.previousError = currentError
        
    def getCSVHeader( self ):
        return "PC,IC,DC,error,previousError,integral,target,current,lastInterval,derivative,output,minOutput,maxOutput,outputCalculated,currentTime"

    def getCSV( self ):
        return str(self.pc) + "," + str(self.ic) + "," + str(self.dc) \
                            + "," + str(self.error) + "," + str(self.previousError) \
                            + "," + str(self.integral) + "," + str(self.target) \
                            + "," + str(self.current) + "," + str(self.lastInterval) \
                            + "," + str(self.derivative) + "," + str(self.output) \
                            + "," + str(self.minOutput) + "," + str(self.maxOutput) \
                            + "," + str(self.outputCalculated) + "," + str(self.currentTime) \
                        
    def setTarget( self, target ):
        self.target = target
    
    def getTarget( self ):
        return self.target
                        
    def getNewSetting( self, current, deltaT, printState ):
        #print "getNewSetting => current=" + str(current) + " deltaT=" + str(deltaT) + " printState=" + str(printState)
        self.currentTime = self.currentTime + deltaT
        self.current = current
        self.error = self.target - current
        self.integral = self.integral + ( self.error * deltaT )
        # deltaT must be greater than 0.0!
        self.derivative = ( self.error - self.previousError ) / deltaT
        
        self.outputCalculated = self.bias + ( self.pc * self.error ) + ( self.ic * self.integral ) + ( self.dc * self.derivative )
        self.output = self.outputCalculated
        # Clamp the output
        self.output = max( min( self.output, self.maxOutput ), self.minOutput )
    
        # Print out the object prior to updating the previousError member, otherwise previousError == error
        if ( printState ):
            print self.getCSV()
            #print self
            
        self.previousError = self.error

        return self.output
        
def haveReachedTarget( target, current, tolerance ):
    # Handle the case where the refresh rate is not quick enough
    return abs( target - current ) < tolerance


def callbackSimulator( myPID, tolerance, refreshRate ):
    print "CallbackSimulator => tolerance=" + str(tolerance) + " refreshRate=" + str(refreshRate)
    print myPID.getCSVHeader()
    
    currentPosition = 0.0

    print myPID.getCSV()
    currentVelocity = myPID.getNewSetting( currentPosition, refreshRate, False )
    while ( not haveReachedTarget( currentPosition, myPID.getTarget(), tolerance ) ):
        print myPID.getCSV()
        currentPosition = currentPosition + ( currentVelocity * refreshRate )
        currentVelocity = myPID.getNewSetting( currentPosition, refreshRate, False )
        
    print myPID.getCSV()
    
    # Stop the bot at this point
        
def callbackSimulatorSecondBot( myPID, refreshRate ):
    
    print "CallbackSimulatorSecondBot => refreshRate=" + str(refreshRate)

    print myPID.getCSVHeader()
    
    currentMasterBotX = 0.00
    currentMasterBotVelocity = 0.0
    currentTrackingBotX = 0.0
    currentTrackingBotVelocity = 0.0
    
    segmentVelocities = [ 0.25, 0.50, 0.25, 0.50, 0.25 ]    

    for ii in range ( 0, 5 ):  # 5 moves (changes in velocity)
        currentMasterBotVelocity = segmentVelocities[ii] # MasterBot velocity is constant
        for jj in range ( 0, 200 ):

            currentMasterBotX = currentMasterBotX + ( currentMasterBotVelocity * refreshRate )
            currentTrackingBotX = currentTrackingBotX + ( currentTrackingBotVelocity * refreshRate )
            
            currentTrackingBotVelocity = currentMasterBotVelocity + myPID.getNewSetting( currentTrackingBotX - currentMasterBotX, refreshRate, True )

def moveRobot():
    print "Move robot called"
    
    #myPID = PIDController( 0.35, 0.001, 0.1, 0.1, 0.6 )
    #print myPID
    #myPID.setTarget( 10.0 )
    #callbackSimulator( myPID, 0.03, 0.1 )

    myPID = PIDController( 5.0, 0.0, 0.0, -0.6, 0.6 )
    print myPID
    myPID.setTarget( 0.0 )
    callbackSimulatorSecondBot( myPID, 0.01 )

if __name__ == '__main__':
    try:    
        moveRobot()
    except:
        print "Failure calling moveRobot()"
        pass
        

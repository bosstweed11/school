#!/usr/bin/python
import rospy
import math
from datetime import datetime
 
from std_srvs.srv import * 
from kobuki_msgs.msg import BumperEvent
from cs1567p0.srv import *
from cs1567p4.srv import *
from cs1567p4.msg import *
from nav_msgs.msg import *
from geometry_msgs.msg import *

#####################################################################################################
# Constants associated with the bumper sensors
#####################################################################################################

LEFT_BUMPER   = 0
CENTER_BUMPER = 1
RIGHT_BUMPER  = 2

RELEASED = 0
PRESSED  = 1

#####################################################################################################
# The following define the types of moves that are supported
#####################################################################################################

MOVE_NONE                       = 0

MOVE_FORWARD_STOP_ON_CONTACT    = 10
MOVE_FORWARD_SPECIFIC_DISTANCE  = 11
MOVE_FORWARD_CONSTANT_VELOCITY  = 12

TURN_IN_PLACE                   = 20

#####################################################################################################
# Constant for converting x,y from the kinect sensor to x,y in meters
#####################################################################################################

KINECT_TO_METERS = 1.0

#####################################################################################################
# Global variables
#####################################################################################################

moveType = 0

bumperStates = None
lastOdometryMessage = None

moveCommand = None
sendCommandService = None

forwardMoveConfiguration = None
trackBotConfiguration = None

lastOdometryUpdate = None

endGoal = None
boxPosition = None

myBotID = 0
botToTrackID = 1

locationLastUpdateTime = None

#####################################################################################################
#####################################################################################################
class BumperSensors:
    def __init__(self):
        self.leftBumperIspressed = False
        self.rightBumperIspressed = False
        self.centerBumperIspressed = False
        
    def __str__( self ):
        return "Bumper Status => leftIspressed=" + str(self.leftBumperIspressed) \
                              + " rightIspressed=" + str(self.rightBumperIspressed) \
                              + " centerIspressed=" + str(self.centerBumperIspressed)

    def setBumperState( self, bumper, pressed ):
        if ( bumper == LEFT_BUMPER and pressed == 1 ):
            self.leftBumperIspressed = True
            print "Left Bumper pressed!"
        elif ( bumper == LEFT_BUMPER and pressed == 0 ):
            self.leftBumperIspressed = False
            print "Left Bumper released!"
        elif ( bumper == RIGHT_BUMPER and pressed == 1 ):
            self.rightBumperIspressed = True
            print "Right Bumper pressed!"
        elif ( bumper == RIGHT_BUMPER and pressed == 0 ):
            self.rightBumperIspressed = False
            print "Right Bumper released!"
        elif ( bumper == CENTER_BUMPER and pressed == 1 ):
            self.centerBumperIspressed = True
            print "Center Bumper pressed!"
        elif ( bumper == CENTER_BUMPER and pressed == 0 ):
            self.centerBumperIspressed = False
            print "Center Bumper released!"
        else:
            print "Invalid bumper / state!    => bumper" + str(bumper) + " pressed=" + str(pressed)

    def isBumperPressed( self, bumper ):
        if ( bumper == LEFT_BUMPER ):
            return self.leftBumperIspressed
        elif ( bumper == RIGHT_BUMPER ):
            return self.rightBumperIspressed
        elif ( bumper == CENTER_BUMPER ):
            return self.centerBumperIspressed
        else:
            print "Invalid bumper!    => bumper" + str(bumper)
            
#####################################################################################################
#####################################################################################################
class ForwardMoveConfiguration:
    def __init__( self, xTarget, yTarget, tolerance, pid ):
        self.xTarget = xTarget
        self.yTarget = yTarget
        self.tolerance = tolerance
        self.pid = pid
        
    def getXTarget( self ):
        return self.xTarget
    def getYTarget( self ):
        return self.yTarget
    def getTolerance( self ):
        return self.tolerance
    def getPID( self ):
        return self.pid

#####################################################################################################
#####################################################################################################
class TrackBotConfiguration:
    def __init__( self, botToTrackID, pid ):
        self.botToTrackID = botToTrackID
        self.pid = pid

#####################################################################################################
#####################################################################################################
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
            print self
            
        self.previousError = self.error

        return self.output
        
#####################################################################################################
## Determine if current is in range of the tolerance.
## TODO: Implement a check to see if the bot blows through the tolerance range
## TODO: Handle angles, specifically at 0/360 degrees
#####################################################################################################
def haveReachedLinearTarget( xTarget, yTarget, tolerance, xCurrent, yCurrent ):
    # Handle the case where the refresh rate is not quick enough
    return ( abs( xTarget - xCurrent ) < tolerance ) and ( abs( yTarget - yCurrent ) < tolerance )

#####################################################################################################
#####################################################################################################
def stopMoving( odometry, pid ):
    global moveType, moveCommand
    
    print "Move Stopped, Pos: x=" + str( odometry.pose.pose.position.x ) \
                          + " y=" + str( odometry.pose.pose.position.y ) \
                          + " z=" + str( odometry.pose.pose.orientation.z ) \
                          + " type=" + str( moveType )
    print bumperStates
    if ( pid != None ):
        print pid
    
    moveCommand.angular.z = 0.0
    moveCommand.linear.x = 0.0
    moveCommand.linear.y = 0.0
            
    moveType = MOVE_NONE

    return

#####################################################################################################
# TODO:  Figure out constant to get from kinect coordinates to meters
# KINECT_TO_METERS
# TODO:  Support moves in the y direction
#####################################################################################################
def getErrorBetweenBots( myPositionMessage, targetPositionMessage ):
    if ( ( myPositionMessage == None ) or ( targetPositionMessage == None ) ):
        return 0.0

    return KINECT_TO_METERS * ( myPositionMessage.x - targetPositionMessage.x )

#####################################################################################################
## TODO:  Support backing up, for now just set the approach speed low enough to not overshoot!
#####################################################################################################
def odometryCallback( odometry ):
    global moveType, moveCommand, lastOdometryMessage, bumperState, lastOdometryUpdate, forwardMoveConfiguration

    # The PID controller needs a deltaT
    # datetime should be in micro-seconds
    currentTime = datetime.now()
    if ( lastOdometryUpdate == None ):
        lastOdometryUpdate = currentTime
    
    deltaTInTime = currentTime - lastOdometryUpdate
    deltaTInSeconds = float(deltaTInTime.microseconds) / 1e6
 
    lastOdometryUpdate = currentTime
    
    # If we are not moving with odometry just return, but update the lastOdometryMessage anyway
    if ( ( moveType == MOVE_NONE ) or ( moveType == MOVE_FORWARD_CONSTANT_VELOCITY ) ):
        lastOdometryMessage = odometry
        return
    
    print "Current Odometry => x=" + str(odometry.pose.pose.position.x) \
                           + " y=" + str(odometry.pose.pose.position.y) \
                           + " z=" + str(odometry.pose.pose.orientation.z) \
    
    if ( moveType == MOVE_FORWARD_STOP_ON_CONTACT ):
        if ( bumperStates.isBumperPressed( CENTER_BUMPER ) ):
            stopMoving( odometry, None )
        print bumperStates

    elif ( moveType == MOVE_FORWARD_SPECIFIC_DISTANCE ):
        if ( haveReachedLinearTarget( forwardMoveConfiguration.getXTarget(), \
                                      forwardMoveConfiguration.getYTarget(), \
                                      forwardMoveConfiguration.getTolerance(), \
                                      odometry.pose.pose.position.x, \
                                      odometry.pose.pose.position.y ) ):
            stopMoving( odometry, forwardMoveConfiguration.getPID() )
        else:
            # TODO:  Consider angular moves, although this may work well enough for now
            moveCommand.linear.x = moveCommand.linear.x \
                                   + forwardMoveConfiguration.getPID().getNewSetting( odometry.pose.pose.position.x, \
                                                                                      deltaTInSeconds, True )

    #elif ( moveType == TURN_IN_PLACE ):
        # TODO: Implement
    else:
        print "Invalid move type!    => moveType =" + str(moveType)

    sendCommandService( moveCommand )

    lastOdometryMessage = odometry

    return 0

#####################################################################################################
## Bumper Callback - check to see if any of the bumpers have been hit yet
#####################################################################################################
def bumperEventCallback( data ):
    global bumperStates

    bumperStates.setBumperState( data.bumper, data.state )
    
    return 0

#####################################################################################################
##Check to stop- check to see if the box overlaps the target
#####################################################################################################
def checkToStop( targetPositionMessage, boxPositionMessage):
    if ( ( targetPositionMessage == None ) or ( boxPositionMessage == None ) ):
        return 0.0

    a = targetPositionMessage
    b = boxPositionMessage

    # this is supposed to check a tolerance around each corner to see if we have overlapped the goal
    # it doesnt seem right to me though.....
    return ( abs(a.x - b.x) < tolerance
             and abs(a.y - b.y) < tolerance \
             and abs(a.x + a.width) - (b.x + b.width) < tolerance
             and abs(a.y - b.y) < tolerance \
             and abs(a.x + a.width) - (b.x + b.width) < tolerance
             and abs(a.y + a.height) - (b.y + b.height) < tolerance \
             and abs(a.x - b.x) < tolerance
             and abs( (a.y + a.height) - (b.y + b.height) < tolerance ))
                                                                
############################################################################
# The goal here is to somehow compare two rectangles to see if they overlap
# ideally we have a tolerance on all 4 corners but as the box covers up the goal we'll have some
# issues if we update the goal, maybe we only update the goal the first time.
# targetpositionmessage and boxposition message can be the same callback with
# an id if we want to, just written like this for now to be explicit in my thoughts
#####################################################################################################
# The targetPositionMessage will be a list/array of Rectangle.msg
#
# float32 x
# float32 y
# float32 height
# float32 width
#####################################################################################################
def targetPositionEventCallback( targetPositionMessage ):
    global endGoal
    if ( endGoal is None ):
        endGoal = targetPositionMessage

#####################################################################################################
# The boxPositionMessage will be a list/array of Rectangle.msg
#
# float32 x
# float32 y
# float32 height
# float32 width
#####################################################################################################
def boxPositionEventCallback( boxPositionMessage ):
    global boxPosition
    boxPosition = boxPositionMessage

#####################################################################################################
# The botPositionMessage will be a list/array of Location.msg
#
# int32 robot_num
# float32 x
# float32 y
# float32 theta
#####################################################################################################
def botPositionEventCallback( botPositionMessage ):
    global moveType, moveComand, myBotID, botToTrackID, locationLastUpdateTime
    global endGoal, boxPosition

    for botPositionIdx in range( 0, len( botPositionMessage ) ):
        if ( botPositionMessage[botPositionIdx].robot_num == botToTrackID ):
            targetPositionMessage = botPositionMessage[botPositionIdx]
        else:
            myPositionMessage = botPositionMessage[botPositionIdx]

    currentTime = datetime.now()
    if ( locationLastUpdateTime == None ):
        locationLastUpdateTime = currentTime

    deltaTInTime = currentTime - locationLastUpdateTime
    deltaTInSeconds = float(deltaTInTime.microseconds) / 1e6

    locationLastUpdateTime = currentTime

    if ( moveType == MOVE_FORWARD_CONSTANT_VELOCITY ):
        currentError = getErrorBetweenBots( myPositionMessage, targetPositionMessage )

        # if we have reached our linear target, and we are 'close enough', we stop
        # this requires us to set a linear target from the messages as well
        if ( checkToStop( endGoal, boxPosition ) ):
            stopMoving()
        else:
            # TODO:  Figure out how to stop the move
             moveCommand.linear.x = trackBotConfiguration.getPID().getNewSetting( currentError, \
                                                                                  deltaTInSeconds, True )

    return

#####################################################################################################
#####################################################################################################
def waitForMoveToComplete():
    while ( moveType != MOVE_NONE ):
        rospy.sleep(0.1)
    rospy.sleep( 0.5 )

    return

#####################################################################################################
# The following will move the robot forward and make contact with the object, typically this would
# be a slow forward move
#####################################################################################################
def makeContactWithObject( velocity ):
    global moveType, moveCommand
    
    if ( moveType != MOVE_NONE ):
        print "Trying to start a move to make contact while bot is already moving!"
        return
    
    moveCommand.angular.z = 0.0
    moveCommand.linear.x = velocity
    moveCommand.linear.y = 0.0

    # Wait for the move to complete then pause for 0.5 seconds
    moveType = MOVE_FORWARD_STOP_ON_CONTACT
    waitForMoveToComplete()

    return

#####################################################################################################
#####################################################################################################
def moveForward( xTarget, yTarget, angularVelocity, tolerance, minVelocity, maxVelocity ):
    global moveType, moveCommand, forwardMoveConfiguration
    
    if ( moveType != MOVE_NONE ):
        print "Trying to start a move forward while bot is already moving!"
        return

    # _pc, _ic, _dc, _minOutput, _maxOutput ):
    pid = PIDController( 0.35, 0.001, 0.1, 0.1, 0.6 )
    pid.setTarget( target ) # TODO:  Add to the constructor, but leave set in case
    forwardMoveConfiguration = ForwardMoveConfiguration( xTarget, yTarget, tolerance, pid )

    moveCommand.angular.z = angularVelocity
    moveCommand.linear.x = minVelocity
    moveCommand.linear.y = 0.0
    
    # Wait for the move to complete then pause for 0.5 seconds
    moveType = MOVE_FORWARD_SPECIFIC_DISTANCE
    waitForMoveToComplete()

    return

#####################################################################################################
#####################################################################################################
def moveTrackingOtherBot():
    global moveType, moveCommand, trackBotConfiguration
    
    if ( moveType != MOVE_NONE ):
        print "Trying to start a move tracking other bot while bot is already moving!"
        return

    # _pc, _ic, _dc, _minOutput, _maxOutput ):
    pid = PIDController( 0.35, 0.001, 0.0, 0.1, 0.6 )
    pid.setTarget( 0.0 ) # TODO:  Add to the constructor, but leave set in case
    trackBotConfiguration = TrackBotConfiguration( botToTrackID, pid )

    # Wait for the move to complete then pause for 0.5 seconds
    moveType = MOVE_FORWARD_SPECIFIC_DISTANCE
    waitForMoveToComplete()

    return

#####################################################################################################
#####################################################################################################
def moveTrackingOtherBotSimulation():
    global moveType, moveCommand, trackBotConfiguration
    
    if ( moveType != MOVE_NONE ):
        print "Trying to start a move tracking other bot while bot is already moving!"
        return

    # _pc, _ic, _dc, _minOutput, _maxOutput ):
    pid = PIDController( 0.35, 0.001, 0.0, 0.1, 0.6 )
    pid.setTarget( 0.0 ) # TODO:  Add to the constructor, but leave set in case
    trackBotConfiguration = TrackBotConfiguration( botToTrackID, pid )

    # Wait for the move to complete then pause for 0.5 seconds
    moveType = MOVE_FORWARD_SPECIFIC_DISTANCE

    # Do 5 2 meter moves, changing the velocity for each of the moves
    
    currentMasterBotX = 0.0
    currentMasterBotVelocity = 0.0
    botLocations = []
    
    segmentVelocities = [ 0.25, 0.50, 0.25, 0.50, 0.25 ]
    
    refreshRate = 0.01
    for ii in range ( 0, 5 ):  # 5 moves (changes in velocity)
        currentMasterBotVelocity = segmentVelocities[ii] # MasterBot velocity is constant
        for jj in range ( 0, 100 ):
            rospy.sleep( refreshRate )  # 1ms odometry update rate

            botLocations = []

            currentMasterBotX = currentMasterBotX + ( currentMasterBotVelocity * refreshRate )

            masterBot = Location()
            masterBot.robot_num = botNum
            masterBot.x = currentMasterBotX
            masterBot.y = 0
            masterBot.theta = 0
            botLocations.append( masterBot )            

            trackingBot = Location()
            trackingBot.robot_num = botNum
            trackingBot.x = lastOdometryUpdate.pose.pose.position.x
            trackingBot.y = 0
            trackingBot.theta = 0
            botLocations.append( trackingBot )            

            botPositionEventCallback( botLocations )

            del botLocations

#####################################################################################################
#####################################################################################################
def initializeCommands():
    global moveCommand, sendCommandService, bumperStates
    rospy.init_node( 'Mover', anonymous=True )

    rospy.wait_for_service( 'constant_command' )

    # Subscribe for the message we care about
    #   We are using odometry for moving and turning
    #   We are using the bumper events to detect if we are up against,
    #       the object to move.  Maybe also to detect if we hit something
    #       we did not mean to.
    rospy.Subscriber( "/odom", Odometry, odometryCallback )
    rospy.Subscriber( "/mobile_base/events/bumper", BumperEvent, bumperEventCallback )
    rospy.Subscriber("/jump/constantBotPosition", BotPositionEvent, botPositionEventCallback )
    rospy.Subscriber("/jump/targetPosition", targetPositionEvent, targetPositionEventCallback )
    rospy.Subscriber("/jump/boxPosition", boxPositionEvent, boxPositionEventCallback )
    ## Create a command object
    moveCommand = Twist()
    sendCommandService = rospy.ServiceProxy( 'constant_command', ConstantCommand )

    bumperStates = BumperSensors()

    return
    
if __name__ == "__main__":
    try: 
        
        initializeCommands()
        
        #makeContactWithObject( 0.1 )
        #rospy.sleep( 5.0 )
        # TODO:  Make sure we can still move forward!!!, then try tracking simulation
        moveForward( 5.00, 0.00, 0.00, 0.00, -0.60, 0.60 )
        #moveTrackingOtherBotSimulation()

    except rospy.ROSInterruptException: pass

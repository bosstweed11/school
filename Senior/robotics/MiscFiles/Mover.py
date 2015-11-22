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

# NOTE:  Each callback should update the botStatus at the end, make sure to define myBotStatus as global in the callbacks
# TOOD:  Post the bot status - Done!
# TODO:  Better name the message queues!
# TODO:  Implement callback to receive status from the other bot, it probably just needs to
#        update the other bot status - Done
# TODO:  Once we have the status from the other bot, then when we are moving with a constant velocity
#        we can trigger the stop on when it has stopped - Coded, but need the status to be received - Done
# TODO:  Implement the "process" portion of moving the box with two robots
# TODO:  How do we know when the box is in place("master bot") => see checkToStop
# TODO:  Make sure the BotStatus.msg file makes it into the msg directory on the bot
# TODO:  Can the BumperStatus class be removed and just use the BotStatus class instead
# TODO:  Figure out how to use a python mutex that can be used in a callback since we cant sleep
# TODO:  Figure out what the KINECT_TO_METERS constant should be
# TODO:  If it is not true that you do not have to define a global variable as global if you are just reading
#        it in a function then check each function to make sure the globals are referenced correctly

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

maxTrackBotVelocity = 2.0

locationLastUpdateTime = None

lessThanLowerLimit = False
greaterThanUpperLimit = False

myBotStatus = None
otherBotStatus = None

botStatusPublisher = None

#####################################################################################################
#####################################################################################################
def postBotStatusUpdate():
    botStatusPublisher.publish( myBotStatus )
    return

#####################################################################################################
#####################################################################################################
class BotStatus:
    def __init__( self, botId, xVelocity, yVelocity, zVelocity, \
                               xPosition, yPosition, zPosition, \
                               moveType, leftBumperSensor, rightBumperSensor, centerBumperSensor ):
        self.botId = botId
        self.xVelocity = xVelocity
        self.yVelocity = yVelocity
        self.zVelocity = zVelocity
        self.xPosition = xPosition
        self.yPosition = yPosition
        self.zPosition = zPosition
        self.moveType = moveType
        self.leftBumperSensor = leftBumperSensor
        self.rightBumperSensor = rightBumperSensor
        self.centerBumperSensor = centerBumperSensor

    def getMoveType( self ):
        return self.moveType

    def setXVelocity( self, xVelocity ):
        self.xVelocity = xVelocity
    def setYVelocity( self, yVelocity ):
        self.yVelocity = yVelocity
    def setZVelocity( self, zVelocity ):
        self.zVelocity = zVelocity
    def setXPosition( self, xPosition ):
        self.xPosition = xPosition
    def setYPosition( self, yPosition ):
        self.yPosition = yPosition
    def setZPosition( self, zPosition ):
        self.zPosition = zPosition
    def setMoveType( self, moveType ):
        self.moveType = moveType
    def updateBumperState( self, bumper, pressed ):
        if ( bumper == LEFT_BUMPER ):
            self.leftBumperSensor = pressed
        elif ( bumper == RIGHT_BUMPER ):
            self.rightBumperSensor = pressed
        elif ( bumper == CENTER_BUMPER ):
            self.centerBumperSensor = pressed
        else:
            print "Invalid bumper / state!    => bumper" + str(bumper) + " pressed=" + str(pressed)

    def __str__( self ):
        return "Bot status => botId=" + str(self.botId) \
                          + " xVel=" + str(self.xVelocity) + " yVel=" + str(self.yVelocity) + " zVel=" + str(self.zVelocity) \
                          + " moveType=" + str(self.moveType) \
                          + " leftBumper=" + str(self.leftBumperSensor) \
                          + " rightBumper=" + str(self.rightBumperSensor) \
                          + " centerBumper=" + str(self.centerBumperSensor)

#####################################################################################################
#####################################################################################################
class BumperSensors:
    def __init__( self ):
        self.leftBumperIsPressed = False
        self.rightBumperIsPressed = False
        self.centerBumperIsPressed = False
        
    def __str__( self ):
        return "Bumper Status => leftIsPressed=" + str(self.leftBumperIsPressed) \
                              + " rightIsPressed=" + str(self.rightBumperIsPressed) \
                              + " centerIsPressed=" + str(self.centerBumperIsPressed)

    def setBumperState( self, bumper, pressed ):
        if ( bumper == LEFT_BUMPER and pressed == 1 ):
            self.leftBumperIsPressed = True
            print "Left Bumper pressed!"
        elif ( bumper == LEFT_BUMPER and pressed == 0 ):
            self.leftBumperIsPressed = False
            print "Left Bumper released!"
        elif ( bumper == RIGHT_BUMPER and pressed == 1 ):
            self.rightBumperIsPressed = True
            print "Right Bumper pressed!"
        elif ( bumper == RIGHT_BUMPER and pressed == 0 ):
            self.rightBumperIsPressed = False
            print "Right Bumper released!"
        elif ( bumper == CENTER_BUMPER and pressed == 1 ):
            self.centerBumperIsPressed = True
            print "Center Bumper pressed!"
        elif ( bumper == CENTER_BUMPER and pressed == 0 ):
            self.centerBumperIsPressed = False
            print "Center Bumper released!"
        else:
            print "Invalid bumper / state!    => bumper" + str(bumper) + " pressed=" + str(pressed)

    def isBumperPressed( self, bumper ):
        if ( bumper == LEFT_BUMPER ):
            return self.leftBumperIsPressed
        elif ( bumper == RIGHT_BUMPER ):
            return self.rightBumperIsPressed
        elif ( bumper == CENTER_BUMPER ):
            return self.centerBumperIsPressed
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
class TurningBotConfiguration:
    def __init__( self, target, tolerance, pid ):
        self.target = target
        self.tolerance = tolerance
        self.pid = pid

    def getTarget( self ):
        return self.target
    def getTolerance( self ):
        return self.tolerance
    def getPID():
        return self.pid

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
#####################################################################################################
def doneTurning( currentTheta, target, tolerance ):
    global lessThanLowerLimit, greaterThanUpperLimit
    
    doneTurning = False
    
    #print "ct=" + str(currentTheta)
    if ( abs(currentTheta) == 1.0 ):
        degrees = 180
    elif ( currentTheta >= 0 ):
        degrees = 180 / math.pi * 2 * math.asin( currentTheta )
    else:
        degrees = 180 + ( 180 - ( 180 / math.pi * 2 * math.asin( abs(currentTheta) ) ) )
    
    if ( target == 0 ):
        # TODO: Handle passing through the tolerance at 0
        if ( ( degrees >= 0 ) and ( degrees < tolerance ) ):
            doneTurning = True
        elif ( ( degrees <= 360 ) and ( degrees > ( 360 - tolerance ) ) ):
            doneTuring = True
    else:
        if ( degrees < ( target - tolerance ) ):
            lessThanLowerLimit = True
        elif ( degrees > ( target + tolerance ) ):
            greaterThanUpperLimit = True
        doneTurning = abs( degrees - target ) < tolerance
    
    #print "degrees=" + str(degrees) + " lt=" + str(lessThanLowerLimit) + " gt=" + str(greaterThanUpperLimit) \
                      + " dt=" + str(doneTurning) + " abs=" + str(abs( degrees - target ))
    if ( lessThanLowerLimit and greaterThanUpperLimit ):
        doneTurning = True
    
    return doneTurning

#####################################################################################################
## TODO:  Support backing up, for now just set the approach speed low enough to not overshoot!
#####################################################################################################
def odometryCallback( odometry ):
    global moveType, moveCommand, lastOdometryMessage, bumperState, lastOdometryUpdate
    global myBotStatus, forwardMoveConfiguration, turingConfiguration

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

    elif ( moveType == TURN_IN_PLACE ):
        if ( doneTurning( odometry, turningConfiguration.getTarget(), turingConfiguration.getTolerance() ) ):
            stopMoving( odometry, turningConfiguration.getPID() )
        else:
            moveCommand.angular.z = turningConfiguration.getPID().getNewSetting( odometry.pose.pose.orientation.z, \
                                                                                 deltaTInSeconds, True )

    elif ( moveType != MOVE_FORWARD_CONSTANT_VELOCITY ):
        print "Invalid move type!    => moveType =" + str(moveType)

    sendCommandService( moveCommand )

    lastOdometryMessage = odometry

    # TODO: Need a mutex here to make sure this callback does not overwrite botPositionEventCallback
    myBotStatus.setMovetype( moveType )
    # The velocities do not get updated when the moveType is MOVE_FORWARD_CONSTANT_VELOCITY, but they should still
    # be valid
    myBotStatus.setXVelocity( moveCommand.linear.x )
    myBotStatus.setYVelocity( moveCommand.linear.y )
    myBotStatus.setZVelocity( moveCommand.angular.z )
    myBotStatus.setXPosition( odometry.pose.pose.position.x )
    myBotStatus.setYPosition( odometry.pose.pose.position.y )
    myBotStatus.setZPosition( odometry.pose.pose.orientation.z )
    postBotStatusUpdate()

    return 0

#####################################################################################################
## Bumper Callback - check to see if any of the bumpers have been hit yet
#####################################################################################################
def bumperEventCallback( data ):
    global bumperStates, myBotStatus

    bumperStates.setBumperState( data.bumper, data.state )
    
    myBotstatus.updateBumperState( data.bumper, data.state )
    postBotStatusUpdate()
    
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
#####################################################################################################
def otherBotStopped():
    # If we do not have the status of the other bot yet, then assume it is moving
    if ( otherbotStatus == None ):
        return False
    return ( otherBotStatus.getMoveType() == MOVE_NONE )

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
    global endGoal, boxPosition, myBotStatus

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

        # If the other bot has stopped, then we can also
        # TODO: What happens if this gets called before the other bot ever starts moving!
        #       The "process" should not let this happen
        if ( otherBotStopped() ):
            stopMoving()
        else:
            # TODO:  Figure out how to stop the move
            trackBotConfigVelocity = trackBotConfiguration.getPID().getNewSetting( currentError, \
                                                                                   deltaTInSeconds, True )
            if ( trackBotConfigVelocity < maxTrackBotVelocity ):
                moveCommand.linear.x = trackBotConfigVelocity
            else:
                moveCommand.linear.x = maxTrackBotVelocity

    myBotStatus.setMovetype( moveType )
    myBotStatus.setXVelocity( moveCommand.linear.x )
    myBotStatus.setYVelocity( moveCommand.linear.y )
    myBotStatus.setZVelocity( moveCommand.angular.z )
    # Position information is updated in the odometry callback
    postBotStatusUpdate()
    return

#####################################################################################################
#####################################################################################################
def botStatusCallback( botStatus ):
    global otherBotStatus

    otherBotStatus = botStatus

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
            masterBot.robot_num = 0
            masterBot.x = currentMasterBotX
            masterBot.y = 0
            masterBot.theta = 0
            botLocations.append( masterBot )            

            trackingBot = Location()
            trackingBot.robot_num = 1 # does this matter? not sure yet i think we can just hard code it
            trackingBot.x = lastOdometryUpdate.pose.pose.position.x
            trackingBot.y = 0
            trackingBot.theta = 0
            botLocations.append( trackingBot )            

            botPositionEventCallback( botLocations )

            del botLocations
            
    return

#####################################################################################################
#####################################################################################################
def turnBot( targetInDegress, toleranceInDegress, velocity ):
    
    pid = PIDController( 1.0, 0.000, 0.0, 0.1, 0.5 )

    turningConfiguration = TurningBotConfiguration( targetInDegress, toleranceInDegress, pid )
    
    moveCommand.angular.z = velocity
    moveCommand.linear.x = 0.0
    moveCommand.linear.y = 0.0
    
    moveType = TURN_IN_PLACE
    waitForMoveToComplete()
    
    return

#####################################################################################################
#####################################################################################################
def executeMasterBot():
    
    makeContactWithObject( 0.2 )
    waitForTrackingBotToMakeContact()
    
    return

#####################################################################################################
#####################################################################################################
def executeTrackingBot():

    makeContactWithObject( 0.2 )
    waitForTrackingBotToStart()

    return

#####################################################################################################
#####################################################################################################
def initializeCommands():
    global moveCommand, sendCommandService, bumperStates
    global botStatusPublisher
    
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
    
    # TODO:  Figure out naming for this queu
    rospy.Subscriber("/rossie/botStatus", botStatus, botStatusCallback )
    ## Create a command object
    moveCommand = Twist()
    sendCommandService = rospy.ServiceProxy( 'constant_command', ConstantCommand )
    
    # TODO: Better name for the message queue
    botStatusPublisher = rospy.Publisher( "/mrdata/botStatus", BotStatus ) # Publish Bot Status

    bumperStates = BumperSensors()
    
    myBotStatus = BotStatus( 0, 0, 0, 0, MOVE_NONE, RELEASED, RELEASED, RELEASED )

    return
    
if __name__ == "__main__":
    try: 
        
        initializeCommands()
        
        #makeContactWithObject( 0.1 )
        #rospy.sleep( 5.0 )
        # TODO:  Make sure we can still move forward!!!, then try tracking simulation
        moveForward( 5.00, 0.00, 0.00, 0.00, -0.60, 0.60 )
        #moveTrackingOtherBotSimulation()
        turnBot( 90, 0.5, 0.25 )
        turnBot( 180, 0.5, 0.25 )
        turnBot( 270, 0.5, 0.25 )
        turnBot( 360, 0.5, 0.25 )
    
        if ( masterBot == 1 ):
            executeMasterBot()
        else:
            executeTrackingBot()

    except rospy.ROSInterruptException: pass

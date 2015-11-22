#!/usr/bin/env python
import rospy
from std_msgs.msg import *
from nav_msgs.msg import *
from geometry_msgs.msg import *
from cs1567p0.srv import *
import math

lastDirection = 1 	# 1 if we are moving forward, 0 if backward
currentEdge = 0 	# how many times we have turned
isTurning = 0 		# 1 if we are turning, 0 if not
direction = 1 		# 1 if we are increasing in our coordinate value, -1 if decreasing
aTarget = 0 		# global for angular target needed for correction when moving forward
'''
# get_in_range
# this function will adjust values and move the robot until its in the range we want it to be in
def get_in_range()
	if ( ( lastDirection == 1 and ( coordinate >= target and coordinate < ( target + offset ) ) ) or ( lastDirection == 0 and ( coordinate > ( target - offset ) and coordinate <= target ) ) ):
		isTurning = 1
	else:
		rospy.loginfo(data.pose.pose)

		if ( coordinate < target ):
			if ( lastDirection == 0 ):
				command.linear.x = 0.0
				send_command(command)
			else:
				command.linear.x = 0.15 * direction
				send_command(command)

		lastDirection = 1

		elif ( coordinate > target ):
			if ( lastDirection == 1 ):
				command.linear.x = 0.0
				send_command(command)
			else:
				command.linear.x = -0.15 * direction
				send_command(command)

		lastDirection = 0
		
		else:
			command.linear.x = 0.0
			send_command(command)


'''

def quat_to_angle( data )

        x = data.pose.pose.position.x
        y = data.pose.pose.position.y
        z = data.pose.pose.position.z
        w = data.pose.pose.position.w

        mag = math.sqrt( x**2 + y**2 + z**2 + w**2 )

        x = x / mag
        y = y / mag
        z = z / mag
        w = w / mag

        ## tbd
        

# turn_degrees
#
# input:
# data: odometry data
# target: degrees ( 90 in most cases )
#
# description: function for turning left
def turn_degrees( data, target ):
	global lastDirection
	global currentEdge
	global aTarget

	command = Twist()
	send_command = rospy.ServiceProxy('constant_command', ConstantCommand)
	
	print data.pose.pose

	angle = quat_to_angle( data )


	## tbd

	target = 0	# value we are trying to reach by going forward ( either 1.0 or 0.0 )
	coordinate = data.pose.pose.orientation.z	# coordinate we are traveling across ( x or y )
	offset = .01	# how close we care to be

	value = math.fabs(aTarget) - math.fabs(coordinate)

	if ( value < offset and value > -offset ):
                        return 1 # We've reached our turning goal
	else:
                
                        if ( math.fabs(coordinate) < math.fabs(target) ):
                                if ( lastDirection == 0 ):
                                        command.angular.z = 0.0
                                        send_command(command)
                                else:
                                        command.angular.z = 0.35
                                        send_command(command)

                                lastDirection = 1

                        elif ( math.fabs(coordinate) > math.fabs(target) ):
                                if ( lastDirection == 1 ):
                                        command.angular.z = 0.0
                                        send_command(command)
                                else:
                                        command.angular.z = -0.35
                                        send_command(command)

                                lastDirection = 0
                        
                        else:
                                command.angular.z = 0.0
                                send_command(command)

                        return 0 # We must keep rotating


# move_forward
#
# input:
# data: odometry data
# target: distance we want to move forward
#
# output: none
#
# description: function for moving forward
def move_forward( data, target ):
	global lastDirection
	
	command = Twist()
	send_command = rospy.ServiceProxy('constant_command', ConstantCommand)

	# Maybe sleeping in between move_forward() and turn_left() will help this problem
	angularCorrection = data.twist.twist.angular.z

                # Find out where we are
	value = data.pose.pose.position.x

	# Set offset, should this be a param?
	offset = .03

                if ( value <  target + offset and value >  target - offset ):
		return 1 # We are done moving forward
	else:
                        # We aren't to our target distance yet, keep moving
                        if ( value < target- offset ):
                                if ( lastDirection == 0 ):
                                        command.linear.x = 0.0
                                        send_command(command)
                                    else:
                                        command.linear.x = 0.3
                                        command.angular.z = -(angularCorrection + .02 )
                                        send_command(command)

                                lastDirection = 1

                        elif ( value > target  + offset ):
                                if ( lastDirection == 1 ):
                                        command.linear.x = 0.0
                                        send_command(command)
                                else:
                                        command.linear.x = -0.3
                                        command.angular.z = -(angularCorrection + .02)
                                        send_command(command)

                                lastDirection = 0
                            
                        else:
                                command.linear.x = 0.0
                                send_command(command)

                        return 0 # We need to call this again


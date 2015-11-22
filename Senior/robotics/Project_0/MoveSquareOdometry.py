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
# turn_left
# function for turning left
def turn_left(data):
	global isTurning
	global direction
	global lastDirection
	global currentEdge
	global aTarget

	command = Twist()
	send_command = rospy.ServiceProxy('constant_command', ConstantCommand)
	
	print data.pose.pose

	target = 0	# value we are trying to reach by going forward ( either 1.0 or 0.0 )
	coordinate = data.pose.pose.orientation.z	# coordinate we are traveling across ( x or y )
	offset = .01	# how close we care to be

	# Choose target distance based off of which edge you are on
	if ( currentEdge == 0 ):
		aTarget = .65
	elif ( currentEdge == 1 ):
		aTarget = 1.008
	elif ( currentEdge == 2 ):
		aTarget = -.74
	elif ( currentEdge == 3 ):
		aTarget = 0.0
	else:
		aTarget = 0.0

	if ( currentEdge < 2 ):
		direction = 1
	else:
		direction = -1

	value = math.fabs(aTarget) - math.fabs(coordinate)

	if ( value < offset and value > -offset ):
		isTurning = 0
		currentEdge = currentEdge + 1
	else:
		

		if ( math.fabs(coordinate) < math.fabs(aTarget) ):
			if ( lastDirection == 0 ):
				command.angular.z = 0.0
				send_command(command)
			else:
				command.angular.z = 0.35 * direction
				send_command(command)

			lastDirection = 1

		elif ( math.fabs(coordinate) > math.fabs(aTarget) ):
			if ( lastDirection == 1 ):
				command.angular.z = 0.0
				send_command(command)
			else:
				command.angular.z = -0.35 * direction
				send_command(command)

			lastDirection = 0
		
		else:
			command.angular.z = 0.0
			send_command(command)


# move_forward
# functin for moving forward
def move_forward(data):
	global direction
	global lastDirection
	global isTurning
	global currentEdge
	
	command = Twist()
	send_command = rospy.ServiceProxy('constant_command', ConstantCommand)

	#print data.pose.pose
	
	angularCorrection = data.twist.twist.angular.z
	
	
	target = 0	# value we are trying to reach by going forward ( either 1.0 or 0.0 )
	coordinate = 0	# coordinate we are traveling across ( x or y )
	offset = .35	# how close we care to be

	# Choose target distance based off of which edge you are on
	if ( currentEdge < 2 ):
		target = .98
		direction = 1
	else:
		target = 0.05
		direction = -1

	# Choose coordinate based off of w
	if ( currentEdge % 2 == 0 ):
		coordinate = data.pose.pose.position.x
	else:
		coordinate = data.pose.pose.position.y


	if ( ( lastDirection == 1 and ( coordinate >= target and coordinate < ( target + offset ) ) ) or ( lastDirection == 0 and ( coordinate > ( target - offset ) and coordinate <= target ) ) ):
		isTurning = 1
	else:
		rospy.loginfo(data.pose.pose)

		if ( coordinate < target ):
			if ( lastDirection == 0 ):
				command.linear.x = 0.0
				send_command(command)
			else:
				command.linear.x = 0.3 * direction
				command.angular.z = -(angularCorrection + .02 )
				send_command(command)

			lastDirection = 1

		elif ( coordinate > target ):
			if ( lastDirection == 1 ):
				command.linear.x = 0.0
				send_command(command)
			else:
				command.linear.x = -0.3 * direction
				command.angular.z = -(angularCorrection + .02)
				send_command(command)

			lastDirection = 0
		
		else:
			command.linear.x = 0.0
			send_command(command)


def odometry_callback(data):
    global currentEdge
    global isTurning
    global lastDirection

    command = Twist()
    send_command = rospy.ServiceProxy('constant_command', ConstantCommand)

    
    rospy.loginfo(currentEdge)
    rospy.loginfo(data.pose.pose.orientation.z)
    rospy.loginfo(data.pose.pose)

    # If for going in a square
    if ( currentEdge < 4 ):
	# We are not yet finished, if turning -> turn, else move forward
    	if ( isTurning == 1 ):
		turn_left(data)
   	else:
		move_forward(data)

    else:
	#Here we have completed, so stay still
	command.linear.x = 0.0
	send_command(command)


def initialize():
    pub = rospy.Publisher('/mobile_base/commands/reset_odometry', Empty, queue_size=10)
    rospy.Subscriber('/odom', Odometry, odometry_callback)
    rospy.init_node('MoveSquareOdometry', anonymous=True)
    rospy.wait_for_service('constant_command')

    while pub.get_num_connections() < 1:
	rospy.sleep(0.1)
    pub.publish(Empty())
    rospy.spin()
    

if __name__ == "__main__":
    initialize()


#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from cs1567p0.srv import *

straightSleepTime = 2.7
turnSleepTime = 2.1

def move_forward():
	global straightSleepTime

	command = Twist()
	send_command = rospy.ServiceProxy('constant_command', ConstantCommand)
        command.linear.x = 0.50
        command.angular.z = -.105
        response = send_command(command)
        rospy.sleep(straightSleepTime)
        command.linear.x = 0.0
        response = send_command(command)
	straightSleepTime = straightSleepTime - 0.07

def move_left():
	global turnSleepTime

	command = Twist()
	send_command = rospy.ServiceProxy('constant_command', ConstantCommand)
        command.angular.z = 1.0
        response = send_command(command)
        rospy.sleep(turnSleepTime)
        command.angular.z = 0.0
        response = send_command(command)
	turnSleepTime = turnSleepTime - 0.285

def move_square():
    global turnSleepTime
    rospy.init_node('MoveSquareNoOdometry', anonymous=True)
    rospy.wait_for_service('constant_command')
    
    
    try:
        move_forward()
	move_left()
	move_forward()
	move_left()
	move_forward()
	move_left()
	move_forward()
	turnSleepTime = 1.95
	move_left()
        print response

    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

if __name__ == "__main__":
    move_square()

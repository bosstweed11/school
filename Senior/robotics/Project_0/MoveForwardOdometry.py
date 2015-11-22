#!/usr/bin/env python
import rospy
from std_msgs.msg import *
from nav_msgs.msg import *
from geometry_msgs.msg import *
from cs1567p0.srv import *

lastDirection = 1

def odometry_callback(data):
    global lastDirection
    command = Twist()
    send_command = rospy.ServiceProxy('constant_command', ConstantCommand)

    print data.pose.pose

    if ( ( lastDirection == 1 and ( data.pose.pose.position.x >= 0.99 and data.pose.pose.position.x < 1.02 ) ) or ( lastDirection == 0 and ( data.pose.pose.position.x > 0.98 and data.pose.pose.position.x <= 1.01 ) ) ):
    	command.linear.x = 0.0
	send_command(command)
    else:
	
	    rospy.loginfo(data.pose.pose)

	    if(data.pose.pose.position.x < 1.0):
		if ( lastDirection == 0 ):
			command.linear.x = 0.0
			send_command(command)
		else:
			command.linear.x = 0.15
			command.angular.z = (-data.pose.pose.orientation.z)
			send_command(command)
			
		lastDirection = 1
	    elif (data.pose.pose.position.x > 1.0):
		if ( lastDirection == 1 ):
			command.linear.x = 0.0
			send_command(command)
		else:
			command.linear.x = -0.15
			send_command(command)
			
		lastDirection = 0
	    else:
		command.linear.x = 0.0
		send_command(command)
	    


def initialize():
    pub = rospy.Publisher('/mobile_base/commands/reset_odometry', Empty, queue_size=10)
    rospy.Subscriber('/odom', Odometry, odometry_callback)
    rospy.init_node('MoveForwardOdometry', anonymous=True)
    rospy.wait_for_service('constant_command')
    
    while pub.get_num_connections() < 1:
	rospy.sleep(0.1)
    pub.publish(Empty())
    rospy.spin()
    

if __name__ == "__main__":
    initialize()


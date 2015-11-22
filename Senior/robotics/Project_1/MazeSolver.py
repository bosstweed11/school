#!/usr/bin/python
import rospy
from cs1567p1.srv import *
from std_srvs.srv import * 

LEFT = 0
RIGHT = 1
UP = 2
DOWN = 3

make_maze_service = None
print_maze_service = None
get_wall_service = None
constant_command_service = None

# Globals
lastDirection = 0

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

def turn_90_degrees():
    return 1

def solve_maze():
    return 1

def initialize_commands():
    rospy.init_node('mazesolvernode', anonymous=True)
    rospy.wait_for_service('make_maze')
    rospy.wait_for_service('print_maze')
    rospy.wait_for_service('get_wall')
#    rospy.wait_for_service('constant_command')

    global make_maze_service, print_maze_service, get_wall_service
    global constant_command_service

    make_maze_service = rospy.ServiceProxy('make_maze', MakeNewMaze)
    print_maze_service = rospy.ServiceProxy('print_maze', Empty)
    get_wall_service = rospy.ServiceProxy('get_wall', GetMazeWall)
#    constant_command_service = rospy.ServiceProxy('constant_command', ConstantCommand)

    make_maze_service(5,5)
    print_maze_service()
    solve_maze()

     
if __name__ == "__main__":   
    try: 
        initialize_commands()
    except rospy.ROSInterruptException: pass


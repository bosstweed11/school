#!/usr/bin/python
#import rospy
#from cs1567p1.srv import *
#from std_srvs.srv import *
import sys
from random import randint
from random import shuffle

MazeGrid = [[]]
PruningGrid = [[]]
ActivePath = [[]]
Columns = 0
Rows = 0

LEFT = 0
RIGHT = 1
UP = 2
DOWN = 3

DIRECTION_MAPPING = [ UP, LEFT, DOWN, RIGHT ]
ANGLE_TARGET = [ 0.0, 0.7, 1.0, -0.7 ]

def createGrid(rows,cols):
    global MazeGrid, PruningGrid, ActivePath, LEFT, RIGHT, UP, DOWN
    global Columns 
    Columns = cols
    global Rows 
    Rows = rows
#index col,then row
    MazeGrid = [[[LEFT, RIGHT, UP, DOWN] for x in xrange(rows)] for y in xrange(cols)]
    PruningGrid = [[0 for x in xrange(rows)] for y in xrange(cols)]
    ActivePath = [[0 for x in xrange(rows)] for y in xrange(cols)]
    for i in xrange(cols):
        MazeGrid[i][0].remove(UP)
        MazeGrid[i][rows-1].remove(DOWN)
    for j in xrange(rows):
        MazeGrid[0][j].remove(LEFT)
        MazeGrid[cols-1][j].remove(RIGHT)
    #print MazeGrid #printing each column left to right

def getWall(col, row, direction):
    if row == 0 and direction == UP:
        return True
    if row == Rows-1 and direction == DOWN:
        return True
    if col == 0 and direction == LEFT:
        return True
    if col == Columns-1 and direction == RIGHT:
        return True
    return MazeGrid[col][row].count(direction) > 0

def printMaze():
    print "Print Maze : " + str(Columns)
    s = ""
    for j in xrange(0,Columns):
        s = s+" _"
    s = s+"\n"
    for i in xrange(0,Rows):
        s = s+"|"
        for j in xrange(0,Columns):
            if MazeGrid[j][i].count(DOWN) > 0 or i == Rows-1:
                s = s+"_"
            else:
                s = s+" "
            if MazeGrid[j][i].count(RIGHT) > 0 or j == Columns-1:
                s = s+"|"
            else:
                s = s+" "
            if j == Columns-1:
                s = s+"\n"
    print s
    return s

def makeMaze():
    global LEFT, RIGHT, UP, DOWN

    visited = []
    stack = []

    while len(visited) < Rows*Columns:
        if len(stack) > 0:
            [col,row] = stack[0]
            
            neighbors = MazeGrid[col][row]
            shuffle(neighbors)
            nextidx = -1
            neighbordirection = LEFT
            for i in xrange(len(neighbors)):
                [neighborcol,neighborrow] = [col,row]
                if neighbors[i] == LEFT:
                    neighborcol = col-1
                    neighbordirection = RIGHT
                elif neighbors[i] == RIGHT:
                    neighborcol = col+1
                    neighbordirection = LEFT
                elif neighbors[i] == UP:
                    neighborrow = row-1
                    neighbordirection = DOWN
                elif neighbors[i] == DOWN:
                    neighborrow = row+1
                    neighbordirection = UP
                else:
                    print "ERROR"
                if visited.count([neighborcol,neighborrow]) == 0:
                    nextidx = i
                    break

            if len(neighbors) > 0 and nextidx > -1:
                #pick unvisited neighbor, remove line, add neighbor to stack
                stack.insert(0,[neighborcol,neighborrow])
                visited.append([neighborcol,neighborrow])
                #print "removing "+str([neighborcol,neighborrow,neighbordirection])+" and "+str([col,row,MazeGrid[col][row][nextidx]])
                MazeGrid[neighborcol][neighborrow].remove(neighbordirection)
                MazeGrid[col][row].remove(neighbors[nextidx])

            else:
                stack.pop(0)
        else:
            randrow = randint(0,Rows-1)
            randcol = randint(0,Columns-1)
            visited.append([randcol,randrow])
            stack = [[randcol,randrow]]
    #print MazeGrid

def make_maze(rows, cols):
    createGrid(rows, cols)
    makeMaze()
    return 1

def print_maze(parameters):
    print printMaze()
#return EmptyResponse()

def get_maze_wall(col, row, direction):
    if getWall(col, row, direction):
        return 1
    return 0

def solveMaze(col, row, dir, currentPath):
    global Rows
    global Cols
    global ActivePath, PruningGrid

    print "solveMazeEntry"
    if ( PruningGrid[col][row] == 1 ):
        print "Skipping because of pruning"
        return 0
    print "col/row not pruned"

    currentPath = currentPath + "(" + str(col) + "," + str(row) + ")-"
    myStartPath = currentPath #Are strings the same as in java??

    print "solveMaze => col=" + str(col) + " row=" + str(row) + " currentPath=" + currentPath
    print "myStartPath = " + myStartPath

    if ( row == 4 and col == 4 ):
        print "FOUND PATH!!!!"
        print currentPath
        return 1
    else:
        print "Inelse"
        print "xrange = " + str(range(dir, dir + 4) )
        
        startIdx = DIRECTION_MAPPING.index( dir)
        
        for dirIdx in range( startIdx, startIdx + 4 ):
            directionInfoIdx = dirIdx % 4
            directionToTry = DIRECTION_MAPPING[directionInfoIdx]
            colToTry = col
            rowToTry = row
            print "Loop => dirIdx=" + str(dirIdx) + " dirToTry=" + str(directionToTry)
            if ( get_maze_wall( col, row, directionToTry ) == 0 ):
                if ( directionToTry == 0 ):
                    colToTry = colToTry - 1
                elif ( directionToTry == 1 ):
                    colToTry = colToTry + 1
                elif ( directionToTry == 2 ):
                    rowToTry = rowToTry - 1
                elif ( directionToTry == 3):
                    rowToTry = rowToTry + 1
                else:
                    print "Error"
            
                print "Active Path => " + str(ActivePath)
                if ( ActivePath[colToTry][rowToTry] == 0 ):
                    print "Move => col=" + str(colToTry) + " row=" + str(rowToTry) + " dir=" + str(directionToTry)
                    ActivePath[colToTry][rowToTry] = 1
                    print "Turn robot to " + str( ANGLE_TARGET[directionInfoIdx] )
                    print "Move robot 1/2 meter TODO FIGURE THIS OUT " + str( ANGLE_TARGET[directionInfoIdx] )
                    if ( solveMaze( colToTry, rowToTry, directionToTry, currentPath ) == 1 ):
                        return 1
                    ActivePath[colToTry][rowToTry] = 0
                    currentPath = myStartPath
                else:
                    print "Skipping, in active path"
            else:
                print "Found Wall => col=" + str(col) + " row=" + str(row) + " dir=" + str(directionToTry)
        print "Prune => col=" + str(col) + " row=" + str(row)
        PruningGrid[col][row] = 1

    return 0;

def initialize_commands():
    make_maze( 5, 5 )
    printMaze()
    solveMaze( 0, 0, UP, "" )
    #rospy.init_node('mazegenerationnode', anonymous=True)
    
    #s1 = rospy.Service('make_maze', MakeNewMaze, make_maze)
    #s2 = rospy.Service('print_maze', Empty, print_maze)
    #s3 = rospy.Service('get_wall', GetMazeWall, get_maze_wall)

    #rospy.spin()

     
if __name__ == "__main__":   
    try: 
        initialize_commands()
    except: pass
#except rospy.ROSInterruptException: pass

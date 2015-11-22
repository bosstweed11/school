#!/usr/bin/env python

# ON JUMP
import rospy
import sensor_msgs.point_cloud2 as pc2
import math
from sensor_msgs.msg import *
from cs1567p2.msg import *

color_mask_list = []
threshold = 20
locpub = None
kinect1pub = None
kinect2pub = None
top_mask = Image()
mid_mask = Image()

###############################################################################################
##  List of bot ("Location") objects to publish
###############################################################################################
myLocationList = []

###############################################################################################
#  Global color constants
#  These have variation and appear much brighter on the kinect image then in real life
#  We may have to have a fairly large window for error, or tune it as we go
###############################################################################################
YELLOW = [ 0xBB, 0xF8, 0xFC ];
ORANGE = [ 0x8E, 0xBC, 0xF1 ];
BROWN = [ 0x91, 0xB2, 0xCC ];
RED = [ 0x9D, 0x87, 0xE7 ];

#PINK = [ 0xB2, 0xBF, 0xDD ];
#PINK = [ 0xB2, 0xBF, 0xE0 ];
BLUE = [ 0xE0, 0xAE, 0x84];

DIRECTION_COLOR = RED

myBotList = []

###############################################################################################
##  We only want to write the image from each sensor 1 time, the following arrays keep track
##  of whether or not the information has been written for each sensor.
###############################################################################################
#wroteImage = [ 0, 0, 0 ]
#wrotePointCloud = [ 0, 0, 0 ]
wroteImage = [ 0, 0, 0 ]
wrotePointCloud = [ 1, 1, 1 ]

###############################################################################################
##  Class used to keep track of information for a given bot
###############################################################################################
class MyBot:
    def __init__( self, ID, RGBCode, currentIDX, currentIDY, currentDirX, currentDirY, posX, posY, currentTheta ):
        self.ID = ID
        self.RGBCode = RGBCode
        self.currentIDX = currentIDX
        self.currentIDY = currentIDY
        self.currentDirX = currentDirX
        self.currentDirY = currentDirY
        self.posX = posX
        self.posY = posY
        self.currentTheta = currentTheta
    
    def __str__(self):
        myRGBString = "0x"
        for val in self.RGBCode:
            myRGBString = myRGBString + "%02X " % val
        
        return "BOT => ID=%d RGB=%s currentIDX=%d currentIDY=%d currentDirX=%d currentDirY=%d posX=%d posY=%d currentTheta=%0.3f" \
            % ( self.ID, myRGBString, self.currentIDX, self.currentIDY, self.currentDirX, self.currentDirY, \
               self.posX, self.posY, self.currentTheta )

###############################################################################################
##  Create a structure for each bot and add it to the list of bots.
###############################################################################################
def initializeBotStructures():
    global myBotList
    global color_mask_list
    
    myBotList = []
    
    myBot = MyBot( 1, BLUE, -1, -1, -1, -1, -1, -1, -1 )
    myBotList.append( myBot )
    
    myBot2 = MyBot( 2, BROWN, -1, -1, -1, -1, -1, -1, -1 )
    myBotList.append( myBot2 )
    
    myBot3 = MyBot( 3, YELLOW, -1, -1, -1, -1, -1, -1, -1 )
    myBotList.append( myBot3 )
    
    myBot4 = MyBot( 4, ORANGE, -1, -1, -1, -1, -1, -1, -1 )
    myBotList.append( myBot4 )
    
    color_mask_list.append( BLUE )
	color_mask_list.append( RED )
#color_mask_list.append( YELLOW )

#color_mask_list.append( ORANGE )
    
    print color_mask_list
    
    print myBotList[0]

###############################################################################################
##  DEBUG, write a bitmap to a file
###############################################################################################
def writeImageToFile( filename, image, height, width ):
    print "WriteImageToFile : " + filename
    
    imageFile = open( filename, "w" )
    
    for h in xrange( height ):
        for w in xrange( width ):
            imageFile.write( '%02X' % ord(image[h*(width * 3) + (w*3) + 0]) )
            imageFile.write( " " )
            imageFile.write( '%02X' % ord(image[h*(width * 3) + (w*3) + 1]) )
            imageFile.write( " " )
            imageFile.write( '%02X' % ord(image[h*(width * 3) + (w*3) + 2]) )
            imageFile.write( "(" + str(h) + "-" + str(w) + ")," )
            imageFile.write( "  " )
        imageFile.write( "\n" )
    
    imageFile.close()

###############################################################################################
##  DEBUG, write a point cloud to a file
###############################################################################################
def writePointCloudToFile( filename, pointCloud ):
    print "WritePointCloudToFile : " + filename
    
    pointCloudFile = open( filename, "w" )
    
    i=0
    iteration1 = next(pointCloud) #format x,y,z,rgba
    while iteration1 != None:
        pointCloudFile.write( str(i) + " : " )
        #pointCloudFile.write( '%03d, %03d, %03d, %08X' % iteration1.x, iteration1.y, iteration1.z, iteration1.rgba )
        iteration1 = next(pointCloud)
        i=i+1

###############################################################################################
##  Bot location list management
###############################################################################################
def clearBotListMessage( ll ):
    
    del ll
    ll = []
    
    return 0

def addBotToListMessage( ll, botNum, x, y, theta ):
    
    bot = Location()
    
    bot.robot_num = botNum
    bot.x = x
    bot.y = y
    bot.theta = theta
    
    ll.append( bot )
    
    return 0

def postBotList( locationList ):
    global locpub
    
    print "Publish locationList"
    locpub.publish( locationList )
    
    return 0

def compareRGBValues( constVal, image, imagePos ):
    #print constVal
    #print "compareRGBValues %02X " % ord(image[imagePos])
    #print "compareRGBValues : %02X %02X %02X   %d" % ord(image[imagePos]), ord(image[imagePos + 1]), ord(image[imagePos+2]), imagePos
    matches = 0
    
    #if ( ord(image[imagePos + 0]) == 0xDF ):
    #        print str('%02X' % ord(image[imagePos]))
    #       matches = 1
    if ( constVal[0] == ord(image[imagePos + 0]) \
        and constVal[1] == ord(image[imagePos + 1]) \
        and constVal[2] == ord(image[imagePos + 2]) ):
        matches = 1
        
    return matches

###############################################################################################
###############################################################################################
# This takes the min x, y and the max x, y.  The other option would be to splice the array
# before passing it in, but then you would have to figure out how to square it off...
def findCenter( image, imageWidth, imageHeight, targetColor, minXToCheck, maxXToCheck, minYToCheck, maxYToCheck ):
    print "findCenter imageWidth=" + str(imageWidth) + ", imageHeight=" + str(imageHeight) + ", targetColor=" + str(targetColor) \
        + ", minXToCheck=" + str(minXToCheck) + ", maxXToCheck=" + str(maxXToCheck) + ", minYToCheck=" + str(minYToCheck)      \
        + ", maxYToCheck=" + str(maxYToCheck)
    xMin = imageWidth   # assumption is the object will not be found
    xMax = 0
    yMin = imageHeight  # assumption is the object will not be found
    yMax = 0
    
    # TODO: Change the loop ranges to only check necessary cells
    for h in xrange( max( 0, minYToCheck ), min( imageHeight, maxYToCheck ) ):
        for w in xrange( max( 0, minXToCheck ), min( imageWidth, maxXToCheck ) ):
            #print "h=" + str(h) + " w=" + str(w)
            # TODO:  Call checkPixelForColor instead
            #        width and w will need multiplied by the number of bytes per pixel (3 - RGB)
            if ( compareRGBValues( targetColor, image, h*(imageWidth*3) + (w*3) ) == 1 ):
                if ( w < xMin ):
                    xMin = w
                if ( w > xMax ):
                    xMax = w
                
                # The following check is not really needed, once a y is found it will be the min forever
                if ( h < yMin ):
                    yMin = h
                if ( h > yMax ):
                    yMax = h
    
    if ( xMin == imageWidth ):
        print "sticky not found!"
        return -1, -1
    else:
        print "findCenter xMin=" + str(xMin) + " xMax=" + str(xMax) + " yMin=" + str(yMin) + " yMax=" + str(yMax)
        return ( ( ( xMax - xMin ) / 2 ) + xMin), ( ( ( yMax - yMin )  / 2 ) + yMin )

###############################################################################################
###############################################################################################
# Calculate theta given two points (2D)
# Bot position is the center of the two points
def calculatePosAndTheta( startX, startY, endX, endY ):
    print "calculatePosAndTheta => startX=" + str(startX) + " startY=" + str(startY) \
        + " endX=" + str(endX) + " endY=" + str(endY)
    
    if ( ( endY - startY ) == 0 ):
        if ( endX < startX ):
            theta = -1
        else:
            theta = 0
    else:
        theta = math.atan( ( endX - startX ) / ( 1.0 * ( startY - endY ) ) )
    
    print "theta=" + str(theta)
    print "degrees =" + str(180 * theta / math.pi)
    
    return ( min( startX, endX ) + abs( ( startX - endX ) / 2 ) ), \
        ( min( startY, endY ) + abs( ( startY - endY ) / 2 ) ), \
        theta

###############################################################################################
###############################################################################################
def findBots( image, tag, imageWidth, imageHeight, xOffset, yOffset ):
    global myBotList
    global myLocationList
    
    
    for idx, bot in enumerate( myBotList ):
        print ""
        print ""
        if ( bot.currentIDX == -1 ):
            print "Find the bot " + str(bot.ID)
            ( bot.currentIDX, bot.currentIDY )   = findCenter( image, imageWidth, imageHeight, bot.RGBCode, 0, imageWidth - 1, 0, imageHeight - 1 )
            print "Find Result = " + str(bot.currentIDX)
            if ( bot.currentIDX != -1 ):
                ( bot.currentDirX, bot.currentDirY ) = findCenter( image, imageWidth, imageHeight, DIRECTION_COLOR, bot.currentIDX-10, bot.currentIDX+10, bot.currentIDY -3, bot.currentIDY + 10 )
                if ( bot.currentDirX != -1 ):
                    ( bot.posX, bot.posY, bot.currentTheta ) = calculatePosAndTheta( bot.currentIDX, bot.currentIDY, bot.currentDirX, bot.currentDirY )
                    print "Image=" + tag + " ID=" + str(bot.ID) + " RGBCode=" + str(bot.RGBCode)                              \
                        + " currentIDX=" + str(bot.currentIDX) + " currentIDY=" + str(bot.currentIDY)     \
                        + " currentDirX=" + str(bot.currentDirX) + " currentDirY=" + str(bot.currentDirY) \
                        + " currentTheta=" + str(bot.currentTheta)
                addBotToListMessage( myLocationList, bot.ID, bot.posX, bot.posY, bot.currentTheta )
    locpub.publish( myLocationList )
    print "Location List : "
    print myLocationList

###############################################################################################
###############################################################################################
def imageCallbackWorker( message, imageOffset, maskPublisher, tag, idx ):
    global color_mask_list
    global top_mask
    global threshold
    global kinect1pub
    global wroteImage
    
    print "imageCallbackWorker => tag=" + str(tag)
    
    #make a new image if you want to view your mask
    top_mask = Image()
    top_mask.height = message.height
    top_mask.width = message.width
    top_mask.encoding = message.encoding
    top_mask.is_bigendian = message.is_bigendian
    top_mask.step = message.step
    if message.encoding == "bgr8": #this is image_color encoding
        byte_array = list(message.data) #convert unit8[] from string to chars
        byte_arrayOut = [0 for x in range(3*message.height*message.width)]
        if ( wroteImage[idx] == 0 ):
            writeImageToFile( "../" + tag + "_image.txt", byte_array, message.height, message.width )
        print color_mask_list
        for index in xrange(message.height*message.width): #iterate through
            byte_arrayOut[3*index] = chr(0)
            byte_arrayOut[3*index+1] = chr(0)
            byte_arrayOut[3*index+2] = chr(0)
            for k in xrange(len(color_mask_list)):
                #iterate through color list, if the bytes match, save the color
                #in the mask
                #print str(color_mask_list[k][0]) + " : " + str( ord(byte_array[3*index+0] ) )
                #print str(color_mask_list[k][1]) + " : " + str( ord(byte_array[3*index+1] ) )
                #print str(color_mask_list[k][2]) + " : " + str( ord(byte_array[3*index+2] ) )
                #print ""
                if abs(color_mask_list[k][0] - ord(byte_array[3*index])) < threshold\
                    and abs(color_mask_list[k][1] - ord(byte_array[3*index+1])) < threshold\
                    and abs(color_mask_list[k][2] - ord(byte_array[3*index+2])) < threshold:
                        byte_arrayOut[3*index+0] = chr(color_mask_list[k][0])
                        byte_arrayOut[3*index+1] = chr(color_mask_list[k][1])
                        byte_arrayOut[3*index+2] = chr(color_mask_list[k][2])
        #print "Found Something"
        
        if ( wroteImage[idx] == 0 ):
            writeImageToFile( "../" + tag + "_masked_image.txt", byte_arrayOut, message.height, message.width )
            wroteImage[idx] = 1
        
        #findBots( byte_arrayOut, tag, 640, 480, 0, 0 )
    
    top_mask.data = "".join(byte_arrayOut) #make char[] back into uint8[] string
    maskPublisher.publish(top_mask) #publish the mask for viewing
    print "done - " + tag

###############################################################################################
##  Entry points for each of the individual callbacks(Sensor Image).  These just pass the specific
##  information for the respective sensor off to the imageCallbackWorker
###############################################################################################
def top_image_callback(message):
    imageCallbackWorker( message, 0, kinect1pub, "sensor1", 0 )

def mid_image_callback(message):
    imageCallbackWorker( message, 0, kinect2pub, "sensor2", 1 )

###############################################################################################
###############################################################################################
def cloudCallbackWorker( message, tag, idx ):
    global wrotePointCloud
    
    try:
        #make a generator, skipping points that have no depth, on points in
        # list of uvs (index into image [col,row]) or if empty list, get all pt
        data_out = pc2.read_points(message, field_names=None, skip_nans=True, uvs=[])
        if ( wrotePointCloud[idx] == 0 ):
            writePointCloudToFile( "../" + tag + "_pointcloud.txt", data_out )
            wrotePointCloud[idx] = 1
        
        data_out = pc2.read_points(message, field_names=None, skip_nans=True, uvs=[])
        i=0
        iteration1 = next(data_out) #format x,y,z,rgba
        while iteration1 != None:
            iteration1 = next(data_out)
            i=i+1
    
    except StopIteration:
        print tag + " complete"

###############################################################################################
##  Entry points for each of the individual callbacks(PointCloud2).  These just pass the specific
##  information for the respective sensor off to the imageCallbackWorker
###############################################################################################
def top_cloud_callback(message):
    cloudCallbackWorker( message, "sensor1", 0 )

def mid_cloud_callback(message):
    cloudCallbackWorker( message, "sensor2", 1 )

###############################################################################################
##  Startup initalialization
###############################################################################################
def initialize():
    global kinect1pub
    global kinect2pub
    global locpub
    global myLocationList
    
    rospy.init_node("localize_mrdata_andy")
    locpub = rospy.Publisher("/mrdata3/location",LocationList ) #publish your locations
    kinect1pub = rospy.Publisher("/mrdata3/mask",Image) #test your mask
    kinect2pub = rospy.Publisher("/mrdata3/mask",Image)
    #rospy.Subscriber("/kinect1/rgb/image_color", Image, top_image_callback)
    #rospy.Subscriber("/kinect1/depth_registered/points", PointCloud2, top_cloud_callback)
    rospy.Subscriber("/kinect2/rgb/image_color", Image, mid_image_callback)
    rospy.Subscriber("/kinect2/depth_registered/points", PointCloud2, mid_cloud_callback)
    
    initializeBotStructures()
    clearBotListMessage( myLocationList )
    
    # TODO: Move this somewhere it makes sense, only adding here to test
    #addBotToListMessage( myLocationList, 1, 1, 1, 0.1 )
    #addBotToListMessage( myLocationList, 2, 2, 2, 0.2 )
    #addBotToListMessage( myLocationList, 3, 3, 3, 0.3 )
    #addBotToListMessage( myLocationList, 4, 5, 4, 0.4 )
    
    #print myLocationList
    
    #print "Post Bot List"
    #rospy.sleep(15)
    #locpub.publish( myLocationList )
    #postBotList( myLocationList )
    #print "Posted Bot List"
    
    
    rospy.spin()

if __name__ == "__main__":
    initialize()

# ON JUMP

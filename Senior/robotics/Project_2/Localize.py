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
kinect2pub = None
kinect3pub = None
top_mask = Image()
mid_mask = Image()

TAG_SIZE = 50
IMAGE_OVERLAP = 50
THETA_ADJUST = math.pi / 2

###############################################################################################
##  List of bot ("Location") objects to publish
###############################################################################################
myLocationList = []

###############################################################################################
#  Global color constants
#  These have variation and appear much brighter on the kinect image then in real life
#  We may have to have a fairly large window for error, or tune it as we go
###############################################################################################
ORANGE = [ 0xCC, 0xFE, 0xF0 ];  ## Actually Yellow
YELLOW = [ 0xFF, 0xFF, 0xCC ];
GREEN = [ 0x6D, 0x78, 0x6F ];
RED = [ 0xAB, 0x98, 0xFF ];
#RED = [ 0xAB, 0x98, 0xE0 ];
BROWN = [ 0x9F, 0x87, 0x88 ];
BLACK = [ 0x84, 0x77, 0x66 ];

#PINK = [ 0xB2, 0xBF, 0xDD ];
PINK = [ 0xB2, 0xAA, 0xEF ];
BLUE = [ 0xE0, 0xBF, 0xB2 ];

DIRECTION_COLOR = PINK

myBotList = []

###############################################################################################
##  We only want to write the image from each sensor 1 time, the following arrays keep track
##  of whether or not the information has been written for each sensor.
###############################################################################################
#wroteImage = [ 0, 0, 0 ]
#wrotePointCloud = [ 0, 0, 0 ]
wroteImage = [ 1, 1, 1 ]
wrotePointCloud = [ 1, 1, 1 ]

###############################################################################################
##  Class used to keep track of information for a given bot
###############################################################################################
class MyBot:
    def __init__( self, ID, RGBCode, currentIDX, currentIDY, currentDirX, currentDirY, posX, posY, currentTheta, IDyOffset, DiryOffset ):
        self.ID = ID
        self.RGBCode = RGBCode
        self.currentIDX = currentIDX
        self.currentIDY = currentIDY
        self.IDyOffset = IDyOffset
        self.DiryOffset = DiryOffset
        self.currentDirX = currentDirX
        self.currentDirY = currentDirY
        self.posX = posX
        self.posY = posY
        self.currentTheta = currentTheta
        self.inList = False
        self.idSensorIdx = -1
        self.dirSensorIdx = -1
    
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
    
    myBot = MyBot( 1, BLUE, -1, -1, -1, -1, -1, -1, -1, -1, -1 )
    myBotList.append( myBot )
    
    #myBot2 = MyBot( 2, YELLOW, -1, -1, -1, -1, -1, -1, -1, -1, -1 )
    #myBotList.append( myBot2 )
    
    #myBot3 = MyBot( 3, YELLOW, -1, -1, -1, -1, -1, -1, -1, -1, -1 )
    #myBotList.append( myBot3 )
    
    #myBot4 = MyBot( 4, RED, -1, -1, -1, -1, -1, -1, -1, -1, -1 )
    #myBotList.append( myBot4 )
    
    color_mask_list.append( BLUE )
    color_mask_list.append( PINK )
    
    
    #color_mask_list.append( BROWN )
    #color_mask_list.append( YELLOW )
    #color_mask_list.append( GREEN )
    #color_mask_list.append( BLACK )
    color_mask_list.append( ORANGE )
    
    print color_mask_list
    
    print myBotList[0]

# overlap is about 75
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
    matches = 0
    
    if ( constVal[0] == ord(image[imagePos + 0]) \
        and constVal[1] == ord(image[imagePos + 1]) \
        and constVal[2] == ord(image[imagePos + 2]) ):
        matches = 1
        
    return matches

###############################################################################################
###############################################################################################
# This takes the min x, y and the max x, y.  The other option would be to splice the array
# before passing it in, but then you would have to figure out how to square it off...
def findCenter( tag, image, imageOverlap, imageWidth, imageHeight, targetColor, minXToCheck, maxXToCheck, minYToCheck, maxYToCheck ):
    print "findCenter " + tag + " imageWidth=" + str(imageWidth) + ", imageHeight=" + str(imageHeight) + ", targetColor=" + str(targetColor) \
        + ", minXToCheck=" + str(minXToCheck) + ", maxXToCheck=" + str(maxXToCheck) + ", minYToCheck=" + str(minYToCheck)      \
        + ", maxYToCheck=" + str(maxYToCheck) + ", imageOverlap=" + str(imageOverlap)
    xMin = imageWidth   # assumption is the object will not be found
    xMax = 0
    yMin = imageHeight  # assumption is the object will not be found
    yMax = 0
    
    # TODO: Change the loop ranges to only check necessary cells
    for h in xrange( max( imageOverlap, minYToCheck ), min( imageHeight, maxYToCheck ) ):
        for w in xrange( max( 0, minXToCheck ), min( imageWidth, maxXToCheck ) ):
            #print "h=" + str(h) + " w=" + str(w)
            # TODO:  Call checkPixelForColor instead
            #        width and w will need multiplied by the number of bytes per pixel (3 - RGB)
            if ( compareRGBValues( targetColor, image, h*(imageWidth*3) + (w*3) ) == 1 ):
                count = 0
                cellsChecked = 0
                for t1 in xrange ( max( 0, h - 4 ), min( h + 4, imageHeight ) ):
                    for t2 in xrange( max( 0, w - 4 ), min( w + 4 , imageWidth ) ):
                        cellsChecked = cellsChecked + 1
                        if ( compareRGBValues( targetColor, image, t1*(imageWidth*3) + (t2*3) ) == 1 ):
                            count = count + 1
                    if ( cellsChecked > 10 and count * 1.0 / cellsChecked > 0.30 ):
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
        print tag + "   findCenter xMin=" + str(xMin) + " xMax=" + str(xMax) + " yMin=" + str(yMin) + " yMax=" + str(yMax) + " imageOverlap=" + str(imageOverlap)
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
        deltaX = startX - endX
        deltaY = startY - endY
        theta = math.atan( abs( deltaX ) / ( 1.0 * abs( deltaY ) ) )
        if ( deltaX < 0 and deltaY < 0 ):
            theta = -1.0* ( math.pi - theta )
        if ( deltaX < 0 and deltaY > 0 ):
            theta = -1.0 * theta
        if ( deltaX > 0 and deltaY < 0 ):
            theta = theta
    
    print "theta=" + str(theta)
    print "degrees =" + str(180 * theta / math.pi)
    #print "adjusted degrees = " + str(180 * ( theta + THETA_ADJUST ) / math.pi)
    
    return ( min( startX, endX ) + abs( ( startX - endX ) / 2 ) ), \
        ( min( startY, endY ) + abs( ( startY - endY ) / 2 ) ), \
        theta

###############################################################################################
###############################################################################################
def findBots( sensorIdx, image, imageOverlap, tag, imageWidth, imageHeight, xOffset, yOffset ):
    global myBotList
    global myLocationList
    
    print "Find Bot Call " +str(imageOverlap) + " " + str(yOffset)
    
    for idx, bot in enumerate( myBotList ):
        print "Find Bot Loop " + str(bot.currentIDX)
        print ""
        
        # If we have not found the bot ID sticky, look for it
        if ( bot.currentIDX == -1 ):
            print "Find the bot ID " + str(bot.ID)
            ( bot.currentIDX, bot.currentIDY ) = findCenter( tag, image, imageOverlap,     \
                                                            imageWidth, imageHeight, \
                                                            bot.RGBCode,             \
                                                            0, imageWidth - 1,       \
                                                            0, imageHeight - 1 )
                                                            if ( bot.currentIDX != -1 ):
                                                                bot.IDyOffset = yOffset - imageOverlap
                                                                bot.idSensorIdx = sensorIdx
                                                            print tag + " Find Result = " + str(bot.currentIDX) + " " + str( bot.IDyOffset )
        
        # If we have found the ID stick but have not found the direction sticky, look for the direction sticky
        # TBD, account for the fact that it may be in the "other" image
        if ( ( bot.currentIDX != -1 ) and ( bot.currentDirX == -1 ) ):
            print tag + " Find the bot DIR " + str(bot.ID)
            yMin = max( 0, bot.currentIDY - TAG_SIZE )
            yMax = min( imageHeight, bot.currentIDY + TAG_SIZE )
            
            print str( yOffset ) + " " + str( bot.IDyOffset ) + " " + str(bot.currentIDY)
            if ( yOffset == 0 and bot.IDyOffset != 0 and bot.currentIDY < 100 ):
                yMin = imageHeight - 2 * TAG_SIZE
                yMax = imageHeight
            elif ( yOffset != 0 and bot.IDyOffset == 0 and bot.currentIDY > 425 ):
                yMin = 0
                yMax = 2 * TAG_SIZE
            
            ( bot.currentDirX, bot.currentDirY ) = findCenter( tag, image, imageOverlap, \
                                                              imageWidth, imageHeight, DIRECTION_COLOR, \
                                                              max( 0, bot.currentIDX - TAG_SIZE ), \
                                                              min( imageWidth, bot.currentIDX + TAG_SIZE ), \
                                                              yMin, yMax )
                                                              if ( bot.currentDirX != -1 ):
                                                                  bot.DiryOffset = yOffset - imageOverlap
                                                                  bot.dirSensorIdx = sensorIdx
                                                              print tag + " Find Result = " + str(bot.currentDirX)+ " " + str( bot.DiryOffset )

###############################################################################################
###############################################################################################
def imageCallbackWorker( message, imageOverlap, imageOffsetFactor, maskPublisher, tag, idx ):
    global color_mask_list
    global top_mask
    global threshold
    global kinect1pub
    global wroteImage
    
    print "imageCallbackWorker => tag=" + str(tag)
    
    imageOffset = imageOffsetFactor * message.height
    
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
        
        findBots( idx, byte_arrayOut, imageOverlap, tag, top_mask.width, top_mask.height, 0, imageOffset )
    
    top_mask.data = "".join(byte_arrayOut) #make char[] back into uint8[] string
    maskPublisher.publish(top_mask) #publish the mask for viewing
    print "done - " + tag

###############################################################################################
##  Entry points for each of the individual callbacks(Sensor Image).  These just pass the specific
##  information for the respective sensor off to the imageCallbackWorker
###############################################################################################
def mid_image_callback(message):
    #Anythng found in sensor 3 should be offset by the image height - overlap
    imageCallbackWorker( message, 0, 0, kinect3pub, "sensor3", 2 )

def bottom_image_callback(message):
    imageCallbackWorker( message, IMAGE_OVERLAP, 1, kinect2pub, "sensor2", 1 )

###############################################################################################
###############################################################################################
def cloudCallbackWorker( message, tag, idx ):
    global wrotePointCloud
    
    try:
        for bot in myBotList:
            
            print "CCW " + str(bot.idSensorIdx) + " " + str(idx) + " " + str(bot.currentIDX) + " " + str(bot.inList) + " " + str(bot.currentDirX)
            # If we have the ID and Dir stickies, go ahead and post the bots position
            if ( (bot.idSensorIdx == idx ) and ( bot.currentIDX != -1 ) \
                and ( bot.currentDirX != -1 ) and ( bot.inList == False ) ):
                data_out = pc2.read_points( message, field_names=None, skip_nans=True, uvs=[ [bot.currentIDX, bot.currentIDY] ] )
                iteration1 = next( data_out )
                
                if ( iteration1 != None ):
                    print "PC2 x=" + str( iteration1[0] ) + " y=" + str( iteration1[1] ) + " z=" + str( iteration1[2] ) + " rgba=%08X" % iteration1[3]
                    
                    print "Image=" + tag + " ID=" + str(bot.ID) + " RGBCode=" + str(bot.RGBCode)          \
                        + " currentIDX=" + str(bot.currentIDX) + " currentIDY=" + str(bot.currentIDY)     \
                        + " currentDirX=" + str(bot.currentDirX) + " currentDirY=" + str(bot.currentDirY) \
                        + " currentTheta=" + str(bot.currentTheta)
                    
                    
                    # If we have the ID and Dir stickies, go ahead and post the bots position
                    #if ( ( bot.currentIDX != -1 ) and ( bot.currentDirX != -1 ) and ( bot.inList == False ) ):
                    ( bot.posX, bot.posY, bot.currentTheta ) = calculatePosAndTheta( bot.currentIDX, bot.currentIDY + bot.IDyOffset, \
                                                                                    bot.currentDirX, bot.currentDirY + bot.DiryOffset )
                                                                                    bot.currentTheta = bot.currentTheta
                                                                                    print "Image=" + tag + " ID=" + str(bot.ID) + " RGBCode=" + str(bot.RGBCode)                              \
                                                                                        + " currentIDX=" + str(bot.currentIDX) + " currentIDY=" + str(bot.currentIDY)     \
                                                                                        + " currentDirX=" + str(bot.currentDirX) + " currentDirY=" + str(bot.currentDirY) \
                                                                                        + " currentTheta=" + str(bot.currentTheta)                                       \
                                                                                        + " IDOffset=" + str(bot.IDyOffset) + " DirOffset=" + str(bot.DiryOffset) + " yOffset" + str(bot.IDyOffset) + " DiryOffset=" + str(bot.DiryOffset)
                                                                                    addBotToListMessage( myLocationList, bot.ID, bot.posX, bot.posY, bot.currentTheta )
                                                                                    bot.inList = True
                                                                                    
                                                                                    locpub.publish( myLocationList )
                                                                                    print "Location List : "
                    print myLocationList
    
    except StopIteration:
        print tag + " complete"


# 320, 240 => (0.002465714467689395, 0.002465714467689395, 2.5890002250671387, 1.1151704137509542e-38)
# 100, 100 => (-1.115896224975586, -0.7091915011405945, 2.6690001487731934, 1.2075812830583972e-38)
# 50,  50  => (-1.3700867891311646, -0.9633819460868835, 2.6690001487731934, 1.0784723687881373e-38)

###############################################################################################
##  Entry points for each of the individual callbacks(PointCloud2).  These just pass the specific
##  information for the respective sensor off to the imageCallbackWorker
###############################################################################################
def mid_cloud_callback(message):
    cloudCallbackWorker( message, "sensor3", 2 )

def bottom_cloud_callback(message):
    print "PC callback 2"
    cloudCallbackWorker( message, "sensor2", 1 )

###############################################################################################
##  Startup initalialization
###############################################################################################
def initialize():
    global kinect2pub
    global kinect3pub
    global locpub
    global myLocationList
    
    rospy.init_node("localize_mrdata8")
    locpub = rospy.Publisher("/mrdata99/location",LocationList ) #publish your locations
    kinect2pub = rospy.Publisher("/mrdata3/mask2",Image) #test your mask
    kinect3pub = rospy.Publisher("/mrdata3/mask3",Image)
    rospy.Subscriber("/kinect3/rgb/image_color", Image, mid_image_callback)
    rospy.Subscriber("/kinect3/depth_registered/points", PointCloud2, mid_cloud_callback)
    rospy.Subscriber("/kinect2/rgb/image_color", Image, bottom_image_callback)
    rospy.Subscriber("/kinect2/depth_registered/points", PointCloud2, bottom_cloud_callback)
    
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
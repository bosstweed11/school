/**************************************
FILE: CS1566 Readme.txt
AUTHOR: Andrew Beers
EMAIL: acb75@pitt.edu
PLATFORM: macosx
HOMEWORK: 2
**************************************/


/* Under 'platform' above, let us know on what departmental
machine your homework will build and run, by choosing one 
of the below, as applicable: 
linux  macosx windowsvs windowscyg*/


Basic functionalities that my program is providing:
-ability to draw a sphere
-ability to draw a cylinder
-ability to draw a cone
-ability to draw a torus
-ability to change the tessellation in all of the above shapes
-ability to rotate each of the above shapes
-ability to draw normals on all above shapes




Known bugs (if any):
-normals are not perfect, some of them don't point out correctly (please check my math lines 468-542)
-when testing the normals, if you change the tessellation, rotate the shape to get the correct normal calculations again, as they will not be correct during the tessellations


Extra credit (describe what you did, if anything): N/A



Comments:  
-This code is in plain c which will be painful to read, i did my best to comment and display what i was doing, i will probably port this to a cpp program with objects after just for reusability but this is just how i ended up doing it with the time provided.

-The cylinder normals originally pointed inside the cylinder, i don't expect points from this, i just subtracted the normal in this case for visibility purposes in the program(and my own sanity)



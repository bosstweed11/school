// CS 401 Fall 2011
// Tree class as another implementation of the MyShape interface.
// This class also uses composition, with 2 Polygons being the primary
// components of a Tree object.  For more information on Polygons, see
// the Java API.

import java.awt.*;
import java.awt.geom.*;
import java.util.*;

class Snowman implements MyShape
{
	// Represent a Tree in two parts -- a Polygon for the top part 
	// (the branches) and another Polygon for the trunk.  Since the
	// trunk is rectangular, a Rectangle2D could have been used, but
	// to keep consistent (especially with the move() method) I used
	// Polygon objects for both.
	

	// X, Y and size instance variables
	private int X, Y;
	private int size;

	private boolean isHighlighted;
	
	// Create a new Tree object.  Note how the Polygons are built,
	// adding one point at a time to each.  If you plot the points out
	// on paper you will see how the shapes are formed.
	public Snowman(int startX, int startY, int sz)
	{
		X = startX;
		Y = startY;
		size = sz;
		
	}

	public void highlight(boolean b)
	{
		isHighlighted = b;
	}

	// The Polygon class can also be drawn with a predefined method in
	// the Graphics2D class.  There are two versions of this method:
	// 1) draw() which only draws the outline of the shape
	// 2) fill() which draws a solid shape
	// In this class the draw() method is used when the object is
	// highlighted.
	public void draw(Graphics2D g)
	{
		g.setColor(Color.white);
		if (isHighlighted)
			g.drawOval(X,Y,size,size);
		else
			g.fillOval(X,Y,size,size);
		g.setColor(Color.white);
		if (isHighlighted)
			g.drawOval(X+11*size/60,Y-size/2,2*size/3,2*size/3);
		else
			g.fillOval(X+11*size/60,Y-size/2,2*size/3,2*size/3);
		g.setColor(Color.white);
		if (isHighlighted)
			g.drawOval(X+204*size/768,Y-9*size/10,size/2,size/2);
		else
			g.fillOval(X+204*size/768,Y-9*size/10,size/2,size/2);
		
	}

	// Looking at the API, we see that Polygon has a translate() method
	// which can be useful to us.  All we have to do is calculate the
	// difference of the new (x,y) and the old (X,Y) and then call
	// translate() for both parts of the tree.
	public void move(int x, int y)
	{
		int deltaX = x - X;
		int deltaY = y - Y;
		
		X = x;
		Y = y;
	}

	// Polygon also has a contains() method, so this method is also
	// simple
	public boolean contains(double x, double y)
	{
		if(x<=X+size && y<=Y+size && y>=Y && x>=X)
			return true;
		if(x<=X+2*size/3 && y<=Y+2*size/3 && y>=Y-size/2 && x>=X+11*size/60)
			return true;
		if(x<=X+size/2 && y<=Y+size/2 && y>=Y-9*size/10 && x>=X+204*size/768)
			return true;
		return false;
	}

	// The move() method for the Polygons that are in Tree are not
	// reconfigured like in Snowflake, so we can't use the trick used
	// there.  Instead, we just create new Polygons with the new size.
	// The old ones will be garbage collected by the system.
	public void resize(int newsize)
	{
		size = newsize;
		
	}

	// Note again the format
	public String saveData()
	{
		return ("Snowman:" + X + ":" + Y + ":" + size);
	}
}
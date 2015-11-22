// CS 401 Fall 2011
// Tree class as another implementation of the MyShape interface.
// This class also uses composition, with 2 Polygons being the primary
// components of a Tree object.  For more information on Polygons, see
// the Java API.

import java.awt.*;
import java.awt.geom.*;
import java.util.*;

class House implements MyShape
{
	// Represent a Tree in two parts -- a Polygon for the top part 
	// (the branches) and another Polygon for the trunk.  Since the
	// trunk is rectangular, a Rectangle2D could have been used, but
	// to keep consistent (especially with the move() method) I used
	// Polygon objects for both.

	private Polygon box,roof,door;

	// X, Y and size instance variables
	private int X, Y;
	private int size;

	private boolean isHighlighted;
	
	// Create a new Tree object.  Note how the Polygons are built,
	// adding one point at a time to each.  If you plot the points out
	// on paper you will see how the shapes are formed.
	public House(int startX, int startY, int sz)
	{
		X = startX;
		Y = startY;
		size = sz;

		box = new Polygon();
		box.addPoint(X-size/4,Y);
		box.addPoint(X-3*size/2,Y);
		box.addPoint(X-3*size/2,Y+size);
		box.addPoint(X-size/4,Y+size);
		door = new Polygon();
		door.addPoint(X-size/2,Y+size);
		door.addPoint(X-5*size/4,Y+size);
		door.addPoint(X-5*size/4,Y+3*size/5);
		door.addPoint(X-size/2,Y+3*size/5);
		roof = new Polygon();
		roof.addPoint(X,Y);
		roof.addPoint(X-7*size/4,Y);
		roof.addPoint(X-7*size/8,Y-size);
		
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
		g.setColor(Color.red);
		if (isHighlighted)
			g.draw(box);
		else
			g.fill(box);
		g.setColor(Color.gray);
		if (isHighlighted)
			g.draw(door);
		else
			g.fill(door);
		g.setColor(Color.black);
		if (isHighlighted)
			g.draw(roof);
		else
			g.fill(roof);
		
			
	}

	// Looking at the API, we see that Polygon has a translate() method
	// which can be useful to us.  All we have to do is calculate the
	// difference of the new (x,y) and the old (X,Y) and then call
	// translate() for both parts of the tree.
	public void move(int x, int y)
	{
		int deltaX = x - X;
		int deltaY = y - Y;
		
		box.translate(deltaX, deltaY);
		door.translate(deltaX, deltaY);
		roof.translate(deltaX, deltaY);
		X = x;
		Y = y;
	}

	// Polygon also has a contains() method, so this method is also
	// simple
	public boolean contains(double x, double y)
	{
		
		if (door.contains(x,y))
			return true;
		if (box.contains(x,y))
			return true;
		if (roof.contains(x,y))
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
		box = new Polygon();
		box.addPoint(X-size/4,Y);
		box.addPoint(X-3*size/2,Y);
		box.addPoint(X-3*size/2,Y+size);
		box.addPoint(X-size/4,Y+size);
		door = new Polygon();
		door.addPoint(X-size/2,Y+size);
		door.addPoint(X-5*size/4,Y+size);
		door.addPoint(X-5*size/4,Y+3*size/5);
		door.addPoint(X-size/2,Y+3*size/5);
		roof = new Polygon();
		roof.addPoint(X,Y);
		roof.addPoint(X-7*size/4,Y);
		roof.addPoint(X-7*size/8,Y-size);
	}

	// Note again the format
	public String saveData()
	{
		return ("House:" + X + ":" + Y + ":" + size);
	}
}
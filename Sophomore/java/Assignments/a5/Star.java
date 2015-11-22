// CS 401 Fall 2011
// Tree class as another implementation of the MyShape interface.
// This class also uses composition, with 2 Polygons being the primary
// components of a Tree object.  For more information on Polygons, see
// the Java API.

import java.awt.*;
import java.awt.geom.*;
import java.util.*;

class Star implements MyShape
{
	// Represent a Tree in two parts -- a Polygon for the top part 
	// (the branches) and another Polygon for the trunk.  Since the
	// trunk is rectangular, a Rectangle2D could have been used, but
	// to keep consistent (especially with the move() method) I used
	// Polygon objects for both.

	private Polygon sq,top,right,left,bright,bleft;

	// X, Y and size instance variables
	private int X, Y;
	private int size;

	private boolean isHighlighted;
	
	// Create a new Tree object.  Note how the Polygons are built,
	// adding one point at a time to each.  If you plot the points out
	// on paper you will see how the shapes are formed.
	public Star(int startX, int startY, int sz)
	{
		X = startX;
		Y = startY;
		size = sz;

		sq = new Polygon();
		sq.addPoint(X-size/2,Y-size/3);
		sq.addPoint(X-size/4,Y-size/2);
		sq.addPoint(X-size/3,Y-2*size/3);
		sq.addPoint(X-2*size/3,Y-2*size/3);
		sq.addPoint(X-3*size/4,Y-size/2);
		top = new Polygon();
		top.addPoint(X-size/3,Y-2*size/3);
		top.addPoint(X-2*size/3,Y-2*size/3);
		top.addPoint(X-size/2,Y-size);
		
		right = new Polygon();
		right.addPoint(X-size/4,Y-size/2);
		right.addPoint(X-size/3,Y-2*size/3);
		right.addPoint(X+size/5,Y-2*size/3);
		
		bright = new Polygon();
		bright.addPoint(X-size/2,Y-size/3);
		bright.addPoint(X-size/4,Y-size/2);
		bright.addPoint(X,Y);
		
		left = new Polygon();
		left.addPoint(X-2*size/3,Y-2*size/3);
		left.addPoint(X-3*size/4,Y-size/2);
		left.addPoint(X-6*size/5,Y-2*size/3);
		
		bleft = new Polygon();
		bleft.addPoint(X-3*size/4,Y-size/2);
		bleft.addPoint(X-size/2,Y-size/3);
		bleft.addPoint(X-size,Y);
		
		
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
		g.setColor(Color.yellow);
		if (isHighlighted)
			g.draw(sq);
		else
			g.fill(sq);
		g.setColor(Color.yellow);
		if (isHighlighted)
			g.draw(top);
		else
			g.fill(top);
		g.setColor(Color.yellow);
		if (isHighlighted)
			g.draw(right);
		else
			g.fill(right);
		g.setColor(Color.yellow);
		if (isHighlighted)
			g.draw(bright);
		else
			g.fill(bright);
		g.setColor(Color.yellow);
		if (isHighlighted)
			g.draw(left);
		else
			g.fill(left);
		g.setColor(Color.yellow);
		if (isHighlighted)
			g.draw(bleft);
		else
			g.fill(bleft);
	}

	// Looking at the API, we see that Polygon has a translate() method
	// which can be useful to us.  All we have to do is calculate the
	// difference of the new (x,y) and the old (X,Y) and then call
	// translate() for both parts of the tree.
	public void move(int x, int y)
	{
		int deltaX = x - X;
		int deltaY = y - Y;
		
		sq.translate(deltaX, deltaY);
		top.translate(deltaX,deltaY);
		right.translate(deltaX,deltaY);
		bright.translate(deltaX,deltaY);
		left.translate(deltaX,deltaY);
		bleft.translate(deltaX,deltaY);
		
		X = x;
		Y = y;
	}

	// Polygon also has a contains() method, so this method is also
	// simple
	public boolean contains(double x, double y)
	{
		
		if (sq.contains(x,y))
			return true;
		else if(top.contains(x,y))
			return true;
		else if(right.contains(x,y))
			return true;
		else if(bright.contains(x,y))
			return true;
		else if(left.contains(x,y))
			return true;
		else if(bleft.contains(x,y))
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
		sq = new Polygon();
		sq.addPoint(X-size/2,Y-size/3);
		sq.addPoint(X-size/4,Y-size/2);
		sq.addPoint(X-size/3,Y-2*size/3);
		sq.addPoint(X-2*size/3,Y-2*size/3);
		sq.addPoint(X-3*size/4,Y-size/2);
		top = new Polygon();
		top.addPoint(X-size/3,Y-2*size/3);
		top.addPoint(X-2*size/3,Y-2*size/3);
		top.addPoint(X-size/2,Y-size);
		
		right = new Polygon();
		right.addPoint(X-size/4,Y-size/2);
		right.addPoint(X-size/3,Y-2*size/3);
		right.addPoint(X+size/5,Y-2*size/3);
		
		bright = new Polygon();
		bright.addPoint(X-size/2,Y-size/3);
		bright.addPoint(X-size/4,Y-size/2);
		bright.addPoint(X,Y);
		
		left = new Polygon();
		left.addPoint(X-2*size/3,Y-2*size/3);
		left.addPoint(X-3*size/4,Y-size/2);
		left.addPoint(X-6*size/5,Y-2*size/3);
		
		bleft = new Polygon();
		bleft.addPoint(X-3*size/4,Y-size/2);
		bleft.addPoint(X-size/2,Y-size/3);
		bleft.addPoint(X-size,Y);
	}

	// Note again the format
	public String saveData()
	{
		return ("Star:" + X + ":" + Y + ":" + size);
	}
}
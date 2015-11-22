//Assignment 5 Andrew Beers CS 1501 7/29
import java.util.*;
import java.io.*;
public class Assignment5
{

	public static EdgeWeightedGraph theGraph;
	
	public static void main( String [] args )throws IOException
	{
		Scanner inScan = new Scanner(System.in);
		boolean running = true;
		if ( args.length < 1 )
		{
			System.out.println("Please start the program with an input file.");
		
		}
		else
		{
			In a = new In(args[0]);
			theGraph = new EdgeWeightedGraph(a);
			
		}
		
		//System.out.println("The graph is " + theGraph.toString());
		
		while ( running )
		{
			String s = null;
			
			System.out.println("\nEnter the next command: ");
			
			s = inScan.nextLine();
			
			
			
			//System.out.println("s = " + s );
			char input = s.charAt(0);
			
			if ( input == 'R')
			{
				displayReport();
			
			}
			else if ( input == 'M')
			{
			
				displayMinSpanningTree();
				
			}
			else if ( input == 'S')
			{
				
				String[] inputString = new String[3];
				inputString = s.split("\\s+");
				
				int vert1 = Integer.parseInt( inputString[1] );
				
				int vert2 = Integer.parseInt( inputString[2] );

				displayShortestPath(vert1,vert2);
			
			}
			else if ( input == 'P')
			{
				String[] inputString = new String[4];
				inputString = s.split("\\s+");
				
				int vert1 = Integer.parseInt( inputString[1] );
				
				int vert2 = Integer.parseInt( inputString[2] );
				
				int maxWeight = Integer.parseInt( inputString[3] );
				
				displayDistinctPaths( vert1, vert2, maxWeight);

				
			
			}
			else if ( input == 'D')
			{
				String[] inputString = new String[2];
				inputString = s.split("\\s+");
				
				int vert1 = Integer.parseInt( inputString[1] );
		
				downNode( vert1 );
				
			
			}
			else if ( input == 'U')
			{
			
				String[] inputString = new String[2];
				inputString = s.split("\\s+");
				if ( inputString[1] != null)
				{
					int vert1 = Integer.parseInt( inputString[1] );
		
					upNode( vert1 );
				}
				else
				{
					System.out.println("To bring a node up type: U [vertex#].");
				}
			
			}
			else if ( input == 'C')
			{
				String[] inputString = new String[4];
				inputString = s.split("\\s+");
				
				if ( inputString[3] != null )
				{
				
					int vert1 = Integer.parseInt( inputString[1] );
				
					int vert2 = Integer.parseInt( inputString[2] );
				
					double maxWeight = Double.parseDouble( inputString[3] );
				
					changeEdgeWeight( vert1, vert2, maxWeight);
				}
				else
				{
					System.out.println("To bring a node up type: U [vertex#].");

				}

			
			}
			else if ( input == 'Q')
			{
			
				running = false;
			}
			else
			{
				System.out.println("\nPlease enter one of the supported inputs.\n");
			
			}
			
		}
		
		System.out.println("Goodbye");
		
	
	}
	
	public static void displayReport()
	{
	
		//display report
		//all active nodes-edges-edge weights
		//connected or not
		//connected components of the graph
		
		LazyPrimMSTTrace prim = new LazyPrimMSTTrace(theGraph);
		int count = 1;
		int V = theGraph.V;
        for (Edge e : prim.edges())
        {
        	count++;
        }
     
        if ( count  < (V - theGraph.numDown) )
        {
        	System.out.println("The graph is not connected.\n");
        }
        else
        {
        	System.out.println("The graph is connected.\n");
        }
		
		System.out.println(theGraph.toString());
		
		
	
	}
	
	public static void displayMinSpanningTree()
	{
	
		LazyPrimMSTTrace prim = new LazyPrimMSTTrace(theGraph);
		StdOut.println("Total Weight = " + prim.weight());
        for (Edge e : prim.edges())
            StdOut.println(e);
	
	}
	
	public static void displayShortestPath( int v1, int v2)
	{
	
		if ( theGraph.adj(v1) == null || theGraph.adj(v2) == null )
		{
			System.out.println("At least one of the vertices are currently down.");
		}
		else
		{
			DijkstraSP path = new DijkstraSP(theGraph, v1 );
			StdOut.println();


			// print shortest path
			StdOut.println("Shortest path from (reverse order)" + v1 + " - " + v2);
			if (path.hasPathTo(v2)) 
			{
				StdOut.printf("%d to %d (%.2f)  \n", v1, v2, path.distTo(v2));
				for (Edge e : path.pathTo(v2)) 
				{
					StdOut.println(e);
				}
				StdOut.println();
			}
			else 
			{
				StdOut.printf("%d to %d         no path\n", v1, v2);
			}
		}
	
	
	}
	
	public static void displayDistinctPaths( int v1, int v2, double w )
	{
	
		//display all paths from v1-v2 under weight w
		
		theGraph.allPathsInit(v1, 0.0, null, w, v2);
	
	}
	
	public static void downNode( int v1 )
	{
	
		//bring node down
		theGraph.downVert(v1);
	
	}
	
	public static void upNode( int v2 )
	{
	
		//bring node up
		theGraph.upVert(v2);
	
	}
	
	public static void changeEdgeWeight( int v1, int v2, double w )
	{
	
		//change weight of edge v1-v2 and v2-v1 to w
		
		theGraph.changeWeight(v1,v2,w);
	
	}
	
	
}
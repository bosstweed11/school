/*************************************************************************
 *  Compilation:  javac EdgeWeightedGraph.java
 *  Execution:    java EdgeWeightedGraph V E
 *  Dependencies: Bag.java Edge.java
 *
 *  An edge-weighted undirected graph, implemented using adjacency lists.
 *  Parallel edges and self-loops are permitted.
 *
 *************************************************************************/

/**
 *  The <tt>EdgeWeightedGraph</tt> class represents an undirected graph of vertices
 *  named 0 through V-1, where each edge has a real-valued weight.
 *  It supports the following operations: add an edge to the graph,
 *  in the graph, iterate over all of the neighbors incident to a vertex.
 *  Parallel edges and self-loops are permitted.
 *  <p>
 *  For additional documentation, see <a href="http://algs4.cs.princeton.edu/43mst">Section 4.3</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 */

import java.util.Iterator;
public class EdgeWeightedGraph {
    public final int V;
    private int E;
    public Bag<Edge>[] adj;
    private Bag<Edge>[] down;
    public int numDown = 0;
    public String path = "";

    
   /**
     * Create an empty edge-weighted graph with V vertices.
     */
    public EdgeWeightedGraph(int V) {
        if (V < 0) throw new RuntimeException("Number of vertices must be nonnegative");
        this.V = V;
        this.E = 0;
        adj = (Bag<Edge>[]) new Bag[V];
        down = (Bag<Edge>[]) new Bag[V]; 
        for (int v = 0; v < V; v++) adj[v] = new Bag<Edge>();
    }

   /**
     * Create a random edge-weighted graph with V vertices and E edges.
     * The expected running time is proportional to V + E.
     */
    public EdgeWeightedGraph(int V, int E) {
        this(V);
        if (E < 0) throw new RuntimeException("Number of edges must be nonnegative");
        for (int i = 0; i < E; i++) {
            int v = (int) (Math.random() * V);
            int w = (int) (Math.random() * V);
            double weight = Math.round(100 * Math.random()) / 100.0;
            Edge e = new Edge(v, w, weight);
            addEdge(e);
        }
    }

   /**
     * Create a weighted graph from input stream.
     */
    public EdgeWeightedGraph(In in) {
        this(in.readInt());
        int E = in.readInt();
        for (int i = 0; i < E; i++) {
            int v = in.readInt();
            int w = in.readInt();
            double weight = in.readDouble();
            Edge e = new Edge(v, w, weight);
            addEdge(e);
        }
    }

   /**
     * Return the number of vertices in this graph.
     */
    public int V() {
        return V;
    }

   /**
     * Return the number of edges in this graph.
     */
    public int E() {
        return E;
    }


   /**
     * Add the edge e to this graph.
     */
    public void addEdge(Edge e) {
        int v = e.either();
        int w = e.other(v);
        adj[v].add(e);
        adj[w].add(e);
        E++;
    }


   /**
     * Return the edges incident to vertex v as an Iterable.
     * To iterate over the edges incident to vertex v, use foreach notation:
     * <tt>for (Edge e : graph.adj(v))</tt>.
     */
    public Iterable<Edge> adj(int v) {
        return adj[v];
    }

   /**
     * Return all edges in this graph as an Iterable.
     * To iterate over the edges, use foreach notation:
     * <tt>for (Edge e : graph.edges())</tt>.
     */
    public Iterable<Edge> edges() {
        Bag<Edge> list = new Bag<Edge>();
        for (int v = 0; v < V; v++) {
            int selfLoops = 0;
            for (Edge e : adj(v)) {
                if (e.other(v) > v) {
                    list.add(e);
                }
                // only add one copy of each self loop
                else if (e.other(v) == v) {
                    if (selfLoops % 2 == 0) list.add(e);
                    selfLoops++;
                }
            }
        }
        return list;
    }



   /**
     * Return a string representation of this graph.
     */
    public String toString() 
    {
    	
    	
    	String upNodes = "The following nodes are currently up: ";
    	String downNodes = "The following nodes are currently down: ";
    	boolean downNode = false;
    	boolean upNode = false;
    	System.out.println(upNodes);
    	for( int i = 0; i < this.V; i++ )
    	{
    	
    		if ( adj[i] != null )
    		{
    			System.out.print(i + " ");
    			upNode = true;
    		}
    	
    	}
    	if ( !upNode )
    	{
    		System.out.println("none\n");
    	}
    	else
    	{
    		System.out.println("\n");
    	}
    	
    	System.out.println(downNodes);
    	for( int i = 0; i < this.V; i++ )
    	{
    	
    		if ( adj[i] == null )
    		{
    			System.out.print(i + " ");
    			downNode = true;
    		}
    	
    	}
    	
    	if ( !downNode )
    	{
    		System.out.println("none\n");
    	}
    	else
    	{
    		System.out.println("\n");
    	}
    	
        String NEWLINE = System.getProperty("line.separator");
        StringBuilder s = new StringBuilder();
        for (int v = 0; v < V; v++) 
        {
            s.append("Vertice: " +v + ": \n");
            if (adj[v] == null )
            {
            	s.append("No edges: vertex is down.");
            }
            else
            {
            	
				for (Edge e : adj[v]) 
				{
					s.append("Edge: " + e + "  \n");
				}
			}
            s.append(NEWLINE);
        }
        return s.toString();
    }
    
    public void downVert( int a )
    {
    	System.out.println("Putting down vertex: " + a);
    	
    	Bag<Edge> vertDown = (Bag<Edge>)new Bag();
    	
    	if ( adj[a] !=null )
    	{
			Iterator<Edge> iter = adj[a].iterator();
		
			while ( iter.hasNext() )
			{
				vertDown.add( iter.next() );
		
			}
			down[a] = vertDown;
			adj[a] = null;
			numDown++;
		}
    	/*
    	String NEWLINE = System.getProperty("line.separator");
        StringBuilder s = new StringBuilder();
    	for (int v = 0; v < V; v++) 
        {
            s.append("Vertice: " +v + ": \n");
            if (adj[v] == null )
            {
            	s.append("No edges: vertex is down.");
            }
            else
            {
            	
				for (Edge e : adj[v]) 
				{
					s.append("Edge: " + e + "  \n");
				}
			}
            s.append(NEWLINE);
        }
    	
    	System.out.println(s.toString());
    	*/
    }
    
    public void upVert( int a )
    {
    	System.out.println("Bringing up vertex: " + a);
		Bag<Edge> vertUp = (Bag<Edge>)new Bag();
    	
    	if ( down[a] !=null )
    	{
			Iterator<Edge> iter = down[a].iterator();
		
			while ( iter.hasNext() )
			{
				vertUp.add( iter.next() );
		
			}
			down[a] = null;
			adj[a] = vertUp;
			numDown--;
		}
		/*
		String NEWLINE = System.getProperty("line.separator");
        StringBuilder s = new StringBuilder();
		for (int v = 0; v < V; v++) 
        {
            s.append("Vertice: " +v + ": \n");
            if (adj[v] == null )
            {
            	s.append("No edges: vertex is down.");
            }
            else
            {
            	
				for (Edge e : adj[v]) 
				{
					s.append("Edge: " + e + "  \n");
				}
			}
            s.append(NEWLINE);
        }
        
        System.out.println(s.toString());
        */
    }
    
    public void changeWeight(int v1, int v2, double weight )
    {
    	boolean found = false;
		if ( adj[v1] != null && adj[v2] != null )
		{
			for (Edge e : adj[v1]) 
			{
				int b = e.other(v1);
				
				if ( b == v2 )
				{
					if ( weight <= 0 )
					{
						System.out.println("Removing Edge");
						adj[v1].remove(e);
						adj[v2].remove(e);
					}
					else
					{
						System.out.println("Changing Edge Weight");
						e.setWeight(weight);
					}
					
					found = true;
				}
			}
			
			if ( !found && weight > 0 )
			{
				
				System.out.println("Added New Edge");
				Edge newEdge = new Edge( v1,v2, weight);
				adj[v1].add(newEdge);
				adj[v2].add(newEdge);
			
			}
		
		
		}    
    	
    }
    
    public void allPathsInit( int currVertex, double currWeight, String currentPath, double maxWeight, int endPoint )
    {
    
    	allPaths(currVertex, currWeight, currentPath , maxWeight, endPoint);
    	System.out.println("All distinct paths: ");
    	System.out.println(this.path);
    	this.path = "";
    }

    
    public void allPaths( int currVertex, double currWeight, String currentPath, double maxWeight, int endPoint )
    {
    	boolean repeatedPath = false;
    	if ( currentPath != null)
    	{
			String[] inputString = new String[currentPath.length()];
			inputString = currentPath.split("->");
			//System.out.println("Checking for dupes: " + currentPath + " : " + currVertex);
			for ( int i = 0; i< inputString.length && !repeatedPath; i++ )
			{
				if ( Integer.parseInt( inputString[i] ) == currVertex )
				{
					repeatedPath = true;
					//System.out.println("Found Repeated!\nold-" + currentPath);
					int index = currentPath.length() - 4;
					currentPath = currentPath.substring( 0, index);
					//System.out.println("new-" + currentPath);
				}
			}
			
		}
		
		if ( currWeight > maxWeight || repeatedPath)
		{
			//System.out.println("Backtracking from" + currVertex);
			return;
		}
    	
    	if ( currVertex == endPoint)
    	{
    		path = new String(path + currentPath +"->" + currVertex + " : " + currWeight  +"\n");
    		//System.out.println("Found path: " + path);
    	}
    	else
    	{
    		for (Edge e : adj[currVertex]) 
    		{
    			//System.out.println("continuing to " + e.other(currVertex));
    			if ( currentPath == null )
    			{
    				
    				if ( adj[e.other(currVertex)] != null )
    				{
    					currentPath = currVertex + "";
    					allPaths( e.other(currVertex), currWeight + e.weight(), currentPath, maxWeight, endPoint );
    				}

    			}
    			else
    			{
    				if ( adj[e.other(currVertex)] != null )
    				{
    					allPaths( e.other(currVertex), currWeight + e.weight(), new String( currentPath +"->" + currVertex), maxWeight, endPoint );
    				}
    			}
    		
    		}
    	
    	}
    
    }

   /**
     * Test client.
     */
    public static void main(String[] args) {
        EdgeWeightedGraph G;

        if (args.length == 0) {
            // read graph from stdin
            G = new EdgeWeightedGraph(new In());
        }
        else {
            // random graph with V vertices and E edges, parallel edges allowed
            int V = Integer.parseInt(args[0]);
            int E = Integer.parseInt(args[1]);
            G = new EdgeWeightedGraph(V, E);
        }

        StdOut.println(G);
    }

}
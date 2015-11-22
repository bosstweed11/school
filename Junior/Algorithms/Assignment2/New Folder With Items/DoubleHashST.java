//Assignment 2 Andrew Beers 6/18/13

import java.lang.*;
public class DoubleHashST<Key, Value>  implements STInterface<Key, Value>
{
    private static final int INIT_CAPACITY = 4;

    private int N = 0;           // number of key-value pairs in the symbol table
    private int M;           // size of linear probing table
    private int D;		     // prime number 
    private Key[] keys;      // the keys
    private Value[] vals;    // the values
    private int deleted = 0; 	// count deleted cells
    private int probes = 0;
    private boolean resize = true;


    // create an empty hash table - use 16 as default size
    public DoubleHashST() 
    {
        this(INIT_CAPACITY);
    }

    // create double hashing hash table of given capacity
    public DoubleHashST(int capacity) 
    {
        M = capacity;
        
        boolean foundD = false;
        
        while( !foundD )
        {
        	capacity--;
        	
        	if ( isPrime( capacity ) )
        	{
        		D = capacity;
        		foundD = true;
        	}
        
        }
        keys = (Key[])   new Object[M];
        vals = (Value[]) new Object[M];
    }
    
    //constructor with resize option
    public DoubleHashST(int capacity, boolean resizeOption) 
    {
       	resize = resizeOption;
       	M = capacity;
        
        boolean foundD = false;
        
        while( !foundD )
        {
        	capacity--;
        	
        	if ( isPrime( capacity ) )
        	{
        		D = capacity;
        		foundD = true;
        	}
        
        }
        keys = (Key[])   new Object[M];
        vals = (Value[]) new Object[M];
    }

    // return the number of key-value pairs in the symbol table
    public int size() 
    {
        return N;
    }

    // is the symbol table empty?
    public boolean isEmpty() 
    {
        return size() == 0;
    }

    // does a key-value pair with the given key exist in the symbol table?
    public boolean contains(Key key) 
    {
        return get(key) != null;
    }

    // hash function for keys - returns value between 0 and M-1
    private int hash(Key key) 
    {
        return (key.hashCode() & 0x7fffffff) % M;
    }
    
    //double hashing function
    private int doubleHash(Key key)
    {
    	return this.D - ((key.hashCode() & 0x7fffffff) % this.D);
    }

    // resize the hash table to the given capacity by re-hashing all of the keys
    private void resize(int capacity) 
    {
    	if ( resize )
    	{
			System.out.println("Resizing from M = " + M + " and N = " + N );
			boolean foundD = false;
			int cap = capacity;
			
			//find D value
			while( !foundD )
			{
				cap--;
		
				if ( isPrime( cap ) )
				{
					D = cap;
					foundD = true;
				}
	
			}
			foundD = false;
	
			//find prime capacity
			if ( !isPrime( capacity ) )
			{
				while( !foundD )
				{
					capacity++;
		
					if ( isPrime( capacity ) )
					{
						foundD = true;
					}
	
				}
			}
	
			DoubleHashST<Key, Value> temp = new DoubleHashST<Key, Value>(capacity);
			for (int i = 0; i < M; i++) 
			{
				if (keys[i] != null && !keys[i].equals('&') ) 		//dont rehash ampersands
				{
					temp.put(keys[i], vals[i]);
				}

			}
			keys = temp.keys;
			vals = temp.vals;
			M    = temp.M;
			System.out.println("\tNew Values:  M = " + M + " and D = " + D );
			this.deleted = 0;
		}
    }

    // insert the key-value pair into the symbol table
    public void put(Key key, Value val) 
    {

        if (val == null ) delete(key);

		
        // double table size if 75% full
        if ( N >= ( 3*M/4 ) ) 
        {

        	//System.out.println ( N + " is bigger/equal than 3/4s of " + M + ": resizing\n\n");
        	resize( 2 * M );
        }
    

        int i;
        int j = doubleHash( key );
        for (i = hash(key); keys[i] != null; i = (i + j) % M) 
        {
            if (keys[i].equals(key) && keys[i] == vals[i] ) //this will not find '&' as null so no problems here, we wont put in a deleted place
            { 
            	vals[i] = val; 
            	return; 
            }
        }
        keys[i] = key;
        vals[i] = val;
        N++;
    }

    // return the value associated with the given key, null if no such value
    public Value get(Key key) 
    {
    	int j = doubleHash( key );
        for (int i = hash(key); keys[i] != null; i = ( i + j) % M) //double hash value
        {
        	this.probes++;
            if (keys[i].equals(key) )	// this will find '&' but not quit out, it wont match any get, (shouldnt be searching for strings?chars?)
            {
                return vals[i];
            }
        }
        this.probes++;
        return null;
    }

    // delete the key (and associated value) from the symbol table
    public void delete(Key key) 
    {
        if (!contains(key)) return;

        // find position i of key
        int i = hash(key);
        int j = doubleHash(key);
        while (!key.equals(keys[i])) 
        {
            i = (i + j) % M;
        }

        // delete key and associated value
        keys[i] = (Key) new Character('&'); //mark as deleted
        vals[i] = (Value) new Character('&'); // deleted
        this.deleted++;

        N--;        

		
        // halves size of array if it's 12.5% full or less
        if (N > 0 && N <= M/8)
        {
        	resize(M/2);
        }
        

		//if 25% of table is 'deleted', resize
        if ( this.deleted > M/4 )
        {
        	resize( M );
        }

        assert check();
    }

    // return all of the keys as in Iterable
    public Iterable<Key> keys() 
    {
        Queue<Key> queue = new Queue<Key>();
        for (int i = 0; i < M; i++)
            if (keys[i] != null && !keys[i].equals('&')) queue.enqueue(keys[i]);
        return queue;
    }

    // integrity check - don't check after each put() because
    // integrity not maintained during a delete()
    private boolean check() 
    {
		 // check that hash table is at most 50% full
        if (M < 2*N) 
        {
            System.err.println("Hash table size M = " + M + "; array size N = " + N);
            return false;
        }

        // check that each key in table can be found by get()
        for (int i = 0; i < M; i++) 
        {
            if (keys[i] == null || keys[i].equals('&')) continue;
            else if (get(keys[i]) != vals[i]) 
            {
                System.err.println("get[" + keys[i] + "] = " + get(keys[i]) + "; vals[i] = " + vals[i]);
                return false;
            }
        }
        return true;


    }
    
    //is prime function
    public boolean isPrime( int num )
    {
    	for ( int i = 2; i < num; i++ )
     	{
        	if ( num % i == 0 ) 
        	{
        		return false; 
        	}
     	}
        
        return true;
    }
    
    //probing
    public int getProbes()
    {
    
    	int a = this.probes;
    	this.probes = 0;
    	return a;
    
    }
}
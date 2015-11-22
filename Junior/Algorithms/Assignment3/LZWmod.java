//Assignment 3 CS 1501 7/3/13 Andrew Beers
public class LZWmod 
{
    private static final int R = 256;       // number of input chars
    public static int L = 512;       		// number of codewords = 2^W
    public static int W = 9;         		// codeword width

    public static void compress(char compressChar) 
    { 
        String input = BinaryStdIn.readString();
        TST<Integer> st = new TST<Integer>();
        int charsReadIn =0; 
        double totalWritten=0; //total encoding bits
        double totalInFile;		//size of actual file
        
        //used in 'm' to monitor compressionRatio
        double compressionRatio = 0;	
        double oldRatio=0;
        boolean first = true;		
        
        //load init dictionary
        for (int i = 0; i < R; i++)
        {
            st.put("" + (char) i, i);
        }
        int code = R+1;  // R is codeword for EOF
        
        
		BinaryStdOut.write(compressChar);      // Print s's encoding.
		
        while (input.length() > 0) 
        {
            String s = st.longestPrefixOf(input);  // Find max prefix match s.
            BinaryStdOut.write(st.get(s), W);      // Print s's encoding.
            int t = s.length(); //size of input
            totalWritten += W;
            charsReadIn += t;
            
            if ( t < input.length() ) 
            {   
            	// Add s to symbol table.
            	//if we have room for codewords at this W value
            	if ( code < L)
            	{
                	st.put(input.substring(0, t + 1), code++);
                	System.err.println("Codeword: " + code + "\nString: " + input.substring(0, t + 1)+ "\n");
                }
                else if ( W < 16 )	//if we have room to increase W
                {
                	st.put(input.substring(0, t + 1), code++);
                	System.err.println("NEW L value was gotten was : " + L +"\n");
                	System.err.println("Codeword: " + code + "\nString: " + input.substring(0, t + 1) );

                	W++;
                	L = L*2;
                	System.err.println("NEW L value was gotten new : " + L + " Code: " + code +"\n");
                }
                else	//decide on our coding scheme
                {
                	if ( compressChar == 'n' )
                	{
                		//do nothing
                	}
                	else if ( compressChar == 'r' )
                	{
                		//reset dictionary
                		System.err.println("\nRESET DICTIONARY\n"  );
                		st = new TST<Integer>();
                		for (int i = 0; i < R; i++)
        				{
            				st.put("" + (char) i, i);
        				}
                		W = 9;
                		L = 512;
                		code = 257;
                		st.put(input.substring(0, t + 1), code++);
                		System.err.println("Codeword: " + code + "\nString: " + input.substring(0, t + 1) );
                		
                	}
                	else if ( compressChar == 'm' && first)
                	{
                		//get initial monitoring info
                		totalInFile = charsReadIn*8;
                		
                		compressionRatio = totalInFile / totalWritten;
                		oldRatio = compressionRatio;
                		
                		System.err.println("first\nchars: " + totalInFile + "\n" );
                		System.err.println("bits: " + totalWritten + "\n" );
                		System.err.println("Compression Ratio: " + compressionRatio + "\n" );
                		
                		first = false;
                		
                	}
                	else if ( compressChar == 'm' && !first)
                	{
                		//actively monitor
                		
                		totalInFile = charsReadIn*8;	//get curr bits of file
                		
                		compressionRatio = 1.0*(totalInFile / totalWritten); 	//get curr ratio
                		
                		System.err.println("chars: " + totalInFile + "\n" );
                		System.err.println("bits: " + totalWritten + "\n" );
                		if ( 1.0*(oldRatio / compressionRatio) > 1.1 )	// compare vs old ratio
                		{
                			//reset
                			System.err.println("\nRESET DICTIONARY\n"  );
							st = new TST<Integer>();
							for (int i = 0; i < R; i++)
							{
								st.put("" + (char) i, i);
							}
							W = 9;
							L = 512;
							code = 257;
							st.put(input.substring(0, t + 1), code++);
							System.err.println("Codeword: " + code + "\nString: " + input.substring(0, t + 1) );

                		}
                		
                		System.err.println("Compression Ratio: " + oldRatio / compressionRatio + "\n" );
                	
                		
                	}
                
                
                }
                
            }
            input = input.substring(t);            // Scan past s in input.
        }
        BinaryStdOut.write(R, W);
        BinaryStdOut.close();
    } 


    public static void expand() 
    {
        String[] st = new String[65536]; // init to max value
        int i; // next available codeword value
		boolean resetDict = false;
		
		int charsReadIn = 0; //compression char
        double totalRead=0;	//bits from compressed file read in
        double totalInFile=0;	//bits from actual file outputted
        
        //vars to monitor compression
        double compressionRatio = 0;
        double oldRatio=0;
        boolean first = true;
        
        
        // initialize symbol table with all 1-character strings
        for (i = 0; i < R; i++)
            st[i] = "" + (char) i;
        st[i++] = "";                        // (unused) lookahead for EOF
        
        char expandChar = BinaryStdIn.readChar();
        System.err.println("Character: " + expandChar+"\n");
		
        int codeword = BinaryStdIn.readInt(W);
        totalRead = W + 8;//initial char
        String val = st[codeword];

        while (true) 
        {
            BinaryStdOut.write(val);
            charsReadIn += val.length();
            if ( (expandChar == 'r') && i == 65536 )
            {
            	//if reseting immediately, reset here so we can change W before the read
            	System.err.println("\nRESET DICTIONARY\n"  );
				W = 9;
				resetDict = true;
            	
            }
            
            if ( (expandChar == 'm') && i == 65536 )
            {	
            	//monitor compression here so that we can change W if we need to
            	totalInFile = charsReadIn*8;
            	
            	System.err.println("chars: " + totalInFile + "\n" );
                System.err.println("bits: " + totalRead + "\n" );
            	compressionRatio = 1.0*(totalInFile / totalRead);
            	
            	if ( 1.0*(oldRatio / compressionRatio) > 1.1 )
            	{
            		W = 9;
					resetDict = true;
            	
            	}
            	System.err.println("Compression Ratio: " + oldRatio / compressionRatio + "\n" );
            	
            }
            codeword = BinaryStdIn.readInt(W);
            totalRead += W;	//inc W
            
        	//end case
            if (codeword == R && i != L && (expandChar == 'r') ) 
            {
            	System.err.println("\nFinished: codeword = 256\n"  );
            	break;
            }
            
            
            String s = st[codeword];
            if (i == codeword) s = val + val.charAt(0);   // special case hack
            
            if (i < L  && !resetDict) 
            {	
            	st[i++] = val + s.charAt(0);
            	System.err.println("Codeword: " + i + "\nString: " + val + s.charAt(0)+"\nW:" + W+"\n");
            }
            else if( W < 16 && !resetDict)
            {
            	codeword <<= 1;
            	int bit = BinaryStdIn.readInt(1);
            	if ( bit == 1 )
            	{
            	 	codeword |= 1;
            	}
            	if (codeword == R)
            	{
            		System.err.println("\nFinished: codeword2 = 256\n"  );
            		break;
            	}
            	s = st[codeword];
            	if (i == codeword) s = val + val.charAt(0);   // special case hack
            	 
            
            	st[i++] = val + s.charAt(0);

            	System.err.println("NEW L value was gotten was : " + L +"\n");
            	System.err.println("Codeword: " + i + "\nString: " + val + s.charAt(0)+"\n");
            	W++;
            	L = 2*L;
            	System.err.println("NEW L value was gotten new : " + L +"\n");
            
            }
            else
            {
            	
            	if ( expandChar == 'n' )
				{
					//nothing
				}
				else if ( expandChar == 'r' || ( expandChar == 'm' && resetDict ) )
				{
				
					System.err.println("\nRESET DICTIONARY\n"  );
					st = new String[65536];
					W = 9;
					L = 512;
					for (i = 0; i < R; i++)
           				st[i] = "" + (char) i;
					
					i = 257;
					st[i++] = val + s.charAt(0);
					
					//here i only want 9 bits because the dictionary was reset, but i have 16, so i should give 7 back ? hold 7 ask for 2 next time?
					System.err.println("Codeword: " + i + "\nString: " + val + s.charAt(0)+"\n");
					
					resetDict = false;
					
				
				}
				else if ( expandChar == 'm' && first)
				{
					totalRead -= 16;//This W doesnt count
					totalInFile = charsReadIn*8;
					System.err.println("total in file: " + totalInFile + "\ntotal read: " + totalRead+"\n");
					
					compressionRatio = totalInFile / totalRead;
                	oldRatio = compressionRatio;
					
					System.err.println("first ratio: " + oldRatio+"\n");
					first = false;
					totalRead += 16;
					
				}
				
            
            }
            val = s;
        }
        BinaryStdOut.close();
    }



    public static void main(String[] args) 
    {
        if      (args[0].equals("-")) 
        {
        	
        	compress(args[1].charAt(0));
        }
        else if (args[0].equals("+")) expand();
        else throw new RuntimeException("Illegal command line argument");
    }

}
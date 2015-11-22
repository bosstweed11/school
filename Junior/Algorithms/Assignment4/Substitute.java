
import java.util.*;
import java.io.*;
import java.net.*;

public class Substitute implements SymCipher
{

	public byte[] key = new byte[256];
	

	public Substitute ()
	{	
		
		
		ArrayList<Byte> ObjectKeys = new ArrayList<Byte>(); 
		
		for( int i = 0; i < 256; i++ )
		{
			byte b = (byte) i;
			if (b < 0 )
			{
				b += 256;
			}
			ObjectKeys.add(b);
			
			//System.out.println("Added: " + b );

		}
   
		Collections.shuffle(ObjectKeys);

		for (int i = 0; i < key.length; i++) 
		{
		  	
		  
			key[i] = ObjectKeys.get(i);
		}

		 
		
		//System.out.println("Finished constructing sub");
	
	}
	
	public Substitute ( byte[] inKey )
	{
	
		key = inKey;
	
	}
	
	public byte [] encode(String S)
	{
		int size = S.length();
		byte[] stringArray = new byte[size];
		
		stringArray = S.getBytes();
		
		for (int i = 0; i < size; i++)
		{
			stringArray[i] = key[stringArray[i]];
		
		}
		
		return stringArray;
		
	}
	
	public String decode(byte [] bytes)
	{
		int size = bytes.length;
		int index = 0;
		
		for ( int i = 0; i < size; i++ )
		{
			bytes[i] = findIndexOf(bytes[i]);
			if (bytes[i] < 0 )
			{
				bytes[i] += 256;
			}
		} 
		
		String ans = new String(bytes);
		
		return ans;
	}
	
	public byte [] getKey()
	{
	
		return this.key;
	}
	
	public byte findIndexOf(byte theByte)
	{
		boolean found = false;
		byte index = 0;
		while(!found )
		{
			if (theByte == key[index] )
			{
				found = true;
				index--;
			}
			index++;
		}
	
		return index;
	}
}


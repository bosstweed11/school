
import java.util.*;
import java.io.*;
import java.net.*;

public class Add128 implements SymCipher
{

	public byte[] key = new byte[128];

	public Add128 ()
	{
		Random rand = new Random();
		
		for( int i = 0; i < 128; i++ )
		{
			byte randomNum = (byte) rand.nextInt(256);
			key[i] = randomNum;
			
			if (key[i] < 0 )
			{
				key[i] += 256;
			}
		}
		
		//System.out.println("key array:");
		for( int i = 0; i < 128; i++ )
		{
			//System.out.println("index " + i + ": " + key[i]);
		}
		
		//System.out.println("Finished constructing add");
	}
	
	public Add128 ( byte[] inKey )
	{
		key = inKey;
	
	}
	
	public byte [] encode(String S)
	{
		
		int size = S.length();
		byte[] stringArray = new byte[size];
		
		stringArray = S.getBytes();
		
		for ( int i = 0; i < size; i++ )
		{
			stringArray[i] += key[i % 128] % 256;
		} 
		
		return stringArray;

	}
	
	public String decode(byte [] bytes)
	{
	
		int size = bytes.length;
		int index = 0;
		
		for ( int i = 0; i < size; i++ )
		{
			bytes[i] -= key[i % 128] % 256;
		} 
		
		String ans = new String(bytes);
		
		return ans;
		
	
	}
	
	public byte [] getKey()
	{
	
		return this.key;
	
	}
	
	
}


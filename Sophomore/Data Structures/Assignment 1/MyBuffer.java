//Written by Andrew Beers 1/24/12

import java.io.*;


public class MyBuffer<T> implements Bufferer<T>,SaveRestore,Reverser
{
	private T[] bag;
	private int num,svnum;
	private int initial = 20;
	
	//Constructor
	public MyBuffer(int size)
	{
		num =0;
		bag = (T[]) new Object[size];
		
	}
	
	//produce item
	public boolean produceItem(T item)
	{
		
		boolean check;
		
		//if full dont produce(fixed array)
		if(isFull())
			check = false;
		//else assign parameter to next open bag entry and increment entries
		else
		{
			bag[num]= item;
			num++;
			check = true;
		}
		return check;
		
	}
	//Consume Item
	public T consumeItem()
	{
		//if none left, cant consume
		if(num==0)
			return null;
			
		//otherwise set the entry to consume as the first and shift the bag down, decrement entries
		T temp = bag[0];
		
		for(int i=1;i<bag.length;i++)
		{
			bag[i-1] = bag[i];
			
		}
		num--;
		return temp;
			
	
	}


	//check if bag is full
	public boolean isFull()
	{
		
		if(num == bag.length)
			return true;
		else
			return false;
	
	}
	//check if bag is empty
	public boolean isEmpty()
	{
		
		if(num == 0)
			return true;
		else
			return false;
	
	}
	//return size of bag
	public int size()
	{
		return num;
	
	}
	//clear all entries from bag, making them all null
	public void clear()
	{
		int temp = num;
		
		for(int i =0;i<temp;i++)
		{
			bag[i]=null;
			num--;
		}
		
	}
	//reverse order of the bag
	public void reverse()
	{
		//initialize a new array of the same size
		T[] temp = (T[])new Object[num];
		//copy data
		for(int i=0; i<num;i++)
		{
			temp[i] = bag[i];
		
		}
		//switch data in bag array
		for(int i=0; i<num;i++)
		{
			int k = temp.length-(i+1);
			
			bag[i] = temp[k];
		}
		
	
	}
	// save to file
	public boolean saveToFile(String filename)
	{
	
		
		boolean check = false;
		
		//writing object to file
		try
		{
		
			FileOutputStream sv = new FileOutputStream(filename);
			ObjectOutputStream obj = new ObjectOutputStream(sv);
			for(int i=0;i<num;i++)
			{
				obj.writeObject(bag[i]);
			}
			obj.close();
			sv.close();
			check = true;
		}
		catch(Exception a)
		{
			System.out.println("An error has occurred");
			return check;
		}
		//save number of objects for rewrite
		svnum=num;
		return check;
		
	
	}
	//restore 
	public boolean restoreFromFile(String filename)
	{
		boolean restore = false;
		
		if(isEmpty())
		{
		
			//read objects from file, increment num as you go
			try
			{
				FileInputStream read = new FileInputStream(filename);
				ObjectInputStream in = new ObjectInputStream(read);
				for(int i=0;i<svnum;i++)
				{
					bag[i] = (T) in.readObject();
					num++;
				}
				
					in.close();
					read.close();
					restore = true;
			}
			catch(Exception a)
			{
				System.out.println("An error has occurred");
				return restore;
			}
		}
		
		//clear the bag if for some reason it is not empty
		else
		{
			clear();
			try
			{
				FileInputStream read = new FileInputStream(filename);
				ObjectInputStream in = new ObjectInputStream(read);
				for(int i=0;i<svnum;i++)
				{
					bag[i] = (T) in.readObject();
					num++;
				}
					in.close();
					read.close();
					restore = true;
			}	
			catch(Exception a)
			{
				System.out.println("An error has occurred");
				return restore;
			}
			
		}
		
	return restore;
	}
	
	// to string method
	public String toString()
	{
		String s ="Here is the buffer: \n";
		String a="";
		for(int i=0;i<num;i++)
		{
			a+=bag[i].toString()+" ";
			
		}
		
		return s+a;
	
	}
}
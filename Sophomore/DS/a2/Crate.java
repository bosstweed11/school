//Written by Andrew Beers 2/13/12
public class Crate implements Comparable<Crate>
{
	int date,icount,i=0,count;
	double cost;
	
	
	//constructor
	public Crate(int d,int c, double co)
	{
		i++;
		date = d;
		icount = c;
		cost = co;
		count = icount;
		
	}
	
	//return initial count
	public int getCount()
	{
		return icount;
	}
	
	//set current bananas
	public void setBananas(int c)
	{
		count = c;
	}
	
	//return current bananas
	public int getBananas()
	{
		return count;
	}
	
	//return the expiration date
	public int getDate()
	{
		return date;
	}
	
	//return cost
	public double getCost()
	{
		return cost;
	}
	
	//compareto method
	public int compareTo(Crate rhs)
	{
		if(date >rhs.getDate())
			return 1;
		else if(date < rhs.getDate())
			return -1;
		else
			return 0;
	}



}
//Written by Andrew Beers 1/24/12

public class Fixed extends Product
{
	private double price;
	
	//constructor calls product constructor
	public Fixed (String s, double p)
	{
		super(s);
		price = p;
	
	}
	
	//cost function for price
	public double cost()
	{
		return price;
	
	}
	//to string method
	public String toString()
	{
		String s =super.toString() +" Price: " + cost();
		return s;
	
	}



}
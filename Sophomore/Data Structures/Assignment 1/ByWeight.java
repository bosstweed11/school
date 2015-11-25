//Written by Andrew Beers 1/24/12
public class ByWeight extends Variable
{
	
	private double weight;
	//constructor, call Variable constructor
	public ByWeight(String s, double  p, double w )
	{
		
		super(s,p);
		weight = w;
	}
	
	//cost function
	
	public double cost()
	{
	 	
	 	return super.price()*weight;
	}
	
	//to String method
	public String toString()
	{
		String s=super.toString() + " Weight: " + weight + " oz. Price: " + this.cost();
		return s;
	}
	




}
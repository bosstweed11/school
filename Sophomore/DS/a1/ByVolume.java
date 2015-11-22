//Written by Andrew Beers 1/24/12
public class ByVolume extends Variable
{
	private double volume;
	
	//constructor
	public ByVolume(String s, double  p, double v)
	{
		super(s,p);
		
		volume = v;
	}
	
	//cost function
	public double cost()
	{
	 	
	 	return super.price()*volume;
	}
	//to String method
	public String toString()
	{	
		String s = super.toString()+ " Volume: " + volume + " oz. Price: " + this.cost();
		return s;
	}
}
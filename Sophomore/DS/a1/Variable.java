
//Written by Andrew Beers 1/24/12
public abstract class Variable extends Product
{	
	 private double price;
	 
	 //constructor calls product constructor
	 public Variable(String s, double c)
	 {
	 	super(s);
	 	price =c;
	 	
	 }
	 
	 //return price for byweight and byvolume
	 public double price()
	 {
	 	return price;
	 }
	 //to string method
	 public String toString()
	 {
	 	return super.toString();
	 }
	 
	 
}
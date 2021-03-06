//Written by Andrew Beers 2/13/12
import java.util.*;
import java.io.*;

public class Assignment2
{	

	public static void main(String [] args) throws IOException
	{
		String file,check,check1;
		int num=0,date,count,curr,clock=0,bananas,moves=0,tmoves=0,tnum=0,tcount=0;
		double cost=0,bcost=0,lcost=0,tcost=0,ttcost=0,tbcost=0,tlcost=0,paid=0;
		LinkedStack<Crate> LStack = new LinkedStack<Crate>();
		LinkedStack<Crate> RStack = new LinkedStack<Crate>();
		Crate c,d,e,u=null,s=null,k=null,l=null;

	
	
		Scanner inscan = new Scanner(System.in);
		// read in file from command line
		file = args[0];
		
		Scanner scan = new Scanner(new File(file));
		// main program while loop
		while(scan.hasNextLine())
		{
		String temp =scan.next();
		
			//receive function
			if(temp.equals("receive"))
			{
				bcost=0;
				moves=0;
				lcost=0;
				
				//read in number of crates to receive
				num = scan.nextInt();
				System.out.println("Receiving "+ num + " crates of bananas");
				System.out.println();
				
				//for each crate you receive
				for(int i=0;i<num;i++)
				{
				
					//compare to crate
					if(RStack.peek()!=null)
						k = RStack.peek();
					
						
					//get all info for each crate
					date = scan.nextInt();
					count = scan.nextInt();
					cost = scan.nextDouble();
					bcost +=cost;
					
					// create the crate
					c = new Crate(date,count,cost);
					
					// if first RStack
					if(LStack.peek()==null && RStack.peek()==null)
					{
						RStack.push(c);
						//System.out.println(i+" On Right Stack");
						moves++;
					}
					//if equal to RStack, push
					else if (k.compareTo(c)==0)
					{
						RStack.push(c);
						//System.out.println(i +" On the Right Stack");
						moves++;
					}
					//if greater than RStack
					else if (k.compareTo(c)>0)
					{
						// if empty, push on LStack
						if(LStack.peek()==null)
						{
							LStack.push(c);
						}
						
						// if greater push on rstack
						else if(LStack.peek().getDate()<=date)
						{
							RStack.push(c);
							moves++;
							
						}
						
						//if less than while less than is not null check if less than is greater than, then pop and push
						else if(LStack.peek().getDate()>date)
						{
							if(LStack.peek()!=null)
							{
								while(LStack.peek()!=null && LStack.peek().getDate()>date)
								{
									d=LStack.pop();
									RStack.push(d);
									//System.out.println("Popped and Pushed on Right Stack");	
									moves++;
								}
								
							}
							
								
							LStack.push(c);
							moves++;
							
							//when last, push all on rstack
							if(i==(num-1))
							{
								while(LStack.peek()!=null)
								{
									e=LStack.pop();
									RStack.push(e);
									moves++;
								}
							}
									
						}
						
					}
					//if less than RStack
					else if (k.compareTo(c)<0)
					{
						// if lstack empty, while rstack is less than, pop and push
						if(LStack.peek()==null)
						{
							while(RStack.peek()!=null && RStack.peek().getDate()<=date)
							{
									d=RStack.pop();
									LStack.push(d);
									//System.out.println("Popped and pushed onto Left Stack");
									moves++;
							}
									
							RStack.push(c);
							moves++;
							
							//if last push all on rstack
							if(i==(num-1))
							{
								while(LStack.peek()!=null)
								{
									e=LStack.pop();
									RStack.push(e);
									moves++;
								}
							}
									
						}
						else
						{
							while(RStack.peek()!=null && RStack.peek().getDate()<=date)
							{
									d=RStack.pop();
									LStack.push(d);
									//System.out.println("Popped and pushed onto Left Stack");
									moves++;
							}
							RStack.push(c);
							moves++;
							//System.out.println("Pushed onto Right Stack");
							if(i==(num-1))
							{
								while(LStack.peek()!=null)
								{
									e=LStack.pop();
									RStack.push(e);
									moves++;
								}
							}
						}
					}
					//increase total number of crates
					tcount +=count;
					//if last, push all onto RStack
					if(i==(num-1))
					{
						//if last pop and push all on rstack
						if(LStack.peek()!=null)
						{
							do
							{
								c=LStack.pop();
								RStack.push(c);
								moves++;
							}while(LStack.peek()!=null);
						}
						tnum +=num;
						tmoves+=moves;
						tbcost+=bcost;
						lcost=moves;
						tlcost+=lcost;
						tcost=bcost+lcost;
						ttcost = tbcost + tlcost;
						
					}
				}
	
	
			}
			//use function
			else if(temp.equals("use"))
			{
				//read in number to use
				curr = scan.nextInt();
				
				System.out.println(curr+ " bananas needed for order");
				
				
				//if no crates, no bananas
				if(RStack.peek()==null)
					System.out.println("Store is out of bananas!  The horror!");
				//use loop
				else
				{
					//get a current crate if you dont have one
					if(u==null)
					{
						u= RStack.pop();
						System.out.println("Getting crate: Expires " + u.getDate() + " Start Count " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost() + " from the stack" );
					}		
					//get bananas
					int	 a = u.getBananas();
					//if too many, no bananas
					if(curr > tcount)
					{
						System.out.println("Sorry we don't have that many bananas left");
					}
				
					//if less than crate, take what curr is
					if(curr<a)
					{	
						u.setBananas(a-curr);
						System.out.println(curr + " bananas used from current crate");
					}
					//if greater than, take all from crate, get the next crate
					else if(curr>a)
					{
						//while multiple crates would be used
						do
						{
							curr =curr-u.getBananas();
							System.out.println(u.getBananas() + " bananas used from current crate");
							u = RStack.pop();
							System.out.println("Getting crate: Expires: " + u.getDate() + " Start Count: " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost() + " from the stack" );
						}while(curr>u.getBananas());
						
						//if exact, use all and pop
						if(curr == u.getBananas())
						{
							System.out.println(curr + " bananas used from current crate");
							u=RStack.pop();
							System.out.println("Getting crate: Expires: " + u.getDate() + " Start Count: " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost() + " from the stack" );
						}
						// if less take what you need and leave
						else
						{
							u.setBananas(u.getBananas()-curr);
							System.out.println(curr + " bananas used from current crate");
						}
					}
					//if exact take all and pop
					else
					{
						System.out.println(curr + " bananas used from current crate");
						u=RStack.pop();
						System.out.println("Getting crate: Expires: " + u.getDate() + " Start Count: " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost() + " from the stack" );
					}
				}
				
			
			}
			//display function
			else if(temp.equals("display"))
			{
				//if none, read error message
				if(RStack.peek()==null)
					System.out.println("No crates in the stack - please reorder!");
				
				// read current crate, if there is one, then pop and read off
				else
				{
					if(u!=null)
					{
						System.out.println("Current Crate: Expires: " + u.getDate() + " Start Count: " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost());
					}
					System.out.println("Stack crates (top to bottom): ");
					int j = 0;
					while(RStack.peek()!=null)
					{
						e=RStack.pop();
						cost = e.getCost();
						date = e.getDate();
						count = e.getCount();
						bananas = e.getBananas();
						System.out.println("Expires: " +date + " Start Count: " + count + " Remain: " + bananas + " Cost: " + cost);
						j++;
						LStack.push(e);
					}
					//push all back on stack
					while(LStack.peek()!=null)
					{
						e=LStack.pop();
						RStack.push(e);
					}
					System.out.println();
				}
			}
			//skip function
			else if(temp.equals("skip"))
			{
				// increase global clock
				clock++;
				System.out.println("It is now Day " + clock);
				
				//while its not empty, check all crates vs the global clock, if they are less than the global clock, the expire and are popped off
				if(RStack.peek()!=null)
				{
					if(u!=null)
					{	
						if(u.getDate()<clock)
						{	
							System.out.println("Current crate: Expires: " + u.getDate() + " Start Count: " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost() + " is expired!");
							u=null;
						}
					}
					if(RStack.peek().getDate()<clock)
					{
						do
						{
							s=RStack.pop();
							System.out.println("Top crate: Expires: " + s.getDate() + " Start Count: " + s.getCount() + " Remain: " + s.getBananas() + " Cost: " + s.getCost() + " is expired!");
						}while(RStack.peek().getDate()<clock);
					}
					System.out.println();
				}
			}
			//report function
			else if(temp.equals("report"))
			{
			
				//text output for the report function
				System.out.println("Lickety Splits Financial Statement:");
				System.out.println("\t\t Most Recent Shipment:");
				System.out.println("\t\t\t\t Crates: " + num);
				System.out.println("\t\t\t\t Banana Cost: " + bcost);
				System.out.println("\t\t\t\t Labor (moves): " + moves);
				System.out.println("\t\t\t\t Labor cost: " + lcost);
				System.out.println("\t\t\t\t ----------------------");
				System.out.println("\t\t\t\t Total: " + tcost);
				System.out.println("\t\t Overall Expenses:");
				System.out.println("\t\t\t\t Crates: " + tnum);
				System.out.println("\t\t\t\t Banana Cost: " + tbcost);
				System.out.println("\t\t\t\t Labor (moves): " + tmoves);
				System.out.println("\t\t\t\t Labor cost: " + tlcost);
				System.out.println("\t\t\t\t Revenue: " + paid);
				System.out.println("\t\t\t\t ----------------------");
				System.out.println("\t\t\t\t Total: " + ttcost);
				System.out.println("\t\t\t\t Profit: " + (paid-ttcost));
				
				System.out.println();
			}
			//purchase function
			else if(temp.equals("purchase"))
			{
				curr = scan.nextInt();
				System.out.println(curr+ " bananas needed for order");
				if(RStack.peek()==null)
					System.out.println("Store is out of bananas!  The horror!");
				else
				{
					if(u==null)
					{
						u= RStack.pop();
						System.out.println("Getting crate: Expires " + u.getDate() + " Start Count " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost() + " from the stack" );
					}	
					int a = u.getBananas();
					if(curr > tcount)
					{
						System.out.println("Sorry we don't have that many bananas left");
					}
				
				
					if(curr<a)
					{	
						u.setBananas(a-curr);
						System.out.println(curr + " bananas used from current crate");
						paid+= u.getCost()*curr;
					}
					else if(curr>a)
					{
						do
						{
							curr =curr-u.getBananas();
							System.out.println(u.getBananas() + " bananas used from current crate");
							paid+= u.getCost()*u.getBananas();
							u = RStack.pop();
							System.out.println("Getting crate: Expires: " + u.getDate() + " Start Count: " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost() + " from the stack" );
							
						}while(curr>u.getBananas());
					
						if(curr == u.getBananas())
						{
							paid+= u.getCost()*curr;
							System.out.println(curr + " bananas used from current crate");
							u=RStack.pop();
							System.out.println("Getting crate: Expires: " + u.getDate() + " Start Count: " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost() + " from the stack" );
							
						}
						else
						{
							u.setBananas(u.getBananas()-curr);
							System.out.println(curr + " bananas used from current crate");
							paid+= u.getCost()*curr;
						}
					}	
					else
					{
						paid+= u.getCost()*curr;
						System.out.println(curr + " bananas used from current crate");
						u=RStack.pop();
						System.out.println("Getting crate: Expires: " + u.getDate() + " Start Count: " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost() + " from the stack" );
						
					}
				}
			}
	
		}
	}
}
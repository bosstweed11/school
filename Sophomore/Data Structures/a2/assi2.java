//Written by Andrew Beers 1/24/12
import java.util.*;
import java.io.*;

public class assi2
{	

	public static void main(String [] args) throws IOException
	{
		String file,check,check1;
		int num=0,date,count,curr,clock=0,bananas,moves=0,tmoves=0,tnum=0,tcount=0;
		double cost=0,bcost=0,lcost=0,tcost=0,ttcost=0,tbcost=0,tlcost=0;
		LinkedStack<Crate> LStack = new LinkedStack<Crate>();
		LinkedStack<Crate> RStack = new LinkedStack<Crate>();
		Crate c,d,e,u=null,s=null,k=null,l=null;

	
	
		Scanner inscan = new Scanner(System.in);
		System.out.println("What is the name of the file you wish to read from?");
		file = inscan.nextLine();
		
		Scanner scan = new Scanner(new File(file));
		
		while(scan.hasNextLine())
		{
		String temp =scan.next();
		
			if(temp.equals("receive"))
			{
				bcost=0;
				moves=0;
				lcost=0;
				
				
				num = scan.nextInt();
				System.out.println("Receiving "+ num + " crates of bananas");
				System.out.println();
				for(int i=0;i<num;i++)
				{
					if(RStack.peek()!=null)
						k = RStack.peek();
					if(LStack.peek()!=null)
						l = LStack.peek();
					date = scan.nextInt();
					count = scan.nextInt();
					cost = scan.nextDouble();
					bcost +=cost;
					//System.out.println("Date: " + date + " count: " +count + " cost: "+cost);
					c = new Crate(date,count,cost);
					if(LStack.peek()==null && RStack.peek()==null)
					{
						RStack.push(c);
						//System.out.println(i+" On Right Stack");
						moves++;
					}
					else if (k.compareTo(c)==0)
					{
						RStack.push(c);
						//System.out.println(i +" On the Right Stack");
						moves++;
					}
					else if (k.compareTo(c)>0)
					{
						if(LStack.peek()==null)
						{
							LStack.push(c);
						}
						else if(LStack.peek().getDate()<=date)
						{
							RStack.push(c);
							moves++;
							
						}
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
							//System.out.println(i+" Pushed on Left Stack");
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
					else if (k.compareTo(c)<0)
					{
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
							//System.out.println(i+" Pushed onto Right Stack");
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
					tcount +=count;
					if(i==(num-1))
					{
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
			else if(temp.equals("use"))
			{
				
				curr = scan.nextInt();
				System.out.println(curr+ " bananas needed for order");
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
				}
				else if(curr>a)
				{
					do
					{
						curr =curr-u.getBananas();
						System.out.println(u.getBananas() + " bananas used from current crate");
						u = RStack.pop();
						System.out.println("Getting crate: Expires: " + u.getDate() + " Start Count: " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost() + " from the stack" );
					}while(curr>u.getBananas());
					
					if(curr == u.getBananas())
					{
						System.out.println(curr + " bananas used from current crate");
						u=RStack.pop();
						System.out.println("Getting crate: Expires: " + u.getDate() + " Start Count: " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost() + " from the stack" );
					}
					else
					{
						u.setBananas(u.getBananas()-curr);
						System.out.println(curr + " bananas used from current crate");
					}
				}
				else
				{
					System.out.println(curr + " bananas used from current crate");
					u=RStack.pop();
					System.out.println("Getting crate: Expires: " + u.getDate() + " Start Count: " + u.getCount() + " Remain: " + u.getBananas() + " Cost: " + u.getCost() + " from the stack" );
				}
				
			
			}
			else if(temp.equals("display"))
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
				while(LStack.peek()!=null)
				{
					e=LStack.pop();
					RStack.push(e);
				}
				System.out.println();
			
			}
			else if(temp.equals("skip"))
			{
				
				clock++;
				System.out.println("It is now Day " + clock);
				
				
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
			else if(temp.equals("report"))
			{
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
				System.out.println("\t\t\t\t ----------------------");
				System.out.println("\t\t\t\t Total: " + ttcost);
				
				System.out.println();
			}
			
		}
	
	
	 
	 
	 
	 
	 
	 
	 }
	 
	 
}
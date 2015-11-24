// This program was written by Andrew Beers on 9/17/11


import java.util.Scanner;
import java.text.*;

public class extra
{
	public static void main(String [] args)
		{
			Scanner inScan = new Scanner(System.in);
			NumberFormat formatter = NumberFormat.getCurrencyInstance();
			int again=1;
			
		
		//start of program, beginning while loop and error checking
		while (again==1)
			{
			//initialization of doubles in loop so they get reinitialized in the case that the loop repeats.
				double discount=0.00,rgasPrice=3.50,pgasPrice=3.75,proPrice=20.00,burPrice=1.00,numGallons=0.00,numTanks=0.00,gasTotal=0.00,proTotal=0.00,fburPrice=0.00,sburPrice=0.00,tburPrice=0.00,fburTotal=0.00,sburTotal=0.00,tburTotal=0.00,sTotal=0.00,taxTotal=0.00,tTotal=0.00,tDiscount=0,discTotal=0,payPrice,change,gasMoney;
				int card,temp,gasBal,gasInput,numBur=0,numBursale=0,numBurmoresale=0,gasChoice;
				
				System.out.println("Welcome to Gas Gas and Gas!");
				System.out.println("Is there a customer in line? (1=yes, 2=no)");
				again = inScan.nextInt();
				while (again!=1 && again!=2)
					{
						System.out.println("Please enter a 1 or 2: ");
						again = inScan.nextInt();
					}
			
			
			if(again==1)
				{	
					System.out.println("Are you a GasVantage Customer?(1=yes, 2=no): ");
					card = inScan.nextInt();
					while (card!=1 && card!=2)
						{
							System.out.println("Please enter a 1 or 2: ");
							card = inScan.nextInt();
		
						}
					
			//GasVantage card and discount if structure
				if (card == 1)
					{
						System.out.println("Welcome Back, GasVantage Customer!");
						System.out.println("What is your current GasVantage balance?");
						gasBal = inScan.nextInt();
						
						while (gasBal <0)
							{
								System.out.println("Please enter a positive number or 0");
								gasBal = inScan.nextInt();
							}	
						//Discounting
						temp = (gasBal/50);
						discount = (double)temp*0.10;
						System.out.println("You will receive:\n\t\t " + formatter.format(discount) +"/gal. discount on gasoline\n\t\t 10% off your propane tanks\n\t\t $0.10/ea discount on burritos");
						rgasPrice= rgasPrice - discount;
						pgasPrice= pgasPrice - discount;
						proPrice = 20*.9;
						burPrice = 1.00-0.10;
					}
					
				System.out.println("Here is the menu:\n\t\tGasoline: " + formatter.format(rgasPrice) + "/gal.reg., " + formatter.format(pgasPrice) + "/gas. plus\n\t\tPropane: " + formatter.format(proPrice) + "/20 lb tank\n\t\tBurritos: " + formatter.format(burPrice) + "/each [with volume discounts]");
				System.out.println("Do you want:\n\t\t1) Regular Gasoline\n\t\t2) Plus Gasoline\n\t\t3) No Gasoline");
				gasInput = inScan.nextInt();
				//Error proof with gas inputs
				while (gasInput!=1 && gasInput!=2 && gasInput!=3)
					{
						System.out.println("Please enter a 1,2, or 3: ");
						gasInput = inScan.nextInt();
		
					}
				if(gasInput != 3)
					{
						System.out.println("Would you like to order gas by the gallon, or by price?(1=by price;2=by gallon) ");
						gasChoice = inScan.nextInt();
						
						while (gasChoice!=1 && gasChoice!=2)
							{
								System.out.println("Please enter a 1 or 2: ");
								gasChoice = inScan.nextInt();
		
							}
							if(gasChoice ==1)
								{
									System.out.println("How much would you like to spend on gas? ");
									gasMoney = inScan.nextDouble();
									while (gasMoney <0)
										{
											System.out.println("Please enter a positive number or 0");
											gasMoney = inScan.nextInt();
										}
									if(gasInput==1)
										{
											numGallons = (float)gasMoney/rgasPrice;
										}
									if(gasInput==2)
										{	
											numGallons = (float)gasMoney/pgasPrice;
										}
								}
						
							if(gasChoice==2)
								{
									System.out.println("How many gallons do you want? ");
									numGallons = inScan.nextInt();
								}
					}
				//Error checking, no negative values
				while (numGallons <0)
					{
						System.out.println("Please enter a positive number or 0");
						numGallons = inScan.nextInt();
					}
					
				System.out.println("How many propane tanks do you want? ");
				numTanks = inScan.nextInt();
				
				while (numTanks <0)
					{
						System.out.println("Please enter a positive number or 0");
						numTanks = inScan.nextInt();
					}
					
				System.out.println("How many burritos do you want? ");
				numBur = inScan.nextInt();
				
				while (numBur <0)
					{
						System.out.println("Please enter a positive number or 0");
						numBur = inScan.nextInt();
					}
				
				
				//Figure out price of gas either plus or regular
				
				if (gasInput != 3)
					{
					if (gasInput ==1)
						gasTotal = numGallons * rgasPrice;
					else
						gasTotal = numGallons * pgasPrice;
					}
				//Figure out price of propane
				
				if (numTanks != 0)
					proTotal = numTanks * proPrice;
				
				//Figure out price of burritos with volume discounts
				if (numBur !=0)
					{
					if(numBur <= 4)
						{
							fburTotal = numBur*burPrice;
							
						}
					if (numBur >4)
						{
							fburTotal = 4*burPrice;
							numBursale = numBur - 4;
						}
					if(numBursale <= 4)
						{	
							sburPrice = numBursale * (burPrice-.10);
							sburTotal = fburPrice + sburPrice;
						}
					if (numBursale >4)
						{
							sburTotal = 4 * (burPrice-.10);
							numBurmoresale = numBursale - 4;
						}
					if( numBurmoresale !=0)
						{
							tburPrice = numBurmoresale * (burPrice-.20);
							tburTotal = fburPrice + sburPrice + tburPrice;
						}
					
					}
					
				//Calculating subtotals
				System.out.println("Here is your subtotal");
				
				if(numGallons >0 && gasInput ==1)
				System.out.println("\t\tGasoline: " + numGallons + " gallons at " + formatter.format(rgasPrice) + " per gallon: \t" + formatter.format(gasTotal));
				
				else if( numGallons >0 && gasInput == 2)
				System.out.println("\t\tGasoline: " + numGallons + " gallons at " + formatter.format(pgasPrice) + " per gallon: \t" + formatter.format(gasTotal));
				
				if(numTanks != 0 && numTanks <= 9)
				System.out.println("\t\tPropane: " + numTanks + " tanks at " + formatter.format(proPrice) + " per tank: \t\t" + formatter.format(proTotal));
				
				if(numTanks !=0 && numTanks >9)
				System.out.println("\t\tPropane: " + numTanks + " tanks at " + formatter.format(proPrice) + " per tank: \t" + formatter.format(proTotal));
				
				if (numBur !=0)
					{
					if(numBur <= 4)
						{
							System.out.println("\t\tBurritos: " + numBur + " at " + formatter.format(burPrice) + " per burrito: \t\t" + formatter.format(fburTotal));
						
						}
					
					else if(numBursale <= 4)
						{	
							System.out.println("\t\tBurritos: " + 4 + " at " + formatter.format(burPrice) + " per burrito: \t\t" + formatter.format(fburTotal) + "\n\t\tBurritos: " + numBursale + " at " + formatter.format((burPrice-.10)) + " per burrito: \t\t" + formatter.format(sburTotal));
							
						}
					
					else if( numBurmoresale !=0)
						{
							System.out.println("\t\tBurritos: " + 4 + " at " + formatter.format(burPrice) + " per burrito: \t\t" + formatter.format(fburTotal) + "\n\t\tBurritos: " + 4 + " at " + formatter.format((burPrice-.10)) + " per burrito: \t\t" + formatter.format(sburTotal) + "\n\t\tBurritos: " + numBurmoresale + " at " + formatter.format((burPrice-.20)) + " per burrito: \t\t" + formatter.format(tburTotal));
						
						}
					
					}
					//Subtotal
				System.out.println("\t\t\t\t\t\t\t\t ------");
				sTotal = gasTotal + proTotal + fburTotal + sburTotal + tburTotal;
				System.out.println("Subtotal:\t\t\t\t\t\t\t" + formatter.format(sTotal));
				
				//10% discount
				if(sTotal>50)
					{
						discTotal = sTotal * .1;
						sTotal = sTotal -discTotal;
						System.out.println("Bonus discount of 10%:\t\t\t\t\t\t" + "(" + formatter.format(discTotal) + ")");
						System.out.println("New subtotal:\t\t\t\t\t\t\t" + formatter.format(sTotal));
					}
				
				//Tax
				taxTotal = sTotal * .07;
				System.out.println("Tax:\t\t\t\t\t\t\t\t" + formatter.format(taxTotal));
				System.out.println("\t\t\t\t\t\t\t\t ------");
				tTotal = sTotal + taxTotal;
				System.out.println("Total:\t\t\t\t\t\t\t\t" + formatter.format(tTotal));
					
				//GasVantage card discount
				if(numBur<=4)
					tDiscount = 2*numTanks + discount*numGallons + .1*numBur;
				
				if(numBursale!=0)
					tDiscount = 2*numTanks + discount*numGallons + .1*4 + .1*numBursale;
				
				if(numBurmoresale!=0)
					tDiscount = 2*numTanks + discount*numGallons + .1*4 + .1*4 + .1*numBurmoresale;
				
				if(card ==1)
					System.out.println("As a GasVantage card holder, you saved: " +  formatter.format(tDiscount));
				
				System.out.println("Please enter payment amount(in dollars): ");
				payPrice= inScan.nextDouble();
				//Error checking payment
				while (payPrice<tTotal)
					{	
						System.out.println("Not enough money. Please enter payment amount(in dollars): ");
						payPrice= inScan.nextDouble();
					}
				
				change = payPrice - tTotal;
				
				System.out.println("Your change:\t\t\t\t\t\t\t " + formatter.format(change));
				System.out.println("Thank you for shopping at GG&G!");
				System.out.println("\n");
				
				}
			}
		}

}
	
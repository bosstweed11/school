// Andrew Beers Assignment 1
// Pies, Pies, and Pis. A restaurant checkout program

//Todo
// payment
// discounts
// replace amount, don't add to amount
// macros for prices and discounts and tax
// break out into functions where i can
// extra credit
import java.util.*;
public class Assignment1 {

	// Price constants
	public static final double TAX = .06;
	public static final double PLAIN_SLICE_COST = 10.0;
	public static final double PEPPERONI_SLICE_COST = 12.0;
	public static final double CHERRY_PIE_SLICE_COST = 2.0;
	public static final double CHERRY_PIE_COST = 10.0;
	public static final double CHARM_COST = 50.0;

	// Discount constants
	public static final double PEPPERONI_DISCOUNT = PEPPERONI_SLICE_COST - PLAIN_SLICE_COST;
	public static final double CHERRY_PIE_SLICE_DISCOUNT = 0.25;
	public static final double CHERRY_PIE_DISCOUNT = 2.00;
	public static final double CHARM_PERCENT_OFF = 10.0;
	public static final double PERCENT_OFF_ORDER_OVER_100 = 10.0;

	public static void main(String [] args){
		boolean again = false;
		Scanner scan = new Scanner(System.in);
		do {
			again = restaurantLoop(scan);
		} while(again);
	}

	public static boolean restaurantLoop(Scanner scan){

		System.out.println("Welcome to Pies, Pies and Pis");
		int isPersonThere = -1;
		do {
			System.out.print("Is there a customer in line? (1 = yes, 2 = no) > ");
			isPersonThere = scan.nextInt();
		} while(isPersonThere != 1 && isPersonThere != 2);

		if (isPersonThere == 1){
			orderLoop(scan);
		}

		return (isPersonThere == 1);
	}

	public static void orderLoop(Scanner scan){
		int isPieCardMember = -1;
		int menuOptionChoice = -1;

		// Card holder processing
		do {
			System.out.print("Are you a Pie Card member? (1 = yes, 2 = no) > ");
			isPieCardMember = scan.nextInt();
		} while(isPieCardMember != 1 && isPieCardMember != 2);

		if (isPieCardMember == 1){
			System.out.println("Welcome Back, Pie Card holder!");
			System.out.println("You will recieve:");
			System.out.println("\tPepperoni pizza for the price of plain");
			System.out.println("\t$0.25 off a slice of cherry pie");
			System.out.println("\t$2.00 off a whole cherry pie");
			System.out.println("\t10% off each Pi charm");
		}

		int tempInput = 0;
		boolean boughtSomething = false;
		int paymentAmt;
		double subTotal;

		// Pizza Variables
		double pizzaTotal = 0;
		int numberOfPlainSlices = 0;
		int numberOfPepperoniSlices = 0;
		double plainSliceCost = 10.00;
		double pepperoniSliceCost = (isPieCardMember == 1) ? 10.00 : 12.00;

		// Cherry Pie Variables
		double pieTotal = 0;
		int cherryPieSlices = 0;
		double cherryPieSliceCost = (isPieCardMember == 1) ? 1.75 : 2.00;
		double cherryPieCost = (isPieCardMember == 1) ? 8.00 : 10.00;

		// Pi Charm Variables
		double charmTotal = 0;
		int charmPieces = 0;
		double charmCost = (isPieCardMember == 1) ? 45.00 : 50.00;

		// Main option loop
		while (menuOptionChoice != 4){
			do {
				System.out.println("Please choose an option:");
				System.out.println("\t1) Update Pizza Order");
				System.out.println("\t2) Update Cherry Pie Order");
				System.out.println("\t3) Update Charm Order");
				System.out.println("\t4) Check Out");
				menuOptionChoice = scan.nextInt();
			} while(menuOptionChoice != 1 && menuOptionChoice != 2 && menuOptionChoice != 3 && menuOptionChoice != 4);

			// Pizza loop
			if (menuOptionChoice == 1){

				// Display order information
				System.out.println("Here is your current order:");
				if (numberOfPlainSlices == 0 && numberOfPepperoniSlices == 0){
					System.out.println("\tNo pizzas ordered");
				}
				else{
					System.out.println("\t" + numberOfPlainSlices + " plain pizzas");
					System.out.println("\t" + numberOfPepperoniSlices + " pepperoni pizzas");
				}

				// Take plain pizza order
				System.out.printf("How many plain pizzas would you like for $%.2f each?\n", plainSliceCost);
				tempInput = scan.nextInt();
				if (tempInput <= 0){
					System.out.println("Negative number taken as 0");
					numberOfPlainSlices = 0;
				}
				else {
					boughtSomething = true;
					numberOfPlainSlices = tempInput;
					//numberOfPlainSlices += tempInput;
				}

				// Take pepperoni pizza order
				System.out.printf("How many pepperoni pizzas would you like for $%.2f each?\n", pepperoniSliceCost);
				tempInput = scan.nextInt();
				if (tempInput <= 0){
					System.out.println("Negative number taken as 0");
				}
				else {
					boughtSomething = true;
					numberOfPepperoniSlices = tempInput;
				}
			}
			else if (menuOptionChoice == 2){
				// Display order information
				System.out.println("Here is your current order:");
				if (cherryPieSlices == 0){
					System.out.println("\tNo cherry pie slices ordered");
				}
				else{
					System.out.println("\t" + (cherryPieSlices / 6) + " whole cherry pies");
					System.out.println("\t" + (cherryPieSlices % 6) + " cherry pie slices");
				}

				// Take cherry pie order
				System.out.println("How many cherry pie slices would you like for:");
				System.out.printf("\t$%.2f per slice\n", cherryPieSliceCost);
				System.out.printf("\t$%.2f per pie (6 slices)\n", cherryPieCost);
				tempInput = scan.nextInt();
				if (tempInput <= 0){
					System.out.println("Negative number taken as 0");
				}
				else{
					boughtSomething = true;
					cherryPieSlices = tempInput;
					// cherryPieSlices += tempInput;
				}
			}
			else if (menuOptionChoice == 3){
				// Display order information
				System.out.println("Here is your current order:");
				if (charmPieces == 0){
					System.out.println("\tNo gold Pi charms ordered");
				}
				else{
					System.out.println("\t" + charmPieces + " gold Pi charms");
				}

				// Take charm order
				System.out.printf("How many Pi charms would you like for $%.2f each?\n", charmCost);
				tempInput = scan.nextInt();
				if (tempInput <= 0){
					System.out.println("Negative number taken as 0");
				}
				else {
					boughtSomething = true;
					charmPieces = tempInput;
					// charmPieces += tempInput;
				}
			}
		}

		// Calculate totals
		System.out.println("Here is your subtotal:\n");
		if (boughtSomething){
			if (numberOfPlainSlices > 0)
				System.out.printf("\t%d plain pizzas at $%.2fea.:\t\t\t$%.2f\n",numberOfPlainSlices, plainSliceCost, (numberOfPlainSlices * plainSliceCost));
			if (numberOfPepperoniSlices > 0)
				System.out.printf("\t%d pepperoni pizzas at $%.2fea.:\t\t$%.2f\n",numberOfPepperoniSlices, pepperoniSliceCost, (numberOfPepperoniSlices * pepperoniSliceCost));
			pizzaTotal = (numberOfPlainSlices * plainSliceCost) + (numberOfPepperoniSlices * pepperoniSliceCost);

			if (cherryPieSlices / 6 > 0)
				System.out.printf("\t%d whole cherry pies at $%.2fea.:\t\t$%.2f\n",(cherryPieSlices / 6), cherryPieCost, ((cherryPieSlices / 6) * cherryPieCost));
			if (cherryPieSlices % 6 > 0)
				System.out.printf("\t%d cherry pie slices at $%.2fea.:\t\t$%.2f\n",(cherryPieSlices % 6), cherryPieSliceCost, ((cherryPieSlices % 6) * cherryPieSliceCost));
			pieTotal = ((cherryPieSlices / 6) * cherryPieCost) + ((cherryPieSlices % 6) * cherryPieSliceCost);

			if (charmPieces > 0)
				System.out.printf("\t%d 14K gold Pi charms at $%.2fea.:\t\t$%.2f\n",charmPieces, charmCost, (charmPieces * charmCost));
			charmTotal = (charmPieces * charmCost);

			System.out.println("\t\t\t\t\t\t\t-------");
			subTotal = pizzaTotal + pieTotal + charmTotal;
			System.out.printf("\tSubtotal:\t\t\t\t\t$%.2f\n", subTotal);
			if (isPieCardMember == 1 && subTotal >= 100 ){
				System.out.printf("\tBonus discount of 10%%:\t\t\t\t($%.2f)\n", subTotal * (PERCENT_OFF_ORDER_OVER_100 / 100));
				subTotal -= subTotal * (PERCENT_OFF_ORDER_OVER_100 / 100);
				System.out.println("\t\t\t\t\t\t\t-------");
				System.out.printf("\tNew subtotal:\t\t\t\t\t$%.2f\n", subTotal);
			}
			System.out.printf("\tTax:\t\t\t\t\t\t$%.2f\n", subTotal * .06);
			System.out.println("\t\t\t\t\t\t\t-------");
			System.out.printf("\tTotal:\t\t\t\t\t\t$%.2f\n\n", subTotal * 1.06);

			// Payment
			do {
				System.out.print("\n\tPlease enter your payment amount:\t\t$");
				paymentAmt = scan.nextInt();
			} while(paymentAmt < subTotal * 1.06);
			System.out.printf("\tYour change:\t\t\t\t\t$%.2f\n", paymentAmt - subTotal * 1.06);
			System.out.println("\tThank you for shopping at PP&P!\n");
		}
		else {
			System.out.println("\tNo items purchased! Thanks anyway for stopping!\n");
		}

	}
}
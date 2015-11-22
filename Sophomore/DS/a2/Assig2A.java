public class Assig2
{	
	String file;
	int num,date,count;
	double cost;
	ArrayStack<Crate> LStack;
	
	Scanner inscan = new Scanner(System.in);
	System.out.println("What is the name of the file you wish to read from?");
	file = inscan.nextLine();
	Scanner scan = new Scanner(file);
	while(scan.hasNextLine())
	{
		if(scan.nextLine().equals("recieve"))
		{
			System.out.println("recieve");
			num = scan.nextInt();
			for(int i=0;i<num;i++)
			{
				date = scan.nextInt();
				count = scan.nextInt();
				cost = scan.nextDouble();
				LStack
			}
	
	
		}
		else if(scan.nextLine().equals("use"))
		{
		}
		else if(scan.nextLine().equals("display"))
		{}
		else if(scan.nextLine().equals("skip"))
		{}
		else if(scan.nextLine().equals("report"))
		{}
	}
	
	
	 
	 
	 
	 
	 
	 
	 
	 
	 
}
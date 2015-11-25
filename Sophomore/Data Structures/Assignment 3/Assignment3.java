//Written by Andrew Beers 2/24/12
import java.util.*;
public class Assignment3
{
//Main of the program
//Variables to monitor parenthesis
private static int open,close,p,end;
private static String errormsg=null;
	public static void main(String [] args)
	{
		String exp;
		int out;
        int again=1;
       
		Scanner s = new Scanner(System.in);
        while(again==1)
        {
        	
        	//Prompt for expression
            System.out.println("Please enter an expression: ");
            exp = s.nextLine();
            //Create A3 object of the expression
            Assignment3 A = new Assignment3(exp);
            //Try catch block for exceptions
            try
            {
            	//reset parens vars
            	open=0;
        		close=0;
        		p=0;
            	//Call expression
            	out = A.expression();
            	
            	if(open != close)
            	{
            		exception = true;
            		errormsg = "Unbalanced Parenthesis.";
            	}
    			
            	//If no problems, print the output, else, print invalid
            	if(!exception)
            		System.out.println(exp +" = " + out);
            	else
            		System.out.println("Your expression was invalid. " + errormsg);
            		
            	//prompt to repeat program
            	System.out.println("Again?(1,0)");
            	again = Integer.parseInt(s.nextLine());
            }
            //if exception, print invalid and prompt for repeat
            catch(Exception a)
            {
            	a.printStackTrace();
            	errormsg = "String format error";
            	System.out.println("Your expression was invalid. " + errormsg);
            	System.out.println("Again?(1,0)");
            	again = Integer.parseInt(s.nextLine());
            }
        }
	}
	
	//Instance variables
	private String expression;
	private int index;
	private static boolean exception=false,out,nodig,paren,dieparens;
	
	// Constructor for class object
	public Assignment3(String input)
	{
		expression = new String(input);
		index = 0;
	}
	
	// Digits function, recursively calls itself to read out character string of digits and converts them to ints
	public int digits(int dig) throws Exception
	{
		int first,second;
		nodig = false;
		// get first character
        char now = expression.charAt(index);
        
        //if it is a digit, increase index, convert to int, mult by 10 and recursively repeat
		if (Character.isDigit(now))    
		{
				index++;						
				first = now - '0';			
				dig = dig * 10 + first;		
				second = digits(dig);			
		}
		//else return the digit and start returning recursively
		else
		{
			second = dig;	
		}
		return second;
	}
	
	// Expression function, indirectly recursively calls itself through term()
	public int expression() throws Exception
	{
		out = false;
		exception = false;
		nodig = true;
		dieparens = false;
		if(!expression.endsWith("."))
		{
			exception = true;
			errormsg = "Unexpected end of line";
		}
		int end=0,end1,end2=0,operation;
		// call term, expression must be a term
		end1 = term();
		end = end1;
		char now = expression.charAt(index);
		//while closing parens, increase index
		
			
		while(now == ')')
		{
			close++;
			index++;
			now = expression.charAt(index);
		}
		//while no white space, continue, else return -1
		if(isValid())
		{
			// if there is an operation to be done, go to that structure, else
			operation = isOp();
			if(operation!=-1)
			{
				// if no white space, continue
				if(isValid())
				{
					//if operation, call second term for operation to act on
					end2 = term();
					if(isValid())
					{
						//check next character
						now = expression.charAt(index);
						
						//if close paren, increment closed parens and index, get new char
						if(now == ')')
						{
							close++;
							index++;
							now = expression.charAt(index);
							
						}
						else if(now == '.')
						{
							if(exception && out)
								exception = true;
							else
								exception = false;
						}
						else
							exception = true;
						// choose appropriate operation and return result
						if(operation ==1)
						{
							end = end1+end2;
							return end;
						}
						else if(operation ==2)
						{
							end = end1-end2;
							return end;
						}
						else if(operation ==3)
						{
							end = end1*end2;
							return end;
						}
						else if(operation ==4)
						{
							end = end1/end2;
							return end;
						}
						else 
						{
							end = end1%end2;
							return end;
						}
					}
				}
			}
			
			// if we find end condition, return
			else if(now =='.')
			{
				if(exception && out)
				{
					exception = true;
				}
				else
					exception =false;
				
				return end;
			}
			// if we find an end paren, return
			else if(now ==')')
			{
				return end;
			}
			// anything else, is an exception and improperly formatted
			else
			{
				exception = true;
				return -1;
			}
		}
		
		return end;
		
    }
    
    // Term function, calls expression recursively, or digits directly
	public int term() throws Exception
	{   
		char now = expression.charAt(index);
		int t=-1;
		
		//if start of paren, must lead to (expression), so indirectly recursively call expression
		if(now == '(')
		{
			open++;				//increment parens counter
			index++;
			t = expression();
			now = expression.charAt(index);
			if(now =='.')
				out = true;
		
		}
		//if digit, call digits
		else if(Character.isDigit(now))
		{
			t = digits(0);
		}
		//anything else is an exception and is returned as -1
		else
		{
			exception = true;
			errormsg = "Illegal Character.";
			t=-1;
		}
		return t;
		
    }
    
    // isValid, gets rid of whitespace between characters
    public boolean isValid() throws Exception
    {
    	char now = expression.charAt(index);
    	// if white space, increase index, read again until non whitespace
    	while(now ==' ')
    	{
    		try
    		{
    			index++;
    			now = expression.charAt(index);
    		}
    		catch(Exception a)
    		{
    			exception = true;
    			return false;
			}
    	}
    	return true;
    }
    
    //is Op, checks if the current character is an appropriate operation
    public int isOp() throws Exception
    {
    	int op=-1;
    	char now = expression.charAt(index);
    	
    	//if non whitespace, check if op, if it is, assign op appropriate value, if '.' or ')' just return false, anything else is an exception
    	if(isValid())
    	{
    		if(now == '+')
    		{
    			op =1;
    			index++;
    		}
    		else if(now == '-')
    		{
    			op =2;
    			index++;
    			
    		}
    		else if(now == '*')
    		{
    			op = 3;
    			index++;
    			
    		}
    		else if(now == '/')
    		{
    			op = 4;
    			index++;
    		
    		}
    		else if(now == '%')
    		{
    			op =5;
    			index++;
    			
    		}
    		else if(now == '.')
    		{
    			op = -1;
    		}
    		else if(now == ')')
    		{
    			op =-1;
    		}
    		else
    		{
    			exception = true;
    			errormsg = "Illegal Character.";
    			op = -1;
    		}
    	}
    	return op;
    }

}
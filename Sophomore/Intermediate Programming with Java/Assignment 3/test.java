import java.util.*;
import java.io.*;
public class test
{
	public static void main(String [] args) throws IOException
	{
		//ArrayList<String> P = new ArrayList<String>();
		String player = "words.txt";
		Scanner S = new Scanner(new File(player));
		/*String name,right,wrong;
		int c=0;
		while(inScan.hasNextLine())
		{
			name = inScan.nextLine();
			right = inScan.nextLine();
			wrong = inScan.nextLine();
			P.add(name);
			P.add(right);
			P.add(wrong);
			c= c +3;
		}
		for( int i=0; i < c; i++)
		System.out.println(P.get(i));
		*/
		int index;
	 	String word;
	 	
	 	int c;
	
		Random R = new Random();
		
		index = Integer.parseInt(S.nextLine());
		String[] s = new String[index];
		String[] p = new String[index];
		
		for(int i = 0; i<index; i++)
		{
			s[i] = S.nextLine();
			System.out.println(s[i]);
		}
		
		String g = "-";
		c=R.nextInt(index);
		System.out.println(s[0].length());
		StringBuilder K = new StringBuilder();
		for(int i =0; i<s[0].length(); i++)
		{
			K.append(g);
			System.out.println(i);
		}
		System.out.println(K);
		
		
	}
	
	
	
}

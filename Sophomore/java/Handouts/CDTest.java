import java.util.*;
import java.io.*;
public class CDTest
{
	public static void main(String [] args) throws IOException
	{
		Scanner fScan = new Scanner(new File("songs.txt"));
		
		int numAlbums = Integer.parseInt(fScan.nextLine());
		CD [] myCDs = new CD[numAlbums];
		
		for(int i = 0; i < myCDs.length; i++)
		{
			String aTitle = fScan.nextLine();
			String rDate = fScan.nextLine();
			int numSongs = Integer.parseInt(fScan.nextLine());
			Song [] theSongs = new Song[numSongs];
			
			for(int j = 0; j < theSongs.length; j++)
			{
				String songString = fScan.nextLine();
				String [] songInfo = songString.split(",");
				Song currSong = new Song(songInfo[0],songInfo[1],songInfo[2],songInfo[3]);
				theSongs[j] = currSong;
			}
			
			CD theCD = new CD(aTitle,rDate,theSongs);
			myCDs[i] = theCD;
		}
		
		System.out.println("Here are your CDs: ");
		for (int i = 0; i < myCDs.length; i++)
			System.out.println(myCDs[i].toString());
		System.out.println("\nNow Sorting the CDs");
		Arrays.sort(myCDs);
		System.out.println("\n");
		System.out.println("Here are your CDs: ");
		for (int i = 0; i < myCDs.length; i++)
			System.out.println(myCDs[i].toString());
			
		if (myCDs[0].compareTo(myCDs[1]) > 0)
			System.out.println(myCDs[0] + "\n is > \n\n" + myCDs[1]);
		else
			System.out.println(myCDs[0] + "\n is not > \n\n" + myCDs[1]);
			
	}
}
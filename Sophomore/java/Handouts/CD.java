import java.util.*;
import java.text.*;
public class CD implements Comparable<CD>
{
	private String title;
	private Song [] songList;
	private Date releaseDate;
	private int tracks;
	private int length;
	
	public CD(String t, String d, Song [] songs)
	{
		title = new String(t);
		
		DateFormat df = DateFormat.getDateInstance(DateFormat.SHORT);
		try
		{
			releaseDate = df.parse(d);
		}
		catch ( ParseException e)
		{
			releaseDate = null;
		}
		tracks = songs.length;
		length = 0;
		
		songList = new Song[songs.length];
		for (int i = 0; i < songList.length;i++)
		{
			songList[i] = songs[i];
			length += songList[i].getLength();
		}
	}
	
	public String toString()
	{
		StringBuilder S = new StringBuilder();
		S.append("CD: " + title + "\n");
		
		if (releaseDate != null)
		{
			S.append("Release Date: " + releaseDate.toString() + "\n");
		}
		else
		{
			S.append("No release date found\n");
		}
		
		int min = length/60;
		int sec = length%60;
		S.append("Number of tracks: " + tracks + "\n");
		S.append("Length: " + min + " min. " + sec + " sec. \n");
		S.append("Songs: \n");
		
		for(int i=0; i <songList.length; i++)
			S.append(songList[i].toString());
		return S.toString();
	}
	
	
	public int compareTo(CD rhs)
		{
			return (length -rhs.length);
		}
}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
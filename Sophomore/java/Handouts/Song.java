public class Song implements Comparable<Song>
{
	private String title;
	private String length;
	private String artist;
	private String genre;
	
	public Song(String t, String l, String a, String g)
	{
		title = new String(t);
		artist = new String(a);
		length = new String(l);
		genre = new String(g);
	}
	
	public String toString()
	{
		StringBuffer S = new StringBuffer();
		S.append("Title: " + title + " ");
		S.append("Length: " + length + " ");
		S.append("Artist: " + artist + " ");
		S.append("Genre: " + genre + " ");
		return S.toString();
	}
	
	public int getLength()
	{
		String [] MinSec = length.split(":");
		int sec = Integer.parseInt(MinSec[1]);
		sec += (60* Integer.parseInt(MinSec[0]));
		return sec;
	}
	
	public int compareTo(Song rhs)
	{
		return (this.title.compareTo(rhs.title));
	}
}
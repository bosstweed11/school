

<?php
	//andrew beers initialization of db file
	$pword = file("quotes3.txt");
	$pass = rtrim( $pword[0] );
	$db = new mysqli('localhost', 'BeersA', $pass , 'BeersA');
	
	if ( $db->connect_error):
		die (" Could not connect to db: " . $db->connect_error);
	endif;
	
	$db->query("drop table Words");
	

	//Users table
	
	$db->query( "create table Words ( wordid int primary key not null, word char(30) not null)");
	$userFile = file("words.txt");
	$index = 0;
	//populate users table
	foreach ( $userFile as $userInfo):
	
		$userString = rtrim( $userInfo );
		
		$query = "insert into Words values ('$index','$userString')";
		
		$db->query($query) or die ("invalid insert " . $db->error);
		$index++;
	endforeach;
	
	
	$query = "select * from Words";
	
	$result = $db->query($query) or die ("Invalid query" . $db->error);
	
	echo "<table border = '1'><tr><td>Words table</td></tr>";
	echo "<tr><td>Word ID</td><td>Word</td></tr>";
	
	$rows = $result->num_rows;
	
	if ( $rows < 1 )
	{
	
		echo " No entries</br>";
	
	}
	else
	{
		
		for ( $i = 0; $i < $rows; $i++ )
		{
			echo "<tr>";
			$row = $result->fetch_array();
			echo "<td>" . $row["wordid"] . "</td>";
			echo "<td>" . $row["word"] . "</td>";

			echo "</tr>";
		}
		echo "</table>";
	}
	
?>
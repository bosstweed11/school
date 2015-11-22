<?php
	$pword = file("quotes3.txt");
	$pass = rtrim( $pword[0] );
	$db = new mysqli('localhost', 'BeersA', $pass , 'BeersA');


	if ( $db->connect_error):
		die (" Could not connect to db: " . $db->connect_error);
	endif;
	
	
	$result = $db->query( "select * from Words order by rand() limit 1");
	
	$row = $result->fetch_array();
	
	$resultString = '<?xml version="1.0" encoding="utf-8"?>';
	$word = $row['word'];
	$resultString.="<word><name>$word</name></word>";
	echo "$resultString";
?>
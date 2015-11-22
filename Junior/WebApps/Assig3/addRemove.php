<?php

	function __autoload( $class )
	{

		require_once $class . '.php';
	}
	include 'A2Functions.php';
	require_once 'A2User.php';
	
	date_default_timezone_set( 'America/New_York' );

	$db = new mysqli('localhost', 'BeersA', "newt.chaps", 'BeersA');
    if ($db->connect_error):
    	die ("Could not connect to db " . $db->connect_error);
   	endif;
   	
   	$resultString = "";
   	if ( isset($_POST['add'] ))
   	{
   		//query insert into users  post[first,last,email....]
   		//if exists, switch access -> 1
   	
   	}
   	else if ( isset($_POST['remove']))
   	{
   		$psid = $_POST['psid'];
   		$result = $db->query("update Users set Users.access_level= '-1' where Users.psid = '$psid'");
   		
   		if ( $result )
   		{	
			$resultString.="User removed.";   		
   		}
   		else
   		{
   			$resultString.="Error.";
   		}
   		echo "$resultString";
   	
   	}
   	else
   	{
   		$result = $db->query("select * from Users where Users.access_level > -1");
   		
   		$rows = $result->num_rows;
   		$resultString = '<?xml version="1.0" encoding="utf-8"?>';
		$resultString .= "<user users = '$rows'>";
   		
   		for ( $i = 0; $i < $rows; $i++ )
   		{
   			$row = $result->fetch_array();
   			$resultString.="<psid>".$row['psid']."</psid>";
   			
   		
   		}
   		$resultString .= "</user>";
   		
   	
   	}
   
   	echo "$resultString";
?>
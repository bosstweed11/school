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
   	
   	$old = $_POST['oldPass'];
   	$oldpass = hash('sha256',$old);
   	$new = $_POST['newPass'];
   	$newpass = hash('sha256',$new);
   	$psid = $_POST['psid'];
   	
   	
   	$resultString = "$";
   	$result = $db->query("select * from Users where Users.psid = '$psid' and Users.password = '$oldpass'");

	if ( $result->num_rows > 0)
	{	
		$row = $result->fetch_array();
		
		$result = $db->query("update Users set Users.password= '$newpass' where Users.psid = '$psid'");
		
		$resultString ="Password Changed.";
		
	}
   	else
   	{
   	
   		$resultString ="Error Occured.";
   	}
   	echo "$resultString";
?>
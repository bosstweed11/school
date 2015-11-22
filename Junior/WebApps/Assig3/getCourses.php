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
   	
   	
   	$psid = $_POST['psid'];
   	
   	$result = $db->query("select * from Courses where Courses.psid = '$psid'");
   	
   	$rows = $result->num_rows;
   	$resultString = "";
   	if ( $rows < 1 )
   	{
  		
   	
   	}
   	else
   	{
   	
   		$resultString = '<?xml version="1.0" encoding="utf-8"?>';
		$resultString .="<Student courses='$rows'>";
		for( $i = 0; $i < $rows; $i++ )
		{
			$row = $result->fetch_array();
			$resultString .= "<Course num='$i'><Term>".$row['Term']."</Term><Department>".$row['Department']."</Department><ClassNumber>".$row['Class_Number']."</ClassNumber><Grade>".$row['Grade']."</Grade><gpa>".$row['GPA']."</gpa></Course>";	

		}
		$resultString .="</Student>";
		echo "$resultString";
   	
   	}
   	
   	
   	
   	
?>
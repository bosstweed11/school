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
   	
   	
   	$file = $_POST['file'];
   	
   	
   	$gpas = array(
			"A+" => 4.00,
    		"A" => 4.00,
    		"A-" => 3.75,
    		"B+" => 3.25,
    		"B" => 3.00,
    		"B-" => 2.75,
    		"C+" => 2.25,
    		"C" => 2.00,
    		"C-" => 1.75,
    		"D+" => 1.25,
    		"D" => 1.00,
    		"D-" => 0.75,
    		"F" => 0.00
		);
	$courseFile = file("$file");
	
	if ( $courseFile == null )
	{
		echo "error";
	}
	else
	{
		$result = $db->query("select * from Courses");
		$rows = $result->num_rows;
	
		$courseid = $rows + 1;
		// populate courses table
		foreach ( $courseFile as $courseInfo):
	
			$courseString = rtrim( $courseInfo );
			$course = preg_split("/:/", $courseString );
			$result = $db->query("select * from Courses where Courses.Term = '$course[2]' and Courses.psid = '$course[3]' and Courses.Class_Number = $course[1] and Courses.Department = $course[0]");
			$query = "insert into Courses values ('$courseid','$course[3]','$course[0]','$course[1]','$course[2]','$course[4]', ". $gpas[$course[4]] . ")";
		
			$db->query($query) or die ("invalid insert " . $db->error);
			$courseid += 1;
		endforeach;
			echo "Added Courses.";
   }
   
   	
   	
   	
   	
?>
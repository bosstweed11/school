<?php

	function __autoload( $class )
	{

		require_once $class . '.php';
	}
	include 'A2Functions.php';
	require_once 'A2User.php';

	$db = new mysqli('localhost', 'BeersA', "newt.chaps", 'BeersA');
    if ($db->connect_error):
    	die ("Could not connect to db " . $db->connect_error);
   	endif;
   	if ( isset($_POST['type'] ) )
   	{
		$type = $_POST["type"]; 
	}
	
	if ( $type == 1 )
   	{
   		$setting = $_POST["setting"];
      	$psid = $_POST["psid"];
      	
      	$result = $db->query( "select * from Users where Users.psid = '$psid'");
      	
      	$row = $result->fetch_array();
      	
      	$info = new A2User( $row['UserID'] , $row['password'], $row['psid'], $row['email'], $row['last_name'], $row['first_name'], $row['access_level'] );
			

      	
      	$result = $db->query("select * from Courses where Courses.psid = '$psid'");
				
		$rows = $result->num_rows;
	
		$studentCourseList = array();
		for ( $i =0; $i < $rows; $i++ )
		{
			$course = $result->fetch_array();
		
			$studentCourse = new Course($course['Term'], $course['Department'], $course['Class_Number'], $course['Grade'], $course['GPA'] );
			$studentCourseList[$i] = $studentCourse;
			
		
		}
		$info->addCourses( $studentCourseList );
		$infoString = serialize($info);
      	
      	if ( $setting == 1 )
      	{
      		$result = show_term_classes ( $info, $db );
      		echo "$result";
      	}
      	elseif ( $setting == 2 )
      	{
      		$result = show_alpha_classes ( $info, $db );
      		echo "$result";
      	}
      	elseif ( $setting == 3 )
      	{
      		$result = show_reqs_classes ( $info, $db );
      		echo "$result";
      	}
      	
    }
    else if ( $type == 2 )
    {
    	//student search
    	if ( isset( $_POST['psid'] ) ) 
    	{
			$setting = $_POST["setting"];
			$psid = $_POST["psid"];
		
			$result = $db->query( "select * from Users where Users.psid = '$psid'");
		}
		else
		{
			$first = $_POST["first"];
			$last= $_POST["last"];
			$result = $db->query( "select * from Users where Users.first_name = '$first' and Users.last_name = '$last'");
		
		}
      	
      	$row = $result->fetch_array();
      	
      	$info = new A2User( $row['UserID'] , $row['password'], $row['psid'], $row['email'], $row['last_name'], $row['first_name'], $row['access_level'] );
		
		$psid = $row['$psid'];
		/*
		$resultString = '<?xml version="1.0" encoding="utf-8"?>';
		
		$resultString .="<studentcourses psid='$psid'>";
		$resultString .="<alpha>";
		$resultString .= show_alpha_classes( $info, $db, $resultString );
		$resultString .="</alpha>";
		
		$resultString .="<byterm>";
		$resultString .= show_term_classes( $info, $db, $resultString );
		$resultString .="</byterm>";
		
		$resultString .="<progress>";
		$resultString .= show_reqs_classes( $info, $db, $resultString );
		$resultString .="</progress>";
		$resultString .="</studentcourses>";
		*/
		echo "$psid";
		
		
    
    
    }
    else if ( $type == 3 )
    {
    	$course = $_POST['course'];
    	$department = $_POST['department'];
    	
    	$result = $db->query("select Courses.psid, Courses.Grade, Courses.GPA from Courses where Courses.Department = '$department' and Courses.Class_Number = '$course' ");
		
		$resultString = "";
			
		$rows = $result -> num_rows;
		
		if ( $rows < 1 )
		{
			
		}
		else
		{
		
			$resultString = '<?xml version="1.0" encoding="utf-8"?>';
			$resultString .= "<return courses='$rows' course='$course' dept='$department'>";
			$cumulativeGPA = 0;
			for ( $i =0; $i < $rows; $i++ )
			{
				$row = $result->fetch_array();
				$resultString .= "<course>";
				$resultString .= "<psid>". $row['psid'] . "</psid><grade>". $row['Grade']. "</grade><gpa>". $row['GPA']."</gpa>";
				$resultString .= "</course>";
				$cumulativeGPA += $row['GPA'];
	
			}
			$avgGPA = $cumulativeGPA / $rows;
			$resultString .= "<avggpa>$avgGPA</avggpa>";
			
			
			$resultString .= "</return>";
			
			
		}
		
		echo "$resultString";
			

    
    }
    else if ( $type == 4 )
    {
    	//add/remove users
    
    }
    else if ( $type == 5 )
    {
    	//upload course data
    
    }
    










?>
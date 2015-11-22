<?php


//function to show all sessions in order from most to least recent
function show_sessions( $type, $info, $db)
{
	$found;
	$studentPsid = $info->getPsid();
	
	$result = $db->query( "select * from Sessions where Sessions.psid = '$studentPsid' order by Sessions.year desc,Sessions.month desc,Sessions.day desc,Sessions.hour desc,Sessions.min desc,Sessions.second desc");
	
	$rows = $result->num_rows;
	
	if ( $rows < 1 ) 
	{
		echo "No sessions have been logged for this student: <br/>";
	
	}
	else
	{
		if ( $type > 0 )
		{
			for ( $i = 0; $i < $rows; $i++ )
			{
				$row = $result->fetch_array();
				$dateString = $row['year']."-".$row['month']."-".$row['day']."-".$row['hour']."-".$row['min']."-".$row['second'];
				$psid = $row['psid'];
				$rdo = $psid.":".$dateString;
				echo "Select Session $i: PeopleSoft: $psid Date: $dateString : <input type='radio' value='$rdo' name='rdochoice' /><br/>";
		
			}
		}
		else
		{
			for ( $i = 0; $i < $rows; $i++ )
			{
				$row = $result->fetch_array();
				echo "Select Session $i: PeopleSoft: ". $row['psid']. " Date: ".$row['year']."-".$row['month']."-".$row['day']."-".$row['hour']."-".$row['min']."-".$row['second']."</br>";
		
			}
		
		}
	}
	
	$infoString = serialize( $info );
	$_SESSION['student'] = $infoString;
	
	return;
}

function show_notes( $type, $info, $db)
{
	$found;
	$studentPsid = $info->getPsid();
	
	$result = $db->query( "select * from Notes where Notes.psid = '$studentPsid' order by Notes.year desc, Notes.month desc, Notes.day desc,Notes.hour desc,Notes.min desc,Notes.second desc");
	 
	$rows = $result->num_rows;
	
	if ( $rows < 1 ) 
	{
		echo "No sessions have been logged for this student: <br/>";
	
	}
	else
	{
		if ( $type == 1 )
		{
			for ( $i = 0; $i < $rows; $i++ )
			{
				$row = $result->fetch_array();
				$dateString = $row['year']."-".$row['month']."-".$row['day']."-".$row['hour']."-".$row['min']."-".$row['second'];
				$psid = $row['psid'];
				$rdo = $psid.":".$dateString;
				echo "Select Note $i: PeopleSoft: $psid Date: $dateString : <input type='radio' value='$rdo' name='rdochoice' /><br/>";

			}
		}
		else
		{
			for ( $i = 0; $i < $rows; $i++ )
			{
				echo "Select Note $i: PeopleSoft: ". $row['psid']. " Date: ".$row['date']."</br>";
		
			}
		
		}
	}
	
	$infoString = serialize( $info );
	$_SESSION['student'] = $infoString;
	
	return;
}

function advisor_action()
{

	?>
	<form name = "infoform"
		action = "Assignment2.php"
		method = "POST">
		
		<br/><br/>Choose your preferred action:<br/>
		1.) Log Advising Session<input type='radio' value=1 name='action' /><br/>
		2.) Show Advising Sessions<input type='radio' value=2 name='action' /><br/>
		3.) Add Advising Notes<input type='radio' value=3 name='action' /><br/>
		4.) Review Advising Notes<input type='radio' value=4 name='action' /><br/>
	
		<input type = "submit" value = "Search"><br/>
		<input type = "submit" name = "newstudent" value = "Back To Student Search"><br/><br/>
		<input type = "submit" name = "logoff" value = "Log Out">
		</form>
	<?php
	
	$_SESSION['option'] = "advisoraction";
}

function student_search_a3()
{

	

	return $resultString;
}

function student_search()
{
	?>
		<form name = "infoform"
				action = "userLoggedOn.php"
				method = "POST">

		PeopleSoft ID: <input type = "text" name = "psid"><br/>
		FirstName: <input type = "text" name = "first"><br/>
		LastName: <input type = "text" name = "last"><br/>
		<input type = "submit" value = "Search Student"><br/><br/>
		</form>
	<?php

	echo "</br> Search for a course: </br>";
	?>
		<form name = "infoform"
				action = "userLoggedOn.php"
				method = "POST">

		Department: <input type = "text" name = "dept"><br/>
		Course Number: <input type = "text" name = "coursenum"><br/>
		<input type = "submit" value = "Search Course"><br/><br/>
		<input type = "submit" name = "logoff" value = "Log Out">
		</form>
	<?php
}

function change_password( $userid )
{
	echo "Changing password for $userid</br>";
?>
	<form name = "infoform"
			action = "processLogin.php"
			method = "POST">
	
	Old Password: <input type = "password" name = "oldpass"><br/>
	New Password: <input type = "password" name = "newpass"><br/>
	<input type="hidden" id="id" name="userid" value="<?php echo $userid ?>">
	<br/><input type = "submit" name = "addeduser" value = "Change Password">
<?php




}

function get_info()
{
	
	echo "Please enter your User ID and Password<br/><br/>";
?>
	<form name = "infoform"
			action = "processLogin.php"
			method = "POST"
			align = 'center'>
	
	User ID: <input type = "text" name = "userid"><br/>
	Password: <input type = "password" name = "password"><br/>
	Forget Password<input type = "checkbox" name = "forgetpassword" value = "Forgot Password"><br/><br/>
	Change Password<input type = "checkbox" name = "changepassword" value = "Change Password"><br/><br/>

	<input type = "submit" value = "Log On">
	</form>
<?php

}

function log_out()
{
	?>
		<form name = "infoform"
			action = "Assignment2.php"
			method = "POST">
		<br/><input type = "submit" name = "logoff" value = "Log Out">
		</form>
	<?php
}

function show_header()
{

	?>
	<!DOCTYPE html>
	<html>
	<head>
	<title>CS 1520 PHP Session Demo with Objects</title>
	</head>
	<?php

}

function show_term_classes ( $student, $db )
{
	$psid = $student->getPsid();
	$result = $db->query("select * from Courses where Courses.psid = '$psid' order by Courses.Term");
	
	
	
	//echo "<br/>Here are the classes taken ( alphabetically ), with the respective grades:<br/>";
	
	$rows = $result->num_rows;
	$resultString = '<?xml version="1.0" encoding="utf-8"?>';
	$resultString .="<Student courses='$rows'>";
	for( $i = 0; $i < $rows; $i++ )
	{
		$row = $result->fetch_array();
		$resultString .= "<Course num='$i'><Term>".$row['Term']."</Term><Department>".$row['Department']."</Department><ClassNumber>".$row['Class_Number']."</ClassNumber><Grade>".$row['Grade']."</Grade></Course>";	

	}
	$resultString .="</Student>";
	return $resultString;

}

function show_alpha_classes ( $student, $db )
{
	$psid = $student->getPsid();
	$result = $db->query("select * from Courses where Courses.psid = '$psid' order by Courses.Department, Courses.Class_Number");

	
	
	
	//echo "<br/>Here are the classes taken ( alphabetically ), with the respective grades:<br/>";
	
	$rows = $result->num_rows;
	$resultString = '<?xml version="1.0" encoding="utf-8"?>';
	$resultString .="<Student courses='$rows'>";
	for( $i = 0; $i < $rows; $i++ )
	{
		$row = $result->fetch_array();
		$resultString .= "<Course num='$i'><Term>".$row['Term']."</Term><Department>".$row['Department']."</Department><ClassNumber>".$row['Class_Number']."</ClassNumber><Grade>".$row['Grade']."</Grade></Course>";	

	}
	$resultString .="</Student>";
	return $resultString;


}

function show_reqs_classes ( $student, $db)
{
	$psid = $student->getPsid();
	$coursesTaken = $student->getCourses();
	$requiredCourses = array();
	$electiveCount = 0;
	$resultString = '<?xml version="1.0" encoding="utf-8"?>';
	
	
	for ( $i = 0; $i < count( $coursesTaken ); $i++ )
	{
		$currNumber = $coursesTaken[$i]->getNumber();
		$currDepartment = $coursesTaken[$i]->getDepartment();
		$result = $db->query("select * from Reqs where Reqs.Class_Number = '$currNumber' and Reqs.Department = '$currDepartment'");
		
		$rows = $result->num_rows;
		
		
		
		if ( $rows >0 )
		{
			
			if ( $coursesTaken[$i]->getGPA() >= 2.00 )
			{
				$req = $result->fetch_array();
				
				if ( preg_match ( "/CSElec/", $req['Requirement'] ) )
				{
					if ( $electiveCount < 5 )
					{	
						$electiveCount++;
						
						$courseString = $coursesTaken[$i];
						$requiredCourses["CSElec$electiveCount"] = $courseString;

					}
				
				}
				else
				{
					$courseString =  $coursesTaken[$i];
					$requiredCourses[$req['Requirement']] = $courseString;
				}
			
			}
			else
			{
				
				$courseString = "<requirement>N</requirement><class>".$currDepartment.$currNumber."</class>"
				."<term>N/A</term><grade>N/A</grade><gpa>N/A</gpa>";
				$requiredCourses[$req['Requirement']] = $courseString;
			}
		
		}
		
	}

	$result = $db->query("select distinct Requirement from Reqs");
	
	$rows = $result->num_rows;
	
	
	$resultString .="<requirements courses = '$rows'>";
	for ( $i = 0; $i < $rows; $i++ )
	{
		$req = $result->fetch_array();
		
		$resultString .= "<course>";
		if( array_key_exists( $req['Requirement'], $requiredCourses ) )
		{
			$resultString .= $requiredCourses[$req['Requirement']];
		
		}
		else
		{	
			$resultString .= "<requirement>N</requirement>".
			"<class>".$req['Requirement']."</class>".
			"<term>N/A</term>".
			"<grade>N/A</grade>".
			"<gpa>N/A</gpa>";
		
		}
		
		$resultString .= "</course>";
	
	
	}
	
	$resultString .= "</requirements>";
	return $resultString;


}


function show_reqs ( $student)
{
	$psid = $student->getPsid();
	echo "<br/>Here is the progress on required courses:<br/>";
	$coursesTaken = $student->getCourses();
	$requiredCourses = array();
	$electiveCount = 0;
	for ( $i = 0; $i < count( $coursesTaken ); $i++ )
	{
		$currNumber = $coursesTaken[$i]->getNumber();
		$currDepartment = $coursesTaken[$i]->getDepartment();
		$result = $db->query("select * from Reqs where Reqs.Class_Number = '$currNumber' and Reqs.Department = '$currDepartment'");
		
		$rows = $result->num_rows;
		
		
		
		if ( $rows >0 )
		{
			if ( $coursesTaken[$i]->getGPA() >= 2.00 )
			{
				$req = $result->fetch_array();
				
				if ( preg_match ( "/CSElec/", $req['Requirement'] ) )
				{
					if ( $electiveCount < 5 )
					{	
						$electiveCount++;
						
						$courseString = "S - Req: CSElec".$electiveCount. " ". $coursesTaken[$i];
						$requiredCourses["CSElec$electiveCount"] = $courseString;

					}
				
				}
				else
				{
					$courseString = "S - Req: ".$req['Requirement']. " ". $coursesTaken[$i];
					$requiredCourses[$req['Requirement']] = $courseString;
				}
			
			}
			else
			{
				echo "Course denied: $coursesTaken[$i]</br>";
				$courseString = "N - Req: ".$req['Requirement']."</br>";
				$requiredCourses[$req['Requirement']] = $courseString;
			}
		
		}
		
	}

	$result = $db->query("select distinct Requirement from Reqs");
	
	$rows = $result->num_rows;
	
	for ( $i = 0; $i < $rows; $i++ )
	{
		$req = $result->fetch_array();
		if( array_key_exists( $req['Requirement'], $requiredCourses ) )
		{
			echo $requiredCourses[$req['Requirement']];
		
		}
		else
		{	
			echo "N - " . $req['Requirement'] . "</br>";
		
		}
	
	
	
	}

}

//shows all classes taken in sorted order for a certain psid
function show_classes( $student )
{

	$psid = $student->getPsid();
	$db = new mysqli('localhost', 'BeersA', 'newt.chaps' , 'BeersA');
	
	if ( $db->connect_error):
		die (" Could not connect to db: " . $db->connect_error);
	endif;

	$result = $db->query("select * from Courses where Courses.psid = '$psid' order by Courses.Term");
	
	echo "<br/>Here are the classes taken ( term by term ), with the respective grades:<br/>";
	
	$rows = $result->num_rows;
	for( $i = 0; $i < $rows; $i++ )
	{
		$row = $result->fetch_array();
		echo "Term: ". $row['Term'] . " Class: ".$row['Department'].$row['Class_Number']." Grade: ".$row['Grade']."<br/>";
	
	}

	$result = $db->query("select * from Courses where Courses.psid = '$psid' order by Courses.Department, Courses.Class_Number");

	echo "<br/>Here are the classes taken ( alphabetically ), with the respective grades:<br/>";
	
	$rows = $result->num_rows;
	
	for( $i = 0; $i < $rows; $i++ )
	{
		$row = $result->fetch_array();
		echo "Term: ". $row['Term'] . " Class: ".$row['Department'].$row['Class_Number']." Grade: ".$row['Grade']."<br/>";	
	}
	
	/* Report card approach:
		1. get all the courses that the student has taken
		2. query for the course number and id in the reqs table
		3. store solution in array keyed by the req.requirement
			a.) if the req is an elective then number them until 5
		4.) query distinct for each req
		5.) search req array for the key of the requirement, if found print with grades, else N
	*/
	
	echo "<br/>Here is the progress on required courses:<br/>";
	$coursesTaken = $student->getCourses();
	$requiredCourses = array();
	$electiveCount = 0;
	for ( $i = 0; $i < count( $coursesTaken ); $i++ )
	{
		$currNumber = $coursesTaken[$i]->getNumber();
		$currDepartment = $coursesTaken[$i]->getDepartment();
		$result = $db->query("select * from Reqs where Reqs.Class_Number = '$currNumber' and Reqs.Department = '$currDepartment'");
		
		$rows = $result->num_rows;
		
		
		
		if ( $rows >0 )
		{
			if ( $coursesTaken[$i]->getGPA() >= 2.00 )
			{
				$req = $result->fetch_array();
				
				if ( preg_match ( "/CSElec/", $req['Requirement'] ) )
				{
					if ( $electiveCount < 5 )
					{	
						$electiveCount++;
						
						$courseString = "S - Req: CSElec".$electiveCount. " ". $coursesTaken[$i];
						$requiredCourses["CSElec$electiveCount"] = $courseString;

					}
				
				}
				else
				{
					$courseString = "S - Req: ".$req['Requirement']. " ". $coursesTaken[$i];
					$requiredCourses[$req['Requirement']] = $courseString;
				}
			
			}
			else
			{
				echo "Course denied: $coursesTaken[$i]</br>";
				$courseString = "N - Req: ".$req['Requirement']."</br>";
				$requiredCourses[$req['Requirement']] = $courseString;
			}
		
		}
		
	}

	$result = $db->query("select distinct Requirement from Reqs");
	
	$rows = $result->num_rows;
	
	for ( $i = 0; $i < $rows; $i++ )
	{
		$req = $result->fetch_array();
		if( array_key_exists( $req['Requirement'], $requiredCourses ) )
		{
			echo $requiredCourses[$req['Requirement']];
		
		}
		else
		{	
			echo "N - " . $req['Requirement'] . "</br>";
		
		}
	
	
	
	}
	
	return ;
		
}

function add_remove()
{
	?>
		<form name = "infoform"
			action = "Assignment2.php"
			method = "POST">
		<br/><input type = "submit" name = "add" value = "Add User">
		<br/><input type = "submit" name = "remove" value = "Remove User">
		</form>
	<?php
	add_course();
}

function add_user()
{

	echo "Enter the new users info below:</br>";
	?>
		<form name = "infoform"
			action = "Assignment2.php"
			method = "POST">
		<br/>First Name:<input type = "text" name = "firstname" value = "">
		<br/>Last Name:<input type = "text" name = "lastname" value = "">
		<br/>UserID:<input type = "text" name = "userid" value = "">
		<br/>Password:<input type = "password" name = "password" value = "">
		<br/>PeopleSoftID:<input type = "text" name = "psid" value = "">
		<br/>Email:<input type = "text" name = "email" value = "">
		<br/>Access Level:<input type = "text" name = "access" value = "">
		<br/><input type = "submit" name = "addeduser" value = "Add User">
		</form>
	<?php
$_SESSION['option'] = "added_user";
	

}


function add_course()
{
	?>
	<form action="Assignment2.php" method="post"
	enctype="multipart/form-data">
		Upload Course File:</br>
		<label for="file">Filename:</label>
		<input type="file" name="uploadedfile" ><br>
		<input type="submit" name="submit" value="Submit">
	</form>
	<?php
	

}

function remove_user($db)
{
	$result = $db->query("select distinct * from Users where Users.access_level >= 0 order by Users.UserID");
		
		?>
			<form name = "infoform"
				action = "Assignment2.php"
				method = "POST" >
			<?php
		for( $i = 0; $i < $result->num_rows; $i++ )
		{
		
			$row = $result->fetch_array();
			$psid = $row['psid'];
			echo "Remove User $i: PeopleSoftID: ". $row['psid']. " UserID: ". $row['UserID']. " Email: ". $row['email']. " First Name: ". $row['first_name']. " Last Name: ". $row['last_name']. " access: ". $row['access_level']. " <input type='radio' value='$psid' name='removeduser' /><br/>";
		
		}
		?>
		<input type="submit" value ="Remove">
		</form>
		<?php
}
function show_end()
{
	echo "</html>";

}

//sorts based on term
function sortByTerm($a, $b) 
{
	$aStats = explode ( ":", $a );
	$bStats = explode ( ":", $b );
	
    return strcmp( $aStats[2],$bStats[2] );
}

//sorts based on classname
function sortByAlphabet($a, $b) 
{
    $aStats = explode ( ":", $a );
	$bStats = explode ( ":", $b );
	
    return strcmp( $aStats[0],$bStats[0] );
}

//sorts based on classnumber
function sortByNumber($a, $b) 
{
    $aStats = explode ( ":", $a );
	$bStats = explode ( ":", $b );
	
    return strcmp( $aStats[1],$bStats[1] );
}

//sorts based on date (recent - least recent )
function sortByDate($a, $b) 
{
    $aStats = explode ( ":", $a );
	$bStats = explode ( ":", $b );
	
    return -1 * (strcmp( $aStats[1],$bStats[1] ));
}

//used merge sort here to maintain stability in sorts, used for sorting in all cases in this assignment
function mergesort(&$array, $cmp_function) 
{
    //base case, end
    if ( count( $array ) <= 1 ) 
    {
    	return;
    }
    else
    {
    
		//find middle, get two arrays, recursive call
		$middle = count( $array ) / 2;
		$array1 = array_slice($array, 0, $middle);
		$array2 = array_slice($array, $middle);
	
		mergesort($array1, $cmp_function);
		mergesort($array2, $cmp_function);
		
		//if the arrays are sorted
		if ( call_user_func( $cmp_function, end( $array1 ), $array2[0] ) < 1 ) 
		{
			$array = array_merge($array1, $array2);
			return;
		}
		else
		{

			//populate the array
			$array = array();
			$currIndex1 = 0;
			$currIndex2 = 0;
			
			//while both arrays still have values
			while ($currIndex1 < count( $array1 ) && $currIndex2 < count($array2) ) 
			{
			
				// call my comparison function to sort them
				if (call_user_func( $cmp_function, $array1[$currIndex1], $array2[$currIndex2] ) < 1) 
				{
					$array[] = $array1[$currIndex1++];
				}
				else 
				{
					$array[] = $array2[$currIndex2++];
				}
			}
			
			// find out which array still has values, and fill the rest in
			if ( $currIndex1 < count( $array1 ) )
			{
			
				while ( $currIndex1 < count($array1) ) 
				{
					$array[] = $array1[$currIndex1++];
				}
				
			}
			else
			{
			
				while ( $currIndex2 < count($array2) ) 
				{
					$array[] = $array2[$currIndex2++];
				}
				
			}
		}
		return;
	}
}

?>
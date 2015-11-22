<?php
// CS 1520 Assignment 1 Andrew Beers 6/9/12
// load classes needed

function __autoload( $class )
{

	require_once $class . '.php';
}

session_start();

date_default_timezone_set( 'America/New_York' );

//get the option from the previous page
if ( isset( $_SESSION['option'] ) )
{
	$option = $_SESSION['option'];
}

if ( isset( $_POST['newstudent'] ) )
{
	$option = "logged_on";
}

//if we've been to a previous site before
if ( isset( $option ) )
{
	//if we logged off
	if ( isset( $_POST['logoff'] ) )
	{

		echo "Successfully Logged Off. <br/>";
		unset( $_SESSION["option"] );
		unset( $_SESSION["info"] );
		session_destroy();
	}
	elseif ( strcmp( $option, 'logging_in' ) == 0 )	//if we just logged in
	{
	
		if ( !empty($_POST['userid']) ) //a user id was entered
		{
			if ( !empty( $_POST['password'] ) )	//a password was entered
			{
				$data = file("users.txt");
				$again = true;
				
				//search the file for the userid entered
				for ($i = 0; $i < count($data) && $again == true; $i++):
			
					$line = explode( ":" , $data[$i] );
				
					//if we match, create a student object with the info from the file
					if ( $line[0] === $_POST['userid'] && $line[1] === $_POST['password'] ):
					
						$again = false;
						$info = new Student( $line[0] , $line[1], $line[2], $line[3], $line[4], $line[5], $line[6] );
						$infoString = serialize( $info );
						$_SESSION['info'] = $infoString;
					
					
					endif;
				
				endfor;
			
				//if we matched, and we are a student show the info
				if ( $again == false && $info->getAccess() == 0):
			
					$_SESSION['option'] = "logged_on";
					$coursesTaken = show_classes( $info->getPsid());
					show_requirements( $coursesTaken );
					log_out();
				
				//if we matched, and we are an advisor. show the info and search option
				elseif( $again == false && $info->getAccess() == 1 ):
			
					echo "Search For a Student: <br/><br/>";
					$_SESSION['option'] = "searchstudent";
					student_search();

				
				//no match, try again
				else:
			
					show_header();
					echo "User ID and password do not match. Please enter again." ;
					get_info();
					show_end();
			
				endif;
			}
			elseif ( !empty( $_POST['forgetpassword'] ) )
			{
				$data = file("users.txt");
				$again = true;
				//search the file for the userid entered
				for ($i = 0; $i < count($data) && $again == true; $i++):
			
					$line = explode( ":" , $data[$i] );
				
					//if we match, create a student object with the info from the file
					if ( $line[0] === $_POST['userid'] ):
					
						$again = false;
						
						mail( $line[3], "Password Reminder", $line[1] ); //email the password
					
					
					endif;
				
				endfor;
				if ($again == false )
				{
					echo "Email has been sent<br/>";
				}
				else
				{
					echo "Username not found<br/>";
				}
				
				show_header();
				get_info();
				show_end();
			}
			else
			{
			
				//no password entered
				show_header();
				echo "No Password entered, please enter your User ID and password." ;
				get_info();
				show_end();
			
			}
		
		}
		else
		{
			//no id entered
			show_header();
			echo "No User ID entered, please enter your User ID and password." ;
			get_info();
			show_end();
		
		}
		
	
	
	}
	elseif ( strcmp( $option, 'logged_on' ) == 0 )	//here the user made a choice
	{
		$info = unserialize ( $_SESSION['info'] );
		
		//we've logged on and are an advisor
		if ( $info->getAccess() == 1 )//no action was made, show info and search again
		{
			echo "Search For a Student: <br/><br/>";
			$_SESSION['option'] = "searchstudent";
			
			$infoString = serialize( $info );
			$_SESSION['info'] = $infoString;
			
			student_search();
		
		}
		else	//we've logged on and are a student 
		{
			
			show_header();
	
			$coursesTaken = show_classes( $info->getPsid() );
			show_requirements( $coursesTaken );
			
			log_out();
	
			show_end();
		}

		
	}
	elseif ( strcmp( $option, 'searchstudent' ) == 0 ) //the advisor has searched for a student
	{
		$info = unserialize ( $_SESSION['info'] );
		$infoString = serialize( $info );
		$_SESSION['info'] = $infoString;
		//if the advisor searched by first/last name
		if ( !empty( $_POST['first'] ) && !empty( $_POST['last'] ) )
		{
			echo "Looking for name<br/>";
			$userFile = file("users.txt");
		
			$found = false;
			// search the users file for the first and last name
			for ( $i = 0; $i < count( $userFile ) && $found == false; $i++ ):
			
				$userInfo = explode( ":", $userFile[$i] );
				
				//if found, create a new student object from the info in the file
				if ( ( strcmp( $_POST['first'], $userInfo[5] ) == 0 ) && ( strcmp( $_POST['last'], $userInfo[4] ) == 0 ) )
				{
					$found = true;
					
					$info = new Student( $userInfo[0] , $userInfo[1], $userInfo[2], $userInfo[3], $userInfo[4], $userInfo[5], $userInfo[6] );
					$infoString = serialize( $info );
					$_SESSION['student'] = $infoString;
					$coursesTaken = show_classes( $userInfo[2] );
					show_requirements( $coursesTaken );
			
				}
			
		
			endfor;
			
			if ( $found == false )
			{
				echo "First / Last name not found, search again<br/>";
				student_search();//prompt search again, didnt find psid
			}
			else
			{
				advisor_action();
			}
		
	
		}
		elseif ( !empty( $_POST['psid'] ))	//if the advisor searched by psid
		{
			$userFile = file("users.txt");
			$psid = $_POST['psid'];
			echo "Seaching for psid of $psid<br/>";
			$found = false;
		
			//search for the psid in the users file
			for ( $i = 0; $i < count( $userFile ) && $found == false; $i++ ):
			
				$userInfo = explode( ":", $userFile[$i] );
			
				//if found, create a new student object from the info in the file
				if ( $userInfo[2] == $_POST['psid'] )
				{
					$found = true;
					
					
					$info = new Student( $userInfo[0] , $userInfo[1], $userInfo[2], $userInfo[3], $userInfo[4], $userInfo[5], $userInfo[6] );
					$infoString = serialize( $info );
					$coursesTaken = show_classes( $userInfo[2] );
					show_requirements( $coursesTaken );
					$_SESSION['student'] = $infoString;
			
				}
			
		
			endfor;
			if ( $found == false )
			{
				echo "PSID not found, search again<br/>";
				student_search();//didnt find psid
				
			}
			else
			{
				advisor_action(); //if found prompt for an action
			}
		
		}
		else
		{	
			echo "Error occured, perhaps you only entered one of the names. Enter again.<br/>";
			echo "Search For a Student: <br/><br/>";
			student_search();//prompt user again, didnt find matching first/last names
	
		}

	}
	elseif ( strcmp( $option, 'advisoraction' ) == 0 ) //the advisor has made an action about the student
	{
		$studentPsid;

		if ( isset( $_POST['action'] ) )	//make sure its set
		{
			$actionvalue = $_POST['action']; //grab value
			
			
			if ( $actionvalue != 1 && $actionvalue !=2 && $actionvalue != 3 && $actionvalue != 4 )
			{
				echo "Error occured try again. Type 1, 2, 3 or 4.<br/>";
				advisor_action();	//previous action value invalid, prompt again
			}
			else
			{
				//find out which value was set
				if ( $actionvalue == 1 )
				{
					//action 1- get time, add psid:time to the sessions.txt file
					$_SESSION['option'] = "studentinfo";
					$currDate = date("Y-m-d-H-i-s");
					$info = unserialize( $_SESSION['student'] );
			
					$studentPsid = $info->getPsid();
			
					$printString = "$studentPsid:$currDate\n";
			
					$fp = fopen("sessions.txt", "a");   //open file for appending
			
						fwrite( $fp, $printString ); //append date and psid
					fclose($fp);
			
					echo "Session logged for $studentPsid at $currDate<br/>";
			
					$infoString = serialize( $info );
					$_SESSION['student'] = $infoString;
		
				}
				elseif ( $actionvalue == 2 )
				{
					$_SESSION['option'] = "studentinfo";
					$info = unserialize( $_SESSION['student'] );
			
					$found = show_sessions( 0 , $info);	//show all sessions without radio buttons
		
				}
				elseif ( $actionvalue == 3 || $actionvalue == 4)
				{
			
					$_SESSION['option'] = "studentinfo";
					$info = unserialize( $_SESSION['student'] );
			
					$found = show_sessions( 1 , $info);	//show all sessions with radio buttons
			
				}
				else
				{
					echo "Error occured try again<br/>";
					advisor_action();	//error, prompt again
		
				}
				
				if ( ($actionvalue == 1 || $actionvalue == 2 ) || $found == 0 )
				{
					//nothing else to do here, go back to studentinfo for another action
					?>
					<form name = "infoform"
							action = "Assignment1.php"
							method = "POST">
						<input type="hidden" name="psid" value="$studentPsid">
						<input type="submit" value ="Back to Student Info">
					</form>
					<?php
				}
				elseif ( $actionvalue == 3 )
				{
					$_SESSION['option'] = "addnotes"; 	//go to add notes
				}
				else
				{
					$_SESSION['option'] = "reviewnotes";	//go to review notes
				}
			}
			
		}
		else
		{
			echo "Error occured try again<br/>";
			advisor_action();
		}

	}
	elseif ( strcmp( $option, 'studentinfo' ) == 0 ) //show student info and the advisor action
	{
		$info = unserialize( $_SESSION['student'] );
	
		$coursesTaken = show_classes( $info->getPsid() );
		show_requirements( $coursesTaken );	
		advisor_action();	
	}
	elseif ( strcmp( $option, 'reviewnotes' ) == 0 ) // show advisor all notes from the selected session
	{	
		if ( isset( $_POST['rdochoice'] ) )
		{
			$radiovalue = $_POST['rdochoice'];
			$studentSessions = unserialize ( $_SESSION['sessions'] );
		
			$fileName = $studentSessions[$radiovalue];
		
			if ( file_exists( "notes/$fileName.txt" ) )
			{
				$notes = file("notes/$fileName.txt");
		
		
				for( $i = 0; $i < count( $notes ); $i++ )
				{
					echo "$notes[$i]<br/>";
				}
			}
			else
			{
				echo "No notes have been logged about this session<br/>";
			
			}
		}
		else
		{
			echo "No selection made<br/>";
		}
		
		?>
			<form name = "infoform"
					action = "Assignment1.php"
					method = "POST">
				<input type="hidden" name="psid" value="$studentPsid">
				<input type="submit" value ="Back to Student Info">
			</form>
		<?php

		$_SESSION['option'] = "studentinfo";
	
	
	}
	elseif ( strcmp( $option, 'addnotes' ) == 0 )// allow advisor to add notes for the selected session
	{
	
		if ( isset( $_POST['rdochoice'] ))
		{
			$radiovalue = $_POST['rdochoice'];
			$studentSessions = unserialize ( $_SESSION['sessions'] );
		

		
			$fileName = $studentSessions[$radiovalue];
		
			echo "Please type your note:<br/>";
			?>
				<form name = "infoform"
						action = "Assignment1.php"
						method = "POST">
					<input type="text" name="note" ><br/>
					<input type="submit" value ="Log Note">
				</form>
			<?php
		
		
			$_SESSION['filename'] = $fileName;
		
			$_SESSION['option'] = "notesadded";
		}
		else
		{
			$_SESSION['option'] = "studentinfo";
			echo "No selection made<br/>";
			
			?>
			<form name = "infoform"
					action = "Assignment1.php"
					method = "POST">
				<input type="hidden" name="psid" value="$studentPsid">
				<input type="submit" value ="Back to Student Info">
			</form>
			<?php
			
		
		
		}
		
	
	}
	elseif ( strcmp( $option, 'notesadded' ) ==0 )	//confirm the note was added or not
	{
	
		if ( !empty( $_POST['note'] ) )
		{
			$stringVal = $_POST['note'];
			$fileName = $_SESSION['filename'];
		
			$fp = fopen("notes/$fileName.txt", "a");   // Open file for writing
				
				fwrite( $fp, "$stringVal\n" );
			fclose($fp);
		
			echo "Note has been logged.<br/>";
		}
		else
		{
			echo "No message received.<br/>";
		
		
		}
			$_SESSION['option'] = "studentinfo";
		
		?>
			<form name = "infoform"
					action = "Assignment1.php"
					method = "POST">
				<input type="hidden" name="psid" value="$studentPsid">
				<input type="submit" value ="Back to Student Info">
			</form>
		<?php
	
	}
}
else	//cookie doesn't exist yet
{
	$_SESSION['option'] = 'logging_in';
	show_header();
	get_info();
	show_end();
}


//function to show all sessions in order from most to least recent
function show_sessions( $type, $info )
{
	$found;
	$studentPsid = $info->getPsid();
	if ( file_exists( "sessions.txt" ) )
	{

		$sessions = file( "sessions.txt" );
		$studentSessions = array();

		//add all sessions for this user to an array
		for( $i = 0; $i < count( $sessions ); $i++ )
		{
			$fileLine = explode( ":", $sessions[$i] );
			if ( $fileLine[0] == $studentPsid )
			{
				array_push( $studentSessions, trim( $sessions[$i] ) );
	
			}

		}

		//sort
		mergesort( $studentSessions , 'sortByDate' );

		if ( count ($studentSessions) > 0 )
		{
			//print out the array
			echo "Here are the logged sessions for this student: <br/>";
			for( $i = 0; $i < count( $studentSessions ); $i++ )
			{
				echo "Session $i: $studentSessions[$i]<br/>";

			}
	
			if ( $type == 1 )//if we want radio buttons
			{
				?>
					<form name = "infoform"
						action = "Assignment1.php"
						method = "POST" >
				<?php
				for( $i = 0; $i < count( $studentSessions ); $i++ )
				{
	
					
					echo "Select Session $i<input type='radio' value='$i' name='rdochoice' /><br/>";

				}
				?>
				<input type="submit" value ="Search">
				</form>
				<?php
			}
			$found = 1;
		}
		else
		{
			echo "No sessions have been logged for this student: <br/>";
			$found = 0;
		}

		$sessionsString = serialize ( $studentSessions );
		$_SESSION['sessions'] = $sessionsString;

	}
	else
	{

		echo "No sessions have been logged for this student: <br/>";
	}
	$infoString = serialize( $info );
	$_SESSION['student'] = $infoString;
	
	return $found;
}

function advisor_action()
{
	echo "<br/><br/>Choose your preferred action:<br/>";
	echo "1.) Log Advising Session<br/>";
	echo "2.) Show Advising Sessions<br/>";
	echo "3.) Add Advising Notes<br/>";
	echo "4.) Review Advising Notes<br/>";
	
	?>

	<form name = "infoform"
		action = "Assignment1.php"
		method = "POST">

		Action Number: <input type = "text" name = "action"><br/>
		
		<input type = "submit" value = "Search"><br/>
		<input type = "submit" name = "newstudent" value = "Back To Student Search"><br/><br/>
		<input type = "submit" name = "logoff" value = "Log Out">
		</form>
	<?php
	
	$_SESSION['option'] = "advisoraction";
}

function student_search()
{
	?>
		<form name = "infoform"
				action = "Assignment1.php"
				method = "POST">

		PeopleSoft ID: <input type = "text" name = "psid"><br/>
		FirstName: <input type = "text" name = "first"><br/>
		LastName: <input type = "text" name = "last"><br/>
		<input type = "submit" value = "Search"><br/><br/>
		<input type = "submit" name = "logoff" value = "Log Out">
		</form>
	<?php

}

function get_info()
{
	
	echo "Please enter your User ID and Password<br/><br/>";
?>
	<form name = "infoform"
			action = "Assignment1.php"
			method = "POST">
	
	User ID: <input type = "text" name = "userid"><br/>
	Password: <input type = "text" name = "password"><br/>
	Forget Password<input type = "checkbox" name = "forgetpassword" value = "Forgot Password"><br/><br/>
	<input type = "submit" value = "Log On">
	</form>
<?php

}

function log_out()
{
	?>
		<form name = "infoform"
			action = "Assignment1.php"
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

//shows all requirements for graduating for a student
function show_requirements( $coursesTaken )
{

	$reqs = file("reqs.txt");
	echo "<br/>Here is the progress on required courses:<br/>";
	
	$electives = array();
	for ($i = 0; $i < count( $reqs ); $i++ ):
	
		$reqFields = explode ( ":" , $reqs[$i] ); // separate class name from classes
		$classFields = explode ("|", $reqFields[1] ); // separate classes (for electives)
		
		$again = true;
		for ($j = 0; $j < count( $classFields ) && $again == true; $j++ ):
		
			$class = explode ( ",", $classFields[$j] );
			
			
			for ($k = 0; $k < count($coursesTaken) && $again == true; $k++):
	
				$studentClassInfo = explode( ":" ,$coursesTaken[$k] );
				
				$classNumber1 = trim( $studentClassInfo[1] );
				$classNumber2 = trim( $class[1] );

				
				//for handling repeated electives
				if ( ( strcmp( $class[0], $studentClassInfo[0] ) == 0 ) &&  ( $classNumber1 == $classNumber2 )  )
				{
					//match class
					$again = false;
					if ( preg_match( "/Elec/", $reqFields[0] ) )
					{
						if ( in_array( $classNumber1, $electives ) )
						{
							$again = true;
						}
						else
						{
							array_push( $electives, $classNumber1 );
						}
					
					}
				}
				
			endfor;
			
		endfor;
		
		if ( $again == true )
		{
			echo "N - $reqFields[0]<br/>";
		}
		else
		{
			if ( strcmp( $studentClassInfo[4][0], "C" ) < 0 )
			{
				echo "S - Class: $studentClassInfo[0]$studentClassInfo[1] Term: $studentClassInfo[2] Grade: $studentClassInfo[4] - $reqFields[0]<br/>";
			}
			elseif ( strcmp( $studentClassInfo[4][0], "C" ) == 0 )
			{
				if ( strcmp( $studentClassInfo[4][1], "-") == 0 )
				{
					echo "N - $reqFields[0]<br/>";

				}
				else
				{
					echo "S - Class: $studentClassInfo[0]$studentClassInfo[1] Term: $studentClassInfo[2] Grade: $studentClassInfo[4] - $reqFields[0]<br/>";

				}
			}
			else
			{
				echo "N - $reqFields[0]<br/>";
			}
		}
	endfor;

}

//shows all classes taken in sorted order for a certain psid
function show_classes( $psid )
{

	$info = unserialize ( $_SESSION['info'] );
	$name = $info->getName();

	//student

	$course = file("courses.txt");
	$coursesTaken = array();
	for ($i = 0; $i < count($course); $i++):

		$courseInfo = explode( ":", $course[$i] );
	
		if ( $psid == $courseInfo[3] ):
		
			array_push( $coursesTaken, $course[$i] );
	
		endif;

	endfor;

	$arrayObject = new ArrayObject( $coursesTaken );
	mergesort( $coursesTaken , 'sortByNumber' );

	mergesort( $coursesTaken , 'sortByAlphabet' );

	mergesort( $coursesTaken , 'sortByTerm' );
	echo "<br/>Here are the classes taken, with the respective grades:<br/>";

	for ($i = 0; $i < count($coursesTaken) ; $i++):

		$studentInfo = explode( ":" ,$coursesTaken[$i] );
		echo "Term: $studentInfo[2] Class: $studentInfo[0]$studentInfo[1] Grade: $studentInfo[4]<br/>";

	endfor;

	return $coursesTaken;
		
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
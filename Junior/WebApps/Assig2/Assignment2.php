<?php
// CS 1520 Assignment 2 Andrew Beers 6/28/12
// load classes needed

session_start();

function __autoload( $class )
{

	require_once $class . '.php';
}

include 'A2Functions.php';	//include functions

$pword = file("quotes3.txt");
$pass = rtrim( $pword[0] );
$db = new mysqli('localhost', 'BeersA', $pass , 'BeersA');
	
if ( $db->connect_error):
	die (" Could not connect to db: " . $db->connect_error);
endif;

date_default_timezone_set( 'America/New_York' );

//get the option from the previous page
if ( isset( $_SESSION['option'] ) )
{
	$option = $_SESSION['option'];
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
	elseif (isset( $_POST['add'] ) )
	{
		add_user();
		log_out();
	}
	elseif (isset( $_POST['remove'] ) )
	{
		remove_user($db);
		$_SESSION['option'] = "removed_user";
		log_out();
		
	}
	elseif (  isset($_FILES['uploadedfile']['tmp_name']) && is_uploaded_file($_FILES['uploadedfile']['tmp_name']) )
	{
		
		$file = $_FILES['uploadedfile']['tmp_name'];
		//gpa array for incoming courses
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
		$courses = file( $file );
		
		//see how many courses
		$result = $db->query("select * from Courses");
		
		$courseid = $result->num_rows + 1;
		$badfile = false;
		
		//add courses
		for ( $i = 0; $i < count($courses) && $badfile == false; $i++)
		{
			$courseString = $courses[$i];
			rtrim( $courseString );
			$course = preg_split("/:/", $courseString );
			$grade = preg_replace('/\s+/', '', $course[4]);
			$gpa =  $gpas[$grade];
			
			//insert
			$query = "insert into Courses values ('$courseid','$course[3]','$course[0]','$course[1]','$course[2]','$course[4]', ". $gpas[$grade] . ")";

			$courseid++;
			$result = $db->query($query);
			
			if ( $result == false )
			{
				$badfile = true;
			}
			
		}
		
		if ( $badfile == false )
		{
		
			echo "Courses added.</br>";
		}
		else
		{
			echo "Incorrectly formatted file.</br>";
		}
		
		$info = unserialize( $_SESSION['info'] );
		
		//only admin or advisor can be here, so we send them back
		if( $info->getAccess() == 1 ):

			echo "</br>Search For a Student: <br/><br/>";
			$_SESSION['option'] = "searchstudent";
			student_search();
		
		elseif( $info->getAccess() == 2 ):
			echo "</br>Search For a Student: <br/><br/>";
			$_SESSION['option'] = "searchstudent";
			student_search();
			add_remove();
			
		endif;
		
		
	}
	elseif (  strcmp( $option, 'removed_user' ) == 0 )
	{
		//if remove user
		if ( isset( $_POST['removeduser'] ) )
		{
		
			$psid = $_POST['removeduser'] ;
			//change level to -1
			$result = $db->query("update Users set Users.access_level = '-1' where Users.psid = '$psid'");
			
			
			if( $result == false )
			{
				echo "An error occured</br>";
			}
			else
			{
				echo "Removed user $psid.</br>";
			}
		
		}
			?>
	<form name = "infoform"
		action = "Assignment2.php"
		method = "POST">
		
		
		<input type = "submit" name = "newstudent" value = "Back To Student Search"><br/><br/>
		<input type = "submit" name = "logoff" value = "Log Out">
		</form>
	<?php

			$_SESSION['option'] = "searchstudent";
	
	}
	elseif ( strcmp( $option, 'added_user' ) == 0  )
	{
		//add user
		if( isset( $_POST['firstname'] ) && isset( $_POST['lastname'] ) && isset( $_POST['userid'] ) && isset( $_POST['password'] ) && isset( $_POST['email'] ) && isset( $_POST['psid'] )  && isset( $_POST['access'] ) )
		{
		
			//make sure everything is set, hash password
			$first = $_POST['firstname'];
			$last = $_POST['lastname'];
			$userid = $_POST['userid'];
			$pass = hash('sha256',$_POST['password']);
			$email = $_POST['email'];
			$psid = $_POST['psid'];
			$access = $_POST['access'];
			
			$result = $db->query("select * from Users where Users.UserID = '$userid'");
			//find off userid
			//if -1, reactivate
			if ( $result->num_rows > 0)
			{	
				$row = $result->fetch_array();
				echo "User updated</br>";
				if ( $row['access_level'] == -1 )
				{
					$result = $db->query("update Users set Users.psid = '$psid', Users.password= '$pass', Users.email ='$email', Users.first_name= '$first', Users.last_name='$last', Users.access_level ='$access' where Users.UserID = '$userid'");
					$_SESSION['option'] = "searchstudent";
				}
				else
				{
					echo "Duplicate userid found, please reenter.</br>";
					$_SESSION['option'] = "added_user";
					add_user();
					log_out();	
				}
			
				
			}
			else
			{
			//find off psid
				$result = $db->query("select * from Users where Users.psid = '$psid'");

				if ( $result->num_rows > 0)
				{
					//if -1, reactivate
					$row = $result->fetch_array();
					if ( $row['access_level'] == -1 )
					{
						echo "User updated</br>";
						
						$result = $db->query("update Users set Users.UserID = '$userid', Users.password= '$pass', Users.email ='$email', Users.first_name= '$first', Users.last_name='$last', Users.access_level ='$access' where Users.psid = '$psid'");
						
						$_SESSION['option'] = "added_user";
						?>
						<form name = "infoform"
						action = "Assignment2.php"
						method = "POST">


						<input type = "submit" name = "newstudent" value = "Back To Student Search"><br/><br/>
						<input type = "submit" name = "logoff" value = "Log Out">
						</form>
						<?php
						
					}
					else
					{
						echo "Duplicate userid found, please reenter.</br>";
						$_SESSION['option'] = "added_user";
						add_user();
						log_out();	
					}
				}
				else
				{
					//add
					$result = $db->query("insert into Users values( '$psid', '$userid', '$pass', '$email', '$first', '$last', '$access')");
			
					if ( $result == true )
					{
						echo "Successfully added new user.</br>";
						
						?>
						<form name = "infoform"
						action = "Assignment2.php"
						method = "POST">


						<input type = "submit" name = "newstudent" value = "Back To Student Search"><br/><br/>
						<input type = "submit" name = "logoff" value = "Log Out">
						</form>
						<?php
			
					}
					else
					{
						echo "An error occurred.</br>";
						?>
						<form name = "infoform"
						action = "Assignment2.php"
						method = "POST">


						<input type = "submit" name = "newstudent" value = "Back To Student Search"><br/><br/>
						<input type = "submit" name = "logoff" value = "Log Out">
						</form>
						<?php
					}
				}
			}
					
	
		}
		else
		{
			$_SESSION['option'] = "added_user";
			add_user();
			log_out();
		
		}
		
	}
	elseif (isset( $_POST['newstudent'] ) )//back to student search
	{
		echo "Search For a Student: <br/><br/>";
		$_SESSION['option'] = "searchstudent";
		student_search();
		add_remove();
		
	}
	elseif ( ( strcmp( $option, 'logged_on' ) == 0 ) || ( strcmp( $option, 'searchstudent' ) == 0 ) )	//here the user made a choice
	{
		
		header("Location: userLoggedOn.php");
	}
	elseif ( strcmp( $option, 'logging_in' ) == 0 ) //the advisor has searched for a student
	{

		header("Location: processLogin.php");
	
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
					$date = date("Y-m-d-H-i-s");
					$currDate = explode( "-" , $date );
					$info = unserialize( $_SESSION['info'] );
					
					$student = $info->getStudent();
					$studentPsid = $student->getPsid();
					
					$result = $db->query("select 1 from Sessions");
					//sessions table
					if( $result == false)
					{
						$db -> query( "create table Sessions (psid int not null, year int not null, month int not null, day int not null, hour int not null, min int not null , second int not null)");
					}
					//add session
					$db -> query( "insert into Sessions values('$studentPsid', '$currDate[0]', '$currDate[1]', '$currDate[2]', '$currDate[3]', '$currDate[4]', '$currDate[5]')") or die ("invalid insert " . $db->error);
						
			
					echo "Session logged for $studentPsid at $date<br/>";
			
					$infoString = serialize( $info );
					$_SESSION['info'] = $infoString;
		
				}
				elseif ( $actionvalue == 2 )
				{
					$_SESSION['option'] = "studentinfo";
					$info = unserialize( $_SESSION['info'] );
					$student = $info->getStudent();
					
					$result = $db->query("select 1 from Sessions");
					
					if( $result == false)
					{
						echo "No Sessions logged</br>";
					
					}
					else
					{
						//show sessions if it exists
						$found = show_sessions( 0 , $student, $db);	//show all sessions without radio buttons
					}
		
				}
				elseif ( $actionvalue == 3)
				{
			
					$_SESSION['option'] = "studentinfo";
					$info = unserialize( $_SESSION['info'] );
					$student = $info->getStudent();
					?>
					<form name = "infoform"
					action = "Assignment2.php"
					method = "POST">
					<?php
			
					$found = show_sessions( 2 , $student, $db);	//show all sessions with radio buttons
			
					?>
						<input type="submit" value ="Add Note">
					</form>
					<?php
					$_SESSION['option'] = "addnotes"; 	//go to add notes
					
				}
				elseif ( $actionvalue == 4 )
				{
				
					$_SESSION['option'] = "studentinfo";
					$info = unserialize( $_SESSION['info'] );
					$student = $info->getStudent();
					$result = $db->query("select 1 from Notes");
					
					if( $result == false)
					{
						echo "No Notes logged</br>";
						?>
							<form name = "infoform"
									action = "Assignment2.php"
									method = "POST">
								<input type="submit" value ="Back to Student Info">
							</form>
						<?php
						$_SESSION['option'] = "studentinfo";
					}
					else
					{
						?>
							<form name = "infoform"
							action = "Assignment2.php"
							method = "POST">
						<?php
						$found = show_notes( 1 , $student, $db);	//show all sessions with radio buttons
						?>
							<input type="submit" value ="Review Note">
						</form>
						<?php
						$_SESSION['option'] = "reviewnotes";	//go to review notes
					}
				}
				else
				{
					echo "Error occured try again<br/>";
					advisor_action();	//error, prompt again
		
				}
				
				if ( ($actionvalue == 1 || $actionvalue == 2 ) )
				{
					//nothing else to do here, go back to studentinfo for another action
					?>
					<form name = "infoform"
							action = "Assignment2.php"
							method = "POST">
						<input type="hidden" name="psid" value="$studentPsid">
						<input type="submit" value ="Back to Student Info">
					</form>
					<?php
					$info = unserialize( $_SESSION['info'] );
					$infoString = serialize( $info );
					$_SESSION['info'] = $infoString;
					
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
		$info = unserialize( $_SESSION['info'] );
		
		$student = $info->getStudent();
		$access = $student->getAccess();
		$coursesTaken = show_classes( $student );
		show_requirements( $coursesTaken );	
		advisor_action();	
	}
	elseif ( strcmp( $option, 'reviewnotes' ) == 0 ) // show advisor all notes from the selected session
	{	
		if ( isset( $_POST['rdochoice'] ) )
		{
			$fileName = $_POST['rdochoice'].".txt";
			
		
			if ( file_exists( "notes/$fileName" ) )
			{
				$notes = file("notes/$fileName");
		
		
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
					action = "Assignment2.php"
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
			$fileName = $_POST['rdochoice'].".txt";
			
		//add note
			echo "Please type your note:<br/>";
			?>
				<form name = "infoform"
						action = "Assignment2.php"
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
					action = "Assignment2.php"
					method = "POST">
				<input type="hidden" name="psid" value="$studentPsid">
				<input type="submit" value ="Back to Student Info">
			</form>
			<?php
			
		
		
		}
		
	
	}
	elseif ( strcmp( $option, 'notesadded' ) == 0 )	//confirm the note was added or not
	{
	
		if ( !empty( $_POST['note'] ) )
		{
			$stringVal = $_POST['note'];
			$fileName = $_SESSION['filename'];
			
			$data = explode ( ":", $fileName );
			
			$date = explode ("-", $data[1] );
			$result = $db->query("select 1 from Notes");
					
			//create notes table
			if( $result == false)
			{
				$db -> query( "create table Notes (psid int not null, year int not null, month int not null, day int not null, hour int not null, min int not null , second int not null)");
			}
			
			$db-> query("insert into Notes values( '$data[0]', '$date[0]', '$date[1]', '$date[2]', '$date[3]', '$date[4]', '$date[5]')");
			
			
			
			$fp = fopen("notes/$fileName", "a");   // Open file for writing
				
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
					action = "Assignment2.php"
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


?>
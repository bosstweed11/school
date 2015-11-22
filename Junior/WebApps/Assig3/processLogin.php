<link rel = "stylesheet" type = "text/css" href = "A3Style.css"/>
<?php
// CS 1520 Assignment 2 Andrew Beers 6/28/12
// load classes needed

session_start();


$db = new mysqli('localhost', 'BeersA', "newt.chaps" , 'BeersA');
	
if ( $db->connect_error):
	die (" Could not connect to db: " . $db->connect_error);
endif;


function __autoload( $class )
{

	require_once $class . '.php';
}
include 'A2Functions.php';
require_once 'A2User.php';

date_default_timezone_set( 'America/New_York' );



if ( isset( $_SESSION['option'] ) )
{
	$option = $_SESSION['option'];
	//echo "session option : $option<br/>";
}
else
{
	echo "no session option<br/>";
}





//if we've been to a previous site before
if ( isset( $option ) )
{

	if ( strcmp( $option, 'logging_in' ) == 0 )
	{
		if ( !empty($_POST['userid']) ) //a user id was entered
		{
			if ( !empty( $_POST['password'] ) )	//a password was entered
			{
				$pass = hash('sha256',$_POST['password']);
				$result = $db->query( "select * from Users where Users.UserID = '$_POST[userid]' and Users.password = '$pass' and Users.access_level >= 0");
			
			
				$rows = $result->num_rows;
			
				if ( $rows < 1 )
				{
					show_header();
					echo "User ID and password do not match. Please enter again. You may have been removed from the system</br>" ;

					get_info();
					show_end();
			
				}
				else
				{
					$row = $result->fetch_array();
					$info = new A2User( $row['UserID'] , $row['password'], $row['psid'], $row['email'], $row['last_name'], $row['first_name'], $row['access_level'] );
					
					
					if ( $info->getAccess() == 0):
						// no more refreshes.
						$_SESSION['option'] = "logged_on";
						/*
						$result = $db->query("select * from Courses where Courses.psid = ".$info->getPsid());
				
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
						
						
						$coursesTaken = show_classes( $info );
						show_requirements( $coursesTaken );
						log_out();
						*/
						$psid = $info->getPsid();
						$access = $info->getAccess();
						$_SESSION['psid'] = $psid;
						$_SESSION['access'] = $access;
						header("Location: A3Main.php");
						
						
						
	
					//if we matched, and we are an advisor. show the info and search option
					elseif( $info->getAccess() == 1 ):

						//echo "Search For a Student: <br/><br/>";
						$_SESSION['option'] = "searchstudent";
						//student_search();
						$psid = $info->getPsid();
						$access = $info->getAccess();
						$_SESSION['psid'] = $psid;
						$_SESSION['access'] = $access;
						header("Location: A3Main.php");
					
					elseif( $info->getAccess() == 2 ):
						//echo "Search For a Student: <br/><br/>";
						$_SESSION['option'] = "searchstudent";
						//student_search();
						//add_remove();
						$psid = $info->getPsid();
						$access = $info->getAccess();
						$_SESSION['psid'] = $psid;
						$_SESSION['access'] = $access;
						header("Location: A3Main.php");
					
					else:
						echo "You have been removed from the system. Contact the admin</br>";
						session_destroy();
				
					
					endif;
				
					$infoString = serialize( $info );
					$_SESSION['info'] = $infoString;
			
				}
				//if we matched, and we are a student show the info
			
			}
			elseif ( !empty( $_POST['forgetpassword'] ) )
			{
				// find user
				$users = rtrim($_POST['userid']);
				$result = $db->query( "select * from Users where Users.UserID = '$users'");
			
			
				$rows = $result->num_rows;
	
			
				if ( $rows < 1 )
				{
					echo "Username not found<br/>";
			
				}
				else
				{
					//mail new password, hash of the previous hashed pass
					$row = $result->fetch_array();
					$newpass = hash('sha256',$row['password']);
					$newnewpass = hash('sha256',$newpass);
					$result1 = $db->query( "update Users set Users.password = '$newnewpass' where Users.UserID = '$users'");
					if ( $result1 )
					{
					
						mail( $row['email'], "Password Reminder", $newpass ); //email the password
						echo "Email has been sent<br/>";
					}
					else
					{
						echo "Username not found<br/>";
					}
				}
				show_header();
				get_info();
				show_end();
			}
			elseif ( !empty( $_POST['changepassword'] ) )
			{
				change_password( $_POST['userid'] );
				$_SESSION['option'] = "changingpass";
			
			}
			elseif ( strcmp( $option, 'changingpass' ) == 0 )
			{
				//changing pass, make sure they match
				if ( !empty( $_POST['newpass'] ) && !empty( $_POST['oldpass']) )
				{
				
					$oldpass = hash('sha256', $_POST['oldpass'] );
				
					$user = $_POST['userid'];
					$result = $db->query( "select * from Users where Users.password = '$oldpass' and Users.UserID = '$user'");
				
					$rows = $result->num_rows;
				
					if ( $rows < 1 )
					{
						echo "Incorrect Password</br>";
					}
					else
					{
						$newpass = hash('sha256', $_POST['newpass'] );
				
						$result = $db->query("update Users set Users.password = '$newpass' where Users.UserID = '$user'");
					
						echo "Password Updated.</br>";
						session_destroy();
					}
				
			
				}
				else
				{
			
					echo "Fill in both the old password and the new password fields.</br>";
					change_password( $_POST['userid'] );
				}
			}
			else
			{

				//no password entered
				show_header();
				echo "<h3 align ='center' >No Password entered, please enter your User ID and password.</h3>" ;
				get_info();
				show_end();

			}
		}
		else
		{
			//no id entered
			show_header();
			echo "<h3 align ='center' >No userid entered, please enter your User ID and password.</h3>" ;
			get_info();
			show_end();

		}
	}
	else
	{
		session_destroy();
		header("Location: Assignment3.php");
	
	}
}
else
{
	session_destroy();
	header("Location: Assignment3.php");


}

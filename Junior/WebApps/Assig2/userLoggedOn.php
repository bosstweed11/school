<?php



session_start();

$pword = file("quotes3.txt");
$pass = rtrim( $pword[0] );
$db = new mysqli('localhost', 'BeersA', $pass , 'BeersA');
	
if ( $db->connect_error):
	die (" Could not connect to db: " . $db->connect_error);
endif;

function __autoload( $class )
{

	require_once $class . '.php';
}
include 'A2Functions.php';
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
	if ( isset( $_POST['logoff'] ) )
	{

		echo "Successfully Logged Off. <br/>";
		unset( $_SESSION["option"] );
		unset( $_SESSION["info"] );
		session_destroy();
	}
	elseif ( strcmp( $option, 'logged_on' ) == 0 ) //the advisor has searched for a student
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

			$coursesTaken = show_classes( $info );
			show_requirements( $coursesTaken );

			log_out();

			show_end();
		}
	}
	elseif ( strcmp( $option, 'searchstudent' ) == 0 ) //the advisor has searched for a student
	{
		$info = unserialize ( $_SESSION['info'] );
		//if the advisor searched by first/last name
		if ( !empty( $_POST['first'] ) && !empty( $_POST['last'] ) )
		{
			echo "Looking for name<br/>";
			$first = $_POST['first'];
			$last = $_POST['last'];
			$result = $db->query("select * from Users where Users.first_name = '$first' and Users.last_name = '$last'") or die ("invalid select " . $db->error);
			$rows = $result->num_rows;
			
			echo "First : $first, Last : $last</br>";
			if ( $rows < 1 )
			{
			
				$infoString = serialize( $info );
				$_SESSION['info'] = $infoString;
				echo "First / Last name not found, search again<br/>";
				student_search();//prompt search again, didnt find psid
				if ( $info->getAccess() == 2 )
				{
					add_remove();
				}
			
			}
			else
			{
			
				$row = $result->fetch_array();
				$studentInfo = new A2User( $row['UserID'] , $row['password'], $row['psid'], $row['email'], $row['last_name'], $row['first_name'], $row['access_level'] );
				
				$result = $db->query("select * from Courses where Courses.psid = ".$studentInfo->getPsid());
				
				$rows = $result->num_rows;
				
				$studentCourseList = array();
				for ( $i =0; $i < $rows; $i++ )
				{
					$course = $result->fetch_array();
					
					$studentCourse = new Course($course['Term'], $course['Department'], $course['Class_Number'], $course['Grade'], $course['GPA'] );
					$studentCourseList[$i] = $studentCourse;
					
					
				}
				$studentInfo->addCourses( $studentCourseList );
				
				
				$info->addStudent( $studentInfo );
				$infoString = serialize( $info );
				$_SESSION['info'] = $infoString;
				$coursesTaken = show_classes( $studentInfo);
				
				advisor_action();
				
			
			}
		
	
		}
		elseif ( !empty( $_POST['psid'] ))	//if the advisor searched by psid
		{
			
			$psid = $_POST['psid'];
			echo "Seaching for psid of $psid<br/>";
				
			$result = $db->query("select * from Users where Users.psid = '$_POST[psid]' and Users.access_level > '-1'");
			$rows = $result->num_rows;
			
			if ( $rows < 1 )
			{
			
				$infoString = serialize( $info );
				$_SESSION['info'] = $infoString;
				echo "First / Last name not found, search again<br/>";
				student_search();//prompt search again, didnt find psid
				if ( $info->getAccess() == 2 )
				{
					add_remove();
				}
			
			}
			else
			{
			
				$row = $result->fetch_array();
				
				$studentInfo = new A2User( $row['UserID'] , $row['password'], $row['psid'], $row['email'], $row['last_name'], $row['first_name'], $row['access_level'] );
				$result = $db->query("select * from Courses where Courses.psid = ".$studentInfo->getPsid());
				
				$rows = $result->num_rows;
				
				$studentCourseList = array();
				for ( $i =0; $i < $rows; $i++ )
				{
					$course = $result->fetch_array();
					
					$studentCourse = new Course($course['Term'], $course['Department'], $course['Class_Number'], $course['Grade'], $course['GPA'] );
					$studentCourseList[$i] = $studentCourse;
					
					
				}
				$studentInfo->addCourses( $studentCourseList );
				
				$info->addStudent( $studentInfo );
				$infoString = serialize( $info );
				$_SESSION['info'] = $infoString;
				$coursesTaken = show_classes( $studentInfo );
				
				advisor_action();
				
			
			}
		
		}
		elseif ( !empty( $_POST['coursenum'] ) && !empty ( $_POST['dept']) )
		{
			$dept = $_POST['dept'];
			$coursenum = $_POST['coursenum'];
			
			$result = $db->query("select Courses.psid, Courses.Grade, Courses.GPA from Courses where Courses.Department = '$dept' and Courses.Class_Number = '$coursenum' ");
			
			
			$rows = $result -> num_rows;
			
			if ( $rows < 1 )
			{
				echo "Sorry no classes were found.</br>";
			}
			else
			{
			
				$cumulativeGPA = 0;
				for ( $i =0; $i < $rows; $i++ )
				{
					$row = $result->fetch_array();
			
					echo "PeopleSoft ID: ". $row['psid'] . " Grade: ". $row['Grade']. " GPA: ". $row['GPA']."</br>";
			
					$cumulativeGPA += $row['GPA'];
		
				}
		
				$avgGPA = $cumulativeGPA / $rows;
		
				echo "Average GPA for this course is : $avgGPA</br>";
		
				
			}
			echo "</br> Search again:</br>";
			student_search();
			if ( $info->getAccess() == 2 )
			{
				add_remove();
			}
			
		
		}
		else
		{	
			echo "Error occured, perhaps you only entered one of the names. Enter again.<br/>";
			echo "Search For a Student: <br/><br/>";
			student_search();//prompt user again, didnt find matching first/last names
			if ( $info->getAccess() == 2 )
			{
				add_remove();
			}
	
		}
	}
	else
	{
		//invalid state
		session_destroy();
		header("Location: Assignment2.php");
	
	}
}
else
{
	//invalid state
	session_destroy();
	header("Location: Assignment2.php");


}



?>
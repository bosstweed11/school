<?php
// CS 1520 Assignment 2 Andrew Beers 6/28/12
// load classes needed


/*
Fixes:
	1.) two lists
	2.) radio buttons instead of text for advisor
	3.) user object instead of student
	4.) N- cselec 4? ask.
*/
function __autoload( $class )
{

	require_once $class . '.php';
}

session_start();

date_default_timezone_set( 'America/New_York' );


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
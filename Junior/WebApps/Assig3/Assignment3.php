<link rel = "stylesheet" type = "text/css" href = "A3Style.css"/>
<?php
// CS 1520 Assignment 3 Andrew Beers 6/28/12
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

//if we've been to a previous site before
if ( isset( $option ) )
{

	header("Location: processLogin.php");
}
else
{
	$_SESSION['option'] = 'logging_in';
	show_header();
	get_info();
	show_end();

}

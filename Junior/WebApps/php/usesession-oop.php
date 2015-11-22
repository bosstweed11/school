<?php
#CS 1520 usesession-oop.php

// load classes needed

function __autoload( $class )
{

	require_once $class . '.php';
}

session_start();
date_default_timezone_set( 'America/New_York' );

if ( isset( $_SESSION['option'] ) )
{
	$option = $_SESSION['option'];
}

//check for serialized version of User Object in the cookie

if ( isset( $_COOKIE['zany'] ) )
{
	$info = unserialize( $_COOKIE['zany'] );
}	

if ( isset( $option ) )
{

	if ( strcmp( $option, 'info' ) == 0 )
	{
	
		if ( $POST['name'] )
		{
			//create a user object from the data submitted by the user
			$info = new User( $POST['name'], $POST['street'], $POST['city'], $POST['state'], $POST['zip'] );
			
			//serialize the data to put into a cookie
			$infoString = serialize( $info );
			
			setcookie( "zany", "$infoString", time() + 600 );
			$_SESSION['info'] = $infoString;
			
			$_SESSION['option'] = "choice";
			
			show_header();
			show_menu();
			show_end();
		
		}
		else
		{
		
			show_header();
			echo "Invalid Session -- closing it <br/>" ;
			show_end();
			
			unset( $_SESSION["option"] );
			unset( $_SESSION["info"] );
			unset( $_SESSION["history"] );
			session_destroy();
		
		}
	
	
	}
	else	//here the user made a choice
	{
		
		$value = $_POST["value"];		//get value
		$_SESSION["history"][] = $value; // update history
		
		if ( $value == 1 )
		{
			show_header();
			show_info();
			show_menu();
			show_end();
		
		}
		elseif ( $value == 2 )
		{
			show_header();
			$time = time();
			$strtime = date("D F d h:i:s a", $time ); //date function specifics
			echo "$strtime <br/>";
			show_menu();
			show_end();
			
		
		}
		elseif ( $value == 3 )
		{
			show_header();
			$hist = $_SESSION["history"];
			
			foreach ( $hist as $key => $value )
			{
			
				echo "$key: ";
				
				if ( $value == 1 )
				{
					echo "Show Info <br/>";
					
				}
				elseif ( $value == 2 )
				{
					echo "Show Time <br/>";
				
				}
				elseif ( $value == 3 )
				{
					echo "Show History <br/>";
				
				}
				elseif ( $value == 4 )
				{
					echo "Show Variables <br/>";
				
				}
				else
				{
					echo "Unknown Command <br/>";
				
				}
			
			}
			show_menu();
			show_end();
		
		
		}
		elseif ( $value == 4 )
		{
			show_header();
			show_vars();
			show_menu();
			show_end();
		
		}
		else
		{
			show_header();
			echo "Thanks for visiting! <br/>";
			echo "Your session has been closed";
			show_end();
			session_destroy();	
		
		}
	}
}
elseif ( isSet( $info ) )
{
	$_SESSION['option'] = 'choice';
	
	$_SESSION['info'] = serialize( $info );
	$userid = $info->getName();
	show_header();
	echo "Welcome Back $userid <br/>";
	show_menu();
	show_end();

}
else	//cookie doesn't exist yet
{
	$_SESSION['option'] = 'info';
	
	show_header();
	get_info();
	show_end();


}

function show_end()
{
	echo "</html>";

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

function show_menu()
{

	$info = unserialize( $_SESSION['info'] );
	$name = $info->getName();
	echo "<br/> $name, please choose an option ";
	?>
	<form name = "menuform"
			action = "usesession-oop.php"
			method = "POST">
	1) Show your contact information <br/>
	2) Show current date / time <br/>
	3) Show your command history <br/>
	4) Show your server variables <br/>
	5) Exit <br/>
	<input type = "text" name = "value" size = "4" maxlength = "4"><br/>
	<input type = "submit" value = "Submit">
	</form>
	
<?php

}

function show_info()
{
	echo "Here is your contact information: <br/>";
	$info = unserialize( $_SESSION['info'] );
	echo "info<br/>";


}

function get_info()
{
	echo "This is your first visit to my site <br/>";
	echo "Please enter your contact information <br/>";
	
?>
	
	<form name = "infoform"
			action = "usesession-oop.php"
			method = "POST">
	
	Name: <input type = "text" name = "name"><br/>
	Street: <input type = "text" name = "street"><br/>
	City: <input type = "text" name = "city"><br/>
	State: <input type = "text" name = "state"><br/>
	ZIP: <input type = "text" name = "zip"><br/>
	<input type = "submit" value = "Submit">
	</form>
<?php


}

function show_vars()
{

	echo "Here are some of your variables: <br/>";
	echo "SESSION: <br/>";
	
	foreach ( $_SESSION as $key => $value) :
		if ( is_array( $value ) ):
			echo "$key is an array: <br/>";
			foreach ( $value as $key2 => $value2 ):
				echo "$key2 : $value2 <br/>";
			endforeach;
		else:
			echo "$key : $value <br/>";
			
		endif;
	endforeach;
	
	echo "<br/>";
	
	echo "COOKIE: <br/>";
	
	foreach ( $_COOKIE as $key => $value ):
		if ( is_array( $value ) ):
			echo "$key is an array: <br/>";
			foreach ( $value as $key2 => $value2 ):
				echo "$key2 : $value2 <br/>";
			endforeach;
		else:
			echo "$key : $value <br/>";
		endif;
	endforeach;
	
}


























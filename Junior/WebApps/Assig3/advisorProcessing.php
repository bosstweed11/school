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
   	$actionvalue = $_POST['action'];
   	$studentPsid = $_POST['psid'];
   	
   	if ( !isset( $_POST['date'] ) )
   	{
   		if ( $actionvalue > 2 )
   		{
   			$actionvalue = 2;
   		}
   	}
   	
   	//find out which value was set
	if ( $actionvalue == 1 )
	{
		//action 1- get time, add psid:time to the sessions.txt file
		$date = date("Y-m-d-H-i-s");
		$currDate = explode( "-" , $date );
	
		$result = $db->query("select 1 from Sessions");
		//sessions table
		if( $result == false)
		{
			$db -> query( "create table Sessions (psid int not null, year int not null, month int not null, day int not null, hour int not null, min int not null , second int not null)");
		}
		//add session
		$db -> query( "insert into Sessions values('$studentPsid', '$currDate[0]', '$currDate[1]', '$currDate[2]', '$currDate[3]', '$currDate[4]', '$currDate[5]')") or die ("invalid insert " . $db->error);
			

		echo "Session logged for $studentPsid at $date";

	}
	elseif ( $actionvalue == 2 )
	{
		$result = $db->query("select 1 from Sessions");
		
		
		if( $result == false)
		{
			echo "No Sessions logged</br>";
		
		}
		else
		{
			//show sessions if it exists
			$result = $db->query( "select * from Sessions where Sessions.psid = '$studentPsid' order by Sessions.year desc,Sessions.month desc,Sessions.day desc,Sessions.hour desc,Sessions.min desc,Sessions.second desc");
	
			$rows = $result->num_rows;
			$resultString = "";
			if ( $rows < 1 ) 
			{
				$resultString= "";
	
			}
			else
			{
				$resultString = '<?xml version="1.0" encoding="utf-8"?>';
				$resultString .= "<sessionlist sessions = '$rows'>";
			
				for ( $i = 0; $i < $rows; $i++ )
				{
					$resultString .= "<session>";
					$row = $result->fetch_array();
					
					$resultString .= "<date>".$row['year']."-".$row['month']."-".$row['day']."-".$row['hour']."-".$row['min']."-".$row['second']."</date>";
					$psid = $row['psid'];
					$resultString .= "<psid>$psid</psid>";
					//echo "Select Session $i: PeopleSoft: $psid Date: $dateString : <input type='radio' value='$rdo' name='rdochoice' /><br/>";
					
					$resultString .= "</session>";
				}
				
				$resultString .= "</sessionlist>";
			}
			
			echo "$resultString";
		}

	}
	elseif ( $actionvalue == 3)
	{

		$studentPsid = $_POST['psid'];
		$dateInfo = $_POST['date'];
		$note = $_POST['note'];
		
		$fileName = $studentPsid."_".$dateInfo.".txt";
		$date = explode ("-", $dateInfo );
		$result = $db->query("select 1 from Notes");
				
		//create notes table
		if( $result == false)
		{
			$db -> query( "create table Notes (psid int not null, year int not null, month int not null, day int not null, hour int not null, min int not null , second int not null)");
		}
		
		$db-> query("insert into Notes values( '$studentPsid', '$date[0]', '$date[1]', '$date[2]', '$date[3]', '$date[4]', '$date[5]')");
		
		
		
		$fp = fopen("notes/$fileName", "a");   // Open file for writing
			
			fwrite( $fp, "$note\n" );
		fclose($fp);
	
		echo "$note";
	
	
		
	}
	elseif ( $actionvalue == 4 )
	{
	
		$studentPsid = $_POST['psid'];
		$dateInfo = $_POST['date'];
		
		$result = $db->query("select 1 from Notes");
		
		$fileName = $studentPsid."_".$dateInfo.".txt";
		
		$resultString = "";
		
		if ( file_exists( "notes/$fileName" ) )
		{
			$notes = file("notes/$fileName");
			
	
			for( $i = 0; $i < count( $notes ); $i++ )
			{
				$resultString.= "$notes[$i]\n";
			}
		}
		else
		{
			$resultString = "No notes have been logged about this session";
		
		}
		
		echo "$resultString";
	}

?>
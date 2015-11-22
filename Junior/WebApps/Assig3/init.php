

<?php
	//andrew beers initialization of db file
	$pword = file("quotes3.txt");
	$pass = rtrim( $pword[0] );
	$db = new mysqli('localhost', 'BeersA', $pass , 'BeersA');
	
	if ( $db->connect_error):
		die (" Could not connect to db: " . $db->connect_error);
	endif;
	
	
	$db->query("drop table Users");
	$db->query("drop table Reqs");
	$db->query("drop table Courses");
	$db->query("drop table Sessions");
	$db->query("drop table Notes");
	
	$files = glob('notes/*.txt'); // get all file names
	foreach($files as $file)
	{ 
  		if( is_file($file) )
  		{
    		unlink($file); // delete file
    	}
    
	}

	//Users table
	
	$db->query( "create table Users ( psid int primary key not null, UserID char(30) not null, password char(64) not null, email char(30) not null, last_name char(30) not null, first_name char(30) not null, access_level int not null)");
	$userFile = file("users2.txt");
	
	//populate users table
	foreach ( $userFile as $userInfo):
	
		$userString = rtrim( $userInfo );
		$user = preg_split("/:/", $userString );
		$pass = hash('sha256',$user[1]);
		
		$query = "insert into Users values ('$user[2]','$user[0]','$pass','$user[3]','$user[4]','$user[5]','$user[6]')";
		
		$db->query($query) or die ("invalid insert " . $db->error);
	endforeach;
		
	
	
	//Reqs table
	$db -> query( "create table Reqs ( ReqID int primary key not null, Requirement char(30) not null, Department char(30) not null, Class_Number int not null)");
	$reqsFile = file("reqs.txt");
	$reqid = 0;
	
	//populate reqs table
	foreach ( $reqsFile as $reqInfo):
	
		$reqString = rtrim( $reqInfo );
		$req = preg_split("/:/", $reqString );
		
		$allClasses = preg_split("/\|/", $req[1] );
		
		foreach ( $allClasses as $class )
		{
			
			//echo "variable class: $class</br>";
			$classInfo = preg_split("/,/", $class );
			/*
			echo "Req: $req[0]</br>";
			echo "rest: $req[1]</br>";
			echo "Dept: $classInfo[0]</br>";
			*/
			$query = "insert into Reqs values ('$reqid','$req[0]','$classInfo[0]','$classInfo[1]')";
		
			$db->query($query) or die ("invalid insert " . $db->error);
			$reqid += 1;
		
		}
		
		
	endforeach;
	
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
		
	$courseid = 0;
		
	//courses table
	$db -> query( "create table Courses (CourseID int primary key not null, psid int not null, Department char(30) not null, Class_Number char(30) not null, Term int not null, Grade char(2) not null, GPA float not null)");
	$courseFile = file("courses2.txt");
	
	// populate courses table
	foreach ( $courseFile as $courseInfo):
	
		$courseString = rtrim( $courseInfo );
		$course = preg_split("/:/", $courseString );
		
		
		
		$query = "insert into Courses values ('$courseid','$course[3]','$course[0]','$course[1]','$course[2]','$course[4]', ". $gpas[$course[4]] . ")";
		
		$db->query($query) or die ("invalid insert " . $db->error);
		$courseid += 1;
	endforeach;
	
	//show tables , users, reqs and courses
	
	
	$query = "select * from Users";
	
	$result = $db->query($query) or die ("Invalid query" . $db->error);
	
	echo "<table border = '1'><tr><td>Users table</td></tr>";
	echo "<tr><td>PeopleSoft ID</td><td>UserID</td><td>Password</td><td>Email</td><td>First name</td><td>Last name</td><td>Access</td></tr>";
	
	$rows = $result->num_rows;
	
	if ( $rows < 1 )
	{
	
		echo " No entries</br>";
	
	}
	else
	{
		
		for ( $i = 0; $i < $rows; $i++ )
		{
			echo "<tr>";
			$row = $result->fetch_array();
			echo "<td>" . $row["psid"] . "</td>";
			echo "<td>" . $row["UserID"] . "</td>";
			echo "<td>" . $row["password"] . "</td>";
			echo "<td>" . $row["email"] . "</td>";
			echo "<td>" . $row["first_name"] . "</td>";
			echo "<td>" . $row["last_name"] . "</td>";
			echo "<td>" . $row["access_level"] . "</td>";

			echo "</tr>";
		}
		echo "</table>";
	}

	
	echo "</br></br>";
	
	
	$query = "select * from Reqs";
	
	$result = $db->query($query) or die ("Invalid query" . $db->error);
	
	echo "<table border = '1'><tr><th>Reqs table</th></tr>";
	echo "<tr><td>ReqID</td><td>Requirement</td><td>Department</td><td>Course Number</td>";
	
	$rows = $result->num_rows;
	
	if ( $rows < 1 )
	{
	
		echo " No entries</br>";
	
	}
	else
	{
		
		for ( $i = 0; $i < $rows; $i++ )
		{
			echo "<tr>";
			$row = $result->fetch_array();
			echo "<td>" . $row["ReqID"] . "</td>";
			echo "<td>" . $row["Requirement"] . "</td>";
			echo "<td>" . $row["Department"] . "</td>";
			echo "<td>" . $row["Class_Number"] . "</td>";

			echo "</tr>";
		}
		echo "</table>";
	}

	
	echo "</br></br>";
	
	$query = "select * from Courses";
	
	$result = $db->query($query) or die ("Invalid query" . $db->error);
	
	echo "<table border = '1'><tr><th>Courses table</th></tr>";
	echo "<tr><td>CourseID</td><td>PeopleSoft ID</td><td>Department</td><td>Course Number</td><td>Term</td><td>Grade</td><td>GPA</td>";
	
	$rows = $result->num_rows;
	
	if ( $rows < 1 )
	{
	
		echo " No entries</br>";
	
	}
	else
	{
		
		for ( $i = 0; $i < $rows; $i++ )
		{
			echo "<tr>";
			$row = $result->fetch_array();
			echo "<td>" . $row["CourseID"] . "</td>";
			echo "<td>" . $row["psid"] . "</td>";
			echo "<td>" . $row["Department"] . "</td>";
			echo "<td>" . $row["Class_Number"] . "</td>";
			echo "<td>" . $row["Term"] . "</td>";
			echo "<td>" . $row["Grade"] . "</td>";
			echo "<td>" . $row["GPA"] . "</td>";

			echo "</tr>";
		}
		echo "</table>";
	}

	
	echo "</br></br>";
	
	
	
	
?>
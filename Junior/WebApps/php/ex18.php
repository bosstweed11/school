<!DOCTYPE html>
<html>
<head>
<title>CS 1520 PHP Example 18</title>
</head>
<body>
<?php
   // Simple demonstration of SQL injection and a way to combat it.
   $movie = stripslashes($_POST["movie"]);
   if ($movie):
       #  Log into the DB
       $passlist = file("password.php");
       $passwd = rtrim($passlist[1]);
       $passwd = preg_replace('/#/','',$passwd);
       $db = mysql_connect('localhost', 'nomad', "$passwd");
       if ($db):
          mysql_select_db('nomad');
       else:
          die ("Could not connect to db " . mysql_error());
       endif;

       // Query is not checked for injection and a smart user could see all
       // of the movies in the DB
       $query = "select * from Movies where Movie_name = '$movie'";
       echo "Your query is: $query <br />";
       $result =  mysql_query($query) or die ("Invalid query" . mysql_error());
       $rows = mysql_num_rows($result);
       if ($rows < 1):
           echo "Your movie was not found -- sorry!";
       else:
           echo "Found your movie!<br />";
           $row = mysql_fetch_array($result);
           while ($row):
              print_r($row);
              echo "<br />";
              $row = mysql_fetch_array($result);
           endwhile;
       endif;

       // We add some injection protection with the mysql_real_escape_string()
       // function.  This "escapes" single quotes by putting backslashes in
       // front of them.  It will not defeat all injection attacks, however.
       echo "<br />Now use the 'escaped' version<br />";
       $movie = mysql_real_escape_string($movie);
       $query = "select * from Movies where Movie_name = '$movie'";
       echo "Your query is: $query <br />";
       $result =  mysql_query($query) or die ("Invalid query" . mysql_error());
       $rows = mysql_num_rows($result);
       if ($rows < 1):
           echo "Your movie was not found -- sorry!";
       else:
           echo "Found your movie!<br />";
           $row = mysql_fetch_array($result);
           while ($row):
              print_r($row);
              echo "<br />";
              $row = mysql_fetch_array($result);
           endwhile;
       endif;
   endif;
?>
   <form action  = "ex18.php" method = "post"> 
   Movie Name: <input type = "text" name = "movie" /><br />
   <input type = "submit" value = "Submit" />
   </form>
</body>
</html>

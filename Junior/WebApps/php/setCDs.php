<?php
   // CS 1520
   // Very simple script to initialize a MySQL table
   $passlist = file("password.php");
   $passwd = rtrim($passlist[1]);
   $passwd = preg_replace('/#/','',$passwd);
   $db = mysql_connect('localhost', 'nomad', $passwd);
   if ($db): 
       mysql_select_db('nomad'); 
   else:  
       die ("Could not connect to db " . mysql_error()); 
   endif;
   mysql_query("drop table CD");
   $result = mysql_query("create table CD (id int primary key not null, title char(30) not null, artist char(30))") or die ("Invalid: " . mysql_error());
 
   $query = "insert into CD values (1, 'Version 2.0', 'Garbage')"; 
   mysql_query($query) or die ("Invalid insert " . mysql_error()); 

   $query = "insert into CD values (2, 'Substance', 'Joy Division')";
   mysql_query($query) or die ("Invalid insert " . mysql_error()); 

   $query = "insert into CD values (3, 'U2', 'Zooropa')"; 
   mysql_query($query) or die ("Invalid insert " . mysql_error());
?>
<html>
   <head>
       <title>Generating a Simple Table</title>
   </head>
   <body>
<?php
   echo "CDs should be set up properly";
?>
   </body>
</html> 

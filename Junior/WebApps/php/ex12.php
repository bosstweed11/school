<?php
  // CS 1520 Spring 2012
  // Simple demonstration of cookies and sessions in PHP
session_start();
$s = (int) $_SESSION["count"];
$c1 = $_COOKIE["cookie1"];
$c2 = $_COOKIE["cookie2"];
if (($s == 0) && !$c1):
   setcookie("cookie1","You have visited the site 1 previous time",time()+120);
   $output = "You have not visited the site before";
elseif (($s == 0) && !$c2):
   setcookie("cookie2","You have visited the site > 1 previous times",time()+120);
   $output = $c1;
elseif ($s == 0):
   $output = $c2;
else:
   $output = "";
endif;

$s++;
$_SESSION["count"] = $s;
?>
<!DOCTYPE html>
<html>
<head>
   <title>CS 1520 PHP Example 12</title>
</head>
<body>
<h3>
<?php
     echo "Welcome to the Example 12 PHP Site<br/>";
     if ($output):
         echo "$output<br/>";
     endif;
     echo "You have visited $s pages in the current session";
?>
</body>
</html>  

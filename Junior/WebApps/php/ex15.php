<!DOCTYPE html> 
<html>
<head>
<title>CS 1520 PHP Example 15</title>
</head>
<body>
<?php
  # Simple PHP Script to input a sentence and break it into words.
  
  $sent = $_POST["sentence"];
  if ($sent):
     $sent = strip_tags(stripslashes($sent));
     echo "Your entered sentence is $sent <br />";
     $words = preg_split("/\W+/", $sent);
     foreach ($words as $value):
          echo "$value <br/> ";
     endforeach;
  endif;
?>
<form action="ex15.php" 
      method="POST">
      Please enter a sentence: 
      <input type = "text" name = "sentence">
      <input type = "submit" value = "Process">
</form>
</body>
</html>

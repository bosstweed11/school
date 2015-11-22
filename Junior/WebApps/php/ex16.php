<?php
   // CS 1520 Summer 2012
   // Demonstrating Redirection as well as iframes in PHP / HTML
   $option = $_GET["option"];
?>
<!DOCTYPE html>
<html>
<head>
    <title>CS 1520 -- Using Redirection in PHP</title>
</head>
<body>
    <form action = "redirect.php"
          method = "POST">

<?php
    if ($option == ""):
?>
    <b>What is your name?</b>
    <input type = "text" name = "name">
    <br/>
<?php
    endif;
?>
    <b>What is your choice?</b>
    <select name = "input">
	<option value = "1">Show Weather</option>
        <option value = "2">To to CS Home Page</option>
        <option value = "3">Go to Amazon.com</option>
    </select>
    <br/>
    <input type = "submit" value = "Enter"></input>
    </form>
</body>
</html>

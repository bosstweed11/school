<?php
session_start();
$name= $_SESSION["name"];
?>
<!DOCTYPE html>
<head>
<title><?php echo "Weather for $name";?></title>
</head>
<body>
<iframe src="http://www.weather.com" width="800" height="600"></iframe>
<br/>
<a href = "ex16.php?option=input">Make New Selection</a>
<br/>
<a href = "ex16.php">Change User</a>
</body>
</html>

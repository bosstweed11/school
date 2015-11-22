<?php
session_start();
if ($_POST["name"])
    $_SESSION["name"] = $_POST["name"];
$input = $_POST["input"];
switch ($input)
{
    case 1: header("Location: choice1.php");
    	    break;
    case 2: header("Location: choice2.php");
    	    break;
    case 3: header("Location: choice3.php");
    	    break;
}
?>

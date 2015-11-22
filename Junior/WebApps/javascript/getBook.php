<?php
// CS 1520 Summer 2012
// Simple script to get an ISBN and pass it on to the ISBN site.
// The resulting file is then sent back to the requestor.
header('Content-type: text/xml');
$ISBN = $_GET["isbn"];
$URL = 'http://isbndb.com/api/books.xml?access_key=DROM5N4A&index1=isbn';

$data = $URL . '&' . 'value1=' . $ISBN;
$contents=file_get_contents("$data"); 
echo "$contents";
?>

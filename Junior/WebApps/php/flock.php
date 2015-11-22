<!DOCTYPE html>
<html>
<head>
<title>CS 1520 PHP Example To Demonstrate Flocking</title>
</head>
<body>
<?php
  // Get an id from the $_SERVER variable.
  $my_port = $_SERVER["REMOTE_PORT"];

  // Append id to the file 50 times, stalling a bit between each
  // iteration.  Try commenting out the flock to see the difference
  // in the output without the flock statement.
  for ($i = 1; $i <= 50; $i++):
      $fp = fopen("flockdemo.txt", "a");   // Open file for append 
      if (flock($fp, LOCK_EX)):            // Get exclusive lock
           fwrite($fp, "Writing $my_port for $i th time");
           echo "Writing $my_port for $i th time ";
           for ($j = 0; $j < 500000; $j++);
           fwrite($fp,"...done writing $my_port\n");
           echo "...done writing $my_port<br/>\n";
      endif;
      fclose($fp);   // Closing file releases lock
      for ($j = 0; $j < 500000; $j++);
  endfor;
?>
</body>
</html>

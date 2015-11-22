<?php
#  CS 1520 AJAX - XSLT Example -- server script
#  Compare to tabulate-xml.php
#  If a count is incremented, this behaves just as tabule-xml.php.  However,
#  for additions and CD updates this will now return the entire list of CDs
#  in XML format.

   $passlist = file("password.php");
   $passwd = rtrim($passlist[1]);
   $passwd = preg_replace('/#/','',$passwd);
   $db = mysql_connect('localhost', 'nomad', "$passwd");
   if ($db):
      mysql_select_db('nomad');
   else:
      die ("Could not connect to db " . mysql_error());
   endif;

   // Set header for xml document and add root tag
   header('Content-type: text/xml');
   echo "<?xml version='1.0' encoding='utf-8'?>";
   echo "<CDResponse>";

   $type = $_POST["type"];
   if ($type == 1):
      $choice = $_POST["select"];
      $query = "select Votes from CDs where id = '$choice'";
      $result = mysql_query($query);
      $row = mysql_fetch_array($result);
      $count = $row["Votes"];
      $newcount = $count + 1;
      $query = "update CDs set Votes = '$newcount' where id = '$choice'";
      $result = mysql_query($query) || die("BOGUS A $type");

      // Return new count. 
      echo "<Type>Count</Type>";
      echo "<Count>$newcount</Count>";
   elseif ($type == 2 || $type == 3):
      $numrows = strip_tags($_POST["rows"]);
      if ($type == 2):
         $artist = strip_tags($_POST["artist"]);
         $title = strip_tags($_POST["title"]);
      endif;
      $newrows = "<Type>Update</Type>";
      // Insert new CD first, so that it will be part of the query when
      // we check below.
      if ($type == 2):
          $query = "insert into CDs values (NULL, '$title', '$artist', '1')";
          $result = mysql_query($query) || die(mysql_error());
      endif;
      
      // Select all CDs in the table and put them into the XML result
      $rr = mysql_query("select * from CDs");
      $resrows = mysql_num_rows($rr);
      for ($i = 0; $i < $resrows; $i++):
           mysql_data_seek($rr, $i);
           $curr = mysql_fetch_array($rr);
           $newrows .= "<CD>";
           $newrows .= "<id>" . $curr["id"] . "</id>";
           $newrows .= "<Title>" . $curr["Title"] . "</Title>";
           $newrows .= "<Artist>" . $curr["Artist"] . "</Artist>";
	   $newrows .= "<Votes>" . $curr["Votes"] . "</Votes>";;
           $newrows .= "</CD>";
      endfor;
      echo "$newrows";
   endif;
   // Close with the correct end tag
   echo "</CDResponse>";
?>

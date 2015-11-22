<?php
#  CS 1520 AJAX Example -- server script
#  Compare to tabulate-update.php
#  We have the same 3 options as in the previous version:
#  type == 1: return updated count
#  type == 2: add a new row to the DB plus return any updated rows
#  type == 3: return updated rows without changing db
#  However, the difference in this case is that the information will be
#  returned in each case as an XML document, which will then be parsed
#  by the client
#  See additional comments below.

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

      // Return new count.  Note that we first set the Type tag and then
      // the actual count value.  If you look at CDpoll-xml.php you will note
      // that the Type tag is ignored in this case, but it is included for
      // consistency.
      echo "<Type>Count</Type>";
      echo "<Count>$newcount</Count>";
   elseif ($type == 2):
      $numrows = strip_tags($_POST["rows"]);
      $artist = strip_tags($_POST["artist"]);
      $title = strip_tags($_POST["title"]);
      $newrows = "";
      $query = "lock tables CDs write";
      $result = mysql_query($query) || die(mysql_error());
      $query = "select * from CDs";
      $rr = mysql_query("select * from CDs");
      $resrows = mysql_num_rows($rr);
      
      // Generate new rows to return using XML tags.  The Type is returned
      // as Update followed by a sequence of CD elements.
      if ($numrows < $resrows):
          $newrows .= "<Type>Update</Type>";
          for ($i = $numrows; $i < $resrows; $i++):
	       mysql_data_seek($rr, $i);
	       $curr = mysql_fetch_array($rr);
               $newrows .= "<CD>";
	       $newrows .= "<id>" . $curr["id"] . "</id>";
	       $newrows .= "<Title>" . $curr["Title"] . "</Title>";
	       $newrows .= "<Artist>" . $curr["Artist"] . "</Artist>";
	       $newrows .= "<Votes>" . $curr["Votes"] . "</Votes>";;
               $newrows .= "</CD>";
	  endfor;
      endif;
      // If no rows are added, return Type Ack and Response Ok.
      if ($newrows == ""):
            $newrows .= "<Type>Ack</Type>";
            $newrows .= "<Response>OK</Response>";;
      endif;
      $query = "insert into CDs values (NULL, '$title', '$artist', '1')";
      $result = mysql_query($query) || die(mysql_error());
      $query = "unlock tables";
      $result = mysql_query($query) || die(mysql_error());
      echo "$newrows";
   else: // type = 3
      $numrows = strip_tags($_POST["rows"]);
      $newrows="";
      $rr = mysql_query("select * from CDs");
      $resrows = mysql_num_rows($rr);
      
      // Same idea as above
      if ($numrows < $resrows):
          $newrows .= "<Type>Update</Type>";
          for ($i = $numrows; $i < $resrows; $i++):
	       mysql_data_seek($rr, $i);
	       $curr = mysql_fetch_array($rr);
               $newrows .= "<CD>";
	       $newrows .= "<id>" . $curr["id"] . "</id>";
	       $newrows .= "<Title>" . $curr["Title"] . "</Title>";
	       $newrows .= "<Artist>" . $curr["Artist"] . "</Artist>";
	       $newrows .= "<Votes>" . $curr["Votes"] . "</Votes>";;
               $newrows .= "</CD>";
	  endfor;
      endif;
      if ($newrows == ""):
            $newrows .= "<Type>Ack</Type>";
            $newrows .= "<Response>OK</Response>";;
      endif;
      echo "$newrows";
   endif;
   // Close with the correct end tag
   echo "</CDResponse>";
?>

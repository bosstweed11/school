<?php
#  CS 1520 AJAX Example -- server script
#  Compare to tabulate-xml.php
#  We have the same 3 options as in the previous version:
#  type == 1: return updated count
#  type == 2: add a new row to the DB plus return any updated rows
#  type == 3: return updated rows without changing db
#  However, the difference in this case is that the information will be
#  returned in each case as a JSON document, which will then be converted 
#  by the client into Javascript objects.
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

   // Set header for json document (note despite this header we still be
   // actually processing the return value as text.  Right now AJAX does not
   // have a responseJSON attribute in the XMLHttpRequest object.  See
   // CDpoll-sortjson.php)
   header('Content-type: application/json');

   // Create array for return data.  This will store all of our information
   // and will be converted to JSON to be returned.
   $Adata = Array(); 

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

      // Put the type as Count and the Contents as the value
      $Adata["Type"] = "Count";
      $Adata["Contents"] = $newcount;
   elseif ($type == 2):
      $numrows = strip_tags($_POST["rows"]);
      $artist = strip_tags($_POST["artist"]);
      $title = strip_tags($_POST["title"]);
      $query = "lock tables CDs write";
      $result = mysql_query($query) || die(mysql_error());
      $query = "select * from CDs";
      $rr = mysql_query("select * from CDs");
      $resrows = mysql_num_rows($rr);
      
      if ($numrows < $resrows):
          // Now we must return CDs so we need to make some nested arrays.
          // Our overall new CDs will be an array of arrays.  Each array
          // within the overall array will represent a single CD.
          $Adata["Type"] = "Update";
          // Create new array for all CDs
          $contents = Array();
          for ($i = $numrows; $i < $resrows; $i++):
	       mysql_data_seek($rr, $i);
	       $curr = mysql_fetch_array($rr);
               // Create new array for current CD.  Put the fields in as keyed
               // items.  We could use the $curr array itself if we wanted to, but
               // it has additional fields in it that we don't want to send, so
               // instead we build a new array.
               $currArr = Array();
               $currArr["id"] = $curr["id"];
               $currArr["Title"] = $curr["Title"];
               $currArr["Artist"] = $curr["Artist"];
               $currArr["Votes"] = $curr["Votes"];
               // Append the current CD array to array of CDs.
               $contents[] = $currArr;
	  endfor;
          // Assign the return array to the overall return data as a keyed
          // entry 
          $Adata["Contents"] = $contents;
      else:
          // No new CDs are added, so just return an acknowledgment
          $Adata["Type"] = "Ack";
          $Adata["Contents"] = "OK";
      endif;
      $query = "insert into CDs values (NULL, '$title', '$artist', '1')";
      $result = mysql_query($query) || die(mysql_error());
      $query = "unlock tables";
      $result = mysql_query($query) || die(mysql_error());
   else: // type = 3
      $numrows = strip_tags($_POST["rows"]);
      $rr = mysql_query("select * from CDs");
      $resrows = mysql_num_rows($rr);
      
      // Same idea as above for creating CD array
      if ($numrows < $resrows):
          $Adata["Type"] = "Update";
          $contents = Array();
          for ($i = $numrows; $i < $resrows; $i++):
	       mysql_data_seek($rr, $i);
	       $curr = mysql_fetch_array($rr);
               $currArr = Array();
               $currArr["id"] = $curr["id"];
               $currArr["Title"] = $curr["Title"];
               $currArr["Artist"] = $curr["Artist"];
               $currArr["Votes"] = $curr["Votes"];
               $contents[] = $currArr;
	  endfor;
          $Adata["Contents"] = $contents;
      else:
          $Adata["Type"] = "Ack";
          $Adata["Contents"] = "OK";
      endif;
   endif;
   // The return array is complete.  Now use json_encode to make it into
   // the JSON format, then echo it.
   $returndata = json_encode($Adata);
   echo $returndata;
?>

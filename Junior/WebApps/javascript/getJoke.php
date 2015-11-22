<?php
   // CS 1520
   // Simple script to retrieve a joke and its punchline from a database and
   // return it as an well-formed XML document.
   $passlist = file("password.php");
   $passwd = rtrim($passlist[1]);
   $passwd = preg_replace('/#/','',$passwd);
   $db = mysql_connect('localhost', 'nomad', "$passwd");
   if ($db):
      mysql_select_db('nomad');
   else:
      die ("Could not connect to db " . mysql_error());
   endif;
   $query = "select setup, punchline from Jokes order by rand() limit 1";
   $result = mysql_query($query);
   $rows = mysql_num_rows($result);
   if ($rows >= 1):
      header('Content-type: text/xml');
      $row = mysql_fetch_array($result);
      $setup = $row["setup"];
      $punch = $row["punchline"];
      $ans = '<?xml version="1.0" encoding="utf-8"?>';
      $ans .= "<Joke><Setup>$setup</Setup><Punchline>$punch</Punchline></Joke>";
      echo "$ans";
   else:
      echo "ERROR";
   endif;
?>

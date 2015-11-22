<!DOCTYPE html>
<html>
 <head>
  <title>PHP Example 13</title>
 </head>
 <body>
 <?php
  // Simple intro to Perl-style regular expressions
  function nl()
  {
      echo "<br />";
  }

  // Simple demo of preg_match and preg_match_all
  $pattern = array("/matches/", "/wackiness/", "/atch/");
  $subject = "Look for matches here!  Matches?  We don't need no stinkin' matches!";
  foreach ($pattern as $key):
      if (preg_match($key, $subject, $result)):
          echo "$key was found: \n";
          print_r($result);
          nl();
      else:
          echo "$key not found <br />\n";
      endif;
      if (preg_match_all($key, $subject, $result, PREG_OFFSET_CAPTURE)):
          echo "$key was found: \n";
          print_r($result);
          nl();
      else:
          echo "$key not found <br />\n";
      endif;
  endforeach;
  nl();

  // Simple character classes
  $pattern = "/[aeiou][1234567890][a-z]/";
  $subject = "ae12abc r2d2 a1steaksauce";
  if (preg_match($pattern, $subject, $result)):
          echo "$pattern was found: \n";
          print_r($result);
          nl();
  endif;
  nl();

  // Using repetition together with character classes
  $pattern = "/[aeiou]+[1234567890]*[a-z]?/";
  if (preg_match_all($pattern, $subject, $result, PREG_OFFSET_CAPTURE)):
          echo "$pattern was found: \n";
          print_r($result);
          nl();
  endif;
  $pattern = "/\d+\.\d{5}/";
  $subject = "3 .14159  3.1415 3.14159";
  if (preg_match_all($pattern, $subject, $result)):
          echo "$pattern was found: \n";
          print_r($result);
          nl();
  endif;
  nl();

  // Using anchors (assertions) to restrict matches that would otherwise
  // occur
  $pattern = array("/[a-zA-Z]+\w*/", "/^[a-zA-Z]+\w*/", "/^\s*[a-zA-Z]+\w*\s*$/", "/^[a-zA-Z]+\w*$/");
  $subject = array("Good_var", "11thvariable", "Bad_maybe?? ", "  Good_maybe ");
  foreach ($pattern as $key):
     foreach ($subject as $sub):
         if (preg_match($key, $sub, $result)):
             echo "$key matches ";
             print_r($result);
             nl();
         endif;
     endforeach;
  endforeach;
  nl();

  // Default greedy matching and lazy matching
  $pattern = array("/(\w+)([0-9]+)(\w+)/", "/(\w+?)([0-9]+)(\w+)/", "/(\w*)([0-9]*)(\w*)/");
  $subject = "hello9999helloagain";
  foreach ($pattern as $key):
     if (preg_match_all($key, $subject, $result)):
          echo "$key was found: \n";
          print_r($result);
          nl();
     endif;
  endforeach;
  nl();
 
  // Using submatches as part of an overall match.  Note that the
  // back references have a double backslash
  $pattern = array("/(\w+)\\1/", "/(\w+)(.*?)(\\1)/", "/^(\w+)\\1\b/"); 
  $subject = array("hellohello", "abc123abc", "looklookhere", "look");
  foreach ($pattern as $key):
     foreach ($subject as $sub):
         if (preg_match($key, $sub, $result)):
             echo "$key matches ";
             print_r($result);
             nl();
         endif;
     endforeach;
  endforeach;
 ?>
 </body>
</html>

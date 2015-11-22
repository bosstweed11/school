<?php
   // This function will load any class files we need.  This is handy because
   // it keeps us from having to remember all of the classes used and from
   // making sure that we don't load files more than one time.
   function __autoload($class) {
	require_once $class . '.php';
   }
   session_start();
   if (isset($_SESSION["SubFoo"])):
      $theFoo = $_SESSION["SubFoo"];
   endif;
?>
<!DOCTYPE html>
<html>
<head>
<title>CS 1520 PHP Example 19</title>
</head>
<body>
<?php
  //  Some simple OOP in PHP.
  if (isSet($theFoo))
  {
      echo "Object ", $theFoo, " has been restored <br/>";
  }
  else
  {
      // Creating a new object looks a lot like Java
      $fooVar = new Foo(77, 88, 99);

      // When using the object in a String context, the "magic" toString()
      // method will implicitly be called.  See Foo.php.
      echo "fooVar is ", $fooVar, "<br/>";

      // Unlike Java (but like C++) we use the arrow notation rather than the
      // "dot" notation to access methods and data within an object
      $fooVar->setX(70);
      $fooVar->setY(80);
      // Since z is public, we can access it directly if we wish
      $fooVar->z = 90;
      echo "fooVar is ",  $fooVar, "<br/>";

      $subFooVar = new SubFoo(22, 33, 44, 55);
      echo "subFooVar is ", $subFooVar, "<br/>";

      // We can access setX and setY via inheritance
      $subFooVar->setX(20);
      $subFooVar->setY(30);
      $subFooVar->z = 40;
      $subFooVar->setSub(50);
      echo "subFooVar is ", $subFooVar, "<br/>";

      echo "x field is " . $subFooVar->subGetX() . "<br/>";
      echo "y field is " . $subFooVar->subGetY() . "<br/>";
      echo "<br/>";
      // "Kind of" polymorphic access.  Note that we are mixing a subclass
      // object and a superclass object.  However, unlike with Java, in PHP
      // the variable itself does not have a type until assigned a value.
      echo "Iterating through object array:<br/>";
      $fooArray = array($fooVar, $subFooVar);
      foreach ($fooArray as $val):
         echo "val is ",  $val, "<br/>";
         echo "type is ", get_class($val), "<br/>";
      endforeach;
      echo "<br/>";
      unset($fooVar);
      echo "Nothing destroyed yet, since array is also storing refs <br/>";
      unset($fooArray);
      echo "Foo object is now gone... <br/>";
      echo "<br/>";
      $serial = serialize($subFooVar);
      echo "Serialized version of subFooVar:<br/>";
      print_r($serial); echo "<br/>";
      $newSubFoo = unserialize($serial);
      echo "newSubFoo is ", $newSubFoo, "<br/>";
      $_SESSION["SubFoo"] = $subFooVar;
      echo "Program about to terminate... <br/>";
  }
?>
</body>
</html>

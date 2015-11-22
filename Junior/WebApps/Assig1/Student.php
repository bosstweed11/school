<?php
// CS 1520 User class for usesession-oop.php handout

class Student
{
   private $userid;
   private $pass;
   private $psid;
   private $email;
   private $first;
   private $last;
   private $access;

   public function __construct($u, $pa, $ps, $e, $l, $f, $a)
   {
     $this->userid = $u;
     $this->pass = $pa;
     $this->psid = $ps;
     $this->email = $e;
     $this->last= $l;
     $this->first= $f;
     $this->access= $a;
   }

   public function getName()
   {
     return $this->first;
   }
   
   public function getPsid()
   {
   	 return $this->psid;
   }
   
   public function getAccess()
   {
   	 return $this->access;
   }

   public function __toString()
   {
      $ans = "User ID: " . $this->userid . "<br/>" .
             "Password: " . $this->pass . "<br/>" .
             "PeopleSoft ID: " . $this->psid . "<br/>" .
             "Email: " . $this->email . "<br/>" .
             "Last Name: " . $this->last . "<br/>".
             "First Name: " . $this->first . "<br/>".
             "Access Level: " . $this->access . "<br/>"; 
             
             
      return $ans;
   }

}
?>

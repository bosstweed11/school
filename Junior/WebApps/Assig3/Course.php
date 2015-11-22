<?php
// CS 1520 User class for usesession-oop.php handout

class Course
{
   private $term;
   private $department;
   private $number;
   private $grade;
   private $gpa;

   public function __construct($t, $d, $n, $gr, $gp)
   {
     $this->term = $t;
     $this->department = $d;
     $this->number = $n;
     $this->grade = $gr;
     $this->gpa= $gp;

   }

   public function getDepartment()
   {
     return $this->department;
   }
   
   public function getNumber()
   {
   	 return $this->number;
   }
   
   public function getGPA()
   {
   	return $this->gpa;
   }

public function __toString()
{
	$req = "N";
	if ( $this->gpa >= 2.00 )
	{
		$req = "S";
	}
	$ans = "<requirement>" . $req . "</requirement>".
		"<class>" . $this->department . $this->number ."</class>".
	 	"<term>" . $this->term . "</term>".
	 	"<grade>" . $this->grade ."</grade>".
	 	"<gpa>" . $this->gpa . "</gpa>";
 
 
	return $ans;
}

}
?>

<?php
class SubFoo extends Foo
{
   // Inherits $x, $y and $z from Foo.  Adding $sub in the subclass
   private $sub;

   public function __construct($newX, $newY, $newZ, $newSub)
   {
      parent::__construct($newX, $newY, $newZ);
      $this->sub = $newSub;
   }
 
   public function __destruct()
   {
      parent::__destruct();
      echo "Look out!  SubFoo ", $this, " being destroyed! <br />";
   }

   // SubFoo cannot access x directly, but it can use the getX() method
   // which is inherited.
   public function subGetX()
   {
      //return $this->x;
      return Foo::getX();
   }

   // SubFoo CAN access y directly, since it is protected
   public function subGetY()
   {
      return $this->y;
   }

   public function setSub($data)
   {
      echo "Setting Sub to $data <br />";
      $this->sub = $data;
   }

   public function __toString()
   {
      $ans = Foo::__toString();
      return $ans . " and sub = " . $this->sub;
   }
}
?>

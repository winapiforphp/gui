--TEST--
Win\Gui\Window cannot be cloned
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;

$window = new Window();
clone $window;
?>
--EXPECTF--

Fatal error: Trying to clone an uncloneable object of class Win\Gui\Window in %s on line %d
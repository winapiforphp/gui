--TEST--
Win\Gui\Window var_dump() output
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;

$window = new Window();
var_dump($window);
?>
--EXPECTF--
object(Win\Gui\Window)#%d (0) {
}
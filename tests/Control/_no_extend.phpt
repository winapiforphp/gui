--TEST--
Win\Gui\Control can't be extended
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
Class test extends Win\Gui\Control {
}
try {
  new test;
} catch (Exception $e) {
  echo $e->getMessage();
}
?>
--EXPECT--
Win\Gui\Control cannot be extended, extend a child control instead
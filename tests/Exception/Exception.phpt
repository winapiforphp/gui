--TEST--
Win\Gui\Exception Basics
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
var_dump(class_exists('Win\Gui\Exception'));
// verify it extends base exception
$e = new Win\Gui\Exception();
var_dump($e instanceof \Exception);
?>
--EXPECTF--
bool(true)
bool(true)
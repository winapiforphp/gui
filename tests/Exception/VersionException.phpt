--TEST--
Win\Gui\VersionException Basics
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
var_dump(class_exists('Win\Gui\VersionException'));
// verify it extends base exception
$e = new Win\Gui\VersionException();
var_dump($e instanceof Win\Gui\Exception);
var_dump($e instanceof \Exception);
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
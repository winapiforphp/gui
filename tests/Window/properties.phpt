--TEST--
Win\Gui\Window properties
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;

$window = new Window();

$window->test = 'foo';
var_dump($window->test);
unset($window->test);
var_dump(isset($window->test));
?>
--EXPECTF--
string(3) "foo"
bool(false)
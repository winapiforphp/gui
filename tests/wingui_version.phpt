--TEST--
phpversion('wingui') version
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
var_dump(phpversion('wingui'));
?>
--EXPECTF--
string(%d) "%s"
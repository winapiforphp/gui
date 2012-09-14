--TEST--
phpversion('wingui') version
--SKIPIF--
<?php
include __DIR__ . '/skipif.inc';
?>
--FILE--
<?php
var_dump(phpversion('wingui'));
?>
--EXPECTF--
string(%d) "%s"
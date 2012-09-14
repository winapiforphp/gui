--TEST--
phpinfo specific to wingui
--SKIPIF--
<?php
include __DIR__ . '/skipif.inc';
?>
--FILE--
<?php
$ext = new ReflectionExtension('wingui');
$ext->info();
?>
--EXPECTF--
wingui

Version => %d.%d.%d%s
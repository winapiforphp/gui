--TEST--
Win\Gui\Control\Statusbar var_dump output
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
$statusbar = new Win\Gui\Control\Statusbar;
var_dump($statusbar);
?>
--EXPECTF--
object(Win\Gui\Control\Statusbar)#1 (0) {
}
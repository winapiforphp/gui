--TEST--
Win\Gui\Control\Statusbar->send() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

$statusbar->send();

/* Wrong number args - 1 */
try {
    $statusbar->getBorders(1);
    trigger_error('Win\Gui\Control\Statusbar->getBorders requires no arguments');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
array(3) {
  [0]=>
  int(%s)
  [1]=>
  int(%s)
  [2]=>
  int(%s)
}
Win\Gui\Control\Statusbar::getBorders() expects exactly 0 parameters, 1 given
--TEST--
Win\Gui\Control\Statusbar->getParts() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

var_dump($statusbar->getParts());
$statusbar->setParts(array(55, -1));
var_dump($statusbar->getParts());

/* Wrong number args - any */
try {
    $statusbar->getParts(1);
    trigger_error('Win\Gui\Control\Statusbar->isSimple requires no arguments');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
array(1) {
  [0]=>
  int(-1)
}
array(2) {
  [0]=>
  int(55)
  [1]=>
  int(-1)
}
Win\Gui\Control\Statusbar::getParts() expects exactly 0 parameters, 1 given
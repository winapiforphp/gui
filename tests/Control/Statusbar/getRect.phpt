--TEST--
Win\Gui\Control\Statusbar->getRect() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

var_dump($statusbar->getRect());
var_dump($statusbar->getRect(1));

$statusbar->setParts(array(15, 23));
var_dump($statusbar->getRect(1));

$statusbar->setSimple(true);
var_dump($statusbar->getRect());

/* Wrong number args - too many */
try {
    $statusbar->getRect(1, 1);
    trigger_error('Win\Gui\Control\Statusbar->getRect requires at most 1 arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $statusbar->getRect(array());
    trigger_error('Win\Gui\Control\Statusbar->getRect requires an integer argument for arg 1');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
array(4) {
  ["left"]=>
  int(%d)
  ["right"]=>
  int(%d)
  ["bottom"]=>
  int(%d)
  ["top"]=>
  int(%d)
}
array(4) {
  ["left"]=>
  int(%d)
  ["right"]=>
  int(%d)
  ["bottom"]=>
  int(%d)
  ["top"]=>
  int(%d)
}
array(4) {
  ["left"]=>
  int(%d)
  ["right"]=>
  int(%d)
  ["bottom"]=>
  int(%d)
  ["top"]=>
  int(%d)
}
bool(false)
Win\Gui\Control\Statusbar::getRect() expects at most 1 parameter, 2 given
Win\Gui\Control\Statusbar::getRect() expects parameter 1 to be long, array given
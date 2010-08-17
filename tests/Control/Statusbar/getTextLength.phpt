--TEST--
Win\Gui\Control\Statusbar->getTextLength() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

var_dump($statusbar->getTextLength());
$statusbar->setText("my string");
var_dump($statusbar->getTextLength());

$statusbar->setParts(array(15, 23));
$statusbar->setText("second string", 1);
var_dump($statusbar->getTextLength(1));

$statusbar->setSimple(true);
$statusbar->setText("simple text");
var_dump($statusbar->getTextLength());

/* Wrong number args - too many */
try {
    $statusbar->getTextLength(1, 1);
    trigger_error('Win\Gui\Control\Statusbar->getTextLength requires at most 1 arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $statusbar->getTextLength(array());
    trigger_error('Win\Gui\Control\Statusbar->getTextLength requires an integer argument for arg 1');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
array(2) {
  [0]=>
  int(0)
  [1]=>
  int(0)
}
array(2) {
  [0]=>
  int(9)
  [1]=>
  int(0)
}
array(2) {
  [0]=>
  int(13)
  [1]=>
  int(0)
}
array(2) {
  [0]=>
  int(11)
  [1]=>
  int(0)
}
Win\Gui\Control\Statusbar::getTextLength() expects at most 1 parameter, 2 given
Win\Gui\Control\Statusbar::getTextLength() expects parameter 1 to be long, array given
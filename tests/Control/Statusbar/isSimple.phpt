--TEST--
Win\Gui\Control\Statusbar->isSimple() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

var_dump($statusbar->isSimple());
$statusbar->setSimple(true);
var_dump($statusbar->isSimple());

/* Wrong number args - any */
try {
    $statusbar->isSimple(1);
    trigger_error('Win\Gui\Control\Statusbar->isSimple requires no arguments');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
bool(false)
bool(true)
Win\Gui\Control\Statusbar::isSimple() expects exactly 0 parameters, 1 given
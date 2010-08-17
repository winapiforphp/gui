--TEST--
Win\Gui\Control\Statusbar->setSimple() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

var_dump($statusbar->setSimple(false));
var_dump($statusbar->setSimple(true));
var_dump($statusbar->setSimple(true));

/* Wrong number args - none */
try {
    $statusbar->setSimple();
    trigger_error('Win\Gui\Control\Statusbar->setSimple requires at least one arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - too many */
try {
    $statusbar->setSimple(1, 1);
    trigger_error('Win\Gui\Control\Statusbar->setSimple requires no more than one arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 1 */
try {
    $statusbar->setSimple(array());
    trigger_error('Win\Gui\Control\Statusbar->setSimple requires a boolean argument');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
bool(false)
bool(true)
bool(true)
Win\Gui\Control\Statusbar::setSimple() expects exactly 1 parameter, 0 given
Win\Gui\Control\Statusbar::setSimple() expects exactly 1 parameter, 2 given
Win\Gui\Control\Statusbar::setSimple() expects parameter 1 to be boolean, array given
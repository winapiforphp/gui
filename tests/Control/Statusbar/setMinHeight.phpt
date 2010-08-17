--TEST--
Win\Gui\Control\Statusbar->setMinHeight() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

var_dump($statusbar->setMinHeight(3));

/* Wrong number args - none */
try {
    $statusbar->setMinHeight();
    trigger_error('Win\Gui\Control\Statusbar->setMinHeight requires at least one arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - too many */
try {
    $statusbar->setMinHeight(1, 1);
    trigger_error('Win\Gui\Control\Statusbar->setMinHeight requires no more than one arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 1 */
try {
    $statusbar->setMinHeight(array());
    trigger_error('Win\Gui\Control\Statusbar->setMinHeight requires an int argument');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
NULL
Win\Gui\Control\Statusbar::setMinHeight() expects exactly 1 parameter, 0 given
Win\Gui\Control\Statusbar::setMinHeight() expects exactly 1 parameter, 2 given
Win\Gui\Control\Statusbar::setMinHeight() expects parameter 1 to be long, array given
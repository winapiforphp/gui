--TEST--
Win\Gui\Control\Statusbar->setParts() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

var_dump($statusbar->setParts(array()));
var_dump($statusbar->setParts(array(1, 2)));

/* Wrong number args - none */
try {
    $statusbar->setParts();
    trigger_error('Win\Gui\Control\Statusbar->setParts requires at least one arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - too many */
try {
    $statusbar->setParts(array(), 1);
    trigger_error('Win\Gui\Control\Statusbar->setParts requires no more than one arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 1 */
try {
    $statusbar->setParts(1);
    trigger_error('Win\Gui\Control\Statusbar->setParts requires an array argument');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
bool(false)
bool(true)
Win\Gui\Control\Statusbar::setParts() expects exactly 1 parameter, 0 given
Win\Gui\Control\Statusbar::setParts() expects exactly 1 parameter, 2 given
Win\Gui\Control\Statusbar::setParts() expects parameter 1 to be array, integer given
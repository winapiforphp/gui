--TEST--
Win\Gui\Control\Statusbar->setText() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

var_dump($statusbar->setText("my string"));
var_dump($statusbar->setText("my string", 0));
var_dump($statusbar->setText("my string", 0, Win\Gui\Control\Statusbar::NOBORDERS));
var_dump($statusbar->setText("my string", 1));
$statusbar->setSimple(true);
var_dump($statusbar->setText("my string"));

/* Wrong number args - none */
try {
    $statusbar->setText();
    trigger_error('Win\Gui\Control\Statusbar->setText requires at least one arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - too many */
try {
    $statusbar->setText(1, 1, 1, 1);
    trigger_error('Win\Gui\Control\Statusbar->setText requires at most 3 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $statusbar->setText(array());
    trigger_error('Win\Gui\Control\Statusbar->setText requires a string argument for arg 1');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 2 */
try {
    $statusbar->setText("", array());
    trigger_error('Win\Gui\Control\Statusbar->setText requires an int argument for arg 2');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 3 */
try {
    $statusbar->setText("", 1, array());
    trigger_error('Win\Gui\Control\Statusbar->setText requires an int argument for arg 3');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
Win\Gui\Control\Statusbar::setText() expects at least 1 parameter, 0 given
Win\Gui\Control\Statusbar::setText() expects at most 3 parameters, 4 given
Win\Gui\Control\Statusbar::setText() expects parameter 1 to be string, array given
Win\Gui\Control\Statusbar::setText() expects parameter 2 to be long, array given
Win\Gui\Control\Statusbar::setText() expects parameter 3 to be long, array given
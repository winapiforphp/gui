--TEST--
Win\Gui\Control\Statusbar->setTipText() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Control\Statusbar;

$statusbar = new Statusbar(new Window);

$statusbar->setTipText("my string");
var_dump($statusbar->getTipText() === "my string");

var_dump($statusbar->setParts(array(1, 2)));

$statusbar->setTipText("my new string", 0);
var_dump($statusbar->getTipText(0) === "my new string");

$statusbar->setTipText("my first string", 1);
var_dump($statusbar->getTipText(1) === "my first string");

$statusbar->setSimple(true);
$statusbar->setTipText("my string");
var_dump($statusbar->getTipText() === "my string");

/* Wrong number args - none */
try {
    $statusbar->setTipText();
    trigger_error('Win\Gui\Control\Statusbar->setTipText requires at least one arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - too many */
try {
    $statusbar->setTipText(1, 1, 1);
    trigger_error('Win\Gui\Control\Statusbar->setTipText requires at most 2 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $statusbar->setTipText(array());
    trigger_error('Win\Gui\Control\Statusbar->setTipText requires a string argument for arg 1');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 2 */
try {
    $statusbar->setTipText("", array());
    trigger_error('Win\Gui\Control\Statusbar->setTipText requires an int argument for arg 2');
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
Win\Gui\Control\Statusbar::setTipText() expects at least 1 parameter, 0 given
Win\Gui\Control\Statusbar::setTipText() expects at most 2 parameters, 3 given
Win\Gui\Control\Statusbar::setTipText() expects parameter 1 to be string, array given
Win\Gui\Control\Statusbar::setTipText() expects parameter 2 to be long, array given
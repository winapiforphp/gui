--TEST--
Win\Gui\Window::addFilterMessage() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
if(!Win\Gui::isVista()) die('skip - Win\Gui\Window::addFilterMessage not available');
?>
--FILE--
<?php
use Win\Gui\Window;

// add a message
var_dump(Window::addFilterMessage(1)); // TODO: use a real message constant!

/* Wrong number args - 1 */
try {
    Window::addFilterMessage();
    trigger_error('addFilterMessage requires 1 arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 2 */
try {
    Window::addFilterMessage(1, 1);
    trigger_error('addFilterMessage requires only 1 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    Window::addFilterMessage(array());
    trigger_error('addFilterMessage requires arg 1 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(true)
Win\Gui\Window::addFilterMessage() expects exactly 1 parameter, 0 given
Win\Gui\Window::addFilterMessage() expects exactly 1 parameter, 2 given
Win\Gui\Window::addFilterMessage() expects parameter 1 to be long, array given
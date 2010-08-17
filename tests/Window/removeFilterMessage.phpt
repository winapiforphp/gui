--TEST--
Win\Gui\Window::removeFilterMessage() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
if(!Win\Gui::isVista()) die('skip - Win\Gui\Window::removeFilterMessage not available');
?>
--FILE--
<?php
use Win\Gui\Window;

// add a message
var_dump(Window::removeFilterMessage(1)); // TODO: use a real message constant!

/* Wrong number args - 1 */
try {
    Window::removeFilterMessage();
    trigger_error('removeFilterMessage requires 1 arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 2 */
try {
    Window::removeFilterMessage(1, 1);
    trigger_error('removeFilterMessage requires only 1 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    Window::removeFilterMessage(array());
    trigger_error('removeFilterMessage requires arg 1 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(false)
Win\Gui\Window::removeFilterMessage() expects exactly 1 parameter, 0 given
Win\Gui\Window::removeFilterMessage() expects exactly 1 parameter, 2 given
Win\Gui\Window::removeFilterMessage() expects parameter 1 to be long, array given
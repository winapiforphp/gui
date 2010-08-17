--TEST--
Win\Gui\Window::allowSetForeground() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;

var_dump(Win\Gui\Window::allowSetForeground());
var_dump(Win\Gui\Window::allowSetForeground(null));
var_dump(Win\Gui\Window::allowSetForeground(-1));
var_dump(Win\Gui\Window::allowSetForeground(45));

/* Wrong number args - 1 */
try {
    Window::allowSetForeground();
    trigger_error('allowSetForeground requires 1 arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong number args - 2 */
try {
    Window::allowSetForeground(1, 1, 1);
    trigger_error('allowSetForeground requires at most 2 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type */
try {
    Window::allowSetForeground(array(), 1);
    trigger_error('allowSetForeground requires arg 1 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 2 */
try {
    Window::allowSetForeground(1, array());
    trigger_error('allowSetForeground requires arg 2 to be bool');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(false)
bool(true)
Win\Gui\Window::allowSetForeground() expects at least 1 parameter, 0 given
Win\Gui\Window::allowSetForeground() expects at most 2 parameters, 3 given
Win\Gui\Window::allowSetForeground() expects parameter 1 to be long, array given
Win\Gui\Window::allowSetForeground() expects parameter 2 to be boolean, array given
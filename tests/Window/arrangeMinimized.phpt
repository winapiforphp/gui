--TEST--
Win\Gui\Window::arrangeMinimized() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;

// create a window
$window = new Window();

// arrange desktop minimized
var_dump(Window::arrangeMinimized());

// arrange window minimized
var_dump(Window::arrangeMinimized($window));

/* Wrong number args - 1 */
try {
    Window::arrangeMinimized(1, 1);
    trigger_error('arrangeMinimized requires at most 1 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    Window::arrangeMinimized(1);
    trigger_error('arrangeMinimized requires arg 1 to be Win\Gui\Window object');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
int(%d)
int(%d)
Win\Gui\Window::arrangeMinimized() expects at most 1 parameter, 2 given
Win\Gui\Window::arrangeMinimized() expects parameter 1 to be Win\Gui\Window, integer given

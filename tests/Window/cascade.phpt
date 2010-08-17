--TEST--
Win\Gui\Window::cascade() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;

// arrange our desktop
var_dump(Window::cascade());

$window = new Window();

// arrange our window
var_dump(Window::cascade($window));

/* Wrong number args - 1 */
try {
    Window::cascade($window, 0, array(), array(), 1);
    trigger_error('cascade requires at most 4 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 1 */
try {
    Window::cascade(1);
    trigger_error('cascade requires arg 1 to be Win\Gui\Window or null');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 2 */
try {
    Window::cascade($window, array());
    trigger_error('cascade requires arg 2 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 3 */
try {
    Window::cascade($window, 0, 1);
    trigger_error('cascade requires arg 3 to be array');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type 4 */
try {
    Window::cascade($window, 0, array(), 1);
    trigger_error('cascade requires arg 4 to be array');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
int(%d)
int(%d)
Win\Gui\Window::cascade() expects at most 4 parameters, 5 given
Win\Gui\Window::cascade() expects parameter 1 to be Win\Gui\Window, integer given
Win\Gui\Window::cascade() expects parameter 2 to be long, array given
Win\Gui\Window::cascade() expects parameter 3 to be array, integer given
Win\Gui\Window::cascade() expects parameter 4 to be array, integer given
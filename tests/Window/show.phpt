--TEST--
Win\Gui\Window->show() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;

// create a window
$window = new Window();

// show the window
var_dump($window->show());

// verify it's visible and active
var_dump($window->isActive());
var_dump($window->isVisible());

// hide the window
$window->hide();

// show non-activated
var_dump($window->show(true));

// verify it's visible and non-active
var_dump($window->isActive());
var_dump($window->isVisible());

/* Wrong number args - 1 */
try {
    $window->show(1, 1);
    trigger_error('show requires at most 1 arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $window->show(array());
    trigger_error('show requires arg 1 to be bool');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(false)
bool(true)
bool(true)
bool(false)
bool(false)
bool(true)
Win\Gui\Window::show() expects at most 1 parameter, 2 given
Win\Gui\Window::show() expects parameter 1 to be boolean, array given
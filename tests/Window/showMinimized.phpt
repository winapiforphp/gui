--TEST--
Win\Gui\Window->showMinimized() method
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
var_dump($window->showMinimized());

// verify it's visible and active
var_dump($window->isActive());
var_dump($window->isVisible());
var_dump($window->isMinimized());

// hide the window
$window->hide();

// show non-activated
var_dump($window->showMinimized(true));

// verify it's visible and non-active
var_dump($window->isActive());
var_dump($window->isVisible());
var_dump($window->isMinimized());

/* Wrong number args - 1 */
try {
    $window->showMinimized(1, 1);
    trigger_error('showMinimized requires at most 1 arg');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $window->showMinimized(array());
    trigger_error('showMinimized requires arg 1 to be bool');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(false)
bool(true)
bool(true)
bool(true)
bool(false)
bool(false)
bool(true)
bool(true)
Win\Gui\Window::showMinimized() expects at most 1 parameter, 2 given
Win\Gui\Window::showMinimized() expects parameter 1 to be boolean, array given
--TEST--
Win\Gui\Window->bringToTop() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;

// create a window
$window = new Window();

// bring window to top
var_dump($window->bringToTop());

/* Wrong number args - 1 */
try {
    $window->bringToTop(1);
    trigger_error('bringToTop requires no args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(true)
Win\Gui\Window::bringToTop() expects exactly 0 parameters, 1 given
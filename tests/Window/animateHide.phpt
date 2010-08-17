--TEST--
Win\Gui\Window->animateHide() method
--SKIPIF--
<?php
if(!extension_loaded('wingui')) die('skip - wingui extension not available');
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\Gui\Window\Animate as A;

// create a window
$window = new Window();

// show the window
$window->show();

// show the window
var_dump($window->animateHide(500, A::BLEND));

// verify it's not visible
var_dump($window->isVisible());

/* Wrong number args - 1 */
try {
    $window->animateHide(1, 1, 1);
    trigger_error('animateHide requires at most 2 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $window->animateHide(array());
    trigger_error('animateHide requires arg 1 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 2 */
try {
    $window->animateHide(1, array());
    trigger_error('animateHide requires arg 2 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(true)
bool(false)
Win\Gui\Window::animateHide() expects at most 2 parameters, 3 given
Win\Gui\Window::animateHide() expects parameter 1 to be long, array given
Win\Gui\Window::animateHide() expects parameter 2 to be long, array given

--TEST--
Win\Gui\Window->animateShow() method
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
var_dump($window->animateShow(500, A::BLEND));

// verify it's visible and active
var_dump($window->isVisible());
var_dump($window->isActive());

// hide the window
$window->hide();

// show the window but don't activate
var_dump($window->animateShow(500, A::BLEND, false));

// verify it's visible and non-active
var_dump($window->isVisible());
var_dump($window->isActive());

/* Wrong number args - 1 */
try {
    $window->animateShow(1, 1, 1, 1);
    trigger_error('animateShow requires at most 3 args');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $window->animateShow(array());
    trigger_error('animateShow requires arg 1 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 2 */
try {
    $window->animateShow(1, array());
    trigger_error('animateShow requires arg 2 to be int');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 3 */
try {
    $window->animateShow(1, 1, array());
    trigger_error('animateShow requires arg 3 to be boolean');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
Win\Gui\Window::animateShow() expects at most 3 parameters, 4 given
Win\Gui\Window::animateShow() expects parameter 1 to be long, array given
Win\Gui\Window::animateShow() expects parameter 2 to be long, array given
Win\Gui\Window::animateShow() expects parameter 3 to be boolean, array given
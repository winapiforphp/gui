--TEST--
Win\Gui\Window->show() method
--SKIPIF--
<?php
include __DIR__ . '/../../skipif.inc';
?>
--FILE--
<?php
use Win\Gui\Window;
use Win\System\InvalidArgumentException;

// create a window, default is not visible
$window = new Window([]);

// show the window
var_dump($window->show());

// verify it's visible and active
var_dump($window->isActive());
var_dump($window->isVisible());

// hide and deactivate the window
$window->hide();
$window->disable();

// show non-activated
var_dump($window->show(true));

// verify it's visible and non-active
var_dump($window->isActive());
var_dump($window->isVisible());

// hide the window
$window->hide();

// show async
var_dump($window->show(false, true));

// verify it's visible active
var_dump($window->isActive());
var_dump($window->isVisible());

/* Wrong number args - 1 */
try {
    $window->show(1, 1, 1);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 1 */
try {
    $window->show(array());
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), PHP_EOL;
}

/* Wrong arg type - 2 */
try {
    $window->show(0, array());
} catch (InvalidArgumentException $e) {
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
bool(true)
bool(false)
bool(false)
Win\Gui\Window::show() expects at most 2 parameters, 3 given
Win\Gui\Window::show() expects parameter 1 to be boolean, array given
Win\Gui\Window::show() expects parameter 2 to be boolean, array given